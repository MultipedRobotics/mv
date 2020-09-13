#pragma once

#include <mv/common.hpp>
#include <mv/protocol1.hpp>

namespace AX {
// EEPROM AREA  ///////////////////////////////////////////////////////////
// constexpr uint8_t MODEL_NUMBER           =  0;
// constexpr uint8_t VERSION                =  2;
constexpr uint8_t ID                     =  3;
// constexpr uint8_t BAUD_RATE              =  4;
// constexpr uint8_t RETURN_DELAY_TIME      =  5;
// constexpr uint8_t CW_ANGLE_LIMIT         =  6;
// constexpr uint8_t CCW_ANGLE_LIMIT        =  8;
// constexpr uint8_t SYSTEM_DATA2           =  10;
// constexpr uint8_t LIMIT_TEMPERATURE      =  11;
// constexpr uint8_t DOWN_LIMIT_VOLTAGE     =  12;
// constexpr uint8_t UP_LIMIT_VOLTAGE       =  13;
// constexpr uint8_t MAX_TORQUE             =  14;
// constexpr uint8_t RETURN_LEVEL           =  16;
// constexpr uint8_t ALARM_LED              =  17;
// constexpr uint8_t ALARM_SHUTDOWN         =  18;
// constexpr uint8_t OPERATING_MODE         =  19;
// constexpr uint8_t DOWN_CALIBRATION       =  20;
// constexpr uint8_t UP_CALIBRATION         =  22;

// RAM AREA  //////////////////////////////////////////////////////////////
constexpr uint8_t TORQUE_ENABLE           = 24;
// constexpr uint8_t LED                    =  25;
// constexpr uint8_t CW_COMPLIANCE_MARGIN   =  26;
// constexpr uint8_t CCW_COMPLIANCE_MARGIN  =  27;
// constexpr uint8_t CW_COMPLIANCE_SLOPE    =  28;
// constexpr uint8_t CCW_COMPLIANCE_SLOPE   =  29;
// namespace AX {
// constexpr uint8_t GOAL_POSITION  =       30;
// }
constexpr uint8_t GOAL_POSITION          =  30;
constexpr uint8_t GOAL_SPEED             =  32;
// constexpr uint8_t TORQUE_LIMIT           =  34;
constexpr uint8_t PRESENT_POSITION       =  36;
// constexpr uint8_t PRESENT_SPEED          =  38;
// constexpr uint8_t PRESENT_LOAD           =  40;
constexpr uint8_t PRESENT_VOLTAGE        =  42;
constexpr uint8_t PRESENT_TEMPERATURE    =  43;
// constexpr uint8_t REGISTERED_INSTRUCTION =  44;
// constexpr uint8_t PAUSE_TIME             =  45;
constexpr uint8_t MOVING                 =  46;
// constexpr uint8_t LOCK                   =  47;
// constexpr uint8_t PUNCH                  =  48;
// constexpr uint8_t PUNCH_H              =    49;

// Status Return Levels ///////////////////////////////////////////////////////////////
// constexpr uint8_t RETURN_NONE              0;
// constexpr uint8_t RETURN_READ              1;
// constexpr uint8_t RETURN_ALL               2;

// Instruction Set ///////////////////////////////////////////////////////////////
// constexpr uint8_t PING                  =   1;
// constexpr uint8_t READ_DATA              =  2;
// constexpr uint8_t WRITE_DATA             =  3;
// constexpr uint8_t REG_WRITE              =  4;
// constexpr uint8_t ACTION                 =  5;
// constexpr uint8_t RESET                  =  6;
// constexpr uint8_t REBOOT                 =  8;
// constexpr uint8_t SYNC_WRITE             =  0x83;
// constexpr uint8_t BULK_READ              =  0x92;

// Specials ///////////////////////////////////////////////////////////////
// #define OFF                         0;
// #define ON                          1;
// #define LEFT                        0;
// #define RIGHT                       1;
// constexpr uint8_t BYTE_READ             =   1;
// constexpr uint8_t BYTE_READ_POS          =  2;
// constexpr uint8_t RESET_LENGTH           =  2;
// constexpr uint8_t ACTION_LENGTH          =  2;
// constexpr uint8_t ID_LENGTH              =  4;
// constexpr uint8_t LR_LENGTH              =  4;
// constexpr uint8_t SRL_LENGTH             =  4;
// constexpr uint8_t RDT_LENGTH             =  4;
// constexpr uint8_t LEDALARM_LENGTH        =  4;
// constexpr uint8_t SALARM_LENGTH          =  4;
// constexpr uint8_t TL_LENGTH              =  4;
// constexpr uint8_t VL_LENGTH              =  6;
// constexpr uint8_t CM_LENGTH              =  6;
// constexpr uint8_t CS_LENGTH              =  6;
// constexpr uint8_t CCW_CW_LENGTH          =  8;
// constexpr uint8_t BD_LENGTH              =  4;
// constexpr uint8_t TEM_LENGTH             =  4;
// constexpr uint8_t MOVING_LENGTH          =  4;
// constexpr uint8_t RWS_LENGTH             =  4;
// constexpr uint8_t VOLT_LENGTH            =  4;
// constexpr uint8_t LED_LENGTH             =  4;
// constexpr uint8_t TORQUE_LENGTH          =  4;
// constexpr uint8_t POS_LENGTH             =  4;
// constexpr uint8_t GOAL_LENGTH            =  5;
// constexpr uint8_t MT_LENGTH              =  5;
// constexpr uint8_t PUNCH_LENGTH           =  5;
// constexpr uint8_t SPEED_LENGTH           =  5;
// constexpr uint8_t GOAL_SP_LENGTH         =  7;
// constexpr uint8_t ACTION_CHECKSUM        =  250;
// constexpr uint8_t BROADCAST_ID           =  254;
// constexpr uint8_t START                  =  255;
// constexpr uint8_t CCW_AL                 =  255;
// #define TIME_OUT                    10
// #define LOCK                        1

void delay(uint16_t current, uint16_t last);


}



