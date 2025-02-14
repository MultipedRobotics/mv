/******************************************************************************
MIT License

Copyright (c) 2019 Kevin J. Walchko

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/
#pragma once

#include "protocol1.hpp"

namespace AX12 {
// EEPROM AREA  ///////////////////////////////////////////////////////////
constexpr uint8_t MODEL_NUMBER_REG       = 0;
constexpr uint8_t VERSION_REG            = 2;
constexpr uint8_t ID_REG                 = 3;
constexpr uint8_t BAUD_RATE_REG          = 4;
constexpr uint8_t RETURN_DELAY_TIME_REG  = 5;
constexpr uint8_t CW_ANGLE_LIMIT_REG     = 6;
constexpr uint8_t CCW_ANGLE_LIMIT_REG    = 8;
constexpr uint8_t SYSTEM_DATA2_REG       = 10;
constexpr uint8_t LIMIT_TEMPERATURE_REG  = 11;
constexpr uint8_t DOWN_LIMIT_VOLTAGE_REG = 12;
constexpr uint8_t UP_LIMIT_VOLTAGE_REG   = 13;
constexpr uint8_t MAX_TORQUE_REG         = 14;
constexpr uint8_t RETURN_LEVEL_REG       = 16;
constexpr uint8_t ALARM_LED_REG          = 17;
constexpr uint8_t ALARM_SHUTDOWN_REG     = 18;
constexpr uint8_t OPERATING_MODE_REG     = 19;
constexpr uint8_t DOWN_CALIBRATION_REG   = 20;
constexpr uint8_t UP_CALIBRATION_REG     = 22;

// RAM AREA  //////////////////////////////////////////////////////////////
constexpr uint8_t TORQUE_ENABLE_REG          = 24;
constexpr uint8_t LED_REG                    = 25;
constexpr uint8_t CW_COMPLIANCE_MARGIN_REG   = 26;
constexpr uint8_t CCW_COMPLIANCE_MARGIN_REG  = 27;
constexpr uint8_t CW_COMPLIANCE_SLOPE_REG    = 28;
constexpr uint8_t CCW_COMPLIANCE_SLOPE_REG   = 29;
constexpr uint8_t GOAL_POSITION_REG          = 30;
constexpr uint8_t GOAL_SPEED_REG             = 32;
constexpr uint8_t TORQUE_LIMIT_REG           = 34;
constexpr uint8_t PRESENT_POSITION_REG       = 36;
constexpr uint8_t PRESENT_SPEED_REG          = 38;
constexpr uint8_t PRESENT_LOAD_REG           = 40;
constexpr uint8_t PRESENT_VOLTAGE_REG        = 42;
constexpr uint8_t PRESENT_TEMPERATURE_REG    = 43;
constexpr uint8_t REGISTERED_INSTRUCTION_REG = 44;
constexpr uint8_t PAUSE_TIME_REG             = 45;
constexpr uint8_t MOVING_REG                 = 46;
constexpr uint8_t LOCK_REG                   = 47;
constexpr uint8_t PUNCH_REG                  = 48;
constexpr uint8_t PUNCH_H_REG                = 49;
constexpr float DEG2CNT                      = 1023.0f / 300.0f;

// struct ServoMoveSpeed_t {
//   uint8_t id;
//   uint16_t count; // 0-1023 counts
//   uint16_t speed; // 0-1023
// };

// typedef std::vector<ServoMoveSpeed_t> SyncVec_t;

// // struct ReadStatus_t {
// //   bool ok;
// //   PktArray_t pkts;
// // };

// struct statuspkt_t {
//   uint8_t id;
//   uint8_t err;
// };


static uint16_t deg2cnt(float deg) {
  deg = deg < 300.0f ? (deg > 0.0f ? deg : 0.0f) : 300.0f;
  deg *= DEG2CNT;
  return static_cast<uint16_t>(deg);
}

inline uint16_t merge16(uint16_t lo, uint16_t hi) { return lo + (hi << 8); }

struct Servo_t {
  uint16_t model_number;
  uint8_t firmware_version;
  uint8_t id;
  uint16_t cw_limit;
  uint16_t ccw_limit;
};

static Servo_t get_servo_info(const mvpkt_t &pkt) {
  Servo_t s;
  s.id = -1;

  if (pkt.size() != 6 + 19) return s; // missing bytes?
  if (pkt[4] != 0) return s;          // err?

  // uint8_t *p = &(pkt.data()[5]);
  const uint8_t *p = pkt.data();
  p += 5; // ??

  s.model_number     = merge16(p[0], p[1]); // p[0] + (p[1] << 8);
  s.firmware_version = p[2];
  s.id               = p[3];
  s.cw_limit         = merge16(p[6], p[7]); // p[6] + (p[7] << 8);
  s.ccw_limit        = merge16(p[8], p[9]); // p[8] + (p[9] << 8);

  return s;
}

// move (angle): 0-1023 counts (0-300 deg)
// speed: 0-1023 counts, in increments of 0.111rpm, default is 0 (motor moves at
// max speed)
// no returned status packet since this uses the broadcast address
static mvpkt_t makeMovePacket(const Protocol1::SyncVec_t &info) {
  return Protocol1::makeSyncWritePacket(GOAL_POSITION_REG, info);
}

// position hex: 0 - 512 - 1023
// static mvpkt_t makeMovePacketCnt(const ServoMoveSpeed_t pkt) {
//   return Protocol1::makeWritePacket(pkt.id, GOAL_POSITION_REG, pkt.count);
// }

// position deg: 0 - 150 - 300
static mvpkt_t makeMovePacketDeg(const uint8_t ID, const float position) {
  uint16_t p = deg2cnt(position);
  return Protocol1::makeWritePacket(ID, GOAL_POSITION_REG, p);
}

inline mvpkt_t makeMovePacket(const uint8_t ID, const uint16_t position) {
  return Protocol1::makeWritePacket(ID, GOAL_POSITION_REG, position);
}

static mvpkt_t makeTorquePacket(const uint8_t ID, const bool enable) {
  const uint8_t enabled = enable ? 1 : 0;
  return Protocol1::makeWritePacket(ID, TORQUE_ENABLE_REG, enabled);
}

} // namespace AX12
