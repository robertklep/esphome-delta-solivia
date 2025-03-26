import logging
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.cpp_helpers import gpio_pin_expression
from esphome.components import uart, sensor, text_sensor
from esphome.const import (
    CONF_ID,
    CONF_UART_ID,
    CONF_UPDATE_INTERVAL,
    CONF_FLOW_CONTROL_PIN,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_ENERGY,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_VOLTAGE,
    DEVICE_CLASS_FREQUENCY,
    DEVICE_CLASS_DURATION,
    DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
    STATE_CLASS_TOTAL_INCREASING,
    UNIT_WATT,
    UNIT_WATT_HOURS,
    UNIT_KILOWATT,
    UNIT_KILOWATT_HOURS,
    UNIT_VOLT,
    UNIT_AMPERE,
    UNIT_OHM,
    UNIT_CELSIUS,
    UNIT_HERTZ,
    UNIT_SECOND,
)

LOGGER = logging.getLogger(__name__)

CODEOWNERS   = ["@robertklep"]
DEPENDENCIES = ["uart"]
AUTO_LOAD    = ["sensor", "text_sensor"]

delta_solivia_ns      = cg.esphome_ns.namespace("delta_solivia")
DeltaSoliviaComponent = delta_solivia_ns.class_("DeltaSoliviaComponent", uart.UARTDevice, cg.PollingComponent)
DeltaSoliviaInverter  = delta_solivia_ns.class_("DeltaSoliviaInverter")

# global config
CONF_INVERTERS   = "inverters"
CONF_HAS_GATEWAY = "has_gateway"

# per-inverter config
CONF_INV_ADDRESS  = "address"
CONF_INV_VARIANT  = "variant"
CONF_INV_THROTTLE = "throttle"

# per-inverter measurements
# these should match the values in parser-consts.h
CONF_INV_PART_NUMBER               = "part_number"
CONF_INV_SERIAL_NUMBER             = "serial_number"

CONF_INV_SOLAR_VOLTAGE_INPUT_1     = "solar_voltage_input_1"
CONF_INV_SOLAR_CURRENT_INPUT_1     = "solar_current_input_1"
CONF_INV_SOLAR_POWER_INPUT_1       = "solar_power_input_1"

CONF_INV_SOLAR_VOLTAGE_INPUT_2     = "solar_voltage_input_2"
CONF_INV_SOLAR_CURRENT_INPUT_2     = "solar_current_input_2"
CONF_INV_SOLAR_POWER_INPUT_2       = "solar_power_input_2"

CONF_INV_SOLAR_VOLTAGE_INPUT_3     = "solar_voltage_input_3"
CONF_INV_SOLAR_CURRENT_INPUT_3     = "solar_current_input_3"
CONF_INV_SOLAR_POWER_INPUT_3       = "solar_power_input_3"

CONF_INV_SOLAR_ISO_RES_INPUT_1     = "solar_isolation_resistance_input_1"
CONF_INV_SOLAR_INPUT_MOV_RES       = "solar_input_mov_resistance"

CONF_INV_TEMPERATURE_NTC_DC        = "temperature_ntc_dc"
CONF_INV_TEMPERATURE_NTC_AC        = "temperature_ntc_ac"

CONF_INV_AC_CURRENT                = "ac_current"
CONF_INV_AC_VOLTAGE                = "ac_voltage"
CONF_INV_AC_POWER                  = "ac_power"
CONF_INV_AC_FREQ                   = "ac_frequency"

CONF_INV_AC_CURRENT_PHASE_1        = "ac_current_phase_1"
CONF_INV_AC_VOLTAGE_PHASE_1        = "ac_voltage_phase_1"
CONF_INV_AC_POWER_PHASE_1          = "ac_power_phase_1"
CONF_INV_AC_FREQ_PHASE_1           = "ac_frequency_phase_1"

CONF_INV_AC_CURRENT_PHASE_2        = "ac_current_phase_2"
CONF_INV_AC_VOLTAGE_PHASE_2        = "ac_voltage_phase_2"
CONF_INV_AC_POWER_PHASE_2          = "ac_power_phase_2"
CONF_INV_AC_FREQ_PHASE_2           = "ac_frequency_phase_2"

CONF_INV_AC_CURRENT_PHASE_3        = "ac_current_phase_3"
CONF_INV_AC_VOLTAGE_PHASE_3        = "ac_voltage_phase_3"
CONF_INV_AC_POWER_PHASE_3          = "ac_power_phase_3"
CONF_INV_AC_FREQ_PHASE_3           = "ac_frequency_phase_3"

