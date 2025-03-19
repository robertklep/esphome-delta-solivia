// Based on Public Solar Inverter Communication Protocol (Version 1.2)
#pragma once

#include <map>
#include "esphome.h"
#include "esphome/components/uart/uart.h"
#include "delta-solivia-crc.h"
//#include "delta-solivia-inverter.h"

namespace esphome {
namespace delta_solivia {

using uart::UARTDevice;
using uart::UARTComponent;

// forward definition
class DeltaSoliviaInverter;

using InverterMap = std::map<uint8_t, DeltaSoliviaInverter*>;
using Frame       = std::vector<uint8_t>;

class DeltaSoliviaComponent : public PollingComponent, public UARTDevice {
protected:
  unsigned int throttle_;
  InverterMap inverters_;
  GPIOPin* flow_control_pin_{nullptr};
  bool has_gateway_;

public:
  DeltaSoliviaComponent() : has_gateway_(false), throttle_(10000) {}

  void set_throttle(unsigned int throttle) { this->throttle_ = throttle; }
  void set_flow_control_pin(GPIOPin* flow_control_pin) { this->flow_control_pin_ = flow_control_pin; }
  void set_has_gateway(bool has_gateway) { this->has_gateway_ = has_gateway; }

  void setup() override;
  void update() override;
  void add_inverter(DeltaSoliviaInverter* inverter);
  DeltaSoliviaInverter* get_inverter(uint8_t address);
  bool process_frame(const Frame& frame);
  bool validate_header(const Frame& frame);
  bool validate_size(const Frame& frame);
  bool validate_address(const Frame& frame);
  bool validate_trailer(const Frame& frame);
  void update_without_gateway();
  void update_with_gateway();
};

}
}
