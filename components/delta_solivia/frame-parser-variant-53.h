#pragma once

#include "frame-parser-interface.h"

namespace esphome {
namespace delta_solivia {


class FrameParserVariant53 : public IFrameParser {
    void parse_frame_(const uint8_t* frame, std::size_t pos) override {

        uint32_t status_1 =0;
        uint32_t status_2 =0;
        uint32_t status_3 =0;
        uint32_t status_4 =0;
        // XXX: sensor names should match `CONF_INV_*` values in __init__.py
        publish_text_sensor_(CONF_INV_PART_NUMBER,
                             parse_string(frame, pos, 11));
        pos += 11;
        publish_text_sensor_(CONF_INV_SERIAL_NUMBER,
                             parse_string(frame, pos, 18));
        pos += 18;
        publish_sensor_("SAP_date_code",
                        extract_int32(&frame[pos]));  // Date Code);
        pos += 4;
        publish_sensor_("SAP_revision",
                        extract_int16(&frame[pos]));  // Date Code);
        pos += 2;

        // Correct till here

        publish_sensor_("Software_rev_sc_major",
                        frame[pos++]);  // Major SC revision
        publish_sensor_("Software_rev_sc_minor",
                        frame[pos++]);  // Minor SC revision
        publish_sensor_("Software_rev_sc_bugfix", frame[pos++]);  // SC bugfix

        // Software Revision Power Controller Major , Minor, Bug fixing 3 Byte
        pos += 3;
        // Software Revision ENS controller Major, Minor, Bug fixing 3 Byte
        pos += 3;
        // Software Revision Watch dog controller Major, Minor, Bug fixing 3
        // Byte
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
            publish_sensor_(CONF_INV_SOLAR_POWER_PREFIX + std::to_string(input), extract_int16(&frame[pos])); // W
            pos += 2;
            // Solar voltage at input 1 V 2[UINT16] 1 = 1V
            publish_sensor_(CONF_INV_SOLAR_VOLTAGE_PREFIX + std::to_string(input), extract_int16(&frame[pos])); // Volts
            pos += 2;
            // Solar current at input 1 A 2[UINT16] 1 = 0,1A
            publish_sensor_(CONF_INV_SOLAR_CURRENT_PREFIX + std::to_string(input), extract_int16(&frame[pos]) * 0.1); // Amps
            pos += 2;
        }

        for (int phase = 1; phase <= 3; ++phase) {
            // AC current - L1 A 2[UINT16] 1 = 0,1A
            publish_sensor_(CONF_INV_AC_CURRENT_PHASE_PREFIX + std::to_string(phase), extract_int16(&frame[pos]) / 10.0); // A
            pos += 2;
            // AC voltage - L1 V 2[UINT16] 1= 1V
            publish_sensor_(CONF_INV_AC_VOLTAGE_PHASE_PREFIX + std::to_string(phase), extract_int16(&frame[pos]) ); // V
            pos += 2;
            // AC frequency - L1 Hz 2[UINT16] 1= 0,01Hz
            publish_sensor_(CONF_INV_AC_FREQ_PHASE_PREFIX + std::to_string(phase), extract_int16(&frame[pos]) / 100.0); // Hz
            pos += 2;
            // AC active power - L1 W 2[UINT16] 1 = 1W
            publish_sensor_(CONF_INV_AC_POWER_PHASE_PREFIX + std::to_string(phase), extract_int16(&frame[pos])); // W
            pos += 2;
            // AC reactive power - L1 VAR 2[INT16] 1 = 1VAR
            publish_sensor_("ac_reactive_power_phase_" + std::to_string(phase), extract_int16(&frame[pos])); // W
            pos += 2;
        }
        
    
        // solar isolation plus kOhm 2[UINT16] 1 = 1kOhm
        publish_sensor_(CONF_INV_MAX_SOLAR_ISO_RES_INPUT_1, extract_int16(&frame[pos]) * 1000); // kOhms
        pos += 2;
        // solar isolation minus kOhm 2[UINT16] 1 = 1kOhm
        publish_sensor_(CONF_INV_MIN_SOLAR_ISO_RES_INPUT_1, extract_int16(&frame[pos]) * 1000.0); // kOhms
        pos += 2;
        // Temperature amb °C 2[INT16] 1 = 1°C
        publish_sensor_("ambient_temp", extract_int16(&frame[pos])); // C
        pos += 2;
        // Temperature heatsink °C 2[INT16] 1 = 1°C
        publish_sensor_("heatsink_temp", extract_int16(&frame[pos])); // C
        pos += 2;
        // Supplied ac energy total Wh 8[UINT64] 1 = 1Wh
        publish_sensor_(CONF_INV_SUPPLIED_AC_ENERGY_TOTAL, extract_int64(&frame[pos])); // kWh
        pos += 8;
        // Inverter runtime total Minutes 4[UINT32] 1 = 1 minute
        publish_sensor_(CONF_INV_RUNTIME_TOTAL, extract_int32(&frame[pos]));
        pos += 4;

