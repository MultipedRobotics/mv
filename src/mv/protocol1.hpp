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
// #include <string.h> // memset
// #include <termcolor/termcolor.hpp>

/*
Sync - write/read to all servos using the same registers/data (smaller)
Bulk - write/read to all servos using different registers/data (bigger)
// */
// namespace AX {
// // EEPROM AREA  ///////////////////////////////////////////////////////////
// // constexpr uint8_t MODEL_NUMBER           =  0;
// // constexpr uint8_t VERSION                =  2;
// constexpr uint8_t ID                     =  3;
// // constexpr uint8_t BAUD_RATE              =  4;
// // constexpr uint8_t RETURN_DELAY_TIME      =  5;
// // constexpr uint8_t CW_ANGLE_LIMIT         =  6;
// // constexpr uint8_t CCW_ANGLE_LIMIT        =  8;
// // constexpr uint8_t SYSTEM_DATA2           =  10;
// // constexpr uint8_t LIMIT_TEMPERATURE      =  11;
// // constexpr uint8_t DOWN_LIMIT_VOLTAGE     =  12;
// // constexpr uint8_t UP_LIMIT_VOLTAGE       =  13;
// // constexpr uint8_t MAX_TORQUE             =  14;
// // constexpr uint8_t RETURN_LEVEL           =  16;
// // constexpr uint8_t ALARM_LED              =  17;
// // constexpr uint8_t ALARM_SHUTDOWN         =  18;
// // constexpr uint8_t OPERATING_MODE         =  19;
// // constexpr uint8_t DOWN_CALIBRATION       =  20;
// // constexpr uint8_t UP_CALIBRATION         =  22;
//
// // RAM AREA  //////////////////////////////////////////////////////////////
// constexpr uint8_t TORQUE_ENABLE           = 24;
// // constexpr uint8_t LED                    =  25;
// // constexpr uint8_t CW_COMPLIANCE_MARGIN   =  26;
// // constexpr uint8_t CCW_COMPLIANCE_MARGIN  =  27;
// // constexpr uint8_t CW_COMPLIANCE_SLOPE    =  28;
// // constexpr uint8_t CCW_COMPLIANCE_SLOPE   =  29;
// // namespace AX {
// // constexpr uint8_t GOAL_POSITION  =       30;
// // }
// constexpr uint8_t GOAL_POSITION          =  30;
// constexpr uint8_t GOAL_SPEED             =  32;
// // constexpr uint8_t TORQUE_LIMIT           =  34;
// constexpr uint8_t PRESENT_POSITION       =  36;
// // constexpr uint8_t PRESENT_SPEED          =  38;
// // constexpr uint8_t PRESENT_LOAD           =  40;
// constexpr uint8_t PRESENT_VOLTAGE        =  42;
// constexpr uint8_t PRESENT_TEMPERATURE    =  43;
// // constexpr uint8_t REGISTERED_INSTRUCTION =  44;
// // constexpr uint8_t PAUSE_TIME             =  45;
// constexpr uint8_t MOVING                 =  46;
// // constexpr uint8_t LOCK                   =  47;
// // constexpr uint8_t PUNCH                  =  48;
// // constexpr uint8_t PUNCH_H              =    49;
//
// // Status Return Levels ///////////////////////////////////////////////////////////////
// // constexpr uint8_t RETURN_NONE              0;
// // constexpr uint8_t RETURN_READ              1;
// // constexpr uint8_t RETURN_ALL               2;
//
// // Instruction Set ///////////////////////////////////////////////////////////////
// // constexpr uint8_t PING                  =   1;
// // constexpr uint8_t READ_DATA              =  2;
// // constexpr uint8_t WRITE_DATA             =  3;
// // constexpr uint8_t REG_WRITE              =  4;
// // constexpr uint8_t ACTION                 =  5;
// // constexpr uint8_t RESET                  =  6;
// // constexpr uint8_t REBOOT                 =  8;
// // constexpr uint8_t SYNC_WRITE             =  0x83;
// // constexpr uint8_t BULK_READ              =  0x92;
//
// // Specials ///////////////////////////////////////////////////////////////
// // #define OFF                         0;
// // #define ON                          1;
// // #define LEFT                        0;
// // #define RIGHT                       1;
// // constexpr uint8_t BYTE_READ             =   1;
// // constexpr uint8_t BYTE_READ_POS          =  2;
// // constexpr uint8_t RESET_LENGTH           =  2;
// // constexpr uint8_t ACTION_LENGTH          =  2;
// // constexpr uint8_t ID_LENGTH              =  4;
// // constexpr uint8_t LR_LENGTH              =  4;
// // constexpr uint8_t SRL_LENGTH             =  4;
// // constexpr uint8_t RDT_LENGTH             =  4;
// // constexpr uint8_t LEDALARM_LENGTH        =  4;
// // constexpr uint8_t SALARM_LENGTH          =  4;
// // constexpr uint8_t TL_LENGTH              =  4;
// // constexpr uint8_t VL_LENGTH              =  6;
// // constexpr uint8_t CM_LENGTH              =  6;
// // constexpr uint8_t CS_LENGTH              =  6;
// // constexpr uint8_t CCW_CW_LENGTH          =  8;
// // constexpr uint8_t BD_LENGTH              =  4;
// // constexpr uint8_t TEM_LENGTH             =  4;
// // constexpr uint8_t MOVING_LENGTH          =  4;
// // constexpr uint8_t RWS_LENGTH             =  4;
// // constexpr uint8_t VOLT_LENGTH            =  4;
// // constexpr uint8_t LED_LENGTH             =  4;
// // constexpr uint8_t TORQUE_LENGTH          =  4;
// // constexpr uint8_t POS_LENGTH             =  4;
// // constexpr uint8_t GOAL_LENGTH            =  5;
// // constexpr uint8_t MT_LENGTH              =  5;
// // constexpr uint8_t PUNCH_LENGTH           =  5;
// // constexpr uint8_t SPEED_LENGTH           =  5;
// // constexpr uint8_t GOAL_SP_LENGTH         =  7;
// // constexpr uint8_t ACTION_CHECKSUM        =  250;
// // constexpr uint8_t BROADCAST_ID           =  254;
// // constexpr uint8_t START                  =  255;
// // constexpr uint8_t CCW_AL                 =  255;
// // #define TIME_OUT                    10
// // #define LOCK                        1
// }

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

    // low level
    packet make_write8_packet(uint8_t ID, uint8_t reg, uint8_t data);
    packet make_write16_packet(uint8_t ID, uint8_t reg, uint16_t data);
    packet make_write_packet(uint8_t ID, uint8_t reg, uint16_t data1, uint16_t data2);
    // single servo
    // packet make_move_packet(uint8_t ID, uint16_t position);
    // packet make_read_position_packet(uint8_t ID);
    packet make_ping_packet(uint8_t ID=BROADCAST_ID);
    packet make_reset_packet(uint8_t ID);
    // packet make_moving_packet(uint8_t ID);
    // packet make_torque_packet(uint8_t ID, bool enable);
    // multi-servo sync
    packet make_sync_write_packet(const uint8_t reg, const std::vector<ServoMove_t>& info);
    packet make_sync_write_packet(const uint8_t reg, const std::vector<ServoMoveSpeed_t>& info);
    // packet make_sync_read_packet(uint8_t address, uint8_t length, const std::vector<uint8_t>& ids);
    // feedback
    void decodePacket(const packet& data);
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
};


// class AX12: public Protocol1 {
// public:
//     AX12(){}
// };
