#pragma once

#include "frame-parser-interface.h"

namespace esphome {
namespace delta_solivia {

// page 25
class FrameParserVariant15 : public IFrameParser {
protected:
  void parse_frame_(const uint8_t* frame, std::size_t pos) override {
    // XXX: sensor names should match `CONF_INV_*` values in __init__.py
    this->publish_text_sensor_(CONF_INV_PART_NUMBER, this->parse_string(frame, pos, 11));
    pos += 11;
    this->publish_text_sensor_(CONF_INV_SERIAL_NUMBER, this->parse_string(frame, pos, 18));
    pos += 18;
    this->publish_sensor_("SAP_date_code", this->extract_uint32(&frame[pos])); // Date Code
    pos += 4;
    this->publish_sensor_("SAP_revision", this->extract_uint16(&frame[pos])); // Date Code
    pos += 2;

    // software revisions
    this->publish_sensor_("Software_rev_ac_major", frame[pos++]); // Major AC revision
    this->publish_sensor_("Software_rev_ac_minor", frame[pos++]); // Minor AC revision
    this->publish_sensor_("Software_rev_ac_bugfix", frame[pos++]); // AC bugfix
    this->publish_sensor_("Software_rev_dc_major", frame[pos++]); // Major DC revision
    this->publish_sensor_("Software_rev_dc_minor", frame[pos++]); // Minor DC revision
    this->publish_sensor_("Software_rev_dc_bugfix", frame[pos++]); // DC bugfix
    this->publish_sensor_("Software_rev_display_major", frame[pos++]); // Major Display revision
    this->publish_sensor_("Software_rev_display_minor", frame[pos++]); // Minor Display revision
    this->publish_sensor_("Software_rev_display_bugfix", frame[pos++]); // Display bugfix
    this->publish_sensor_("Software_rev_sc_major", frame[pos++]); // Major SC revision
    this->publish_sensor_("Software_rev_sc_minor", frame[pos++]); // Minor SC revision
    this->publish_sensor_("Software_rev_sc_bugfix", frame[pos++]); // SC bugfix

    // solar
    this->publish_sensor_(CONF_INV_SOLAR_VOLTAGE_PREFIX + std::to_string(1), this->extract_uint16(&frame[pos])); // Volts
    pos += 2;
    this->publish_sensor_(CONF_INV_SOLAR_CURRENT_PREFIX + std::to_string(1), this->extract_uint16(&frame[pos]) * 0.1); // Amps
    pos += 2;
    this->publish_sensor_(CONF_INV_SOLAR_ISO_RES_INPUT_1, this->extract_uint16(&frame[pos]) * 1000); // kOhms
    pos += 2;
    this->publish_sensor_(CONF_INV_TEMPERATURE_NTC_DC, this->extract_int16(&frame[pos])); // Celsius, signed
    pos += 2;
    this->publish_sensor_(CONF_INV_SOLAR_INPUT_MOV_RES, this->extract_uint16(&frame[pos]) * 1000); // kOhms
    pos += 2;

    // AC
    this->publish_sensor_(CONF_INV_AC_CURRENT, this->extract_uint16(&frame[pos]) * 0.1); // Amps
    pos += 2;
    this->publish_sensor_(CONF_INV_AC_VOLTAGE, this->extract_uint16(&frame[pos])); // Volts
    pos += 2;
    this->publish_sensor_(CONF_INV_AC_POWER, this->extract_uint16(&frame[pos])); // Watts
    pos += 2;
    this->publish_sensor_(CONF_INV_AC_FREQ, this->extract_uint16(&frame[pos]) * 0.01); // Hertz
    pos += 2;
    this->publish_sensor_(CONF_INV_TEMPERATURE_NTC_AC, this->extract_int16(&frame[pos])); // Celsius, signed
    pos += 2;

    // SC grid
    this->publish_sensor_(CONF_INV_SC_GRID_VOLTAGE, this->extract_uint16(&frame[pos]) * 0.01); // Volts
    pos += 2;
    this->publish_sensor_(CONF_INV_SC_GRID_FREQUENCY, this->extract_uint16(&frame[pos]) * 0.01); // Hertz
    pos += 2;
    this->publish_sensor_(CONF_INV_SC_GRID_DC_INJ_CURRENT, this->extract_uint16(&frame[pos]) * 0.01); // Amps
    pos += 2;

    // AC grid
    this->publish_sensor_(CONF_INV_AC_GRID_VOLTAGE, this->extract_uint16(&frame[pos]) * 0.01); // Volts
    pos += 2;
    this->publish_sensor_(CONF_INV_AC_GRID_FREQ, this->extract_uint16(&frame[pos]) * 0.01); // Hertz
    pos += 2;
    this->publish_sensor_(CONF_INV_AC_GRID_DC_INJ_CURRENT, this->extract_uint16(&frame[pos]) * 0.01); // Amps
    pos += 2;

    // energy info
    this->publish_sensor_(CONF_INV_SUPPLIED_AC_ENERGY_TODAY, this->extract_uint16(&frame[pos]) * 10.0); // Wh
    pos += 2;
    this->publish_sensor_(CONF_INV_RUNTIME_TODAY, this->extract_uint16(&frame[pos]) * 60.0); // Minutes
    pos += 2;
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
    this->publish_sensor_(CONF_INV_SUPPLIED_AC_ENERGY_TOTAL, this->extract_uint32(&frame[pos]) * 0.1); // kWh
    pos += 4;
    this->publish_sensor_(CONF_INV_RUNTIME_TOTAL, this->extract_uint32(&frame[pos]) * 3600.0); // Hours
    pos += 4;

    // minimums/maximums
    this->publish_sensor_(CONF_INV_MAX_SOLAR_CURRENT_INPUT_1, this->extract_uint16(&frame[pos]) * 0.1); // Amps
    pos += 2;
    this->publish_sensor_(CONF_INV_MAX_SOLAR_VOLTAGE_INPUT_1, this->extract_uint16(&frame[pos])); // Volts
    pos += 2;
    this->publish_sensor_(CONF_INV_MAX_SOLAR_POWER_INPUT_1, this->extract_uint16(&frame[pos])); // Watts
    pos += 2;
    this->publish_sensor_(CONF_INV_MIN_SOLAR_ISO_RES_INPUT_1, this->extract_uint16(&frame[pos]) * 1000.0); // kOhms
    pos += 2;
    this->publish_sensor_(CONF_INV_MAX_SOLAR_ISO_RES_INPUT_1, this->extract_uint16(&frame[pos]) * 1000); // kOhms
    pos += 2;

    // statuses, limits, failures, errors, etc
    this->parse_alarms_status(frame[pos++]);
    this->parse_status_dc_input(frame[pos++]);
    this->parse_limits_dc_input(frame[pos++]);
    this->parse_status_ac_output(frame[pos++]);
    this->parse_limits_ac_output(frame[pos++]);
    this->parse_warnings_status(frame[pos++]);
    this->parse_dc_hardware_failure(frame[pos++]);
    this->parse_ac_hardware_failure(frame[pos++]);
    this->parse_sc_hardware_failure(frame[pos++]);
    this->parse_internal_bulk_failure(frame[pos++]);
    this->parse_internal_communications_failure(frame[pos++]);
    this->parse_ac_hardware_disturbance(frame[pos++]);
    this->parse_dc_hw_stage_error(frame[pos++]);
    this->parse_calibration_status(frame[pos++]);
    this->parse_neutral_error(frame[pos++]);
    this->parse_neutral_error(frame[pos++]);

    // TODO: history status messages
  }

