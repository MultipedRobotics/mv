#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <stdint.h>
#include <mv/common.hpp>

/*
Sync - write/read to all servos using the same registers/data (smaller)
Bulk - write/read to all servos using different registers/data (bigger)
*/
namespace AX {
// EEPROM AREA  ///////////////////////////////////////////////////////////
constexpr uint8_t MODEL_NUMBER           =  0;
constexpr uint8_t VERSION                =  2;
constexpr uint8_t ID                     =  3;
constexpr uint8_t BAUD_RATE              =  4;
constexpr uint8_t RETURN_DELAY_TIME      =  5;
constexpr uint8_t CW_ANGLE_LIMIT         =  6;
constexpr uint8_t CCW_ANGLE_LIMIT        =  8;
constexpr uint8_t SYSTEM_DATA2           =  10;
constexpr uint8_t LIMIT_TEMPERATURE      =  11;
constexpr uint8_t DOWN_LIMIT_VOLTAGE     =  12;
constexpr uint8_t UP_LIMIT_VOLTAGE       =  13;
constexpr uint8_t MAX_TORQUE             =  14;
constexpr uint8_t RETURN_LEVEL           =  16;
constexpr uint8_t ALARM_LED              =  17;
constexpr uint8_t ALARM_SHUTDOWN         =  18;
constexpr uint8_t OPERATING_MODE         =  19;
constexpr uint8_t DOWN_CALIBRATION       =  20;
constexpr uint8_t UP_CALIBRATION         =  22;

// RAM AREA  //////////////////////////////////////////////////////////////
constexpr uint8_t TORQUE_ENABLE           = 24;
constexpr uint8_t LED                    =  25;
constexpr uint8_t CW_COMPLIANCE_MARGIN   =  26;
constexpr uint8_t CCW_COMPLIANCE_MARGIN  =  27;
constexpr uint8_t CW_COMPLIANCE_SLOPE    =  28;
constexpr uint8_t CCW_COMPLIANCE_SLOPE   =  29;
// namespace AX {
// constexpr uint8_t GOAL_POSITION  =       30;
// }
constexpr uint8_t GOAL_POSITION          =  30;
constexpr uint8_t GOAL_SPEED             =  32;
constexpr uint8_t TORQUE_LIMIT           =  34;
constexpr uint8_t PRESENT_POSITION       =  36;
constexpr uint8_t PRESENT_SPEED          =  38;
constexpr uint8_t PRESENT_LOAD           =  40;
constexpr uint8_t PRESENT_VOLTAGE        =  42;
constexpr uint8_t PRESENT_TEMPERATURE    =  43;
constexpr uint8_t REGISTERED_INSTRUCTION =  44;
constexpr uint8_t PAUSE_TIME             =  45;
constexpr uint8_t MOVING                 =  46;
constexpr uint8_t LOCK                   =  47;
constexpr uint8_t PUNCH                  =  48;
// constexpr uint8_t PUNCH_H              =    49;

// Status Return Levels ///////////////////////////////////////////////////////////////
// constexpr uint8_t RETURN_NONE              0;
// constexpr uint8_t RETURN_READ              1;
// constexpr uint8_t RETURN_ALL               2;

// Instruction Set ///////////////////////////////////////////////////////////////
constexpr uint8_t PING                  =   1;
constexpr uint8_t READ_DATA              =  2;
constexpr uint8_t WRITE_DATA             =  3;
constexpr uint8_t REG_WRITE              =  4;
constexpr uint8_t ACTION                 =  5;
constexpr uint8_t RESET                  =  6;
constexpr uint8_t SYNC_WRITE             =  131;
constexpr uint8_t SYNC_READ              =  132;

// Specials ///////////////////////////////////////////////////////////////
// #define OFF                         0;
// #define ON                          1;
// #define LEFT                        0;
// #define RIGHT                       1;
constexpr uint8_t BYTE_READ             =   1;
constexpr uint8_t BYTE_READ_POS          =  2;
constexpr uint8_t RESET_LENGTH           =  2;
constexpr uint8_t ACTION_LENGTH          =  2;
constexpr uint8_t ID_LENGTH              =  4;
constexpr uint8_t LR_LENGTH              =  4;
constexpr uint8_t SRL_LENGTH             =  4;
constexpr uint8_t RDT_LENGTH             =  4;
constexpr uint8_t LEDALARM_LENGTH        =  4;
constexpr uint8_t SALARM_LENGTH          =  4;
constexpr uint8_t TL_LENGTH              =  4;
constexpr uint8_t VL_LENGTH              =  6;
constexpr uint8_t CM_LENGTH              =  6;
constexpr uint8_t CS_LENGTH              =  6;
constexpr uint8_t CCW_CW_LENGTH          =  8;
constexpr uint8_t BD_LENGTH              =  4;
constexpr uint8_t TEM_LENGTH             =  4;
constexpr uint8_t MOVING_LENGTH          =  4;
constexpr uint8_t RWS_LENGTH             =  4;
constexpr uint8_t VOLT_LENGTH            =  4;
constexpr uint8_t LED_LENGTH             =  4;
constexpr uint8_t TORQUE_LENGTH          =  4;
constexpr uint8_t POS_LENGTH             =  4;
constexpr uint8_t GOAL_LENGTH            =  5;
constexpr uint8_t MT_LENGTH              =  5;
constexpr uint8_t PUNCH_LENGTH           =  5;
constexpr uint8_t SPEED_LENGTH           =  5;
constexpr uint8_t GOAL_SP_LENGTH         =  7;
constexpr uint8_t ACTION_CHECKSUM        =  250;
constexpr uint8_t BROADCAST_ID           =  254;
constexpr uint8_t START                  =  255;
constexpr uint8_t CCW_AL                 =  255;
// #define TIME_OUT                    10
// #define LOCK                        1
}


