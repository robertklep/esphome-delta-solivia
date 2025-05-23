#pragma once

namespace esphome {
namespace delta_solivia {

// these should match the values in __init__.py
static const char* CONF_INV_PART_NUMBER = "part_number";
static const char* CONF_INV_SERIAL_NUMBER = "serial_number";

static const char* CONF_INV_SOLAR_VOLTAGE_PREFIX = "solar_voltage_input_";
static const char* CONF_INV_SOLAR_CURRENT_PREFIX = "solar_current_input_";
static const char* CONF_INV_SOLAR_POWER_PREFIX = "solar_power_input_";

static const char* CONF_INV_SOLAR_ISO_RES_INPUT_1 = "solar_isolation_resistance_input_1";
static const char* CONF_INV_SOLAR_INPUT_MOV_RES = "solar_input_mov_resistance";

static const char* CONF_INV_TEMPERATURE_NTC_DC = "temperature_ntc_dc";
static const char* CONF_INV_TEMPERATURE_NTC_AC = "temperature_ntc_ac";

static const char* CONF_INV_AC_CURRENT = "ac_current";
static const char* CONF_INV_AC_VOLTAGE = "ac_voltage";
static const char* CONF_INV_AC_POWER = "ac_power";
static const char* CONF_INV_AC_FREQ = "ac_frequency";

static const char* CONF_INV_AC_CURRENT_PHASE_PREFIX = "ac_current_phase_";
static const char* CONF_INV_AC_VOLTAGE_PHASE_PREFIX = "ac_voltage_phase_";
static const char* CONF_INV_AC_POWER_PHASE_PREFIX = "ac_power_phase_";
static const char* CONF_INV_AC_FREQ_PHASE_PREFIX = "ac_frequency_phase_";
static const char* CONF_INV_AC_REACTIVE_POWER_PHASE_PREFIX = "ac_reactive_power_phase_";

static const char* CONF_INV_SC_GRID_VOLTAGE = "sc_grid_voltage";
static const char* CONF_INV_SC_GRID_FREQUENCY = "sc_grid_frequency";
static const char* CONF_INV_SC_GRID_DC_INJ_CURRENT = "sc_grid_dc_injection_current";

static const char* CONF_INV_AC_GRID_VOLTAGE = "ac_grid_voltage";
static const char* CONF_INV_AC_GRID_FREQ = "ac_grid_frequency";
static const char* CONF_INV_AC_GRID_DC_INJ_CURRENT = "ac_grid_dc_injection_current";

static const char* CONF_INV_MAX_AC_CURRENT_TODAY = "max_ac_current_today";
static const char* CONF_INV_MIN_AC_VOLTAGE_TODAY = "min_ac_voltage_today";
static const char* CONF_INV_MAX_AC_VOLTAGE_TODAY = "max_ac_voltage_today";
static const char* CONF_INV_MAX_AC_POWER_TODAY = "max_ac_power_today";
static const char* CONF_INV_MIN_AC_FREQ_TODAY = "min_ac_frequency_today";
static const char* CONF_INV_MAX_AC_FREQ_TODAY = "max_ac_frequency_today";

static const char* CONF_INV_SUPPLIED_AC_ENERGY_TODAY = "supplied_ac_energy_today";
static const char* CONF_INV_SUPPLIED_AC_ENERGY_TOTAL = "supplied_ac_energy_total";

static const char* CONF_INV_RUNTIME_TOTAL = "runtime_total";
static const char* CONF_INV_RUNTIME_TODAY = "runtime_today";

static const char* CONF_INV_MAX_SOLAR_CURRENT_INPUT_1 = "max_solar_current_input_1";
static const char* CONF_INV_MAX_SOLAR_VOLTAGE_INPUT_1 = "max_solar_voltage_input_1";
static const char* CONF_INV_MAX_SOLAR_POWER_INPUT_1 = "max_solar_power_input_1";

static const char* CONF_INV_MIN_SOLAR_ISO_RES_INPUT_1 = "min_solar_isolation_resistance_input_1";
static const char* CONF_INV_MAX_SOLAR_ISO_RES_INPUT_1 = "max_solar_isolation_resistance_input_1";

static const char* CONF_INV_STATUS = "inverter_status";

static const char* CONF_INV_ALARMS_STATUS = "inverter_alarms_status";
static const char* CONF_INV_DC_INPUT_STATUS = "inverter_dc_input_status";
static const char* CONF_INV_DC_INPUT_LIMITS = "inverter_dc_input_limits";
static const char* CONF_INV_AC_OUTPUT_STATUS = "inverter_ac_output_status";
static const char* CONF_INV_AC_OUTPUT_LIMITS = "inverter_ac_output_limits";
static const char* CONF_INV_WARNINGS = "inverter_warnings";
static const char* CONF_INV_DC_HARDWARE_FAILURES = "inverter_dc_hardware_failures";
static const char* CONF_INV_AC_HARDWARE_FAILURES = "inverter_ac_hardware_failures";
static const char* CONF_INV_SC_HARDWARE_FAILURES = "inverter_sc_hardware_failures";
static const char* CONF_INV_BULK_FAILURES = "inverter_bulk_failures";
static const char* CONF_INV_COMMS_FAILURES = "inverter_comms_failures";
static const char* CONF_INV_AC_HARDWARE_DISTURBANCE = "inverter_ac_hardware_disturbance";
static const char* CONF_INV_DC_HARDWARE_STAGE_ERRORS = "inverter_dc_hardware_stage_errors";
static const char* CONF_INV_CALIBRATION_STATUS = "inverter_calibration_status";
static const char* CONF_INV_NEUTRAL_ERRORS = "inverter_neutral_errors";

static const char* CONF_INV_HISTORY_STATUS_MESSAGES= "inverter_history_status_messages";

}
}
