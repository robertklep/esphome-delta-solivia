#pragma once

#include "frame-parser-interface.h"

namespace esphome {
namespace delta_solivia {


class FrameParserVariant53 : public IFrameParser {
    void parse_frame_(const uint8_t* frame, std::size_t pos) override {
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

        // Solar power at input 1 W 2[UINT16] 1 = 1W
        pos += 2;
        // Solar voltage at input 1 V 2[UINT16] 1 = 1V
        pos += 2;
        // Solar current at input 1 A 2[UINT16] 1 = 0,1A
        pos += 2;

        // Solar power at input 2 W 2[UINT16] 1 = 1W
        pos += 2;
        // Solar voltage at input 2 V 2[UINT16] 1 = 1V
        pos += 2;
        // Solar current at input 2 A 2[UINT16] 1 = 0,1A
        pos += 2;

        // Solar power at input 3 W 2[UINT16] 1 = 1W
        pos += 2;
        // Solar voltage at input 3 V 2[UINT16] 1 = 1V
        pos += 2;
        // Solar current at input 3 A 2[UINT16] 1 = 0,1A
        pos += 2;

        // AC current - L1 A 2[UINT16] 1 = 0,1A
        pos += 2;
        // AC voltage - L1 V 2[UINT16] 1= 1V
        pos += 2;
        // AC frequency - L1 Hz 2[UINT16] 1= 0,01Hz
        pos += 2;
        // AC active power - L1 W 2[UINT16] 1 = 1W
        pos += 2;
        // AC reactive power - L1 VAR 2[INT16] 1 = 1VAR
        pos += 2;

        // AC current – L2 A 2[UINT16] 1 = 0,1A
        pos += 2;
        // AC voltage – L2 V 2[UINT16] 1= 1V
        pos += 2;
        // AC frequency – L2 Hz 2[UINT16] 1= 0,01Hz
        pos += 2;
        // AC active power – L2 W 2[UINT16] 1 = 1W
        pos += 2;
        // AC reactive power – L2 VAR 2[INT16] 1 = 1VAR
        pos += 2;

        // AC current – L3 A 2[UINT16] 1 = 0,1A
        pos += 2;
        // AC voltage – L3 V 2[UINT16] 1= 1V
        pos += 2;
        // AC frequency – L3 Hz 2[UINT16] 1= 0,01Hz
        pos += 2;
        // AC active power – L3 W 2[UINT16] 1 = 1W
        pos += 2;
        // AC reactive power – L3 VAR 2[INT16] 1 = 1VAR
        pos += 2;

        // solar isolation plus kOhm 2[UINT16] 1 = 1kOhm
        pos += 2;
        // solar isolation minus kOhm 2[UINT16] 1 = 1kOhm
        pos += 2;
        // Temperature amb °C 2[INT16] 1 = 1°C
        pos += 2;
        // Temperature heatsink °C 2[INT16] 1 = 1°C
        pos += 2;
        // Supplied ac energy total Wh 8[UINT64] 1 = 1Wh
        pos += 8;
        // Inverter runtime total Minutes 4[UINT32] 1 = 1 minute
        pos += 4;
        // Status 1 4[UINT32] bit description
        pos += 4;
        // Status 2 4[UINT32] bit description
        pos += 4;
        // Status 3 4[UINT32] bit description
        pos += 4;
        // Status 4 4[UINT32] bit description
        pos += 4;
        // Internal status 1 4[UINT32] reserved bit description
        pos += 4;
        // Internal status 2 4[UINT32] reserved bit description
        pos += 4;
        // Internal status 3 4[UINT32] reserved bit description
        pos += 4;
        // Internal status 4 4[UINT32] reserved bit description
        pos += 4;
        // Supplied ac energy day Wh 8[UINT64] 1 = 1Wh
        publish_sensor_(CONF_INV_SUPPLIED_AC_ENERGY_TODAY,
            extract_int64(&frame[pos]));  // Wh        
        pos += 8;
        // Inverter runtime day Minutes 4[UINT32] 1 = 1 minute
        pos += 4;
        // reserved 67
    }
};

}  // namespace delta_solivia
}  // namespace esphome
