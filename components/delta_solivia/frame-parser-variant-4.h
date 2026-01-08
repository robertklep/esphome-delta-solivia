#pragma once

#include "frame-parser-interface.h"
#include <string>

namespace esphome {
namespace delta_solivia {

class FrameParserVariant4 : public IFrameParser {
protected:
  void parse_frame_(const uint8_t* frame, std::size_t pos) override {

    // XXX: sensor names should match `CONF_INV_*` values in __init__.py
    this->publish_text_sensor_(CONF_INV_PART_NUMBER, this->parse_string(frame, pos, 11));
    pos += 11;
    this->publish_text_sensor_(CONF_INV_SERIAL_NUMBER, this->parse_string(frame, pos, 18));
    pos += 18;
    this->publish_sensor_("SAP_date_code", this->extract_uint32(&frame[pos]));
    pos += 4;
    this->publish_sensor_("SAP_revision", this->extract_uint16(&frame[pos]));
    pos += 2;

    this->publish_sensor_("Software_rev_ac_control", this->extract_uint16(&frame[pos]));
    pos += 2;
    this->publish_sensor_("Software_rev_dc_control", this->extract_uint16(&frame[pos]));
    pos += 2;
    this->publish_sensor_("Software_rev_display", this->extract_uint16(&frame[pos]));
    pos += 2;
    this->publish_sensor_("Software_rev_ens_control", this->extract_uint16(&frame[pos]));
    pos += 2;

    // Solar inputs
    for (int input = 1; input <= 2; ++input) {
      this->publish_sensor_(CONF_INV_SOLAR_CURRENT_PREFIX + std::to_string(input), this->extract_uint16(&frame[pos]) * 0.1); // Amps
      pos += 2;
      this->publish_sensor_(CONF_INV_SOLAR_VOLTAGE_PREFIX + std::to_string(input), this->extract_uint16(&frame[pos])); // Volts
      pos += 2;
      this->publish_sensor_("solar_isolation_resistance_input_" + std::to_string(input), this->extract_uint16(&frame[pos])); // kOhm
      pos += 2;
    }

    // AC
    this->publish_sensor_(CONF_INV_AC_CURRENT, this->extract_uint16(&frame[pos]) * 0.1); // Amps
    pos += 2;
    this->publish_sensor_(CONF_INV_AC_VOLTAGE, this->extract_uint16(&frame[pos])); // Volts
    pos += 2;
    this->publish_sensor_(CONF_INV_AC_POWER, this->extract_uint16(&frame[pos])); // Watts
    pos += 2;
    this->publish_sensor_(CONF_INV_AC_FREQ, this->extract_uint16(&frame[pos]) * 0.01); // Hertz
    pos += 2;

    // Today's energy info
    this->publish_sensor_(CONF_INV_SUPPLIED_AC_ENERGY_TODAY, this->extract_uint16(&frame[pos])); // Wh
    pos += 2;
    this->publish_sensor_(CONF_INV_RUNTIME_TODAY, this->extract_uint16(&frame[pos]) * 60.0); // Minutes
    pos += 2;

    // DC temp
    this->publish_sensor_(CONF_INV_TEMPERATURE_NTC_DC, this->extract_int16(&frame[pos])); // Celsius, signed
    pos += 2;

    // Solar input 1/2 MOV resistance
    pos += 2;
    pos += 2;

    // AC temp
    this->publish_sensor_(CONF_INV_TEMPERATURE_NTC_AC, this->extract_int16(&frame[pos])); // Celsius, signed
    pos += 2;

    // AC/ENS control (not sure what do to with these)
    pos += 2; // AC voltage (AC control)
    pos += 2; // AC frequency (AC control)
    pos += 2; // DC injection current (AC control)
    pos += 2; // AC voltage (ENS control)
    pos += 2; // AC frequency (ENS control)
    pos += 2; // DC injection current (ENS control)

    // Solar min/max
    pos += 2; // Maximum solar 1 input current
    pos += 2; // Maximum solar 1 input voltage
    pos += 2; // Maximum solar 1 input power
    pos += 2; // Minimum isolation resistance solar 1
    pos += 2; // Maximum isolation resistance solar 1
    pos += 2; // Maximum solar 2 input current
    pos += 2; // Maximum solar 2 input voltage
    pos += 2; // Maximum solar 2 input power
    pos += 2; // Minimum isolation resistance solar 2
    pos += 2; // Maximum isolation resistance solar 2

    // Today's values
    this->publish_sensor_(CONF_INV_MAX_AC_CURRENT_TODAY, this->extract_uint16(&frame[pos]) * 0.1); // Amps
    pos += 2;
    this->publish_sensor_(CONF_INV_MIN_AC_VOLTAGE_TODAY, this->extract_uint16(&frame[pos])); // Volts
    pos += 2;
    this->publish_sensor_(CONF_INV_MAX_AC_VOLTAGE_TODAY, this->extract_uint16(&frame[pos])); // Volts
    pos += 2;
    this->publish_sensor_(CONF_INV_MAX_AC_POWER_TODAY, this->extract_uint16(&frame[pos])); // Watts
    pos += 2;
    this->publish_sensor_(CONF_INV_MIN_AC_FREQ_TODAY, this->extract_uint16(&frame[pos]) * 0.01); // Hertz
    pos += 2;
    this->publish_sensor_(CONF_INV_MAX_AC_FREQ_TODAY, this->extract_uint16(&frame[pos]) * 0.01); // Hertz
    pos += 2;

    // Totals
    this->publish_sensor_(CONF_INV_SUPPLIED_AC_ENERGY_TOTAL, this->extract_uint32(&frame[pos]) * 0.1); // kWh
    pos += 4;
    this->publish_sensor_(CONF_INV_RUNTIME_TOTAL, this->extract_uint32(&frame[pos]) * 3600.0); // Hours
    pos += 4;

    // Status bytes
    pos += 1; // Inverter runtime
    pos += 1; // Global alarm status
    pos += 1; // Status DC input
    pos += 1; // Limits DC input
    pos += 1; // Status AC output
    pos += 1; // Limits AC output
    pos += 1; // Isolation warning status
    pos += 1; // DC hardware failure
    pos += 1; // AC hardware failure
    pos += 1; // ENS hardware failure
    pos += 1; // Internal Bulk failure
    pos += 1; // Internal communications failure
    pos += 1; // AC hardware disturbance
    pos += 20; // History status messages
  }

public:
  FrameParserVariant4() {}

  static bool supports(uint8_t variant) {
    return (variant == 4 || variant == 14 || variant == 63);
  }
};

}
}
