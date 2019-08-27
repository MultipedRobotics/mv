#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <stdint.h>
#include <common.hpp>
// typedef uint8_t byte;
// typedef std::vector<byte> packet;
// typedef std::vector<byte> parameters;

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