  void parse_alarms_status(uint8_t status) {
    std::string status_text = "";
    if (status & 1<<0) status_text += "GFDI test failure\n";
    if (status & 1<<1) status_text += "Fuse- fault\n";
    if (status & 1<<2) status_text += "Fuse+ fault\n";
    if (status & 1<<3) status_text += "GFDI hardware error\n";
    if (status & 1<<4) status_text += "#4\n";
    if (status & 1<<5) status_text += "Running isolation failure\n";
    if (status & 1<<6) status_text += "#6\n";
    if (status & 1<<7) status_text += "Startup isolation failure\n";
    if (status_text.size()) {
      this->publish_text_sensor_(CONF_INV_ALARMS_STATUS, status_text);
    }
  }

  void parse_status_dc_input(uint8_t status) {
    std::string status_text = "";
    if (status & 1<<0) status_text += "Limits DC\n";
    if (status & 1<<1) status_text += "MPP limited operation\n";
    if (status & 1<<2) status_text += "Power limited operation\n";
    if (status & 1<<3) status_text += "Temperature limited operation\n";
    if (status & 1<<4) status_text += "Input power low\n";
    if (status & 1<<5) status_text += "Internal bulk failure\n";
    if (status & 1<<6) status_text += "Internal communication failure\n";
    if (status & 1<<7) status_text += "Internal DC hardware failure\n";
    if (status_text.size()) {
      this->publish_text_sensor_(CONF_INV_DC_INPUT_STATUS, status_text);
    }
  }

