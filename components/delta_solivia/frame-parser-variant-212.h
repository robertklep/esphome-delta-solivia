#pragma once

#include "frame-parser-interface.h"

namespace esphome {
namespace delta_solivia {

class FrameParserVariant212 : public IFrameParser {
  void parse_frame_(const uint8_t* frame, std::size_t pos) override {
      // SAP part number
      publish_text_sensor_(CONF_INV_PART_NUMBER, parse_string(frame, pos, 11));
      pos += 11;

      // SAP serial number
      publish_text_sensor_(CONF_INV_SERIAL_NUMBER, parse_string(frame, pos, 13));
      pos += 13;

      // SAP date code
      publish_sensor_("SAP_date_code", extract_int32(&frame[pos]));
      pos += 4;

      // SAP revision
      publish_sensor_("SAP_revision", extract_int16(&frame[pos]));
      pos += 2;

      // DSP FW Rev (2 bytes, scaled by 1)
      publish_sensor_("DSP_fw_rev", extract_int16(&frame[pos])); // MA.MI
      pos += 2;

      // DSP FW Date (2 bytes, scaled by 1)
      publish_sensor_("DSP_fw_date", extract_int16(&frame[pos])); // MA.MMDD
      pos += 2;

      // Redundant MCU FW Rev (2 bytes, scaled by 1)
      publish_sensor_("redundant_mcu_fw_rev", extract_int16(&frame[pos])); // MA.MI
      pos += 2;

      // Redundant MCU FW Date (2 bytes, scaled by 1)
      publish_sensor_("redundant_mcu_fw_date", extract_int16(&frame[pos])); // MA.MMDD
      pos += 2;

      // Display MCU FW Rev (2 bytes, scaled by 1)
      publish_sensor_("display_mcu_fw_rev", extract_int16(&frame[pos])); // MA.MI
      pos += 2;

      // Display MCU FW Date (2 bytes, scaled by 1)
      publish_sensor_("display_mcu_fw_date", extract_int16(&frame[pos])); // MA.MMDD
      pos += 2;

      // Display WebPage Ctrl FW Rev (2 bytes, scaled by 1)
      publish_sensor_("display_webpage_ctrl_fw_rev", extract_int16(&frame[pos])); // MA.MI
      pos += 2;

      // Display WebPage Ctrl FW Date (2 bytes, scaled by 1)
      publish_sensor_("display_webpage_ctrl_fw_date", extract_int16(&frame[pos])); // MA.MMDD
      pos += 2;

      // Display WiFi Ctrl FW Rev (2 bytes, scaled by 1)
      publish_sensor_("display_wifi_ctrl_fw_rev", extract_int16(&frame[pos])); // MA.MI
      pos += 2;

      // Display WebPage Ctrl FW Date (2 bytes, scaled by 1)
      publish_sensor_("display_wifi_ctrl_fw_date", extract_int16(&frame[pos])); // MA.MMDD
      pos += 2;

      // AC Voltage/Current/Power/Frequency (all phases)
      for (int phase = 1; phase <= 3; ++phase) {
          publish_sensor_(CONF_INV_AC_VOLTAGE_PHASE_PREFIX + std::to_string(phase), extract_int16(&frame[pos]) / 10.0); // V
          pos += 2;
          publish_sensor_(CONF_INV_AC_CURRENT_PHASE_PREFIX + std::to_string(phase), extract_int16(&frame[pos]) / 100.0); // A
          pos += 2;
          publish_sensor_(CONF_INV_AC_POWER_PHASE_PREFIX + std::to_string(phase), extract_int16(&frame[pos])); // W
          pos += 2;
          publish_sensor_(CONF_INV_AC_FREQ_PHASE_PREFIX + std::to_string(phase), extract_int16(&frame[pos]) / 100.0); // Hz
          pos += 2;
          publish_sensor_("ac_voltage_redundant_phase_" + std::to_string(phase), extract_int16(&frame[pos]) / 10.0); // V
          pos += 2;
          publish_sensor_("ac_frequency_redundant_phase_" + std::to_string(phase), extract_int16(&frame[pos]) / 100.0); // Hz
          pos += 2;
      }

      // Solar Voltage/Current/Power (all inputs)
      for (int input = 1; input <= 2; ++input) {
          publish_sensor_(CONF_INV_SOLAR_VOLTAGE_PREFIX + std::to_string(input), extract_int16(&frame[pos]) / 10.0); // V
          pos += 2;
          publish_sensor_(CONF_INV_SOLAR_CURRENT_PREFIX + std::to_string(input), extract_int16(&frame[pos]) / 100.0); // A
          pos += 2;
          publish_sensor_(CONF_INV_SOLAR_POWER_PREFIX + std::to_string(input), extract_int16(&frame[pos])); // W
          pos += 2;
      }

      // AC power
      publish_sensor_(CONF_INV_AC_POWER, extract_int16(&frame[pos])); // W
      pos += 2;

      // Bus Voltage
      publish_sensor_("bus_voltage_pos", extract_int16(&frame[pos]) / 10.0); // V
      pos += 2;
      publish_sensor_("bus_voltage_neg", extract_int16(&frame[pos]) / 10.0); // V
      pos += 2;

      // Supplied energy today
      publish_sensor_(CONF_INV_SUPPLIED_AC_ENERGY_TODAY, extract_int32(&frame[pos])); // Wh
      pos += 4;

      // Inverter runtime today
      publish_sensor_(CONF_INV_RUNTIME_TODAY, extract_int32(&frame[pos])); // seconds
      pos += 4;

      // Supplied energy total
      publish_sensor_(CONF_INV_SUPPLIED_AC_ENERGY_TOTAL, extract_int32(&frame[pos])); // kWh
      pos += 4;

      // Inverter runtime total
      publish_sensor_(CONF_INV_RUNTIME_TOTAL, extract_int32(&frame[pos])); // seconds
      pos += 4;

      // Inverter module rack temp
      publish_sensor_("inverter_module_rack_temp", extract_int16(&frame[pos])); // C
      pos += 2;

      // Status fields
      for (int i = 1; i <= 4; ++i) {
        publish_sensor_("status_ac_output_" + std::to_string(i), frame[pos++]);
      }

      for (int i = 1; i <= 2; ++i) {
        publish_sensor_("status_dc_input_" + std::to_string(i), frame[pos++]);
      }

      // Error Statuses/Errors/etc
      publish_sensor_("error_status", frame[pos++]);
      publish_sensor_("error_status_ac_1", frame[pos++]);
      publish_sensor_("global_error_1", frame[pos++]);
      publish_sensor_("cpu_error", frame[pos++]);
      publish_sensor_("global_error_2", frame[pos++]);
      publish_sensor_("limits_ac_output_1", frame[pos++]);
      publish_sensor_("limits_ac_output_2", frame[pos++]);
      publish_sensor_("global_error_3", frame[pos++]);
      publish_sensor_("limits_dc_1", frame[pos++]);
      publish_sensor_("limits_dc_2", frame[pos++]);

      // TODO: 20 bytes of history status messages
  }
};

}
}
