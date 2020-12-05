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

void pprint(const packet& pkt, bool hex=false);

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
    uint8_t compute_checksum(packet& pkt);
    // const uint8_t START;

public:
    Protocol1();
    ~Protocol1();

    // low level write
    packet make_write8_packet(uint8_t ID, uint8_t reg, uint8_t data);
    packet make_write16_packet(uint8_t ID, uint8_t reg, uint16_t data);
    packet make_write_packet(uint8_t ID, uint8_t reg, uint16_t data1, uint16_t data2);
    // low level read
    packet make_read_packet(uint8_t ID, uint8_t reg, uint8_t read_len);
    // single servo
    packet make_ping_packet(uint8_t ID=BROADCAST_ID);
    packet make_reset_packet(uint8_t ID);
    // multi-servo sync
    packet make_sync_write_packet(const uint8_t reg, const std::vector<ServoMoveSpeed_t>& info);
    // packet make_sync_read_packet(uint8_t address, uint8_t length, const std::vector<uint8_t>& ids);
    // feedback
    // void decodePacket(const packet& data);
    uint8_t status_error(const packet& pkt);
    AxError decode_error(uint8_t b);

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
