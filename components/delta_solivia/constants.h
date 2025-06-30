#pragma once

#include <cstdint>

// logging tag
static const char *const LOG_TAG = "DeltaSolivia";

// protocol characters (page 7)
static const uint8_t STX = 0x02;
static const uint8_t ETX = 0x03;
static const uint8_t ENQ = 0x05;
static const uint8_t ACK = 0x06;
static const uint8_t NAK = 0x15;