// https://github.com/jumejume1/AX-12A-servo-library/blob/master/src/AX12A.cpp
// https://github.com/mikeferguson/etherbotix/blob/ros2/include/etherbotix/dynamixel.hpp
class Protocol1 {
    uint8_t compute_checksum(packet& pkt){
        uint32_t checksum = 0;
        for (uint8_t i = 2; i < pkt.size(); ++i) checksum += pkt[i];
        // return (255 - checksum);
        return (~checksum) & 0xFF;
    }

public:
    Protocol1();
    ~Protocol1(){}

    packet make_write_packet(uint8_t ID, uint8_t reg, uint16_t data){
        uint8_t hi = data >> 8;
        uint8_t lo = data & 0xff;
        uint8_t Checksum = (~(ID + 2 + AX::WRITE_DATA + reg + lo + hi)) & 0xFF;

        packet pkt {AX::START,AX::START,ID,2,AX::WRITE_DATA,reg,lo,hi,Checksum};

        return pkt;
    }

    packet make_write_packet(uint8_t ID, uint8_t reg, uint16_t data1, uint16_t data2){
        uint8_t hi1 = data1 >> 8;
        uint8_t lo1 = data1 & 0xff;
        uint8_t hi2 = data2 >> 8;
        uint8_t lo2 = data2 & 0xff;
        uint8_t Checksum = (~(ID + 4 + AX::WRITE_DATA + reg + lo1 + hi1 + lo2+hi2)) & 0xFF;

        packet pkt {AX::START,AX::START,ID,4,AX::WRITE_DATA,reg,lo1,hi1,lo2,hi2,Checksum};

        return pkt;
    }

    packet make_move_packet(uint8_t ID, int Position){
        return make_write_packet(ID, AX::GOAL_POSITION, Position);
    }

    packet make_read_position_packet(uint8_t ID){
        uint8_t Checksum = (~(ID + AX::POS_LENGTH + AX::READ_DATA + AX::PRESENT_POSITION+ AX::BYTE_READ_POS)) & 0xFF;

        packet pkt {AX::START,AX::START,ID,AX::POS_LENGTH,AX::READ_DATA,AX::PRESENT_POSITION,AX::BYTE_READ_POS,Checksum};

        return pkt;
    }

