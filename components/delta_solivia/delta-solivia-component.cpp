#include "delta-solivia-component.h"
#include "delta-solivia-crc.h"

namespace esphome {
namespace delta_solivia {

void DeltaSoliviaComponent::setup() {
  if (this->flow_control_pin_ != nullptr) {
    this->flow_control_pin_->setup();
    this->flow_control_pin_->digital_write(false);
  }
}

// add an inverter
void DeltaSoliviaComponent::add_inverter(DeltaSoliviaInverter* inverter) {
  ESP_LOGD(LOG_TAG, "CONFIG - added inverter with address %u", inverter->get_address());
  this->inverters_[inverter->get_address()] = inverter;
}

// get inverter
DeltaSoliviaInverter* DeltaSoliviaComponent::get_inverter(uint8_t address) {
  return this->inverters_.count(address) == 1 ? this->inverters_[address] : nullptr;
}

// process an incoming packet
bool DeltaSoliviaComponent::process_frame(const Frame& frame) {
  if (!this->validate_header(frame)) {
    ESP_LOGD(LOG_TAG, "FRAME - incorrect header");
    return false;
  }

  if (!this->validate_size(frame)) {
    return false;
  }

  if (!this->validate_trailer(frame)) {
    return false;
  }

  // update inverter
  auto inverter = this->get_inverter(frame[2]);
  uint8_t buffer[frame.size()];
  std::copy(frame.begin(), frame.end(), buffer); // copy vector to plain byte buffer
  inverter->update_sensors(buffer);

  // done
  return true;
}

// validate packet header
bool DeltaSoliviaComponent::validate_header(const Frame& frame) {
  if (frame.size() < 6 || frame[0] != STX || frame[1] != ACK || frame[2] == 0 || frame[4] != 0x60 || frame[5] != 0x01) {
    return false;
  }

  // validate address
  if (!this->validate_address(frame)) {
    return false;
  }

#if DEBUG_HEADER
  ESP_LOGD(LOG_TAG, "FRAME - SOP = 0x%02x, PC = 0x%02x, address = %u, data size = %u, cmd = 0x%02x, sub cmd = 0x%02x",
    frame[0],
    frame[1],
    frame[2],
    frame[3],
    frame[4],
    frame[5]
  );
#endif

  return true;
}

bool DeltaSoliviaComponent::validate_size(const Frame& frame) {
  unsigned int expected = 4 + frame[3] + 3;

  if (frame.size() != expected) {
    ESP_LOGD(LOG_TAG, "FRAME - incorrect frame size (was %u, expected %u)", frame.size(), expected);
    return false;
  }
  return true;
}

bool DeltaSoliviaComponent::validate_address(const Frame& frame) {
  unsigned int address = frame[2];
  auto inverter = this->get_inverter(address);

  if (inverter == nullptr) {
    ESP_LOGD(LOG_TAG, "FRAME - unknown address %u", address);
    return false;
  }
  return true;
}

bool DeltaSoliviaComponent::validate_trailer(const Frame& frame) {
  const uint16_t end_of_data     = frame[3] + 4;
  const uint8_t  end_of_protocol = frame[end_of_data + 2];

  if (end_of_protocol != ETX) {
    ESP_LOGE(LOG_TAG, "FRAME - invalid end-of-protocol byte (was 0x%02x, should be 0x%02x)", end_of_protocol, ETX);
    return false;
  }

  const uint16_t packet_crc     = *reinterpret_cast<const uint16_t*>(&frame[end_of_data]);
  uint16_t       calculated_crc = delta_solivia_crc(&frame[1], &frame[end_of_data - 1]);
  if (packet_crc != calculated_crc) {
    ESP_LOGE(LOG_TAG, "FRAME - CRC mismatch (was 0x%04X, should be 0x%04X)", calculated_crc, packet_crc);
    return false;
  }

  return true;
}

void DeltaSoliviaComponent::update() {
  if (this->has_gateway_) {
    this->update_with_gateway();
  } else {
    this->update_without_gateway();
  }
}

void DeltaSoliviaComponent::write_frame(const uint8_t* bytes, unsigned len) {
  if (this->flow_control_pin_ != nullptr) {
    this->flow_control_pin_->digital_write(true);
  }
  this->write_array(bytes, len);
  this->flush();
  if (this->flow_control_pin_ != nullptr) {
    this->flow_control_pin_->digital_write(false);
  }
}

void DeltaSoliviaComponent::update_without_gateway() {
  // toggle between inverters to query
  static InverterMap::const_iterator it = this->inverters_.begin();
  auto inverter = it->second;

  // request an update from the inverter
  inverter->request_update(
    [this](const uint8_t* bytes, unsigned len) -> void {
      this->write_frame(bytes, len);
    }
  );

  // pick inverter for the next update, with wrap around
  if (++it == this->inverters_.end()) {
    it = this->inverters_.begin();
  }

  // protocol timing chart (page 9) says response should arrive within 6ms.
  uint32_t start = millis();
  while (millis() - start < 7 && !this->available())
      ;

  if (! this->available()) {
    ESP_LOGD(LOG_TAG, "RESPONSE - timeout");
    return;
  }

  ESP_LOGD(LOG_TAG, "GOING TO READ");
  this->set_throttle(0);
  this->update_with_gateway();

#if 0
  // absolute maximum packet size
  uint8_t buffer[262];

  // read packet (XXX: this assumes that the full packet is available in the receive buffer)
  unsigned int bytes_read = this->available();
  bool read_success       = this->read_array(buffer, bytes_read);

  if (! read_success) {
    ESP_LOGD(LOG_TAG, "RESPONSE - unable to read packet");
    return;
  }

  // process frame
  Frame frame(buffer, buffer + bytes_read);

  this->process_frame(frame);
#endif
}

void DeltaSoliviaComponent::update_with_gateway() {
  // buffer to store serial data
  static Frame frame;

  // read data off UART
  while (this->available() > 0) {
    // add new bytes to buffer
    frame.push_back(this->read());

    // wait until we've read enough bytes for a header
    if (frame.size() < 6) {
      continue;
    }

    // validate header
    if (! this->validate_header(frame)) {
      frame.erase(frame.begin());
      continue;
    }

    // read full packet
    unsigned int required_size = 4 + frame[3] + 3;
    if (frame.size() != required_size) {
      continue;
    }

    // throttle?
    static unsigned int last_update = 0;
    unsigned int now                = millis();
    if (now - last_update >= this->throttle_) {
      // process frame
      this->process_frame(frame);
      last_update = millis();
    }

    // clear vector for next round
    frame.clear();
  }
}

void DeltaSoliviaComponent::set_power_limit(uint8_t pct, uint8_t address) {
  if (pct > 100) {
    ESP_LOGE(LOG_TAG, "POWER LIMIT - percentage should be <= 100% (value: %u%%)", pct);
    return;
  }
  ESP_LOGD(LOG_TAG, "POWER LIMIT - setting limit to %u%% for address %02X (0xFF = all inverters)", pct, address);

  // page 14
  const uint8_t bytes[] = {
    STX,     // start of protocol
    ENQ,     // enquire
    address, // for inverter with address
    0x03,    // number of data bytes, including commands
    0x0D,    // command
    0x88,    // subcommand
    pct,     // percentage
    0x00,    // CRC low
    0x00,    // CRC high
    ETX      // end of protocol
  };

  // calculate CRC
  *((uint16_t*) &bytes[7]) = delta_solivia_crc((uint8_t *) bytes + 1, (uint8_t *) bytes + 6);

  // write command to bus
  this->write_frame(bytes, sizeof(bytes));
}

}
}
