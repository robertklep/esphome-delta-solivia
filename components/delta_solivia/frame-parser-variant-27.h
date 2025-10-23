#pragma once

#include "frame-parser-interface.h"

namespace esphome {
namespace delta_solivia {

// page 36
class FrameParserVariant27 : public IFrameParser {
protected:
  void parse_frame_(const uint8_t* frame, std::size_t pos) override {
    // SAP part number
    this->publish_text_sensor_(CONF_INV_PART_NUMBER, this->parse_string(frame, pos, 11));
    pos += 11;

    // SAP serial number
    this->publish_text_sensor_(CONF_INV_SERIAL_NUMBER, this->parse_string(frame, pos, 18));
    pos += 18;

    // SAP date code
    this->publish_sensor_("SAP_date_code", this->extract_uint32(&frame[pos]));
    pos += 4;

    // SAP revision
    this->publish_sensor_("SAP_revision", this->extract_uint16(&frame[pos]));
    pos += 2;

    // DSP FW Rev
    this->publish_sensor_("DSP_fw_rev", this->extract_uint16(&frame[pos]));
    pos += 2;

    // DSP FW Date
    this->publish_sensor_("DSP_fw_date", this->extract_uint16(&frame[pos]));
    pos += 2;

    // Redundant MCU FW Rev
    this->publish_sensor_("redundant_mcu_fw_rev", this->extract_uint16(&frame[pos]));
    pos += 2;

    // Redundant MCU FW Date
    this->publish_sensor_("redundant_mcu_fw_date", this->extract_uint16(&frame[pos]));
    pos += 2;

    // Display MCU FW Rev
    this->publish_sensor_("display_mcu_fw_rev", this->extract_uint16(&frame[pos]));
    pos += 2;

    // Display MCU FW Date
    this->publish_sensor_("display_mcu_fw_date", this->extract_uint16(&frame[pos]));
    pos += 2;

    // AC Voltage/Current/Power/Frequency (all phases)
    for (int phase = 1; phase <= 3; ++phase) {
      this->publish_sensor_(CONF_INV_AC_VOLTAGE_PHASE_PREFIX + std::to_string(phase), this->extract_uint16(&frame[pos]) / 10.0); // V
      pos += 2;
      this->publish_sensor_(CONF_INV_AC_CURRENT_PHASE_PREFIX + std::to_string(phase), this->extract_uint16(&frame[pos]) / 100.0); // A
      pos += 2;
      this->publish_sensor_(CONF_INV_AC_POWER_PHASE_PREFIX + std::to_string(phase), this->extract_uint16(&frame[pos])); // W
      pos += 2;
      this->publish_sensor_(CONF_INV_AC_FREQ_PHASE_PREFIX + std::to_string(phase), this->extract_uint16(&frame[pos]) / 100.0); // Hz
      pos += 2;
      this->publish_sensor_("ac_voltage_redundant_phase_" + std::to_string(phase), this->extract_uint16(&frame[pos]) / 10.0); // V
      pos += 2;
      this->publish_sensor_("ac_frequency_redundant_phase_" + std::to_string(phase), this->extract_uint16(&frame[pos]) / 100.0); // Hz
      pos += 2;
    }

    // Solar Voltage/Current/Power (all inputs)
    for (int input = 1; input <= 2; ++input) {
      this->publish_sensor_(CONF_INV_SOLAR_VOLTAGE_PREFIX + std::to_string(input), this->extract_uint16(&frame[pos]) / 10.0); // V
      pos += 2;
      this->publish_sensor_(CONF_INV_SOLAR_CURRENT_PREFIX + std::to_string(input), this->extract_uint16(&frame[pos]) / 100.0); // A
      pos += 2;
      this->publish_sensor_(CONF_INV_SOLAR_POWER_PREFIX + std::to_string(input), this->extract_uint16(&frame[pos])); // W
      pos += 2;
    }

    // AC power
    this->publish_sensor_(CONF_INV_AC_POWER, this->extract_uint16(&frame[pos])); // W
    pos += 2;

    // Bus Voltage
    this->publish_sensor_("bus_voltage_pos", this->extract_uint16(&frame[pos]) / 10.0); // V
    pos += 2;
    this->publish_sensor_("bus_voltage_neg", this->extract_uint16(&frame[pos]) / 10.0); // V
    pos += 2;

    // Supplied energy today
    this->publish_sensor_(CONF_INV_SUPPLIED_AC_ENERGY_TODAY, this->extract_uint32(&frame[pos])); // Wh
    pos += 4;

    // Inverter runtime today
    this->publish_sensor_(CONF_INV_RUNTIME_TODAY, this->extract_uint32(&frame[pos])); // seconds
    pos += 4;

    // Supplied energy total
    this->publish_sensor_(CONF_INV_SUPPLIED_AC_ENERGY_TOTAL, this->extract_uint32(&frame[pos])); // kWh
    pos += 4;

    // Inverter runtime total
    this->publish_sensor_(CONF_INV_RUNTIME_TOTAL, this->extract_uint32(&frame[pos])); // seconds
    pos += 4;

    // Inverter module rack temp
    this->publish_sensor_("inverter_module_rack_temp", this->extract_int16(&frame[pos])); // C, signed
    pos += 2;

    // Status fields
    for (int i = 1; i <= 4; ++i) {
      this->publish_sensor_("status_ac_output_" + std::to_string(i), frame[pos++]);
    }

    for (int i = 1; i <= 2; ++i) {
      this->publish_sensor_("status_dc_input_" + std::to_string(i), frame[pos++]);
    }

    // Error Statuses/Errors/etc
    this->publish_sensor_("error_status", frame[pos++]);
    this->publish_sensor_("error_status_ac_1", frame[pos++]);
    this->publish_sensor_("global_error_1", frame[pos++]);
    this->publish_sensor_("cpu_error", frame[pos++]);
    this->publish_sensor_("global_error_2", frame[pos++]);
    this->publish_sensor_("limits_ac_output_1", frame[pos++]);
    this->publish_sensor_("limits_ac_output_2", frame[pos++]);
    this->publish_sensor_("global_error_3", frame[pos++]);
    this->publish_sensor_("limits_dc_1", frame[pos++]);
    this->publish_sensor_("limits_dc_2", frame[pos++]);

    // TODO: 20 bytes of history status messages
  }
public:
  FrameParserVariant27() {}

  static bool supports(uint8_t variant) {
    return (
      variant == 27 || variant == 28 || variant == 43 || variant == 44 ||
      variant == 89 || variant == 90 || variant == 91 || variant == 93 ||
      variant == 95 || (variant >= 200 && variant <= 206)
    );
  }
};

}
}