    packet make_ping_packet(uint8_t ID){
        uint8_t Checksum = (~(ID + AX::READ_DATA + AX::PING)) & 0xFF;
        packet pkt {AX::START, AX::START, ID, AX::READ_DATA, AX::PING, Checksum};
        return pkt;
    }

    packet make_reset_packet(uint8_t ID){
        uint8_t Checksum = (~(ID + AX::RESET_LENGTH + AX::RESET)) & 0xFF;

        packet pkt {AX::START, AX::START, ID, AX::RESET_LENGTH, AX::RESET, Checksum};

        return pkt;
    }

    packet make_moving_packet(uint8_t ID){
        auto pkt = packet(8);
        uint8_t Checksum = (~(ID + AX::MOVING_LENGTH + AX::READ_DATA + AX::MOVING + AX::BYTE_READ)) & 0xFF;

        pkt[0] = AX::START;
        pkt[1] = AX::START;
        pkt[2] = ID;
        pkt[3] = AX::MOVING_LENGTH;
        pkt[4] = AX::READ_DATA;
        pkt[5] = AX::MOVING;
        pkt[6] = AX::BYTE_READ;
        pkt[7] = Checksum;

        return pkt;
    }

    packet make_sync_write_packet(std::vector<ServoMove_t>& info){
        uint8_t info_len = info.size()*5+2;
        packet pkt {0xff, 0xff, AX::BROADCAST_ID, static_cast<uint8_t>(info_len+2),
            AX::SYNC_WRITE, AX::GOAL_POSITION, 2};
        pkt.reserve(3+info_len+3);

        uint8_t hi, lo;
        for (auto const& s: info) {
            pkt.push_back(s.id);
            // angle
            hi = s.count >> 8;
            lo = s.count & 0xff;
            pkt.push_back(lo);
            pkt.push_back(hi);
        }
        uint8_t csum = compute_checksum(pkt);
        pkt.push_back(csum);
        return pkt;
    }

    packet make_sync_write_packet(std::vector<ServoMoveSpeed_t>& info){
        uint8_t info_len = info.size()*5+2;
        packet pkt {0xff, 0xff, AX::BROADCAST_ID, static_cast<uint8_t>(info_len+2),
            AX::SYNC_WRITE, AX::GOAL_POSITION, 4};
        pkt.reserve(3+info_len+3);

        uint8_t hi, lo;
        for (auto const& s: info) {
            pkt.push_back(s.id);
            // angle
            hi = s.count >> 8;
            lo = s.count & 0xff;
            pkt.push_back(lo);
            pkt.push_back(hi);
            // speed
            hi = s.speed >> 8;
            lo = s.speed & 0xff;
            pkt.push_back(lo);
            pkt.push_back(hi);
        }
        uint8_t csum = compute_checksum(pkt);
        pkt.push_back(csum);
        return pkt;
    }

    packet make_sync_read_packet(uint8_t address, uint8_t length, std::vector<uint8_t>& ids){
        auto pkt = packet(8 + ids.size());
        uint8_t len = 0;

        pkt[len++] = 0xff;
        pkt[len++] = 0xff;
        pkt[len++] = AX::BROADCAST_ID;
        pkt[len++] = 4 + ids.size();  // Length of remaining packet
        pkt[len++] = AX::SYNC_READ;
        pkt[len++] = address;
        pkt[len++] = length;
        for (auto id : ids) pkt[len++] = id;
        pkt[len++] = compute_checksum(pkt); // not right?
        return pkt;
    }

    void decodePacket(const packet& data){

    }
};


