#pragma once

#include "frame-parser-interface.h"
#include <string>

namespace esphome {
namespace delta_solivia {

class FrameParserVariant53 : public IFrameParser {
protected:
  void parse_frame_(const uint8_t* frame, std::size_t pos) override {
    uint32_t status_1 = 0;
    uint32_t status_2 = 0;
    uint32_t status_3 = 0;
    uint32_t status_4 = 0;

    // XXX: sensor names should match `CONF_INV_*` values in __init__.py
    this->publish_text_sensor_(CONF_INV_PART_NUMBER, this->parse_string(frame, pos, 11));
    pos += 11;
    this->publish_text_sensor_(CONF_INV_SERIAL_NUMBER, this->parse_string(frame, pos, 18));
    pos += 18;
    this->publish_sensor_("SAP_date_code", this->extract_uint32(&frame[pos]));
    pos += 4;
    this->publish_sensor_("SAP_revision", this->extract_uint16(&frame[pos]));
    pos += 2;

    // Major SC revision
    this->publish_sensor_("Software_rev_sc_major", frame[pos++]);
    // Minor SC revision
    this->publish_sensor_("Software_rev_sc_minor", frame[pos++]);
    // SC bugfix
    this->publish_sensor_("Software_rev_sc_bugfix", frame[pos++]);

    // Software Revision Power Controller Major , Minor, Bug fixing 3 Byte
    pos += 3;
    // Software Revision ENS controller Major, Minor, Bug fixing 3 Byte
    pos += 3;
    // Software Revision Watch dog controller Major, Minor, Bug fixing 3 Byte
    pos += 3;
    // Software Revision DC controller Major, Minor, Bug fixing 3 Byte
    pos += 3;
    // Software Revision DC 1 Major, Minor, Bug fixing 3 Byte
    pos += 3;
    // Software Revision DC 2 Major, Minor, Bug fixing 3 Byte
    pos += 3;
    // Software Revision DC 3 Major, Minor, Bug fixing 3 Byte
    pos += 3;
    // Software Revision AC Major, Minor, Bug fixing 3 Byte
    pos += 3;
    // Software Revision AC1 Major, Minor, Bug fixing 3 Byte
    pos += 3;
    // Software Revision AC2 Major, Minor, Bug fixing 3 Byte
    pos += 3;
    // Software Revision AC3 Major, Minor, Bug fixing 3 Byte
    pos += 3;
    // Software Revision reserved Major, Minor, Bug fixing 3 Byte
    pos += 3;

    for (int input = 1; input <= 3; ++input) {
      // Solar power at input 1 W 2[UINT16] 1 = 1W
      this->publish_sensor_(CONF_INV_SOLAR_POWER_PREFIX + std::to_string(input), this->extract_uint16(&frame[pos])); // W
      pos += 2;
      // Solar voltage at input 1 V 2[UINT16] 1 = 1V
      this->publish_sensor_(CONF_INV_SOLAR_VOLTAGE_PREFIX + std::to_string(input), this->extract_uint16(&frame[pos])); // Volts
      pos += 2;
      // Solar current at input 1 A 2[UINT16] 1 = 0,1A
      this->publish_sensor_(CONF_INV_SOLAR_CURRENT_PREFIX + std::to_string(input), this->extract_uint16(&frame[pos]) * 0.1); // Amps
      pos += 2;
    }

    for (int phase = 1; phase <= 3; ++phase) {
      // AC current - L1 A 2[UINT16] 1 = 0,1A
      this->publish_sensor_(CONF_INV_AC_CURRENT_PHASE_PREFIX + std::to_string(phase), this->extract_uint16(&frame[pos]) / 10.0); // A
      pos += 2;
      // AC voltage - L1 V 2[UINT16] 1= 1V
      this->publish_sensor_(CONF_INV_AC_VOLTAGE_PHASE_PREFIX + std::to_string(phase), this->extract_uint16(&frame[pos]) ); // V
      pos += 2;
      // AC frequency - L1 Hz 2[UINT16] 1= 0,01Hz
      this->publish_sensor_(CONF_INV_AC_FREQ_PHASE_PREFIX + std::to_string(phase), this->extract_uint16(&frame[pos]) / 100.0); // Hz
      pos += 2;
      // AC active power - L1 W 2[UINT16] 1 = 1W
      this->publish_sensor_(CONF_INV_AC_POWER_PHASE_PREFIX + std::to_string(phase), this->extract_uint16(&frame[pos])); // W
      pos += 2;
      // AC reactive power - L1 VAR 2[INT16] 1 = 1VAR
      this->publish_sensor_(CONF_INV_AC_REACTIVE_POWER_PHASE_PREFIX + std::to_string(phase), this->extract_uint16(&frame[pos])); // W
      pos += 2;
    }
  
    // solar isolation plus kOhm 2[UINT16] 1 = 1kOhm
    this->publish_sensor_(CONF_INV_MAX_SOLAR_ISO_RES_INPUT_1, this->extract_uint16(&frame[pos]) * 1000); // kOhms
    pos += 2;
    // solar isolation minus kOhm 2[UINT16] 1 = 1kOhm
    this->publish_sensor_(CONF_INV_MIN_SOLAR_ISO_RES_INPUT_1, this->extract_uint16(&frame[pos]) * 1000.0); // kOhms
    pos += 2;
    // Temperature amb °C 2[INT16] 1 = 1°C
    this->publish_sensor_("ambient_temp", this->extract_uint16(&frame[pos])); // C
    pos += 2;
    // Temperature heatsink °C 2[INT16] 1 = 1°C
    this->publish_sensor_("heatsink_temp", this->extract_uint16(&frame[pos])); // C
    pos += 2;
    // Supplied ac energy total Wh 8[UINT64] 1 = 1Wh
    this->publish_sensor_(CONF_INV_SUPPLIED_AC_ENERGY_TOTAL, this->extract_uint64(&frame[pos])); // kWh
    pos += 8;
    // Inverter runtime total Minutes 4[UINT32] 1 = 1 minute
    this->publish_sensor_(CONF_INV_RUNTIME_TOTAL, this->extract_uint32(&frame[pos]));
    pos += 4;

    // Status 1 4[UINT32] bit description
    status_1 = this->extract_uint32(&frame[pos]);
    pos += 4;
    // Status 2 4[UINT32] bit description
    status_2 = this->extract_uint32(&frame[pos]);
    pos += 4;
    // Status 3 4[UINT32] bit description
    status_3 = this->extract_uint32(&frame[pos]);
    pos += 4;
    // Status 4 4[UINT32] bit description
    status_4 = this->extract_uint32(&frame[pos]);
    pos += 4;

    this->parse_status(status_1, status_2, status_3, status_4);

    // Internal status 1 4[UIT32] reserved bit description
    this->publish_sensor_("internal_status_1", this->extract_uint32(&frame[pos])); 
    pos += 4;
    // Internal status 2 4[UINT32] reserved bit description
    this->publish_sensor_("internal_status_2", this->extract_uint32(&frame[pos])); 
    pos += 4;
    // Internal status 3 4[UINT32] reserved bit description
    this->publish_sensor_("internal_status_3", this->extract_uint32(&frame[pos])); 
    pos += 4;
    // Internal status 4 4[UINT32] reserved bit description
    this->publish_sensor_("internal_status_4", this->extract_uint32(&frame[pos])); 
    pos += 4;
    // Supplied ac energy day Wh 8[UINT64] 1 = 1Wh
    this->publish_sensor_(CONF_INV_SUPPLIED_AC_ENERGY_TODAY, this->extract_uint64(&frame[pos]));  // Wh    
    pos += 8;
    // Inverter runtime day Minutes 4[UINT32] 1 = 1 minute
    this->publish_sensor_(CONF_INV_RUNTIME_TODAY, this->extract_uint32(&frame[pos])*60);
    pos += 4;
    // reserved 67
  }

