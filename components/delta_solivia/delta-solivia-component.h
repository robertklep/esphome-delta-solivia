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

class DeltaSoliviaComponent: public PollingComponent, public UARTDevice {
  unsigned int throttle;
  InverterMap inverters;
  GPIOPin *flow_control_pin{nullptr};
  bool has_gateway;

  public:
    DeltaSoliviaComponent() : has_gateway(false), throttle(10000) {}

    void set_throttle(unsigned int throttle_) { throttle = throttle_; }
    void set_flow_control_pin(GPIOPin *flow_control_pin_) { flow_control_pin = flow_control_pin_; }
    void set_has_gateway(bool has_gateway_) { has_gateway = has_gateway_; }

    void setup() override;
    void update() override;
    void add_inverter(DeltaSoliviaInverter*);
    DeltaSoliviaInverter* get_inverter(uint8_t);
    bool process_frame(const Frame&);
    bool validate_header(const Frame&);
    bool validate_size(const Frame&);
    bool validate_address(const Frame&);
    bool validate_trailer(const Frame&);
    void update_without_gateway();
    void update_with_gateway();
};

}
}
