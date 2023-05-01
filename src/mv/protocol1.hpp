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

#include <stdint.h>
#include <vector>

namespace Protocol1 {

constexpr uint8_t INPUT_VOLTAGE_ERR = 1;
constexpr uint8_t ANGLE_LIMIT_ERR   = 2;
constexpr uint8_t OVERHEATING_ERR   = 4;
constexpr uint8_t RANGE_ERR         = 8;
constexpr uint8_t CHECKSUM_ERR      = 16;
constexpr uint8_t OVERLOAD_ERR      = 32;
constexpr uint8_t INSTRUCTION_ERR   = 64;

static uint8_t compute_checksum(const Packet_t &pkt) {
  uint32_t checksum = 0;
  for (uint8_t i = 2; i < pkt.size(); ++i)
    checksum += pkt[i];
  return (~checksum) & 0xFF;
}

static bool valid_packet(const Packet_t &pkt) {
  if (pkt.size() == 0) return false;
  if ((pkt[0] != 0xff) || (pkt[1] != 0xff)) return false;
  uint8_t chksum = pkt.back();
  return chksum == compute_checksum(pkt);
}

// https://github.com/jumejume1/AX-12A-servo-library/blob/master/src/AX12A.cpp
// https://github.com/mikeferguson/etherbotix/blob/ros2/include/etherbotix/dynamixel.hpp
/**
 * Instruction Packets structure:
 *
 * +----+----+--+------+-------+---------+
 * |0xFF|0xFF|ID|LENGTH|DATA...|CHECKSUM |
 * +----+----+--+------+-------+---------+
 *
 * LENGTH = 4 + number_params + checksum
 * Checksum = ~( ID + Length + Instruction + Parameter1 + ... + ParameterN )
 *
 * Status packet structure:
 *
 * +----+----+--+------+-----+-------+---------+
 * |0xFF|0xFF|ID|LENGTH|ERROR|DATA...|CHECKSUM |
 * +----+----+--+------+-----+-------+---------+
 *
 * LENGTH = 4 + error + number_params + checksum
 * Checksum = ~( ID + Length + Error + Parameter1 + ... + ParameterN )
 */
class PacketManager {

  static constexpr uint8_t BROADCAST_ID = 0xFE;
  static constexpr uint8_t START        = 0xFF;
  static constexpr uint8_t PING         = 1;
  static constexpr uint8_t READ_DATA    = 2;
  static constexpr uint8_t WRITE_DATA   = 3;
  static constexpr uint8_t REG_WRITE    = 4;
  static constexpr uint8_t ACTION       = 5;
  static constexpr uint8_t RESET        = 6;
  static constexpr uint8_t REBOOT       = 8;
  static constexpr uint8_t SYNC_WRITE   = 0x83;
  static constexpr uint8_t BULK_READ    = 0x92;
  static constexpr uint8_t READ8        = 1;
  static constexpr uint8_t READ16       = 2;

  enum PacketState_t : uint8_t { NONE, HD0, HD1, ID, LEN, DATA, CHECKSUM };

  struct DecodePacketState_t {
    int data_len;
    int offset;
    uint8_t state;
    int pkt_len;
  } packet_state;

public:
  PacketManager() {}
  ~PacketManager() {}

  Packet_t gpkt; // global packet

  // low level write
  Packet_t makeWritePacket(uint8_t ID, uint8_t reg, uint8_t data) {
    const uint8_t len      = 4;
    const uint8_t Checksum = (~(ID + len + WRITE_DATA + reg + data)) & 0xFF;

    Packet_t pkt{START, START, ID, len, WRITE_DATA, reg, data, Checksum};

    return pkt;
  }

  Packet_t makeWritePacket(uint8_t ID, uint8_t reg, uint16_t data) {
    const uint8_t hi       = (data >> 8) & 0xFF;
    const uint8_t lo       = data & 0xFF;
    constexpr uint8_t LEN  = 5;
    const uint8_t Checksum = (~(ID + LEN + WRITE_DATA + reg + lo + hi)) & 0xFF;
    return Packet_t{START, START, ID, LEN, WRITE_DATA, reg, lo, hi, Checksum};
  }

  Packet_t makeWritePacket(uint8_t ID, uint8_t reg, uint16_t data1,
                           uint16_t data2) {
    const uint8_t hi1     = data1 >> 8;
    const uint8_t lo1     = data1 & 0xff;
    const uint8_t hi2     = data2 >> 8;
    const uint8_t lo2     = data2 & 0xff;
    constexpr uint8_t LEN = 7;
    const uint8_t Checksum =
        (~(ID + LEN + WRITE_DATA + reg + lo1 + hi1 + lo2 + hi2)) & 0xFF;

    Packet_t pkt{START, START, ID,  LEN, WRITE_DATA, reg,
                 lo1,   hi1,   lo2, hi2, Checksum};

    return pkt;
  }