        // Status 1 4[UINT32] bit description
        status_1=extract_int32(&frame[pos];
        pos += 4;
        // Status 2 4[UINT32] bit description
        status_2=extract_int32(&frame[pos];
        pos += 4;
        // Status 3 4[UINT32] bit description
        status_3=extract_int32(&frame[pos];
        pos += 4;
        // Status 4 4[UINT32] bit description
        status_4=extract_int32(&frame[pos];
        pos += 4;

        parseStatus(status_1, status_2, status_3, status_4);

        // Internal status 1 4[UIT32] reserved bit description
        publish_sensor_("internal_status_1", extract_int32(&frame[pos])); 
        pos += 4;
        // Internal status 2 4[UINT32] reserved bit description
        publish_sensor_("internal_status_2", extract_int32(&frame[pos])); 
        pos += 4;
        // Internal status 3 4[UINT32] reserved bit description
        publish_sensor_("internal_status_3", extract_int32(&frame[pos])); 
        pos += 4;
        // Internal status 4 4[UINT32] reserved bit description
        publish_sensor_("internal_status_4", extract_int32(&frame[pos])); 
        pos += 4;
        // Supplied ac energy day Wh 8[UINT64] 1 = 1Wh
        publish_sensor_(CONF_INV_SUPPLIED_AC_ENERGY_TODAY, extract_int64(&frame[pos]));  // Wh        
        pos += 8;
        // Inverter runtime day Minutes 4[UINT32] 1 = 1 minute
        publish_sensor_(CONF_INV_RUNTIME_TODAY, extract_int32(&frame[pos])*60);
        pos += 4;
        // reserved 67
    }

    void parseStatus(uint32_t status_1, uint32_t status_2, uint32_t status_3, uint32_t status_4){
        status_text = "";
        if( (status_1 & 1<<0) > 0) status_text=status_text+"Self test ongoing ";
        if( (status_1 & 1<<1) > 0) status_text=status_text+"Firmware update ";
        if( (status_1 & 1<<2) > 0) status_text=status_text+"Night mode ";
        if( (status_1 & 1<<3) > 0) status_text=status_text+"L1 Voltage failure ";
        if( (status_1 & 1<<4) > 0) status_text=status_text+"L2 Voltage failure ";
        if( (status_1 & 1<<5) > 0) status_text=status_text+"L3 Voltage failure ";
        if( (status_1 & 1<<6) > 0) status_text=status_text+"L1 Frequency failure ";
        if( (status_1 & 1<<7) > 0) status_text=status_text+"L2 Frequency failure ";
        if( (status_1 & 1<<8) > 0) status_text=status_text+"L3 Frequency failure ";
        if( (status_1 & 1<<9) > 0) status_text=status_text+"L1 DC Inj. failure ";
        if( (status_1 & 1<<10)> 0) status_text=status_text+"L2 DC Inj. Failure ";
        if( (status_1 & 1<<11)> 0) status_text=status_text+"L3 DC Inj. failure ";
        if( (status_1 & 1<<12)> 0) status_text=status_text+"L1 islanding ";
        if( (status_1 & 1<<13)> 0) status_text=status_text+"L2 islanding ";
        if( (status_1 & 1<<14)> 0) status_text=status_text+"L3 islanding ";
        if( (status_1 & 1<<15)> 0) status_text=status_text+"L1 Grid error ";
        if( (status_1 & 1<<16)> 0) status_text=status_text+"L2 Grid error ";
        if( (status_1 & 1<<17)> 0) status_text=status_text+"L3 Grid error ";
        if( (status_1 & 1<<18)> 0) status_text=status_text+"L1 Long grid out ";
        if( (status_1 & 1<<19)> 0) status_text=status_text+"L2 Long grid out ";
        if( (status_1 & 1<<20)> 0) status_text=status_text+"L3 Long grid out ";
        if( (status_1 & 1<<21)> 0) status_text=status_text+"L1 Grid synchronization error ";
        if( (status_1 & 1<<22)> 0) status_text=status_text+"L2 Grid synchronization error ";
        if( (status_1 & 1<<23)> 0) status_text=status_text+"L3 Grid synchronization error ";
        publish_text_sensor_(CONF_INV_STATUS_TEXT,  status_text);
    }
};

}  // namespace delta_solivia
}  // namespace esphome
