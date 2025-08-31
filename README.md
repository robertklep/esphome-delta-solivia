# Delta Solivia RS485 component for ESPHome

This component for ESPHome can be used to directly read measurement/statistics data from the RS485 port of _some_ Delta Solivia inverters. It does not require having a Solivia Gateway device, but will work if you have one.

## Requirements

* An ESP32
* An RS-485 module
* An ESPHome setup

For more information on the hardware requirements, see ["Hardware"](#hardware) below.

## Supported models

There are dozens of Solivia models, and about 10 different protocol variants. The document ["Public Solar Inverter Communication Protocol (Version 1.2)"](assets/Public%20RS485%20Protocol%201V2.pdf), from Delta Energy Systems (Germany) GmbH, documents the protocol that the inverters use over their RS485 port. The document can be found by DuckDuckGo'ing for its title.

Only a limited number of models is supported by this component at the moment ([see below](#compatibility-table)).

The component does not perform any checks to see if it's talking to a supported inverter, but just assumes it does, so using it for a different type of inverter will probably cause the component to either crash or to provide incorrect data. Nothing bad will happen with your inverter if you do try this.

Because not all models provide the same data fields, the exact component configuration for each model is different. [See below](#sensor-fields) for the fields that are available for each variant. There is no validation to check if a configured field applies to the protocol variant of your inverter, but it's not a problem if you configure a field that isn't supported by your inverter.

## HARDWARE

### MCU

The component should work on most ESP32 boards, and probably also ESP8266 boards, although I have only tested with ESP32-C3 "SuperMini" boards.

### RS-485 modules

#### MAX485-based RS-485-to-TTL modules

These modules are cheap and easy to get. They require explicit flow control (see below, "ESPHome setup") for the component to be able to write to the RS485 bus.

##### Pinout

It's not a requirement to use these exact GPIO pins on the ESP32, you can use others if convenient. Just make sure you change the YAML configuration to reflect the different pins.


| ESP32 | RS485 module | Purpose      |
|-------|--------------|--------------|
| GPIO4 | DI           | TX           |
| GPIO3 | RO           | RX           |
| GPIO2 | RE + DE      | Flow control |

#### Modules with automatic flow control

RS485 boards that have automatic flow control will take care of the correct signalling automatically.

### Connecting to the inverter

The inverter should have one or more ports that accept RJ45 ("Ethernet") connectors. The pinout depends on the inverter model (G3 or G4):

![pinout](assets/pinout.png)

The TX_A and RX_B pins should connect to the A and B pins on the RS-485 module, respectively. The RS-485 GND pin isn't used.

If you don't have the necessary tools to crimp Ethernet cables, or you just want a quick solution, cut off a length of Ethernet cable with connector already attached and connect the TX_A and RX_B wires to the RS-485 module.

## ESPHome setup

```
# Set the correct build flags
esphome:
  ...
  platformio_options:
    build_unflags: '-std=c++11 -std=gnu++11'
    build_flags: '-std=c++17 -std=gnu++17'

# Load as external component
external_components:
  - source: github://robertklep/esphome-delta-solivia@v3.4.1
    components: [ delta_solivia ]

# UART setup (change tx_pin/rx_pin if required, leave the rest as-is)
uart:
  tx_pin: GPIO4
  rx_pin: GPIO3
  id: solivia_uart
  rx_buffer_size: 512
  baud_rate: 19200
  parity: NONE
  stop_bits: 1

# component setup
delta_solivia:
  # The UART configured above
  uart_id: solivia_uart

  # See below
  update_interval: 10s

  # See below
  has_gateway: false

  # Optional flow control pin, only configure this if your board doesn't have automatic flow control.
  # (before writing to the bus, the pin will be pulled up to assume control over the bus)
  flow_control_pin: GPIOX

  # Here you can configure multiple inverters.
  #
  # Each inverter has a unique address, usually starting with "1" for the first inverter, "2" for the second, etc.
  #
  # You need to specify the "variant" used for each inverter. See the compatibility table.
  #
  # As explained in the README, the different sensors that can be configured depend on the variant. Look below for the Sensor Fields documentation to see which fields are available for each variant.
  # If you're not interested in a specific sensor, just leave it out, they're all optional.
  #
  # You can run the component with debug logging in ESPHome enabled, which will log information on the packets it receives, including the address from which the packet originated.

  inverters:
    - address: 1
      variant: 15
      throttle: 10s # see below
      part_number:
        name: 'Inverter#1 Part Number'
      serial_number:
        name: 'Inverter#1 Serial Number'
      ac_power:
        name: 'Inverter#1 Current Power'
      supplied_ac_energy_total:
        name: 'Inverter#1 Total Energy'
      supplied_ac_energy_today:
        name: 'Inverter#1 Today Energy'
      dc_voltage:
        name: 'Inverter#1 DC Voltage'
      dc_current:
        name: 'Inverter#1 DC Current'
      ac_voltage:
        name: 'Inverter#1 AC Voltage'
      ac_current:
        name: 'Inverter#1 AC Current'
      ac_frequency:
        name: 'Inverter#1 AC Frequency'
      grid_voltage:
        name: 'Inverter#1 Grid Voltage'
      grid_frequency:
        name: 'Inverter#1 Grid Frequency'
      runtime_total:
        name: 'Inverter#1 Runtime Total'
      runtime_today:
        name: 'Inverter#1 Runtime Today'
      max_ac_power_today:
        name: 'Inverter#1 AC Power Today'
      max_solar_input_power:
        name: 'Inverter#1 Solar Input Power'
    - address: 2
      variant: 15
      part_number:
        name: 'Inverter#2 Part Number'
      serial_number:
        name: 'Inverter#2 Serial Number'
      ac_power:
        name: 'Inverter#2 Current Power'
      supplied_ac_energy_total:
        name: 'Inverter#2 Total Energy'
      supplied_ac_energy_today:
        name: 'Inverter#2 Today Energy'
      dc_voltage:
        name: 'Inverter#2 DC Voltage'
      dc_current:
        name: 'Inverter#2 DC Current'
      ac_voltage:
        name: 'Inverter#2 AC Voltage'
      ac_current:
        name: 'Inverter#2 AC Current'
      ac_frequency:
        name: 'Inverter#2 AC Frequency'
      grid_voltage:
        name: 'Inverter#2 Grid Voltage'
      grid_frequency:
        name: 'Inverter#2 Grid Frequency'
      runtime_total:
        name: 'Inverter#2 Runtime Total'
      runtime_today:
        name: 'Inverter#2 Runtime Today'
      max_ac_power_today:
        name: 'Inverter#2 AC Power Today'
      max_solar_input_power:
        name: 'Inverter#2 Solar Input Power'
```

There's also a full example YAML file [here](esphome-example-configuration.yaml).

#### With or without gateway

If you have a Solivia gateway, you need to set the `has_gateway` to `true`. It will let the component leave requesting updates to the gateway, to prevent having two primaries active on the bus. Note: even though the component will be passive (not writing) when combined with a gateway, you still need to configure the flow control pin if your board requires manual flow control. This is to make sure the pin is set to LOW during component setup, to prevent locking up the bus.

The `update_interval` option of the component serves different purposes depending on whether you have a gateway or not. With a gateway, it will automatically be set to a low value (0.5s) to prevent missing updates sent by the inverters. Without a gateway, it will be the interval at which the component will request a single inverter (in a round-robin fashion) to send an update. The default of 10 seconds should be sufficient, although you can decrease it if you want faster updates or if you have more than one inverter.

The `throttle` option for each inverter will limit the amount of state updates sent back to HA. This is only relevant if you have a gateway, since a gateway will request updates for each inverter about every second (remember that each state update will also be stored in HA's database). The default throttle interval is 10 seconds. Internally, this is implemented using a [`throttle_average`](https://esphome.io/components/sensor/#throttle-average) filter. When not using a gateway this setting is ignored.

## Commands

The component supports a limited number of commands to execute against each inverter.

#### `set_power_limit(uint8_t percentage, uint8_t address = 0xff)`

This command can be used to limit the output power of the inverter with address `address` (use `0xff`, the default, to limit the power of _all_ inverters on the bus). It can be useful in situations where exporting too much to the grid costs you money.

It can be used, for instance, with a template switch setup:
```
delta_solivia:
  id: my_delta_solivia   # references in the switch setup below
  uart_id: solivia_uart
  ...

switch:
  - platform: template
    name: "Limit all inverters to 30%"
    id: set_power_limit_30
    optimistic: true
    assumed_state: true
    turn_on_action:
      lambda: |-
        id(my_delta_solivia).set_power_limit(30, 0xff);
    turn_off_action:
      lambda: |-
        id(my_delta_solivia).set_power_limit(100, 0xff);
```

## Compatibility table

| Variant | Model Name| Supported? | Parser |
|--------|----------------------------|-----------|--------|
| 1      | SI 2500                   | ❌        |        |
| 3      | SI 3300                   | ❌        |        |
| 4      | SI 5000                   | ❌        |        |
| 9      | SOLIVIA 2.0 EU G3         | ❌        |        |
| 11     | SI 11kW (3 phases module of CM/CS) | ❌ |        |
| 14     | SOLIVIA 5.0 EU G3         | ❌        |        |
| 15     | SOLIVIA 2.5 EU G3         | ✅        | Variant15 |
| 18     | SOLIVIA 3.0 EU G3         | ✅        | " |
| 19     | SOLIVIA 3.3 EU G3         | ✅        | " |
| 20     | SOLIVIA 3.6 EU G3         | ✅        | " |
| 27     | SOLIVIA 15 EU TL          | ❌        |        |
| 28     | SOLIVIA 20 EU TL          | ❌        |        |
| 31     | SOLIVIA 2.5 NA G4         | ✅        | Variant15 |
| 34     | SOLIVIA 3.0 NA G4         | ✅        | " |
| 35     | SOLIVIA 3.3 NA G4         | ✅        | " |
| 36     | SOLIVIA 3.6 NA G4         | ✅        | " |
| 38     | SOLIVIA 4.4 NA G4         | ✅        | " |
| 39     | SOLIVIA 5.0 NA G4         | ✅        | " |
| 43     | SOLIVIA 15 EU TL G4       | ❌        |        |
| 44     | SOLIVIA 20 EU TL G4       | ❌        |        |
| 55     | SOLIVIA 2.5 AP G3         | ✅        | Variant15 |
| 58     | SOLIVIA 3.0 AP G3         | ✅        | " |
| 59     | SOLIVIA 3.3 AP G3         | ✅        | " |
| 60     | SOLIVIA 3.6 AP G3         | ✅        | " |
| 63     | SOLIVIA 5.0 AP G3         | ❌        |        |
| 85     | SOLIVIA 3.0 EU T4 TL      | ❌        |        |
| 88     | SOLIVIA 5.0 EU T4 TL      | ❌        |        |
| 89     | SOLIVIA 6.0 EU T4 TL      | ❌        |        |
| 90     | SOLIVIA 8.0 EU T4 TL      | ❌        |        |
| 91     | SOLIVIA 10 EU T4 TL       | ❌        |        |
| 93     | SOLIVIA 12 EU T4 TL       | ❌        |        |
| 95     | SOLIVIA 30 EU T4 TL       | ❌        |        |
| 99     | SOLIVIA CS                | ❌        |        |
| 100    | SOLIVIA CM                | ❌        |        |
| 102    | SOLIVIA 2.0 EU G4 TR      | ✅        | Variant53 |
| 103    | SOLIVIA 2.5 EU G4 TR      | ✅        | "      |
| 105    | SOLIVIA 3.0 EU G4 TR      | ✅        | "      |
| 106    | SOLIVIA 3.3 EU G4 TR      | ✅        | "      |
| 107    | SOLIVIA 3.6 EU G4 TR      | ✅        | "      |
| 109    | SOLIVIA 4.4 EU G4 TR      | ✅        | "      |
| 110    | SOLIVIA 5.0 EU G4 TR      | ✅        | "      |
| 111    | SOLIVIA 10 EU G4 TR (EVR) | ✅        | "      |
| 113    | SOLIVIA 11 EU G4 TR       | ✅        | "      |
| 114    | SOLIVIA 11 EU G4 TR (EVR) | ✅        | "      |
| 120    | SOLIVIA 3.0 NA G4 TL      | ✅        | "      |
| 121    | SOLIVIA 3.8 NA G4 TL      | ✅        | "      |
| 122    | SOLIVIA 5.0 NA G4 TL      | ✅        | "      |
| 123    | SOLIVIA 7.6 NA G4 TL      | ✅        | "      |
| 124    | SOLIVIA 5.2 NA G4 TL      | ✅        | "      |
| 125    | SOLIVIA 6.6 NA G4 TL      | ✅        | "      |
| 158    | DELTA 20 TL               | ✅        | "      |
| 159    | DELTA 15 TL               | ✅        | "      |
| 160    | DELTA 28 TL               | ✅        | "      |
| 161    | DELTA 24 TL               | ✅        | "      |
| 200    | RPI M6                    | ❌        |        |
| 201    | RPI M8                    | ❌        |        |
| 202    | RPI M10                   | ❌        |        |
| 203    | RPI M12                   | ❌        |        |
| 204    | RPI M15A                  | ❌        |        |
| 205    | RPI M20A                  | ❌        |        |
| 206    | RPI M30                   | ❌        |        |
| 207    | RPI H3                    | ❌        |        |
| 208    | RPI H5                    | ❌        |        |
| 209    | RPI H3A                   | ❌        |        |
| 210    | RPI H4A                   | ❌        |        |
| 211    | RPI H5A                   | ❌        |        |
| 212    | RPI H3A                   | ✅        | Variant 212 |
| 213    | RPI H4A                   | ✅        | "      |
| 214    | RPI H5A                   | ✅        | "      |
| 215    | RPI M6A                   | ✅        | "      |
| 216    | RPI M8A                   | ✅        | "      |
| 217    | RPI M10A                  | ✅        | "      |
| 218    | PRI M50A                  | ✅        | "      |
| 219    | RPI M30A                  | ✅        | "      |
| 220    | RPI M15A                  | ✅        | "      |
| 221    | RPI M20A                  | ✅        | "      |
| 222    | RPI H3                    | ✅        | "      |

## Sensor Fields

These fields can be configured for each variant.

### Variant 15

```
part_number
serial_number
solar_voltage_input_1
solar_current_input_1
solar_isolation_resistance_input_1
temperature_ntc_dc
solar_input_mov_resistance
ac_current
ac_voltage
ac_power
ac_frequency
temperature_ntc_ac
sc_grid_voltage
sc_grid_frequency
sc_grid_dc_injection_current
ac_grid_voltage
ac_grid_frequency
ac_grid_dc_injection_current
supplied_ac_energy_today
runtime_today
max_ac_current_today
min_ac_voltage_today
max_ac_voltage_today
max_ac_power_today
min_ac_frequency_today
max_ac_frequency_today
supplied_ac_energy_total
runtime_total
max_solar_current_input_1
max_solar_voltage_input_1
max_solar_power_input_1
min_solar_isolation_resistance_input_1
max_solar_isolation_resistance_input_1
inverter_alarms_status
inverter_dc_input_status
inverter_dc_input_limits
inverter_ac_output_status
inverter_ac_output_limits
inverter_warnings
inverter_dc_hardware_failures
inverter_ac_hardware_failures
inverter_sc_hardware_failures
inverter_bulk_failures
inverter_comms_failures
inverter_ac_hardware_disturbance
inverter_dc_hardware_stage_errors
inverter_calibration_status
inverter_neutral_errors
```

### Variant 53

```
part_number
serial_number
SAP_date_code
SAP_revision
Software_rev_sc_major
Software_rev_sc_minor
Software_rev_sc_bugfix
solar_power_input_1
solar_power_input_2
solar_power_input_3
solar_voltage_input_1
solar_voltage_input_2
solar_voltage_input_3
solar_current_input_1
solar_current_input_2
solar_current_input_3
ac_current_phase_1
ac_current_phase_2
ac_current_phase_3
ac_voltage_phase_1
ac_voltage_phase_2
ac_voltage_phase_3
ac_frequency_phase_1
ac_frequency_phase_2
ac_frequency_phase_3
ac_power_phase_1
ac_power_phase_2
ac_power_phase_3
ac_reactive_power_phase_1
ac_reactive_power_phase_2
ac_reactive_power_phase_3
min_solar_isolation_resistance_input_1
max_solar_isolation_resistance_input_1
supplied_ac_energy_total
supplied_ac_energy_today
runtime_total
runtime_today
inverter_status
```

### Variant 212

```
part_number
serial_number
ac_voltage_phase_1
ac_current_phase_1
ac_power_phase_1
ac_frequency_phase_1
ac_voltage_phase_2
ac_current_phase_2
ac_power_phase_2
ac_frequency_phase_2
ac_voltage_phase_3
ac_current_phase_3
ac_power_phase_3
ac_frequency_phase_3
solar_voltage_input_1
solar_current_input_1
solar_power_input_1
solar_voltage_input_3
solar_current_input_3
solar_power_input_3
ac_power
runtime_today
runtime_total
supplied_ac_energy_today
supplied_ac_energy_total
```