  Packet_t makeReadPacket(uint8_t ID, uint8_t reg, uint8_t read_len) {
    constexpr uint8_t LEN = 4;
    uint8_t Checksum      = (~(ID + LEN + READ_DATA + reg + read_len)) & 0xFF;
    return Packet_t{START, START, ID, LEN, READ_DATA, reg, read_len, Checksum};
  }

  Packet_t makePingPacket(uint8_t ID = BROADCAST_ID) {
    constexpr uint8_t LEN  = 2;
    const uint8_t Checksum = (~(ID + LEN + PING)) & 0xFF;
    return Packet_t{START, START, ID, LEN, PING, Checksum};
  }

  Packet_t makeResetPacket(uint8_t ID) {
    constexpr uint8_t LEN  = 2;
    const uint8_t Checksum = (~(ID + LEN + RESET)) & 0xFF;
    return Packet_t{START, START, ID, LEN, RESET, Checksum};
  }

  Packet_t makeRebootPacket(uint8_t ID) {
    constexpr uint8_t LEN  = 2;
    const uint8_t Checksum = (~(ID + LEN + REBOOT)) & 0xFF;
    return Packet_t{START, START, ID, LEN, REBOOT, Checksum};
  }

  // multi-servo sync
  Packet_t makeSyncWritePacket(const uint8_t reg, const SyncVec_t &info) {
  // Packet_t makeMovePacket(const uint8_t reg, const SyncVec_t &info) {
    const bool do_speed       = info[0].speed == 0 ? false : true;
    const uint8_t info_bytes  = do_speed ? 5 : 3;
    const uint8_t reg_len     = do_speed ? 4 : 2;
    const uint8_t payload_len = (reg_len + 1) * info.size() + 4;
    // Packet_t pkt = Packet_t(payload_len + 4);
    Packet_t pkt(payload_len + 4);

    pkt[0] = START;
    pkt[1] = START;
    pkt[2] = BROADCAST_ID;
    pkt[3] = payload_len; // static_cast<uint8_t>(payload_len);
    pkt[4] = SYNC_WRITE;
    pkt[5] = reg;
    pkt[6] = reg_len;

    uint8_t hi, lo;
    int i = 7;
    for (ServoMoveSpeed_t const &s : info) {
      pkt[i++] = s.id;

      // angle
      hi       = s.count >> 8;
      lo       = s.count & 0xff;
      pkt[i++] = lo;
      pkt[i++] = hi;
      // mem.val = s.count;
      // pkt[i++] = mem.lo;
      // pkt[i++] = mem.hi;

      // speed
      if (do_speed) {
        hi       = s.speed >> 8;
        lo       = s.speed & 0xff;
        pkt[i++] = lo;
        pkt[i++] = hi;
        // mem.val = s.speed;
        // pkt[i++] = mem.lo;
        // pkt[i++] = mem.hi;
      }
    }
    uint8_t csum = compute_checksum(pkt);
    pkt[i]       = csum;
    return pkt;
  }

  // uint8_t statusError(const Packet_t &pkt) { return pkt[4]; } // only status packets

  bool readPacket(uint8_t b) {
    switch (packet_state.state) {
    case NONE:
      if (b == 0xff) packet_state.state = HD0;
      // buffer[0] = b;
      gpkt.clear();
      gpkt.push_back(b);
      break;
    case HD0:
      if (b == 0xff) packet_state.state = HD1;
      else packet_state.state = NONE;
      // buffer[1] = b;
      gpkt.push_back(b);
      break;
    case HD1:
      packet_state.state = ID;
      // buffer[2]          = b;
      gpkt.push_back(b);
      break;
    case ID:
      packet_state.state    = LEN;
      packet_state.data_len = b;
      // buffer[3]             = b;
      gpkt.push_back(b);
      break;
    case LEN:
      packet_state.offset   = 0;
      packet_state.data_len = b;
      packet_state.pkt_len  = 4 + b;
      // buffer[4]             = b;
      gpkt.push_back(b);
      packet_state.state = DATA;
    case DATA:
      if (--packet_state.data_len > 0) {
        // buffer[4 + (++packet_state.offset)] = b;
        gpkt.push_back(b);
      }
      else packet_state.state = CHECKSUM;
      break;
    case CHECKSUM:
      // buffer[4 + (++packet_state.offset)] = b;
      gpkt.push_back(b);
      // Packet_t pkt(buffer, buffer + packet_state.pkt_len);
      packet_state.offset = 0;
      packet_state.state  = NONE;
      return true;
    }
    return false;
  }
};

} // namespace Protocol1