  void parse_status(uint32_t status_1, uint32_t status_2, uint32_t status_3, uint32_t status_4){
    std::string status_text = "";

    if (status_1 & 1<<0) status_text += "Self test ongoing\n";
    if (status_1 & 1<<1) status_text += "Firmware update\n";
    if (status_1 & 1<<2) status_text += "Night mode\n";
    if (status_1 & 1<<3) status_text += "L1 Voltage failure\n";
    if (status_1 & 1<<4) status_text += "L2 Voltage failure\n";
    if (status_1 & 1<<5) status_text += "L3 Voltage failure\n";
    if (status_1 & 1<<6) status_text += "L1 Frequency failure\n";
    if (status_1 & 1<<7) status_text += "L2 Frequency failure\n";
    if (status_1 & 1<<8) status_text += "L3 Frequency failure\n";
    if (status_1 & 1<<9) status_text += "L1 DC Inj. failure\n";
    if (status_1 & 1<<10) status_text += "L2 DC Inj. Failure\n";
    if (status_1 & 1<<11) status_text += "L3 DC Inj. failure\n";
    if (status_1 & 1<<12) status_text += "L1 islanding\n";
    if (status_1 & 1<<13) status_text += "L2 islanding\n";
    if (status_1 & 1<<14) status_text += "L3 islanding\n";
    if (status_1 & 1<<15) status_text += "L1 Grid error\n";
    if (status_1 & 1<<16) status_text += "L2 Grid error\n";
    if (status_1 & 1<<17) status_text += "L3 Grid error\n";
    if (status_1 & 1<<18) status_text += "L1 Long grid out\n";
    if (status_1 & 1<<19) status_text += "L2 Long grid out\n";
    if (status_1 & 1<<20) status_text += "L3 Long grid out\n";
    if (status_1 & 1<<21) status_text += "L1 Grid synchronization error\n";
    if (status_1 & 1<<22) status_text += "L2 Grid synchronization error\n";
    if (status_1 & 1<<23) status_text += "L3 Grid synchronization error\n";

    if (status_2 & 1<<0) status_text += "PV1 Iso startup failure\n";
    if (status_2 & 1<<1) status_text += "PV1 Iso running failure\n";
    if (status_2 & 1<<2) status_text += "PV1+ grounding failure\n";
    if (status_2 & 1<<3) status_text += "PV1- grounding failure\n";
    if (status_2 & 1<<4) status_text += "PV2 Iso startup failure\n";
    if (status_2 & 1<<5) status_text += "PV2 Iso running failure\n";
    if (status_2 & 1<<6) status_text += "PV2+ grounding failure\n";
    if (status_2 & 1<<7) status_text += "PV2- grounding failure\n";
    if (status_2 & 1<<8) status_text += "PV3 Iso startup failure\n";
    if (status_2 & 1<<9) status_text += "PV3 Iso running failure\n";
    if (status_2 & 1<<10) status_text += "PV3+ grounding failure\n";
    if (status_2 & 1<<11) status_text += "PV3- grounding failure\n";
    if (status_2 & 1<<12) status_text += "PV1 voltage too low failure\n";
    if (status_2 & 1<<13) status_text += "PV2 voltage too low failure\n";
    if (status_2 & 1<<14) status_text += "PV3 voltage too low failure\n";
    if (status_2 & 1<<15) status_text += "Internal failure\n";
    if (status_2 & 1<<16) status_text += "Auto test failure\n";
    if (status_2 & 1<<17) status_text += "PV power too low failure\n";

    if (status_3 & 1<<0) status_text += "PV1 Iso startup warning\n";
    if (status_3 & 1<<1) status_text += "PV1 Iso running warning\n";
    if (status_3 & 1<<2) status_text += "PV1+ grounding warning\n";
    if (status_3 & 1<<3) status_text += "PV1- grounding warning\n";
    if (status_3 & 1<<4) status_text += "PV2 Iso startup warning\n";
    if (status_3 & 1<<5) status_text += "PV2 Iso running warning\n";
    if (status_3 & 1<<6) status_text += "PV2+ grounding warning\n";
    if (status_3 & 1<<7) status_text += "PV2- grounding warning\n";
    if (status_3 & 1<<8) status_text += "PV3 Iso startup warning\n";
    if (status_3 & 1<<9) status_text += "PV3 Iso running warning\n";
    if (status_3 & 1<<10) status_text += "PV3+ grounding warning\n";
    if (status_3 & 1<<11) status_text += "PV3- grounding warning\n";
    if (status_3 & 1<<12) status_text += "PV1 voltage too low warning\n";
    if (status_3 & 1<<13) status_text += "PV2 voltage too low warning\n";
    if (status_3 & 1<<14) status_text += "PV3 voltage too low warning\n";

    if (status_4 & 1<<0) status_text += "Internal fan warning\n";
    if (status_4 & 1<<1) status_text += "External fan warning\n";
    if (status_4 & 1<<2) status_text += "Synchronization ongoing\n";

    this->publish_text_sensor_(CONF_INV_STATUS, status_text);
  }
public:
  FrameParserVariant53() {}

  static bool supports(uint8_t variant) {
    return (
      variant == 53 || variant == 73 || variant == 102 || variant == 103 ||
      variant == 105 || variant == 106 || variant == 107 || variant == 109 ||
      variant == 110 || variant == 111 || variant == 113 || variant == 114 ||
      variant == 120 || variant == 121 || variant == 122 || variant == 123 ||
      variant == 124 || variant == 125 || variant == 158 || variant == 159 ||
      variant == 160 || variant == 161
    );
  }
};

}
}
