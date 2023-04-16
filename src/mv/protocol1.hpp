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

#include <iostream>
#include <string>
#include <vector>
#include <stdint.h>
#include <mv/common.hpp>

typedef struct {
    bool input_voltage; // 0
    bool angle_limit;   // 1
    bool overheating;   // 2
    bool range;         // 3
    bool checksum;      // 4
    bool overload;      // 5
    bool instruction;   // 6
} AxError;

// void pprint(const packet& pkt, bool hex=false);

// https://github.com/jumejume1/AX-12A-servo-library/blob/master/src/AX12A.cpp
// https://github.com/mikeferguson/etherbotix/blob/ros2/include/etherbotix/dynamixel.hpp
/**
 * Packets structure:
 *
 * +----+----+--+------+-------+---------+
 * |0xFF|0xFF|ID|LENGTH|DATA...|CHECK SUM|
 * +----+----+--+------+-------+---------+
 *
 */
class Protocol1 {
    uint8_t compute_checksum(const packet& pkt) {
      uint32_t checksum = 0;
      for (uint8_t i = 2; i < pkt.size(); ++i) checksum += pkt[i];
      // return (255 - checksum);
      return (~checksum) & 0xFF;
    }
    // const uint8_t START;

    enum PacketState_t: uint8_t {
      NONE,
      HD0,
      HD1,
      ID,
      LEN,
      DATA,
      CHECKSUM
    };

    struct DecodePacketState_t {
      int data_len;
      // int id;
      int offset;
      uint8_t state;
    };

    DecodePacketState_t packet_state;
    uint8_t buffer[64]; // vector? too big?

public:
    Protocol1() {}
    ~Protocol1() {}

    // low level write
    // packet make_write8_packet(uint8_t ID, uint8_t reg, uint8_t data);

    packet make_write16_packet(uint8_t ID, uint8_t reg, uint16_t data) {
      const uint8_t hi = data >> 8;
      const uint8_t lo = data & 0xff;
      const uint8_t len = 5;
      const uint8_t Checksum = (~(ID + len + WRITE_DATA + reg + lo + hi)) & 0xFF;

      packet pkt {START,START,ID,len,WRITE_DATA,reg,lo,hi,Checksum};

      return pkt;
    }

    // packet make_write_packet(uint8_t ID, uint8_t reg, uint16_t data1, uint16_t data2);
    // low level read
    // packet make_read_packet(uint8_t ID, uint8_t reg, uint8_t read_len);
    // single servo
    packet make_ping_packet(uint8_t ID=BROADCAST_ID) {
      const uint8_t Checksum = (~(ID + 0x02 + PING)) & 0xFF;
      packet pkt {START, START, ID, 0x02, PING, Checksum};
      return pkt;
    }

    packet make_reset_packet(uint8_t ID) {
      const uint8_t len = 2;
      const uint8_t Checksum = (~(ID + len + RESET)) & 0xFF;

      packet pkt {START, START, ID, len, RESET, Checksum};

      return pkt;
    }

    // multi-servo sync
    packet make_sync_write_packet(const uint8_t reg, const std::vector<ServoMoveSpeed_t>& info) {
      const bool do_speed = info[0].speed == 0 ? false : true;
      const uint8_t info_bytes = do_speed ? 5 : 3;
      const uint8_t reg_len = do_speed ? 4 : 2;
      const uint8_t payload_len = (reg_len+1)*info.size()+4;
      // cout << "payload_len: " << int(payload_len) <<endl;
      packet pkt = packet(payload_len+4);

      pkt[0] = START;
      pkt[1] = START;
      pkt[2] = BROADCAST_ID;
      pkt[3] = payload_len; //static_cast<uint8_t>(payload_len);
      pkt[4] = SYNC_WRITE;
      pkt[5] = reg;
      pkt[6] = reg_len;

      // union {
      //   struct {
      //     uint8_t hi,lo;
      //   };
      //   uint16_t val;
      // } mem;

      uint8_t hi, lo;
      int i = 7;
      for (auto const& s: info) {
        pkt[i++] = s.id;

        // angle
        hi = s.count >> 8;
        lo = s.count & 0xff;
        pkt[i++] = lo;
        pkt[i++] = hi;
        // mem.val = s.count;
        // pkt[i++] = mem.lo;
        // pkt[i++] = mem.hi;

        // speed
        if (do_speed){
          hi = s.speed >> 8;
          lo = s.speed & 0xff;
          pkt[i++] = lo;
          pkt[i++] = hi;
          // mem.val = s.speed;
          // pkt[i++] = mem.lo;
          // pkt[i++] = mem.hi;
        }
      }
      uint8_t csum = compute_checksum(pkt);
      pkt[i] = csum;
      return pkt;
    }
    // packet make_sync_read_packet(uint8_t address, uint8_t length, const std::vector<uint8_t>& ids);
    // feedback
    // void decodePacket(const packet& data);
    uint8_t status_error(const packet& pkt) { return pkt[4]; }

    // AxError decode_error(uint8_t b);

    bool get_packet(uint8_t b) {
      switch (packet_state.state) {
        case NONE:
          if (b == 0xff) packet_state.state = HD0;
          buffer[0] = b;
          break;
        case HD0:
          if (b == 0xff) packet_state.state = HD1;
          else packet_state.state = NONE;
          buffer[1] = b;
          break;
        case HD1:
          packet_state.state = ID;
          buffer[2] = b;
          break;
        case ID:
          packet_state.state = LEN;
          packet_state.data_len = b;
          buffer[3] = b;
          break;
        case LEN:
          packet_state.offset = 0;
          packet_state.data_len = b;
          buffer[4] = b;
          packet_state.state = DATA;
        case DATA:
          if (--packet_state.data_len > 0) {
            buffer[4 + (++packet_state.offset)] = b;
          }
          else packet_state.state = CHECKSUM;
          break;
        case CHECKSUM:
          buffer[4 + (++packet_state.offset)] = b;
          packet_state.offset = 0;
          packet_state.state = NONE;
          return true;
      }
      return false;
    }

    bool valid_packet(const packet& pkt) {
      // uint8_t size = pkt.size();
      uint8_t chksum = pkt.back();
      if ((pkt[0] != 0xff) || (pkt[1] != 0xff)) return false;
      return chksum == compute_checksum(pkt);
    }

    static const uint8_t BROADCAST_ID = 0xFE;
    static const uint8_t START        = 0xFF;
    static const uint8_t PING         = 1;
    static const uint8_t READ_DATA    = 2;
    static const uint8_t WRITE_DATA   = 3;
    static const uint8_t REG_WRITE    = 4;
    static const uint8_t ACTION       = 5;
    static const uint8_t RESET        = 6;
    static const uint8_t REBOOT       = 8;
    static const uint8_t SYNC_WRITE   = 0x83;
    static const uint8_t BULK_READ    = 0x92;
    static const uint8_t READ8        = 1;
    static const uint8_t READ16       = 2;
};