CONF_INV_SC_GRID_VOLTAGE           = "sc_grid_voltage"
CONF_INV_SC_GRID_FREQUENCY         = "sc_grid_frequency"
CONF_INV_SC_GRID_DC_INJ_CURRENT    = "sc_grid_dc_injection_current"

CONF_INV_AC_GRID_VOLTAGE           = "ac_grid_voltage"
CONF_INV_AC_GRID_FREQ              = "ac_grid_frequency"
CONF_INV_AC_GRID_DC_INJ_CURRENT    = "ac_grid_dc_injection_current"

CONF_INV_MAX_AC_CURRENT_TODAY      = "max_ac_current_today"
CONF_INV_MIN_AC_VOLTAGE_TODAY      = "min_ac_voltage_today"
CONF_INV_MAX_AC_VOLTAGE_TODAY      = "max_ac_voltage_today"
CONF_INV_MAX_AC_POWER_TODAY        = "max_ac_power_today"
CONF_INV_MIN_AC_FREQ_TODAY         = "min_ac_frequency_today"
CONF_INV_MAX_AC_FREQ_TODAY         = "max_ac_frequency_today"

CONF_INV_SUPPLIED_AC_ENERGY_TODAY  = "supplied_ac_energy_today"
CONF_INV_SUPPLIED_AC_ENERGY_TOTAL  = "supplied_ac_energy_total"

CONF_INV_RUNTIME_TOTAL             = "runtime_total"
CONF_INV_RUNTIME_TODAY             = "runtime_today"

CONF_INV_MAX_SOLAR_CURRENT_INPUT_1 = "max_solar_current_input_1"
CONF_INV_MAX_SOLAR_VOLTAGE_INPUT_1 = "max_solar_voltage_input_1"
CONF_INV_MAX_SOLAR_POWER_INPUT_1   = "max_solar_power_input_1"

CONF_INV_MIN_SOLAR_ISO_RES_INPUT_1 = "min_solar_isolation_resistance_input_1"
CONF_INV_MAX_SOLAR_ISO_RES_INPUT_1 = "max_solar_isolation_resistance_input_1"

# supported variants and their parser
SUPPORTED_VARIANTS = {
    ( 15, 18, 19, 20, 31, 34, 35, 36, 38, 39, 55, 58, 59, 60 ): 'Variant15Parser',
    ( 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222 ):  'Variant212Parser'
}

def _parser_for_variant(variant):
    for variants in SUPPORTED_VARIANTS:
        if variant in variants:
            return SUPPORTED_VARIANTS[variants]
    return None

def _validate_inverters(config):
    if len(config) < 1:
        raise cv.Invalid("Need at least one inverter to be configured")
    # ensure all configured inverters have a unique address
    addresses = { inverter.get(CONF_INV_ADDRESS) for inverter in config }
    if len(addresses) != len(config):
        raise cv.Invalid("Inverter addresses should be unique")
    # check if variants are supported
    variants = { inverter.get(CONF_INV_VARIANT) for inverter in config }
    for variant in variants:
        parser = _parser_for_variant(variant)
        if parser is None:
            raise cv.Invalid(f"Variant {variant} not supported")
    return config

