#include <mv/ax12.hpp>

void AX::delay(uint16_t current, uint16_t last){
    msleep(int(1000.0*double(abs(current-last))/1207.14));
}

AX12::AX12(){}

AX12::~AX12(){}

packet AX12::make_move_packet(uint8_t ID, uint16_t position){
    return make_write16_packet(ID, AX::GOAL_POSITION, position);
}

packet AX12::make_torque_packet(uint8_t ID, bool enable){
    const uint8_t t = enable ? 1 : 0;
    return make_write8_packet(ID, AX::TORQUE_ENABLE, t);
}

/*
move (angle): 0-1023 counts (0-300 deg)
speed: 0-1023 counts, in increments of 0.111rpm, default is 0 (motor moves at max speed)
*/
packet AX12::make_sync_move_speed_packet(const std::vector<ServoMoveSpeed_t>& info){
    return make_sync_write_packet(AX::GOAL_POSITION, info);
}

packet AX12::read_goal_speed_packet(uint8_t ID){
    return make_read_packet(ID, AX::GOAL_SPEED, 2);
}
