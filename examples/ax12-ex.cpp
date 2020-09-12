#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <array>
#include <stdint.h>

// #include "mv/ax12.hpp"
#include "mv/protocol1.hpp"
#include "mv/SerialPort.hpp"
#include <termcolor/termcolor.hpp>

using namespace std;

void pprint(const packet& pkt){
    cout << '[';
    for (auto const& v: pkt) cout << int(v) << ",";
    cout << ']' << endl;
}

int main() {
    // array<uint8_t, 10> aa;
    // aa[9] = 10;
    // aa[5] = 20;
    // for(auto& v: aa) cout << int(v) << ',';
    // cout << endl;
    // aa.fill(0);
    // for(auto& v: aa) cout << int(v) << ',';
    // cout << endl;
    // return 0;

    AX12 servo;
    Serial serial;
    serial.open("/dev/tty.usbserial-A904MISU");
    // serial.set_speed(1000000);
    packet ping = servo.make_ping_packet(AX::BROADCAST_ID);
    pprint(ping);
    // serial.write(ping);

    vector<ServoMoveSpeed_t> ss {{1,11,111},{2,22,222},{3,33,333},{4,44,444}};
    packet mv = servo.make_sync_write_packet(ss);
    pprint(mv);
    vector<uint8_t> ids {1,2,3,4};
    packet rd = servo.make_sync_read_packet(22, 4, ids);
    pprint(rd);

    cout << termcolor::green << "SUCCESS!" << termcolor::reset << endl;
    cout << termcolor::color<213> << "color?" << termcolor::reset << endl;
    cout << "color?" << endl;

    return 0;
}