INVERTER_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(DeltaSoliviaInverter),
    cv.Required(CONF_INV_ADDRESS): cv.int_range(min = 1),
    cv.Required(CONF_INV_VARIANT): cv.int_range(min = 1, max = 222),
    cv.Optional(CONF_INV_THROTTLE, default = '10s'): cv.update_interval,
    cv.Optional(CONF_INV_PART_NUMBER): text_sensor.text_sensor_schema(),
    cv.Optional(CONF_INV_SERIAL_NUMBER): text_sensor.text_sensor_schema(),
    cv.Optional(CONF_INV_SOLAR_VOLTAGE_INPUT_1): sensor.sensor_schema(
        unit_of_measurement = UNIT_VOLT,
        icon = 'mdi:solar-power',
        accuracy_decimals = 0,
        device_class = DEVICE_CLASS_VOLTAGE,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_SOLAR_CURRENT_INPUT_1): sensor.sensor_schema(
        unit_of_measurement = UNIT_AMPERE,
        icon = 'mdi:current-dc',
        accuracy_decimals = 1,
        device_class = DEVICE_CLASS_CURRENT,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_SOLAR_POWER_INPUT_1): sensor.sensor_schema(
        unit_of_measurement = UNIT_WATT,
        icon = 'mdi:solar-power',
        accuracy_decimals = 0,
        device_class = DEVICE_CLASS_POWER,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_SOLAR_VOLTAGE_INPUT_2): sensor.sensor_schema(
        unit_of_measurement = UNIT_VOLT,
        icon = 'mdi:solar-power',
        accuracy_decimals = 0,
        device_class = DEVICE_CLASS_VOLTAGE,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_SOLAR_CURRENT_INPUT_2): sensor.sensor_schema(
        unit_of_measurement = UNIT_AMPERE,
        icon = 'mdi:current-dc',
        accuracy_decimals = 1,
        device_class = DEVICE_CLASS_CURRENT,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_SOLAR_POWER_INPUT_2): sensor.sensor_schema(
        unit_of_measurement = UNIT_WATT,
        icon = 'mdi:solar-power',
        accuracy_decimals = 0,
        device_class = DEVICE_CLASS_POWER,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_SOLAR_VOLTAGE_INPUT_3): sensor.sensor_schema(
        unit_of_measurement = UNIT_VOLT,
        icon = 'mdi:solar-power',
        accuracy_decimals = 0,
        device_class = DEVICE_CLASS_VOLTAGE,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_SOLAR_CURRENT_INPUT_3): sensor.sensor_schema(
        unit_of_measurement = UNIT_AMPERE,
        icon = 'mdi:current-dc',
        accuracy_decimals = 1,
        device_class = DEVICE_CLASS_CURRENT,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_SOLAR_POWER_INPUT_3): sensor.sensor_schema(
        unit_of_measurement = UNIT_WATT,
        icon = 'mdi:solar-power',
        accuracy_decimals = 0,
        device_class = DEVICE_CLASS_POWER,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_SOLAR_ISO_RES_INPUT_1): sensor.sensor_schema(
        unit_of_measurement = UNIT_OHM,
        icon = 'mdi:omega',
        accuracy_decimals = 0,
        #device_class = DEVICE_CLASS_,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_SOLAR_INPUT_MOV_RES): sensor.sensor_schema(
        unit_of_measurement = UNIT_OHM,
        icon = 'mdi:omega',
        accuracy_decimals = 0,
        #device_class = DEVICE_CLASS_,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_TEMPERATURE_NTC_DC): sensor.sensor_schema(
        unit_of_measurement = UNIT_CELSIUS,
        icon = 'mdi:temperature-celcius',
        accuracy_decimals = 0,
        device_class = DEVICE_CLASS_TEMPERATURE,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_TEMPERATURE_NTC_AC): sensor.sensor_schema(
        unit_of_measurement = UNIT_CELSIUS,
        icon = 'mdi:temperature-celcius',
        accuracy_decimals = 0,
        device_class = DEVICE_CLASS_TEMPERATURE,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_AC_CURRENT): sensor.sensor_schema(
        unit_of_measurement = UNIT_AMPERE,
        icon = 'mdi:current-ac',
        accuracy_decimals = 1,
        device_class = DEVICE_CLASS_CURRENT,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_AC_VOLTAGE): sensor.sensor_schema(
        unit_of_measurement = UNIT_VOLT,
        icon = 'mdi:solar-power',
        accuracy_decimals = 0,
        device_class = DEVICE_CLASS_VOLTAGE,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_AC_POWER): sensor.sensor_schema(
        unit_of_measurement = UNIT_WATT,
        icon = 'mdi:solar-power',
        accuracy_decimals = 0,
        device_class = DEVICE_CLASS_POWER,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_AC_FREQ): sensor.sensor_schema(
        unit_of_measurement = UNIT_HERTZ,
        icon = 'mdi:sine-wave',
        accuracy_decimals = 2,
        device_class = DEVICE_CLASS_FREQUENCY,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_AC_CURRENT_PHASE_1): sensor.sensor_schema(
        unit_of_measurement = UNIT_AMPERE,
        icon = 'mdi:current-ac',
        accuracy_decimals = 1,
        device_class = DEVICE_CLASS_CURRENT,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_AC_VOLTAGE_PHASE_1): sensor.sensor_schema(
        unit_of_measurement = UNIT_VOLT,
        icon = 'mdi:solar-power',
        accuracy_decimals = 0,
        device_class = DEVICE_CLASS_VOLTAGE,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_AC_POWER_PHASE_1): sensor.sensor_schema(
        unit_of_measurement = UNIT_WATT,
        icon = 'mdi:solar-power',
        accuracy_decimals = 0,
        device_class = DEVICE_CLASS_POWER,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_AC_FREQ_PHASE_1): sensor.sensor_schema(
        unit_of_measurement = UNIT_HERTZ,
        icon = 'mdi:sine-wave',
        accuracy_decimals = 2,
        device_class = DEVICE_CLASS_FREQUENCY,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_AC_CURRENT_PHASE_2): sensor.sensor_schema(
        unit_of_measurement = UNIT_AMPERE,
        icon = 'mdi:current-ac',
        accuracy_decimals = 1,
        device_class = DEVICE_CLASS_CURRENT,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_AC_VOLTAGE_PHASE_2): sensor.sensor_schema(
        unit_of_measurement = UNIT_VOLT,
        icon = 'mdi:solar-power',
        accuracy_decimals = 0,
        device_class = DEVICE_CLASS_VOLTAGE,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_AC_POWER_PHASE_2): sensor.sensor_schema(
        unit_of_measurement = UNIT_WATT,
        icon = 'mdi:solar-power',
        accuracy_decimals = 0,
        device_class = DEVICE_CLASS_POWER,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_AC_FREQ_PHASE_2): sensor.sensor_schema(
        unit_of_measurement = UNIT_HERTZ,
        icon = 'mdi:sine-wave',
        accuracy_decimals = 2,
        device_class = DEVICE_CLASS_FREQUENCY,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_AC_CURRENT_PHASE_3): sensor.sensor_schema(
        unit_of_measurement = UNIT_AMPERE,
        icon = 'mdi:current-ac',
        accuracy_decimals = 1,
        device_class = DEVICE_CLASS_CURRENT,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_AC_VOLTAGE_PHASE_3): sensor.sensor_schema(
        unit_of_measurement = UNIT_VOLT,
        icon = 'mdi:solar-power',
        accuracy_decimals = 0,
        device_class = DEVICE_CLASS_VOLTAGE,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_AC_POWER_PHASE_3): sensor.sensor_schema(
        unit_of_measurement = UNIT_WATT,
        icon = 'mdi:solar-power',
        accuracy_decimals = 0,
        device_class = DEVICE_CLASS_POWER,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_AC_FREQ_PHASE_3): sensor.sensor_schema(
        unit_of_measurement = UNIT_HERTZ,
        icon = 'mdi:sine-wave',
        accuracy_decimals = 2,
        device_class = DEVICE_CLASS_FREQUENCY,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_SC_GRID_VOLTAGE): sensor.sensor_schema(
        unit_of_measurement = UNIT_VOLT,
        icon = 'mdi:transmission-tower',
        accuracy_decimals = 2,
        device_class = DEVICE_CLASS_VOLTAGE,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_SC_GRID_FREQUENCY): sensor.sensor_schema(
        unit_of_measurement = UNIT_HERTZ,
        icon = 'mdi:sine-wave',
        accuracy_decimals = 2,
        device_class = DEVICE_CLASS_FREQUENCY,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_SC_GRID_DC_INJ_CURRENT): sensor.sensor_schema(
        unit_of_measurement = UNIT_AMPERE,
        icon = 'mdi:transmission-tower',
        accuracy_decimals = 2,
        device_class = DEVICE_CLASS_CURRENT,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_AC_GRID_VOLTAGE): sensor.sensor_schema(
        unit_of_measurement = UNIT_VOLT,
        icon = 'mdi:transmission-tower',
        accuracy_decimals = 2,
        device_class = DEVICE_CLASS_VOLTAGE,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_AC_GRID_FREQ): sensor.sensor_schema(
        unit_of_measurement = UNIT_HERTZ,
        icon = 'mdi:sine-wave',
        accuracy_decimals = 2,
        device_class = DEVICE_CLASS_FREQUENCY,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_AC_GRID_DC_INJ_CURRENT): sensor.sensor_schema(
        unit_of_measurement = UNIT_AMPERE,
        icon = 'mdi:transmission-tower',
        accuracy_decimals = 2,
        device_class = DEVICE_CLASS_CURRENT,
        state_class = STATE_CLASS_MEASUREMENT
    ),
    cv.Optional(CONF_INV_MAX_AC_CURRENT_TODAY): sensor.sensor_schema(
        unit_of_measurement = UNIT_AMPERE,
        icon = 'mdi:current-ac',
        accuracy_decimals = 0,
        device_class = DEVICE_CLASS_CURRENT,
        state_class = STATE_CLASS_TOTAL_INCREASING
    ),
    cv.Optional(CONF_INV_MIN_AC_VOLTAGE_TODAY): sensor.sensor_schema(
        unit_of_measurement = UNIT_VOLT,
        icon = 'mdi:current-ac',
        accuracy_decimals = 0,
        device_class = DEVICE_CLASS_VOLTAGE,
        state_class = STATE_CLASS_TOTAL_INCREASING
    ),
    cv.Optional(CONF_INV_MAX_AC_VOLTAGE_TODAY): sensor.sensor_schema(
        unit_of_measurement = UNIT_VOLT,
        icon = 'mdi:current-ac', # XXX
        accuracy_decimals = 0,
        device_class = DEVICE_CLASS_VOLTAGE,
        state_class = STATE_CLASS_TOTAL_INCREASING
    ),
    cv.Optional(CONF_INV_MAX_AC_POWER_TODAY): sensor.sensor_schema(
        unit_of_measurement = UNIT_WATT,
        icon = 'mdi:solar-power',
        accuracy_decimals = 0,
        device_class = DEVICE_CLASS_POWER,
        state_class = STATE_CLASS_TOTAL_INCREASING
    ),
    cv.Optional(CONF_INV_MIN_AC_FREQ_TODAY): sensor.sensor_schema(
        unit_of_measurement = UNIT_HERTZ,
        icon = 'mdi:sine-wave',
        accuracy_decimals = 2,
        device_class = DEVICE_CLASS_FREQUENCY,
        state_class = STATE_CLASS_TOTAL_INCREASING
    ),
    cv.Optional(CONF_INV_MAX_AC_FREQ_TODAY): sensor.sensor_schema(
        unit_of_measurement = UNIT_HERTZ,
        icon = 'mdi:sine-wave',
        accuracy_decimals = 2,
        device_class = DEVICE_CLASS_FREQUENCY,
        state_class = STATE_CLASS_TOTAL_INCREASING
    ),
    cv.Optional(CONF_INV_SUPPLIED_AC_ENERGY_TODAY): sensor.sensor_schema(
        unit_of_measurement = UNIT_WATT_HOURS,
        icon = 'mdi:meter-electric',
        accuracy_decimals = 0,
        device_class = DEVICE_CLASS_ENERGY,
        state_class = STATE_CLASS_TOTAL_INCREASING
    ),
    cv.Optional(CONF_INV_SUPPLIED_AC_ENERGY_TOTAL): sensor.sensor_schema(
        unit_of_measurement = UNIT_KILOWATT_HOURS,
        icon = 'mdi:meter-electric',
        accuracy_decimals = 0,
        device_class = DEVICE_CLASS_ENERGY,
        state_class = STATE_CLASS_TOTAL_INCREASING
    ),
    cv.Optional(CONF_INV_RUNTIME_TOTAL): sensor.sensor_schema(
        unit_of_measurement = UNIT_SECOND,
        icon = 'mdi:sun-clock',
        accuracy_decimals = 0,
        device_class = DEVICE_CLASS_DURATION,
        state_class = STATE_CLASS_TOTAL_INCREASING
    ),
    cv.Optional(CONF_INV_RUNTIME_TODAY): sensor.sensor_schema(
        unit_of_measurement = UNIT_SECOND,
        icon = 'mdi:sun-clock',
        accuracy_decimals = 0,
        device_class = DEVICE_CLASS_DURATION,
        state_class = STATE_CLASS_TOTAL_INCREASING
    ),
    cv.Optional(CONF_INV_MAX_SOLAR_CURRENT_INPUT_1): sensor.sensor_schema(
        unit_of_measurement = UNIT_AMPERE,
        icon = 'mdi:current-dc',
        accuracy_decimals = 1,
        device_class = DEVICE_CLASS_CURRENT,
        state_class = STATE_CLASS_TOTAL_INCREASING
    ),
    cv.Optional(CONF_INV_MAX_SOLAR_VOLTAGE_INPUT_1): sensor.sensor_schema(
        unit_of_measurement = UNIT_VOLT,
        icon = 'mdi:solar-power',
        accuracy_decimals = 0,
        device_class = DEVICE_CLASS_VOLTAGE,
        state_class = STATE_CLASS_TOTAL_INCREASING
    ),
    cv.Optional(CONF_INV_MAX_SOLAR_POWER_INPUT_1): sensor.sensor_schema(
        unit_of_measurement = UNIT_WATT,
        icon = 'mdi:solar-power',
        accuracy_decimals = 0,
        device_class = DEVICE_CLASS_POWER,
        state_class = STATE_CLASS_TOTAL_INCREASING
    ),
    cv.Optional(CONF_INV_MIN_SOLAR_ISO_RES_INPUT_1): sensor.sensor_schema(
        unit_of_measurement = UNIT_OHM,
        icon = 'mdi:omega',
        accuracy_decimals = 0,
        #device_class = DEVICE_CLASS_,
        state_class = STATE_CLASS_TOTAL_INCREASING
    ),
    cv.Optional(CONF_INV_MAX_SOLAR_ISO_RES_INPUT_1): sensor.sensor_schema(
        unit_of_measurement = UNIT_OHM,
        icon = 'mdi:omega',
        accuracy_decimals = 0,
        #device_class = DEVICE_CLASS_,
        state_class = STATE_CLASS_TOTAL_INCREASING
    ),
})

