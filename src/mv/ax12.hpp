#pragma once

#include <mv/protocol1.hpp>

namespace AX {
// EEPROM AREA  ///////////////////////////////////////////////////////////
// constexpr uint8_t MODEL_NUMBER           =  0;
// constexpr uint8_t VERSION                =  2;
constexpr uint8_t ID = 3;
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
constexpr uint8_t TORQUE_ENABLE = 24;
// constexpr uint8_t LED                    =  25;
// constexpr uint8_t CW_COMPLIANCE_MARGIN   =  26;
// constexpr uint8_t CCW_COMPLIANCE_MARGIN  =  27;
// constexpr uint8_t CW_COMPLIANCE_SLOPE    =  28;
// constexpr uint8_t CCW_COMPLIANCE_SLOPE   =  29;
constexpr uint8_t GOAL_POSITION = 30;
constexpr uint8_t GOAL_SPEED    = 32;
// constexpr uint8_t TORQUE_LIMIT           =  34;
constexpr uint8_t PRESENT_POSITION = 36;
// constexpr uint8_t PRESENT_SPEED          =  38;
// constexpr uint8_t PRESENT_LOAD           =  40;
constexpr uint8_t PRESENT_VOLTAGE     = 42;
constexpr uint8_t PRESENT_TEMPERATURE = 43;
// constexpr uint8_t REGISTERED_INSTRUCTION =  44;
// constexpr uint8_t PAUSE_TIME             =  45;
constexpr uint8_t MOVING = 46;
// constexpr uint8_t LOCK                   =  47;
// constexpr uint8_t PUNCH                  =  48;
// constexpr uint8_t PUNCH_H              =    49;

// Status Return Levels
// /////////////////////////////////////////////////////////////// constexpr
// uint8_t RETURN_NONE              0; constexpr uint8_t RETURN_READ 1;
// constexpr uint8_t RETURN_ALL               2;

// static void delay(uint16_t current, uint16_t last) {
//   msleep(int(1000.0*double(abs(current-last))/1207.14));
// }

} // namespace AX

class AX12 : public Protocol1 {
public:
  AX12() {}
  ~AX12() {}

  /*
  move (angle): 0-1023 counts (0-300 deg)
  speed: 0-1023 counts, in increments of 0.111rpm, default is 0 (motor moves at
  max speed)
  */
  packet
  make_sync_move_speed_packet(const std::vector<ServoMoveSpeed_t> &info) {
    return make_sync_write_packet(AX::GOAL_POSITION, info);
  }

  packet make_move_packet(uint8_t ID, uint16_t position) {
    return make_write16_packet(ID, AX::GOAL_POSITION, position);
  }

  // packet make_torque_packet(uint8_t ID, bool enable){
  //   const uint8_t t = enable ? 1 : 0;
  //   return make_write8_packet(ID, AX::TORQUE_ENABLE, t);
  // }

  // packet read_goal_speed_packet(uint8_t ID){
  //   return make_read_packet(ID, AX::GOAL_SPEED, 2);
  // }

  void diffdelay(uint16_t current, uint16_t last) {
    delay(int(1000.0 * double(abs(current - last)) / 1207.14));
  }
};
