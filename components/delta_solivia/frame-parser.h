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
      if (variant == 15 || variant == 18 || variant == 19 || variant == 20 || variant == 31 ||
          variant == 34 || variant == 35 || variant == 36 || variant == 38 || variant == 39 ||
          variant == 55 || variant == 58 || variant == 59 || variant == 60) {
        return std::make_unique<FrameParserVariant15>();
      }
      if (variant >= 212 && variant <= 222) {
        return std::make_unique<FrameParserVariant212>();
      }
      return nullptr;
    }
};

}
}
