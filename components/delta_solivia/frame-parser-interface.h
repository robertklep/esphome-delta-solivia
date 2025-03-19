#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <functional>
#include "frame-parser-consts.h"

namespace esphome {
namespace delta_solivia {

using PublishSensorHandler = std::function<void(const std::string&, float)>;
using PublishTextSensorHandler = std::function<void(const std::string&, const std::string&)>;

class IFrameParser {
protected:
  PublishSensorHandler publish_sensor_;
  PublishTextSensorHandler publish_text_sensor_;

  int16_t extract_int16(const uint8_t* data) {
    return (data[0] << 8) | data[1];
  }

  uint32_t extract_int32(const uint8_t* data) {
    return (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
  }

  uint64_t extract_int64(const uint8_t *data) {
    return ((uint64_t) data[0] << 56) | ((uint64_t) data[1] << 48) | ((uint64_t) data[2] << 40) |
           ((uint64_t) data[3] << 32) | ((uint64_t) data[4] << 24) | ((uint64_t) data[5] << 16) |
           ((uint64_t) data[6] << 8)  | (uint64_t)  data[7];
  }

  std::string parse_string(const uint8_t* frame, std::size_t pos, int length) {
    std::string result(frame + pos, frame + pos + length);
    return result;
  }

  virtual void parse_frame_(const uint8_t*, std::size_t) = 0;

public:
  void parse_frame(const uint8_t* frame, bool skip_header = false) {
    std::size_t pos = skip_header ? 6 : 0;
    this->parse_frame_(frame, pos);
  }

  void set_publish_sensor(PublishSensorHandler publish_sensor) {
    this->publish_sensor_ = publish_sensor;
  }

  void set_publish_text_sensor(PublishTextSensorHandler publish_text_sensor) {
    this->publish_text_sensor_ = publish_text_sensor;
  }
};

}
}
