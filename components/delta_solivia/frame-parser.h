#pragma once

#include <memory>

#include "frame-parser-interface.h"
#include "frame-parser-variant-15.h"
#include "frame-parser-variant-212.h"

namespace esphome {
namespace delta_solivia {

using FrameParser = std::unique_ptr<IFrameParser>;

class FrameParserFactory {
   public:
    static FrameParser createParser(uint8_t variant) {
        if (variant == 15 || variant == 18 || variant == 19 || variant == 20 ||
            variant == 31 || variant == 34 || variant == 35 || variant == 36 ||
            variant == 38 || variant == 39 || variant == 55 || variant == 58 ||
            variant == 59 || variant == 60) {
            return std::make_unique<FrameParserVariant15>();
        }
        if (variant >= 212 && variant <= 222) {
            return std::make_unique<FrameParserVariant212>();
        }
        if (variant == 53 || variant == 73 || variant == 102 ||
            variant == 103 || variant == 105 || variant == 106 ||
            variant == 107 || variant == 109 || variant == 110 ||
            variant == 111 || variant == 113 || variant == 114 ||
            variant == 120 || variant == 121 || variant == 122 ||
            variant == 123 || variant == 124 || variant == 125 ||
            variant == 158 || variant == 159 || variant == 160 ||
            variant == 161) {
            return std::make_unique<FrameParserVariant53>();
        }
        return nullptr;
    }
};

}  // namespace delta_solivia
}  // namespace esphome