class AX12: public Protocol1 {
public:
    AX12(){}
};



    // calculate the checksum of the data packet
    // uint8_t check_sum(const std::vector<uint8_t>& data){
    //     int sum_data;
    //     for (auto const& d: data) sum_data += d;
    //     sum_data -= 0xff; // FIXME: hack because i don't want to sum header bytes
    //     sum_data -= 0xff; // FIXME
    //     return (255 - sum_data & 255);
    // }

    /////////////////////////////////

    // uint8_t compute_checksum(uint8_t* buffer, uint8_t length){
    //     uint8_t checksum = 0;
    //     for (uint8_t i = 2; i < length - 1; ++i){
    //         checksum += buffer[i];
    //     }
    //     return (255 - checksum);
    // }

    // packet makePacket(uint8_t id, uint8_t instr, const packet& params={}){
    //     packet pkt {0xff, 0xff, id};
    //     pkt.reserve(3+params.size()+3);
    //     // std::cout << pkt.size() << " " << pkt.capacity() << std::endl;
    //
    //     pkt.push_back(params.size() + 2);
    //     pkt.push_back(instr);
    //     for (auto const& p: params) pkt.push_back(p);
    //     uint8_t csum = compute_checksum(pkt);
    //     pkt.push_back(csum);
    //
    //     // if (params.size() > 0){
    //     //     pkt.push_back(params.size() + 2);
    //     //     pkt.push_back(instr);
    //     //     for (auto const& p: params) pkt.push_back(p);
    //     //     uint8_t csum = compute_checksum(pkt);
    //     //     pkt.push_back(csum);
    //     // }
    //     // else {
    //     //     pkt.push_back(2);
    //     //     pkt.push_back(instr);
    //     //     uint8_t csum = compute_checksum(pkt);
    //     //     pkt.push_back(csum);
    //     // }
    //
    //     return pkt;
    // }


    // packet make_write_packet(uint8_t ID, uint8_t reg, uint8_t data){
    //     uint8_t hi = data >> 8;
    //     uint8_t lo = data & 0xff;
    //     uint8_t Checksum = (~(ID + 1 + AX::WRITE_DATA + reg + lo + hi)) & 0xFF;
    //
    //     packet pkt {AX::START,AX::START,ID,1,AX::WRITE_DATA,reg,lo,hi,Checksum};
    //
    //     return pkt;
    // }


// packet make_move_packet(uint8_t ID, int Position){
//     uint8_t Position_H = Position >> 8;           // 16 bits - 2 x 8 bits variables
//     uint8_t Position_L = Position;
//
//     uint8_t Checksum = (~(ID + AX::GOAL_LENGTH + AX::WRITE_DATA + AX::GOAL_POSITION_L + Position_L + Position_H)) & 0xFF;
//     packet pkt {AX::START,AX::START,ID,AX::GOAL_LENGTH,AX::WRITE_DATA,AX::GOAL_POSITION_L,Position_L,Position_H,Checksum};
//
//     return pkt;
// }