CONFIG_SCHEMA = cv.All(
    cv.Schema({
        cv.GenerateID(): cv.declare_id(DeltaSoliviaComponent),
        cv.Optional(CONF_FLOW_CONTROL_PIN): pins.gpio_output_pin_schema,
        cv.Optional(CONF_HAS_GATEWAY, default = False): cv.boolean,
        cv.Required(CONF_INVERTERS): cv.All(cv.ensure_list(INVERTER_SCHEMA), _validate_inverters),
    })
    .extend(cv.polling_component_schema("5s"))
    .extend(uart.UART_DEVICE_SCHEMA)
)

async def to_code(config):
    component = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(component , config)
    await uart.register_uart_device(component , config)

    if CONF_FLOW_CONTROL_PIN in config:
        pin = await gpio_pin_expression(config[CONF_FLOW_CONTROL_PIN])
        cg.add(component.set_flow_control_pin(pin))

    # update interval for component should be different depending on
    # whether there's a gateway present or not (the gateway will request
    # updates often, and the polling interval needs to be a lot shorter)
    has_gateway     = config[CONF_HAS_GATEWAY]
    update_interval = config[CONF_UPDATE_INTERVAL].total_milliseconds
    if not has_gateway and update_interval < 1000:
        LOGGER.warning("— [Solivia] Component update interval for non-gateway operation shouldn't be lower than 1000ms (is now set to %ums)", update_interval)
    elif has_gateway and update_interval != 500:
        LOGGER.warning("— [Solivia] Fixing component update interval to 500ms")
        cg.add(component.set_update_interval(500))
    cg.add(component.set_has_gateway(has_gateway))

    for inverter_config in config[CONF_INVERTERS]:
        address  = inverter_config[CONF_INV_ADDRESS]
        variant = inverter_config[CONF_INV_VARIANT]
        throttle = inverter_config[CONF_INV_THROTTLE];
        inverter = cg.new_Pvariable(inverter_config[CONF_ID], DeltaSoliviaInverter(address, variant))

        # set throttle interval on component, which is used
        # to prevent excessive work when running in gateway mode
        cg.add(component.set_throttle(throttle));

        # create all numerical sensors, each one with a throttle_average filter
        # to prevent overloading HA
        async def make_sensor(field):
            # create the throttle filter
            filter_id = cv.declare_id(sensor.ThrottleAverageFilter)(f'{field}_{address}')
            filter = cg.new_Pvariable(filter_id, sensor.ThrottleAverageFilter(throttle))
            cg.add(cg.App.register_component(filter))
            cg.add(filter.set_component_source('sensor'))

            sens = await sensor.new_sensor(inverter_config[field])
            cg.add(sens.add_filters([ filter ]))

            cg.add(inverter.add_sensor(field, sens))

        async def make_text_sensor(field):
            sens = await text_sensor.new_text_sensor(inverter_config[field])
            cg.add(inverter.add_text_sensor(field, sens))

        # add all configured sensors and text sensors to the inverter instance
        for field_name in inverter_config:
            field_value = inverter_config.get(field_name)
            try:
                sensor_type = str(field_value.get('id').type);
                if sensor_type == 'sensor::Sensor':
                    await make_sensor(field_name)
                elif sensor_type == 'text_sensor::TextSensor':
                    await make_text_sensor(field_name)
            except Exception as e:
                pass

        # add inverter to component
        cg.add(component.add_inverter(inverter))
