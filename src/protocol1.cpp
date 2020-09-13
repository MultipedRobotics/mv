#include <mv/protocol1.hpp>
#include <termcolor/termcolor.hpp>
#include <string.h> // memset

using namespace std;

void pprint(const packet& pkt, bool hex){
    if ( hex ) std::cout << std::hex;

    std::cout << '[';
    // for (auto const& v: pkt) cout << int(v) << ",";
    for (int i=0; i < pkt.size(); ++i){
        int v = int(pkt[i]);
        if (i == 0 || i == 1) std::cout << termcolor::yellow << v << termcolor::reset << ',';
        else if (i == 2) std::cout << termcolor::blue << v << termcolor::reset << ',';
        else if (i == 3) std::cout << termcolor::green << v << termcolor::reset << ',';
        else if (i == 4) std::cout << termcolor::cyan << v << termcolor::reset << ',';
        else if (i == 5) std::cout << termcolor::magenta << v << termcolor::reset << ',';
        else if (i == pkt.size()-1) std::cout << termcolor::yellow << v << termcolor::reset << ',';
        else std::cout << v << ',';
    }
    std::cout << ']' << std::endl;
    if ( hex ) std::cout << std::dec;
}

Protocol1::Protocol1(){}

Protocol1::~Protocol1(){}

uint8_t Protocol1::compute_checksum(packet& pkt){
    uint32_t checksum = 0;
    for (uint8_t i = 2; i < pkt.size(); ++i) checksum += pkt[i];
    // return (255 - checksum);
    return (~checksum) & 0xFF;
}

packet Protocol1::make_write8_packet(uint8_t ID, uint8_t reg, uint8_t data){
    const uint8_t len = 4;
    const uint8_t Checksum = (~(ID + len + WRITE_DATA + reg + data)) & 0xFF;

    packet pkt {START,START,ID,len,WRITE_DATA,reg,data,Checksum};

    return pkt;
}

packet Protocol1::make_write16_packet(uint8_t ID, uint8_t reg, uint16_t data){
    const uint8_t hi = data >> 8;
    const uint8_t lo = data & 0xff;
    const uint8_t len = 5;
    const uint8_t Checksum = (~(ID + len + WRITE_DATA + reg + lo + hi)) & 0xFF;

    packet pkt {START,START,ID,len,WRITE_DATA,reg,lo,hi,Checksum};

    return pkt;
}

packet Protocol1::make_write_packet(uint8_t ID, uint8_t reg, uint16_t data1, uint16_t data2){
    const uint8_t hi1 = data1 >> 8;
    const uint8_t lo1 = data1 & 0xff;
    const uint8_t hi2 = data2 >> 8;
    const uint8_t lo2 = data2 & 0xff;
    const uint8_t len = 7;
    const uint8_t Checksum = (~(ID + len + WRITE_DATA + reg + lo1 + hi1 + lo2+hi2)) & 0xFF;

    packet pkt {START,START,ID,len,WRITE_DATA,reg,lo1,hi1,lo2,hi2,Checksum};

    return pkt;
}

packet Protocol1::make_move_packet(uint8_t ID, uint16_t position){
    return make_write16_packet(ID, AX::GOAL_POSITION, position);
}

packet Protocol1::make_torque_packet(uint8_t ID, bool enable){
    const uint8_t t = enable ? 1 : 0;
    return make_write8_packet(ID, AX::TORQUE_ENABLE, t);
}

packet Protocol1::make_read_position_packet(uint8_t ID){
    const uint8_t len = 4;
    const uint8_t Checksum = (~(ID + len + READ_DATA + AX::PRESENT_POSITION+ 2)) & 0xFF;

    packet pkt {START,START,ID,len,READ_DATA,AX::PRESENT_POSITION,2,Checksum};

    return pkt;
}

packet Protocol1::make_ping_packet(uint8_t ID){
    const uint8_t Checksum = (~(ID + READ_DATA + PING)) & 0xFF;
    packet pkt {START, START, ID, READ_DATA, PING, Checksum};
    return pkt;
}

packet Protocol1::make_reset_packet(uint8_t ID){
    const uint8_t len = 2;
    const uint8_t Checksum = (~(ID + len + RESET)) & 0xFF;

    packet pkt {START, START, ID, len, RESET, Checksum};

    return pkt;
}

packet Protocol1::make_moving_packet(uint8_t ID){
    auto pkt = packet(8);
    const uint8_t len = 4;
    const uint8_t Checksum = (~(ID + len + READ_DATA + AX::MOVING + 1)) & 0xFF;

    pkt[0] = START;
    pkt[1] = START;
    pkt[2] = ID;
    pkt[3] = len;
    pkt[4] = READ_DATA;
    pkt[5] = AX::MOVING;
    pkt[6] = 1;
    pkt[7] = Checksum;

    // packet pkt {START,START,ID,len,READ_DATA,AX::MOVING,AX::BYTE_READ,Checksum};

    // printf("pkt\n");

    return pkt;
}

packet Protocol1::make_sync_write_packet(const std::vector<ServoMove_t>& info){
    const uint8_t info_len = info.size()*3+2;
    const uint8_t total_len = (2+1)*info.size()+4;
    auto pkt = packet(7);

    pkt[0] = START;
    pkt[1] = START;
    pkt[2] = BROADCAST_ID;
    pkt[3] = static_cast<uint8_t>(info_len+2);
    pkt[4] = SYNC_WRITE;
    pkt[5] = AX::GOAL_POSITION;
    pkt[6] = 2;

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

packet Protocol1::make_sync_write_packet(const std::vector<ServoMoveSpeed_t>& info){
    const uint8_t info_len = info.size()*5+2;
    packet pkt {0xff, 0xff, BROADCAST_ID, static_cast<uint8_t>(info_len+2),
        SYNC_WRITE, AX::GOAL_POSITION, 4};
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

packet Protocol1::make_sync_read_packet(uint8_t address, uint8_t length, const std::vector<uint8_t>& ids){
    auto pkt = packet(8 + ids.size());
    // uint8_t len = 0;
    //
    // pkt[len++] = 0xff;
    // pkt[len++] = 0xff;
    // pkt[len++] = BROADCAST_ID;
    // pkt[len++] = 4 + ids.size();  // Length of remaining packet
    // pkt[len++] = SYNC_READ;
    // pkt[len++] = address;
    // pkt[len++] = length;
    // for (auto id : ids) pkt[len++] = id;
    // pkt[len++] = compute_checksum(pkt); // not right?
    return pkt;
}

void Protocol1::decodePacket(const packet& data){

}

uint8_t Protocol1::status_error(const packet& pkt){
    return pkt[4];
}

AxError Protocol1::decode_error(uint8_t b){
    AxError err;
    memset(&err, 0, sizeof(AxError));
    if (b & 1) err.input_voltage = true;
    if (b & 2) err.angle_limit = true;
    if (b & 4) err.overheating = true;
    if (b & 8) err.range = true;
    if (b & 16) err.checksum = true;
    if (b & 32) err.overload = true;
    if (b & 64) err.instruction = true;

    return err;
}
