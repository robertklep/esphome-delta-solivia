#pragma once

#include "frame-parser-interface.h"

namespace esphome {
namespace delta_solivia {

// page 25
class FrameParserVariant15 : public IFrameParser {
  void parse_frame_(const uint8_t* frame, std::size_t pos) override {
    // XXX: sensor names should match `CONF_INV_*` values in __init__.py
    publish_text_sensor_(CONF_INV_PART_NUMBER, parse_string(frame, pos, 11));
    pos += 11;
    publish_text_sensor_(CONF_INV_SERIAL_NUMBER, parse_string(frame, pos, 18));
    pos += 18;
    publish_sensor_("SAP_date_code", extract_int32(&frame[pos])); // Date Code);
    pos += 4;
    publish_sensor_("SAP_revision", extract_int16(&frame[pos])); // Date Code);
    pos += 2;

    // software revisions
    publish_sensor_("Software_rev_ac_major", frame[pos++]); // Major AC revision
    publish_sensor_("Software_rev_ac_minor", frame[pos++]); // Minor AC revision
    publish_sensor_("Software_rev_ac_bugfix", frame[pos++]); // AC bugfix
    publish_sensor_("Software_rev_dc_major", frame[pos++]); // Major DC revision
    publish_sensor_("Software_rev_dc_minor", frame[pos++]); // Minor DC revision
    publish_sensor_("Software_rev_dc_bugfix", frame[pos++]); // DC bugfix
    publish_sensor_("Software_rev_display_major", frame[pos++]); // Major Display revision
    publish_sensor_("Software_rev_display_minor", frame[pos++]); // Minor Display revision
    publish_sensor_("Software_rev_display_bugfix", frame[pos++]); // Display bugfix
    publish_sensor_("Software_rev_sc_major", frame[pos++]); // Major SC revision
    publish_sensor_("Software_rev_sc_minor", frame[pos++]); // Minor SC revision
    publish_sensor_("Software_rev_sc_bugfix", frame[pos++]); // SC bugfix

    // solar
    publish_sensor_(CONF_INV_SOLAR_VOLTAGE_PREFIX + std::to_string(1), extract_int16(&frame[pos])); // Volts
    pos += 2;
    publish_sensor_(CONF_INV_SOLAR_CURRENT_PREFIX + std::to_string(1), extract_int16(&frame[pos]) * 0.1); // Amps
    pos += 2;
    publish_sensor_(CONF_INV_SOLAR_ISO_RES_INPUT_1, extract_int16(&frame[pos]) * 1000); // kOhms
    pos += 2;
    publish_sensor_(CONF_INV_TEMPERATURE_NTC_DC, extract_int16(&frame[pos])); // Celsius
    pos += 2;
    publish_sensor_(CONF_INV_SOLAR_INPUT_MOV_RES, extract_int16(&frame[pos]) * 1000); // kOhms
    pos += 2;

    // AC
    publish_sensor_(CONF_INV_AC_CURRENT, extract_int16(&frame[pos]) * 0.1); // Amps
    pos += 2;
    publish_sensor_(CONF_INV_AC_VOLTAGE, extract_int16(&frame[pos])); // Volts
    pos += 2;
    publish_sensor_(CONF_INV_AC_POWER, extract_int16(&frame[pos])); // Watts
    pos += 2;
    publish_sensor_(CONF_INV_AC_FREQ, extract_int16(&frame[pos]) * 0.01); // Hertz
    pos += 2;
    publish_sensor_(CONF_INV_TEMPERATURE_NTC_AC, extract_int16(&frame[pos])); // Celsius
    pos += 2;

    // SC grid
    publish_sensor_(CONF_INV_SC_GRID_VOLTAGE, extract_int16(&frame[pos]) * 0.01); // Volts
    pos += 2;
    publish_sensor_(CONF_INV_SC_GRID_FREQUENCY, extract_int16(&frame[pos]) * 0.01); // Hertz
    pos += 2;
    publish_sensor_(CONF_INV_SC_GRID_DC_INJ_CURRENT, extract_int16(&frame[pos]) * 0.01); // Amps
    pos += 2;

    // AC grid
    publish_sensor_(CONF_INV_AC_GRID_VOLTAGE, extract_int16(&frame[pos]) * 0.01); // Volts
    pos += 2;
    publish_sensor_(CONF_INV_AC_GRID_FREQ, extract_int16(&frame[pos]) * 0.01); // Hertz
    pos += 2;
    publish_sensor_(CONF_INV_AC_GRID_DC_INJ_CURRENT, extract_int16(&frame[pos]) * 0.01); // Amps
    pos += 2;

    // energy info
    publish_sensor_(CONF_INV_SUPPLIED_AC_ENERGY_TODAY, extract_int16(&frame[pos]) * 10.0); // Wh
    pos += 2;
    publish_sensor_(CONF_INV_RUNTIME_TODAY, extract_int16(&frame[pos]) * 60.0); // Minutes
    pos += 2;
    publish_sensor_(CONF_INV_MAX_AC_CURRENT_TODAY, extract_int16(&frame[pos]) * 0.1); // Amps
    pos += 2;
    publish_sensor_(CONF_INV_MIN_AC_VOLTAGE_TODAY, extract_int16(&frame[pos])); // Volts
    pos += 2;
    publish_sensor_(CONF_INV_MAX_AC_VOLTAGE_TODAY, extract_int16(&frame[pos])); // Volts
    pos += 2;
    publish_sensor_(CONF_INV_MAX_AC_POWER_TODAY, extract_int16(&frame[pos])); // Watts
    pos += 2;
    publish_sensor_(CONF_INV_MIN_AC_FREQ_TODAY, extract_int16(&frame[pos]) * 0.01); // Hertz
    pos += 2;
    publish_sensor_(CONF_INV_MAX_AC_FREQ_TODAY, extract_int16(&frame[pos]) * 0.01); // Hertz
    pos += 2;
    publish_sensor_(CONF_INV_SUPPLIED_AC_ENERGY_TOTAL, extract_int32(&frame[pos]) * 0.1); // kWh
    pos += 4;
    publish_sensor_(CONF_INV_RUNTIME_TOTAL, extract_int32(&frame[pos]) * 3600.0); // Hours
    pos += 4;

    // minimums/maximums
    publish_sensor_(CONF_INV_MAX_SOLAR_CURRENT_INPUT_1, extract_int16(&frame[pos]) * 0.1); // Amps
    pos += 2;
    publish_sensor_(CONF_INV_MAX_SOLAR_VOLTAGE_INPUT_1, extract_int16(&frame[pos])); // Volts
    pos += 2;
    publish_sensor_(CONF_INV_MAX_SOLAR_POWER_INPUT_1, extract_int16(&frame[pos])); // Watts
    pos += 2;
    publish_sensor_(CONF_INV_MIN_SOLAR_ISO_RES_INPUT_1, extract_int16(&frame[pos]) * 1000.0); // kOhms
    pos += 2;
    publish_sensor_(CONF_INV_MAX_SOLAR_ISO_RES_INPUT_1, extract_int16(&frame[pos]) * 1000); // kOhms
    pos += 2;

    // limits and status
    publish_sensor_("Alarms_status", frame[pos++]); // Alarms status
    publish_sensor_("Status_dc_input", frame[pos++]); // Status DC input
    publish_sensor_("Limits_dc_input", frame[pos++]); // Limits DC input
    publish_sensor_("Status_ac_output", frame[pos++]); // Status AC output
    publish_sensor_("Limits_ac_output", frame[pos++]); // Limits AC output
    publish_sensor_("Warnings_status", frame[pos++]); // Warnings status
    publish_sensor_("DC_hardware_failure", frame[pos++]); // DC hardware failure
    publish_sensor_("AC_hardware_failure", frame[pos++]); // AC hardware failure
  }
public:
  FrameParserVariant15() {}
};

}
}
