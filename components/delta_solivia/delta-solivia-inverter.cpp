#include "delta-solivia-inverter.h"
#include "frame-parser.h"

namespace esphome {
namespace delta_solivia {

DeltaSoliviaInverter::DeltaSoliviaInverter(uint8_t address, uint8_t variant) : address_(address), variant_(variant), parser_(FrameParserFactory::create_parser(variant)) {
  // validate parser
  if (this->parser_ == nullptr) {
    ESP_LOGE(LOG_TAG, "INVERTER%u - unsupported variant %u", this->address_, this->variant_);
    return;
  }

  // set handlers
  this->parser_->set_publish_sensor([this](const std::string& name, float value) {
    this->publish_sensor(name, value, false);
  });
  this->parser_->set_publish_text_sensor([this](const std::string& name, const std::string& value)  {
    this->publish_text_sensor(name, value, false);
  });
}

void DeltaSoliviaInverter::update_sensors(const uint8_t* buffer) {
  ESP_LOGD(LOG_TAG, "INVERTER#%u - updating sensors", this->address_);
  this->parser_->parse_frame(buffer, true);
}

void DeltaSoliviaInverter::publish_sensor(const std::string& name, float value, bool once) {
  ESP_LOGD(LOG_TAG, "INVERTER#%u - sensor '%s', value = %f", this->address_, name.c_str(), value);
  auto entry = this->sensors_.find(name);

  ESP_LOGE("TEST", "name=%s value=%f", name.c_str(), value);
  if (entry != this->sensors_.end()) {
    ESP_LOGE("TEST", "name=%s value=%f MARK 1", name.c_str(), value);
    if (! once || ! entry->second->has_state()) {
      ESP_LOGE("TEST", "name=%s value=%f MARK 2", name.c_str(), value);
      //entry->second->publish_state(value);
    }
  }
}

void DeltaSoliviaInverter::publish_text_sensor(const std::string& name, const std::string& value, bool once) {
  ESP_LOGD(LOG_TAG, "INVERTER#%u - sensor '%s', value = %s", this->address_, name.c_str(), value.c_str());
  auto entry = this->text_sensors_.find(name);

  if (entry != this->text_sensors_.end()) {
    if (! once || ! entry->second->has_state()) {
      //entry->second->publish_state(value);
    }
  }
}

}
}