  void parse_limits_dc_input(uint8_t status) {
    std::string status_text = "";
    if (status & 1<<0) status_text += "#0\n";
    if (status & 1<<1) status_text += "#1\n";
    if (status & 1<<2) status_text += "#2\n";
    if (status & 1<<3) status_text += "#3\n";
    if (status & 1<<4) status_text += "#4\n";
    if (status & 1<<5) status_text += "#5\n";
    if (status & 1<<6) status_text += "Input critical undervoltage\n";
    if (status & 1<<7) status_text += "Input voltage too low\n";
    if (status_text.size()) {
      this->publish_text_sensor_(CONF_INV_DC_INPUT_LIMITS, status_text);
    }
  }

  // XXX: documentation isn't clear about AC output status;
  //      we'll assume it's the "grid errors" table
  void parse_status_ac_output(uint8_t status) {
    std::string status_text = "";
    if (status & 1<<0) status_text += "DC injection current failure\n";
    if (status & 1<<1) status_text += "Islanding detected\n";
    if (status & 1<<2) status_text += "Frequency error (high)\n";
    if (status & 1<<3) status_text += "Frequency error (low)\n";
    if (status & 1<<4) status_text += "Critical overvoltage error\n";
    if (status & 1<<5) status_text += "Overvoltage error\n";
    if (status & 1<<6) status_text += "Undervoltage error\n";
    if (status & 1<<7) status_text += "Critical undervoltage error\n";
    if (status_text.size()) {
      this->publish_text_sensor_(CONF_INV_AC_OUTPUT_STATUS, status_text);
    }
  }

  void parse_limits_ac_output(uint8_t status) {
    std::string status_text = "";
    // AC limits are undocumented
    if (status & 1<<0) status_text += "#0\n";
    if (status & 1<<1) status_text += "#1\n";
    if (status & 1<<2) status_text += "#2\n";
    if (status & 1<<3) status_text += "#3\n";
    if (status & 1<<4) status_text += "#4\n";
    if (status & 1<<5) status_text += "#5\n";
    if (status & 1<<6) status_text += "#6\n";
    if (status & 1<<7) status_text += "#7\n";
    if (status_text.size()) {
      this->publish_text_sensor_(CONF_INV_AC_OUTPUT_LIMITS, status_text);
    }
  }

  void parse_warnings_status(uint8_t status) {
    std::string status_text = "";
    if (status & 1<<0) status_text += "#0\n";
    if (status & 1<<1) status_text += "Varistor warning\n";
    if (status & 1<<2) status_text += "Warnings Status #2\n";
    if (status & 1<<3) status_text += "PV- grounding fault\n";
    if (status & 1<<4) status_text += "Running isolation status\n";
    if (status & 1<<5) status_text += "PV+ grounding fault\n";
    if (status & 1<<6) status_text += "Startup isolation\n";
    if (status & 1<<7) status_text += "#7\n";
    if (status_text.size()) {
      this->publish_text_sensor_(CONF_INV_WARNINGS, status_text);
    }
  }

  void parse_dc_hardware_failure(uint8_t status) {
    std::string status_text = "";
    if (status & 1<<0) status_text += "Bridge error\n";
    if (status & 1<<1) status_text += "Boost error\n";
    if (status & 1<<2) status_text += "#2\n";
    if (status & 1<<3) status_text += "#3\n";
    if (status & 1<<4) status_text += "#4\n";
    if (status & 1<<5) status_text += "SC OK error\n";
    if (status & 1<<6) status_text += "NTC overtemperature\n";
    if (status & 1<<7) status_text += "NTC defect\n";
    if (status_text.size()) {
      this->publish_text_sensor_(CONF_INV_DC_HARDWARE_FAILURES, status_text);
    }
  }

  void parse_ac_hardware_failure(uint8_t status) {
    std::string status_text = "";
    if (status & 1<<0) status_text += "Bulk undervoltage\n";
    if (status & 1<<1) status_text += "Bulk overvoltage\n";
    if (status & 1<<2) status_text += "EEPROM corrupt\n";
    if (status & 1<<3) status_text += "Overcurrent shutdown\n";
    if (status & 1<<4) status_text += "AUX voltage error\n";
    if (status & 1<<5) status_text += "HW bulk undervoltage\n";
    if (status & 1<<6) status_text += "NTC overtemperature\n";
    if (status & 1<<7) status_text += "NTC defect\n";
    if (status_text.size()) {
      this->publish_text_sensor_(CONF_INV_AC_HARDWARE_FAILURES, status_text);
    }
  }

  void parse_sc_hardware_failure(uint8_t status) {
    std::string status_text = "";
    if (status & 1<<0) status_text += "#0\n";
    if (status & 1<<1) status_text += "#1\n";
    if (status & 1<<2) status_text += "#2\n";
    if (status & 1<<3) status_text += "#3\n";
    if (status & 1<<4) status_text += "#4\n";
    if (status & 1<<5) status_text += "#5\n";
    if (status & 1<<6) status_text += "#6\n";
    if (status & 1<<7) status_text += "AC_OK_ERROR\n";
    if (status_text.size()) {
      this->publish_text_sensor_(CONF_INV_SC_HARDWARE_FAILURES, status_text);
    }
  }

