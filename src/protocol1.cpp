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

// https://emanual.robotis.com/docs/en/dxl/protocol1/#sync-write
// [s,s,id, ... payload ..., chksum] => payload_len + 4
//    s: start character, 0xff for AX12
// [payload_len, sync_instr, reg, reg_len ... params] => (reg_len+id)*servo_num + 4
//   reg: servo register address
//   reg_len: how many bytes to write starting at reg
//   sync_instr: 0x83 for AX12
packet Protocol1::make_sync_write_packet(const uint8_t reg, const std::vector<ServoMoveSpeed_t>& info){
    const bool do_speed = info[0].speed == 0 ? false : true;
    const uint8_t info_bytes = do_speed ? 5 : 3;
    const uint8_t reg_len = do_speed ? 4 : 2;
    const uint8_t payload_len = (reg_len+1)*info.size()+4;
    // cout << "payload_len: " << int(payload_len) <<endl;
    auto pkt = packet(payload_len+4);

    pkt[0] = START;
    pkt[1] = START;
    pkt[2] = BROADCAST_ID;
    pkt[3] = static_cast<uint8_t>(payload_len);
    pkt[4] = SYNC_WRITE;
    pkt[5] = reg;
    pkt[6] = reg_len;

    uint8_t hi, lo;
    int i = 7;
    for (auto const& s: info) {
        pkt[i++] = s.id;
        // angle
        hi = s.count >> 8;
        lo = s.count & 0xff;
        pkt[i++] = lo;
        pkt[i++] = hi;
        // speed
        if (do_speed){
            hi = s.speed >> 8;
            lo = s.speed & 0xff;
            pkt[i++] = lo;
            pkt[i++] = hi;
        }
    }
    uint8_t csum = compute_checksum(pkt);
    pkt[i] = csum;
    return pkt;
}

// packet Protocol1::make_sync_read_packet(uint8_t address, uint8_t length, const std::vector<uint8_t>& ids){
//     auto pkt = packet(8 + ids.size());
//     // uint8_t len = 0;
//     //
//     // pkt[len++] = 0xff;
//     // pkt[len++] = 0xff;
//     // pkt[len++] = BROADCAST_ID;
//     // pkt[len++] = 4 + ids.size();  // Length of remaining packet
//     // pkt[len++] = SYNC_READ;
//     // pkt[len++] = address;
//     // pkt[len++] = length;
//     // for (auto id : ids) pkt[len++] = id;
//     // pkt[len++] = compute_checksum(pkt); // not right?
//     return pkt;
// }

// void Protocol1::decodePacket(const packet& data){
//
// }

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
