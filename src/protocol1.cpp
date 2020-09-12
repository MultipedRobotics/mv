#include "mv/protocol1.hpp"

// using namespace std;

Protocol1::Protocol1(){}

/*
def makeSyncWritePacket(self, reg, info):
        """
        Write sync angle information to servos.
        Status Packet will not be returned because Broadcast ID(0xFE) is used
        info = [[ID, data1, ...], [ID, data1, ...], ...]
        """
        data = []
        data.append(reg)  # addr
        data.append(len(info[0])-1)  # data length not counting ID
        for cmd in info:
            data += cmd

        ID = self.BROADCAST_ADDR
        instr = self.SYNC_WRITE
        pkt = self.makePacket(ID, instr, data)  # create packet
        return pkt
*/
// packet Protocol1::make_sync_write_packet(uint8_t reg, std::vector<ServoMove_t>& info){
//     packet data;
//     data.push_back(reg);
//     data.push_back(2); // only doing angle
//     for (auto const& cmd: info) {
//         uint8_t Position_H = cmd.count >> 8;
//         uint8_t Position_L = cmd.count & 0xff;
//         data.push_back(Position_L);
//         data.push_back(Position_H);
//     }
//     return makePacket(BROADCAST_ID, AX_SYNC_WRITE, data);
// }

// packet Protocol1::make_sync_write_packet(uint8_t reg, std::vector<ServoMove_t>& info){
//     packet data;
//     data.push_back(reg);
//     data.push_back(2); // only doing angle
//     for (auto const& cmd: info) {
//         uint8_t Position_H = cmd.count >> 8;
//         uint8_t Position_L = cmd.count & 0xff;
//         data.push_back(Position_L);
//         data.push_back(Position_H);
//     }
//     return makePacket(BROADCAST_ID, AX_SYNC_WRITE, data);
// }

/*
def makeSyncMovePacket(self, info, degrees=True):
        """
        Write sync angle information to servos.
        info = [[ID, angle], [ID, angle], ...]
        ID: servo ID
        angle: 0-300 degrees or in radians
        """
        data = []

        # since all servo angles have the same register addr (GOAL_POSITION)
        # and data size (2), a sinc packet is smart choice
        # compare bulk vs sync for the same commands:
        # bulk = 94 uint8_ts
        # sync = 50 uint8_ts
        for cmd in info:
            data.append(cmd[0])  # ID
            angle = angle2int(cmd[1], degrees)
            data.append(angle[0])  # LSB
            data.append(angle[1])  # MSB

        pkt = self.makeSyncWritePacket(self.GOAL_POSITION, data)
        return pkt
*/
