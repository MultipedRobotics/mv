#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <stdint.h>

typedef uint8_t byte;
typedef std::vector<byte> packet;
typedef std::vector<byte> parameters;

/*
Sync - write/read to all servos using the same registers/data (smaller)
Bulk - write/read to all servos using different registers/data (bigger)
*/
class Protocol1 {
public:
    Protocol1(){}
    ~Protocol1(){}

    // calculate the checksum of the data packet
    byte check_sum(const std::vector<byte>& data){
        int sum_data;
        for (auto const& d: data) sum_data += d;
        sum_data -= 0xff; // FIXME: hack because i don't want to sum header bytes
        sum_data -= 0xff; // FIXME
        return (255 - sum_data & 255);
    }

    packet makePacket(byte id, byte instr, const packet& params={}){
        packet pkt {0xff, 0xff, id};

        if (params.size() > 0){
            pkt.push_back(params.size() + 2);
            pkt.push_back(instr);
            for (auto const& p: params) pkt.push_back(p);
            byte csum = check_sum(pkt);
            pkt.push_back(csum);
        }
        else {
            pkt.push_back(2);
            pkt.push_back(instr);
            byte csum = check_sum(pkt);
            pkt.push_back(csum);
        }

        return std::move(pkt);
    }

    packet make_ping_packet(byte id){}
    packet make_reset_packet(byte id, byte level){}
    packet make_reboot_packet(byte id){}
    packet make_angle_packet(byte id, double angle){}
    packet make_angle_packet(byte id, int angle){}
    packet make_led_packet(byte id, byte value){}
    packet make_speed_packet(byte id, uint16_t speed){}

    packet make_write_packet(byte id, byte reg, const parameters& values={}){}
    packet make_read_packet(byte id, byte reg, const parameters& values={}){}

    packet make_sync_write_packet(byte id, byte reg){}
    packet make_sync_read_packet(byte id, byte reg){}
    packet make_sync_angle_packet(byte id, double angle){}
    packet make_sync_angle_packet(byte id, int angle){}

    uint16_t crc(const std::vector<byte>& data){}
    void decodePacket(const packet& data){}
};

//
// #include <SerialPort.hpp>
//
// // --------- INSTRUCTIONS -----
// constexpr byte PING      = 0x01;
// constexpr byte READ      = 0x02;
// constexpr byte WRITE     = 0x03;
// constexpr byte REG_WRITE = 0x04;
// constexpr byte ACTION    = 0x05;
// constexpr byte RESET     = 0x06;
// // constexpr byte REBOOT    = 0x08;
// // constexpr byte STATUS    = 0x55;
// // constexpr byte SYNC_READ  = 0x82;
// constexpr byte SYNC_WRITE = 0x83;
// constexpr byte BULK_READ  = 0x92;
// // constexpr byte BULK_WRITE = 0x93;
//
// // -------- EEPROM -------------
// constexpr byte MODEL_NUMBER    = 0;
// constexpr byte VER_FIRMWARE    = 2;
// constexpr byte ID              = 3;
// constexpr byte BAUD_RATE       = 4;
// constexpr byte DELAY_TIME      = 5;
// constexpr byte CW_ANGLE_LIMIT  = 6;  // min angle, default 0
// constexpr byte CCW_ANGLE_LIMIT = 8;   // max angle, default 300
// constexpr byte CONTROL_MODE    = 11;  // joint or wheel mode, default joint (servo)
// constexpr byte MAX_TORQUE      = 15;
// constexpr byte RETURN_LEVEL    = 17;
//
// // -------- RAM ----------------
// constexpr byte TORQUE_ENABLE    = 24;  // servo mode on/off - turn into wheel
// constexpr byte LED              = 25;
// constexpr byte GOAL_POSITION    = 30;
// constexpr byte GOAL_VELOCITY    = 32;
// constexpr byte GOAL_TORQUE      = 35;
// constexpr byte PRESENT_POSITION = 37;  // current servo angle
// constexpr byte PRESENT_SPEED    = 39;  // current speed
// constexpr byte PESENT_LOAD      = 41;  // current load
// constexpr byte PESENT_VOLTAGE   = 45;  // current voltage
// constexpr byte PESENT_TEMP      = 46;  // current temperature
// constexpr byte MOVING           = 49;
// constexpr byte HW_ERROR_STATUS  = 50;
// constexpr byte PUNCH            = 51;
//
// // --------- OTHER -------------
// constexpr byte RESET_ALL                  = 0xFF;
// constexpr byte RESET_ALL_BUT_ID           = 0x01;
// constexpr byte RESET_ALL_BUT_ID_BAUD_RATE = 0x02;
// constexpr byte LED_ON                     = 1;
// constexpr byte LED_OFF                    = 0;
// constexpr byte BROADCAST_ADDR             = 0xfe;  // a packet with this ID will go to all servos
// constexpr byte WHEEL_MODE                 = 1;
// constexpr byte JOINT_MODE                 = 2;  // normal servo
// constexpr byte DR_1000000                 = 1;  // bps = 2000000/(data + 1)
//
//
//
// class AX12: public Protocol1 {
// public:
//     AX12(): io(), serial(io){}
//     ~AX12(){}
//     void open(const std::string& port, int pin){
//         serial.open(port, pin);
//     }
//
//     // const int MAX_RPM {59.0/0.111};
//     const int MAX_RPM = 59.0/0.111;
//     const std::string name {"AX-12A"};
//
// protected:
//     boost::asio::io_service io;
//     SerialPort serial;
// };