class AX12: public Protocol1 {
public:
    AX12();
    ~AX12();

    packet make_move_packet(uint8_t ID, uint16_t position);
    packet make_torque_packet(uint8_t ID, bool enable);
    packet make_sync_move_speed_packet(const std::vector<ServoMoveSpeed_t>& info);
};


















// #include <common.hpp>
// #include <protocol1.hpp>
// #include <SerialPort.hpp>
// #include <array>
//
// // // --------- INSTRUCTIONS -----
// // constexpr byte PING      = 0x01;
// // constexpr byte READ      = 0x02;
// // constexpr byte WRITE     = 0x03;
// // constexpr byte REG_WRITE = 0x04;
// // constexpr byte ACTION    = 0x05;
// // constexpr byte RESET     = 0x06;
// // // constexpr byte REBOOT    = 0x08;
// // // constexpr byte STATUS    = 0x55;
// // // constexpr byte SYNC_READ  = 0x82;
// // constexpr byte SYNC_WRITE = 0x83;
// // constexpr byte BULK_READ  = 0x92;
// // // constexpr byte BULK_WRITE = 0x93;
// //
// // // -------- EEPROM -------------
// // constexpr byte MODEL_NUMBER    = 0;
// // constexpr byte VER_FIRMWARE    = 2;
// // constexpr byte ID              = 3;
// // constexpr byte BAUD_RATE       = 4;
// // constexpr byte DELAY_TIME      = 5;
// // constexpr byte CW_ANGLE_LIMIT  = 6;  // min angle, default 0
// // constexpr byte CCW_ANGLE_LIMIT = 8;   // max angle, default 300
// // constexpr byte CONTROL_MODE    = 11;  // joint or wheel mode, default joint (servo)
// // constexpr byte MAX_TORQUE      = 15;
// // constexpr byte RETURN_LEVEL    = 17;
// //
// // // -------- RAM ----------------
// // constexpr byte TORQUE_ENABLE    = 24;  // servo mode on/off - turn into wheel
// // constexpr byte LED              = 25;
// // constexpr byte GOAL_POSITION    = 30;
// // constexpr byte GOAL_VELOCITY    = 32;
// // constexpr byte GOAL_TORQUE      = 35;
// // constexpr byte PRESENT_POSITION = 37;  // current servo angle
// // constexpr byte PRESENT_SPEED    = 39;  // current speed
// // constexpr byte PESENT_LOAD      = 41;  // current load
// // constexpr byte PESENT_VOLTAGE   = 45;  // current voltage
// // constexpr byte PESENT_TEMP      = 46;  // current temperature
// // constexpr byte MOVING           = 49;
// // constexpr byte HW_ERROR_STATUS  = 50;
// // constexpr byte PUNCH            = 51;
// //
// // // --------- OTHER -------------
// // constexpr byte RESET_ALL                  = 0xFF;
// // constexpr byte RESET_ALL_BUT_ID           = 0x01;
// // constexpr byte RESET_ALL_BUT_ID_BAUD_RATE = 0x02;
// // constexpr byte LED_ON                     = 1;
// // constexpr byte LED_OFF                    = 0;
// // constexpr byte BROADCAST_ADDR             = 0xfe;  // a packet with this ID will go to all servos
// // constexpr byte WHEEL_MODE                 = 1;
// // constexpr byte JOINT_MODE                 = 2;  // normal servo
// // constexpr byte DR_1000000                 = 1;  // bps = 2000000/(data + 1)
//
//
//
// class AX12: public Protocol1 {
//     // --------- INSTRUCTIONS -----
//     constexpr byte PING      = 0x01;
//     constexpr byte READ      = 0x02;
//     constexpr byte WRITE     = 0x03;
//     constexpr byte REG_WRITE = 0x04;
//     constexpr byte ACTION    = 0x05;
//     constexpr byte RESET     = 0x06;
//     // constexpr byte REBOOT    = 0x08;
//     // constexpr byte STATUS    = 0x55;
//     // constexpr byte SYNC_READ  = 0x82;
//     constexpr byte SYNC_WRITE = 0x83;
//     constexpr byte BULK_READ  = 0x92;
//     // constexpr byte BULK_WRITE = 0x93;
//
//     // -------- EEPROM -------------
//     constexpr byte MODEL_NUMBER    = 0;
//     constexpr byte VER_FIRMWARE    = 2;
//     constexpr byte ID              = 3;
//     constexpr byte BAUD_RATE       = 4;
//     constexpr byte DELAY_TIME      = 5;
//     constexpr byte CW_ANGLE_LIMIT  = 6;  // min angle, default 0
//     constexpr byte CCW_ANGLE_LIMIT = 8;   // max angle, default 300
//     constexpr byte CONTROL_MODE    = 11;  // joint or wheel mode, default joint (servo)
//     constexpr byte MAX_TORQUE      = 15;
//     constexpr byte RETURN_LEVEL    = 17;
//
//     // -------- RAM ----------------
//     constexpr byte TORQUE_ENABLE    = 24;  // servo mode on/off - turn into wheel
//     constexpr byte LED              = 25;
//     constexpr byte GOAL_POSITION    = 30;
//     constexpr byte GOAL_VELOCITY    = 32;
//     constexpr byte GOAL_TORQUE      = 35;
//     constexpr byte PRESENT_POSITION = 37;  // current servo angle
//     constexpr byte PRESENT_SPEED    = 39;  // current speed
//     constexpr byte PESENT_LOAD      = 41;  // current load
//     constexpr byte PESENT_VOLTAGE   = 45;  // current voltage
//     constexpr byte PESENT_TEMP      = 46;  // current temperature
//     constexpr byte MOVING           = 49;
//     constexpr byte HW_ERROR_STATUS  = 50;
//     constexpr byte PUNCH            = 51;
//
//     // --------- OTHER -------------
//     constexpr byte RESET_ALL                  = 0xFF;
//     constexpr byte RESET_ALL_BUT_ID           = 0x01;
//     constexpr byte RESET_ALL_BUT_ID_BAUD_RATE = 0x02;
//     constexpr byte LED_ON                     = 1;
//     constexpr byte LED_OFF                    = 0;
//     constexpr byte BROADCAST_ADDR             = 0xfe;  // a packet with this ID will go to all servos
//     constexpr byte WHEEL_MODE                 = 1;
//     constexpr byte JOINT_MODE                 = 2;  // normal servo
//     constexpr byte DR_1000000                 = 1;  // bps = 2000000/(data + 1)
//
// public:
//     AX12(): io(), serial(io){}
//     ~AX12(){}
//     void open(const std::string& port, int pin){
//         serial.open(port, pin);
//     }
//
//     void write(const packet& msg){
//         const uint8_t *data = msg.data();
//         // serial.write(data, msg.size());
//     }
//
//     packets read(){
//         std::array<std::uint8_t, 1024> buffer;
//         uint8_t *p = buffer.data();
//         serial.read(1024, p);
//         packets pp;
//         return pp;
//     }
//
//     PingResponse ping(){
//         PingResponse ans;
//         return ans;
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
