#pragma once

#include <memory>
#include "frame-parser-interface.h"
#include "frame-parser-variant-15.h"
#include "frame-parser-variant-27.h"
#include "frame-parser-variant-53.h"
#include "frame-parser-variant-212.h"

namespace esphome {
namespace delta_solivia {

using FrameParser = std::unique_ptr<IFrameParser>;

class FrameParserFactory {
public:
  static FrameParser create_parser(uint8_t variant) {
    if (FrameParserVariant15::supports(variant)) {
      return std::make_unique<FrameParserVariant15>();
    }
    if (FrameParserVariant53::supports(variant)) {
      return std::make_unique<FrameParserVariant53>();
    }
    if (FrameParserVariant27::supports(variant)) {
      return std::make_unique<FrameParserVariant27>();
    }
    if (FrameParserVariant212::supports(variant)) {
      return std::make_unique<FrameParserVariant212>();
    }
    return nullptr;
  }
};

}
}