// packet make_reset_packet(uint8_t id, uint8_t level){}
// packet make_reboot_packet(uint8_t id){}
// packet make_angle_packet(uint8_t id, double angle){}
// packet make_angle_packet(uint8_t id, int angle){}
// packet make_led_packet(uint8_t id, uint8_t value){}
// packet make_speed_packet(uint8_t id, uint16_t speed){}
//
// packet make_write_packet(uint8_t id, uint8_t reg, const parameters& values={}){}
// packet make_read_packet(uint8_t id, uint8_t reg, const parameters& values={}){}
//
// packet make_sync_write_packet(uint8_t reg, std::vector<ServoMove_t>& info);
// packet make_sync_write_packet(uint8_t num, ServoMove_t* info){
//     packet data;
//     uint8_t hi, lo;
//     data.push_back(AX::GOAL_POSITION_L);
//     data.push_back(4); // only doing angle and speed
//     for (int i=0; i<num; i++) {
//         data.push_back(info[i].id);
//         // angle
//         hi = info[i].count >> 8;
//         lo = info[i].count & 0xff;
//         data.push_back(lo);
//         data.push_back(hi);
//         // speed
//         hi = info[i].speed >> 8;
//         lo = info[i].speed & 0xff;
//         data.push_back(lo);
//         data.push_back(hi);
//         // std::cout << "id " << int(info[i].id) << "  cnt "<< info[i].count << std::endl;
//     }
//     return makePacket(AX::BROADCAST_ID, AX::SYNC_WRITE, data);
// }

    // packet make_sync_move_speed_packet(uint8_t num, ServoMove_t* info){
    //     packet data;
    //     uint8_t hi, lo;
    //     data.push_back(AX::GOAL_POSITION_L);
    //     data.push_back(4); // only doing angle and speed
    //     for (int i=0; i<num; i++) {
    //         data.push_back(info[i].id);
    //         // angle
    //         hi = info[i].count >> 8;
    //         lo = info[i].count & 0xff;
    //         data.push_back(lo);
    //         data.push_back(hi);
    //         // speed
    //         hi = info[i].speed >> 8;
    //         lo = info[i].speed & 0xff;
    //         data.push_back(lo);
    //         data.push_back(hi);
    //         // std::cout << "id " << int(info[i].id) << "  cnt "<< info[i].count << std::endl;
    //     }
    //     return makePacket(AX::BROADCAST_ID, AX::SYNC_WRITE, data);
    // }

    // packet make_sync_move_speed_packet(uint8_t num, uint8_t info[][5]){
    //     packet data;
    //     uint8_t hi, lo;
    //     data.push_back(AX::GOAL_POSITION_L);
    //     data.push_back(4); // only doing angle and speed
    //     for (int i=0; i<num; i++) {
    //         data.push_back(info[i][0]); // id
    //         data.push_back(info[i][1]); // lo angle
    //         data.push_back(info[i][2]); // hi
    //         data.push_back(info[i][3]); // lo speed
    //         data.push_back(info[i][4]); // hi
    //         // // std::cout << "id " << int(info[i].id) << "  cnt "<< info[i].count << std::endl;
    //     }
    //     return makePacket(AX::BROADCAST_ID, AX::SYNC_WRITE, data);
    // }
    // packet make_sync_move_packet(uint8_t num, uint8_t info[][3]){
    //     packet data;
    //     // uint8_t hi, lo;
    //     data.push_back(AX::GOAL_POSITION_L);
    //     data.push_back(2); // only doing angle and speed
    //     for (int i=0; i<num; i++) {
    //         data.push_back(info[i][0]); // id
    //         data.push_back(info[i][1]); // lo angle
    //         data.push_back(info[i][2]); // hi
    //         // // std::cout << "id " << int(info[i].id) << "  cnt "<< info[i].count << std::endl;
    //     }
    //     return makePacket(AX::BROADCAST_ID, AX::SYNC_WRITE, data);
    // }
    //
    // packet make_sync_move_packet(int num, uint16_t info[][2]){
    //     packet data;
    //     // uint8_t hi, lo;
    //     data.push_back(AX::GOAL_POSITION_L);
    //     data.push_back(2); // only doing angle and speed
    //     for (int i=0; i<num; i++) {
    //         data.push_back(static_cast<uint8_t>(info[i][0]));        // id
    //         data.push_back(static_cast<uint8_t>(info[i][1] & 0xff)); // lo
    //         data.push_back(static_cast<uint8_t>(info[i][1] >> 8));   // hi
    //         // // std::cout << "id " << int(info[i].id) << "  cnt "<< info[i].count << std::endl;
    //     }
    //     return makePacket(AX::BROADCAST_ID, AX::SYNC_WRITE, data);
    // }
    // packet make_sync_read_packet(uint8_t id, uint8_t reg){}
    // packet make_sync_angle_packet(uint8_t id, double angle){}
    // packet make_sync_angle_packet(uint8_t id, int angle){}
    //
    // uint16_t crc(const std::vector<uint8_t>& data){}

    /**
     * @brief Get a SYNC_READ packet.
     * @param buffer Buffer to fill with packet.
     * @param device_ids Vector of servos to read.
     * @param address Starting address of read.
     * @param length The number of bytes to read.
     */
    // inline uint8_t get_sync_read_packet(
    //   uint8_t * buffer,
    //   std::vector<uint8_t> device_ids,
    //   uint8_t address,
    //   uint8_t length){
    //
    //     uint8_t len = 0;
    //     buffer[len++] = 0xff;
    //     buffer[len++] = 0xff;
    //     buffer[len++] = AX::BROADCAST_ID;
    //     buffer[len++] = 4 + device_ids.size();  // Length of remaining packet
    //     buffer[len++] = SYNC_READ;
    //     buffer[len++] = address;
    //     buffer[len++] = length;
    //     for (auto id : device_ids) buffer[len++] = id;
    //     buffer[len++] = compute_checksum(buffer, 8 + device_ids.size());
    //
    //     // Return number of bytes added to buffer
    //     return 8 + device_ids.size();
    // }

    /**
     * @brief Create a SYNC_WRITE packet.
     * @param buffer Buffer to fill with packet.
     * @param address Starting address of write.
     * @params Each element of the vector should be:
     *         [id, param1, param2, ...]
     */
    // inline uint8_t get_sync_write_packet(
    //   uint8_t * buffer,
    //   uint8_t address,
    //   std::vector<std::vector<uint8_t>> params)
    // {
    //   if (params.empty() || params[0].empty())
    //   {
    //     return 0;
    //   }
    //
    //   uint8_t param_len = params[0].size() * params.size();
    //   uint8_t len = 0;
    //   buffer[len++] = 0xff;
    //   buffer[len++] = 0xff;
    //   buffer[len++] = AX::BROADCAST_ID;
    //   buffer[len++] = 4 + param_len;  // remaining byte length
    //   buffer[len++] = SYNC_WRITE;
    //   buffer[len++] = address;
    //   buffer[len++] = params[0].size() - 1;  // how many byte to write each servo
    //   for (auto p : params)
    //   {
    //     for (auto x : p)
    //     {
    //       buffer[len++] = x;
    //     }
    //   }
    //   buffer[len++] = compute_checksum(buffer, 8 + param_len);
    //
    //   // Return number of bytes added to buffer
    //   return 8 + param_len;
    // }

    /**
     * @brief Get a READ_DATA packet.
     * @param buffer Buffer to fill with packet.
     * @param device_id The ID of the servo to read.
     * @param address Starting address of read.
     * @param length The number of bytes to read.
     */
    // inline uint8_t get_read_packet(
    //   uint8_t* buffer,
    //   uint8_t device_id,
    //   uint8_t address,
    //   uint8_t length)
    // {
    //   buffer[0] = 0xff;
    //   buffer[1] = 0xff;
    //   buffer[2] = device_id;
    //   buffer[3] = 4;  // Length of remaining packet
    //   buffer[4] = READ_DATA;
    //   buffer[5] = address;
    //   buffer[6] = length;
    //   buffer[7] = compute_checksum(buffer, 8);
    //
    //   // Return number of bytes added to buffer
    //   return 8;
    // }

    /**
     * @brief Get a WRITE_DATA packet.
     * @param buffer Buffer to fill with packet.
     * @param device_id The ID of the servo to read.
     * @param address Starting address of read.
     * @param length The number of bytes to read.
     */
    // inline uint8_t get_write_packet(
    //   uint8_t* buffer,
    //   uint8_t device_id,
    //   uint8_t address,
    //   std::vector<uint8_t> params)
    // {
    //   uint8_t len = 0;
    //   buffer[len++] = 0xff;
    //   buffer[len++] = 0xff;
    //   buffer[len++] = device_id;
    //   buffer[len++] = 3 + params.size();  // Length of remaining packet
    //   buffer[len++] = WRITE_DATA;
    //   buffer[len++] = address;
    //   for (auto p : params)
    //   {
    //     buffer[len++] = p;
    //   }
    //   buffer[len++] = compute_checksum(buffer, 7 + params.size());
    //
    //   // Return number of bytes added to buffer
    //   return len;
    // }

    ///////////////