  void parse_internal_bulk_failure(uint8_t status) {
    std::string status_text = "";
    if (status & 1<<0) status_text += "#0\n";
    if (status & 1<<1) status_text += "#1\n";
    if (status & 1<<2) status_text += "#2\n";
    if (status & 1<<3) status_text += "#3\n";
    if (status & 1<<4) status_text += "#4\n";
    if (status & 1<<5) status_text += "#5\n";
    if (status & 1<<6) status_text += "Overvoltage\n";
    if (status & 1<<7) status_text += "Undervoltage\n";
    if (status_text.size()) {
      this->publish_text_sensor_(CONF_INV_BULK_FAILURES, status_text);
    }
  }

  void parse_internal_communications_failure(uint8_t status) {
    std::string status_text = "";
    if (status & 1<<0) status_text += "#0\n";
    if (status & 1<<1) status_text += "LCD error\n";
    if (status & 1<<2) status_text += "AC error\n";
    if (status & 1<<3) status_text += "DC error\n";
    if (status & 1<<4) status_text += "LCD failure\n";
    if (status & 1<<5) status_text += "#5\n";
    if (status & 1<<6) status_text += "AC failure\n";
    if (status & 1<<7) status_text += "DC failure\n";
    if (status_text.size()) {
      this->publish_text_sensor_(CONF_INV_COMMS_FAILURES, status_text);
    }
  }

  void parse_ac_hardware_disturbance(uint8_t status) {
    std::string status_text = "";
    if (status & 1<<0) status_text += "Ext power control active\n";
    if (status & 1<<1) status_text += "IT autotest fail\n";
    if (status & 1<<2) status_text += "#2\n";
    if (status & 1<<3) status_text += "#3\n";
    if (status & 1<<4) status_text += "#4\n";
    if (status & 1<<5) status_text += "#5\n";
    if (status & 1<<6) status_text += "AC relay error\n";
    if (status & 1<<7) status_text += "RTC error\n";
    if (status_text.size()) {
      this->publish_text_sensor_(CONF_INV_AC_HARDWARE_DISTURBANCE, status_text);
    }
  }

  void parse_dc_hw_stage_error(uint8_t status) {
    std::string status_text = "";
    if (status & 1<<0) status_text += "Overvoltage\n";
    if (status & 1<<1) status_text += "AUX voltage error\n";
    if (status & 1<<2) status_text += "Current protection\n";
    if (status & 1<<3) status_text += "Overvoltage 2\n";
    if (status & 1<<4) status_text += "#4\n";
    if (status & 1<<5) status_text += "#5\n";
    if (status & 1<<6) status_text += "#6\n";
    if (status & 1<<7) status_text += "#7\n";
    if (status_text.size()) {
      this->publish_text_sensor_(CONF_INV_DC_HARDWARE_STAGE_ERRORS, status_text);
    }
  }

  void parse_calibration_status(uint8_t status) {
    std::string status_text = "";
    if (status & 1<<0) status_text += "LEM calibration error\n";
    if (status & 1<<1) status_text += "AC calibration error\n";
    if (status & 1<<2) status_text += "SC calibration error\n";
    if (status & 1<<3) status_text += "#3\n";
    if (status & 1<<4) status_text += "#4\n";
    if (status & 1<<5) status_text += "#5\n";
    if (status & 1<<6) status_text += "#6\n";
    if (status & 1<<7) status_text += "#7\n";
    if (status_text.size()) {
      this->publish_text_sensor_(CONF_INV_CALIBRATION_STATUS, status_text);
    }
  }

  void parse_neutral_error(uint8_t status) {
    std::string status_text = "";
    if (status & 1<<0) status_text += "Critical overvoltage N2\n";
    if (status & 1<<1) status_text += "Overvoltage N2\n";
    if (status & 1<<2) status_text += "Undervoltage N2\n";
    if (status & 1<<3) status_text += "Critical undervoltage N2\n";
    if (status & 1<<4) status_text += "Critical overvoltage N1\n";
    if (status & 1<<5) status_text += "Overvoltage N1\n";
    if (status & 1<<6) status_text += "Undervoltage N1\n";
    if (status & 1<<7) status_text += "Critical undervoltage N1\n";
    if (status_text.size()) {
      this->publish_text_sensor_(CONF_INV_NEUTRAL_ERRORS, status_text);
    }
  }
public:
  FrameParserVariant15() {}
};

}
}
