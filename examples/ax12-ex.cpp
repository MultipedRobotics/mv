#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <array>
#include <stdint.h>
#include <cstdlib> // rand, srand
#include <time.h> // seed srand

// #include "mv/ax12.hpp"
#include "mv/protocol1.hpp"
#include "mv/SerialPort.hpp"
#include <termcolor/termcolor.hpp>

using namespace std;

void pprint(const packet& pkt){
    cout << '[';
    for (auto const& v: pkt) cout << int(v) << ",";
    cout << ']' << endl;

    printf("[");
    for (auto const& v: pkt) printf("%02X,", v);
    printf("]\n");
}

int main() {

    AX12 servo;
    Serial serial;
    // string port = "/dev/tty.usbserial-A904MISU";
    string port = "/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A904MISU-if00-port0";
    serial.open(port);
    // serial.set_speed(1000000);
    // packet ping = servo.make_ping_packet(AX::BROADCAST_ID);
    // pprint(ping);
    // serial.write(ping);

    vector<ServoMoveSpeed_t> ss {
        {0, 16, 336}, // id:0 goal: 0x10 speed: 0x150
        {1, 544, 864} // 1, 0x220, 0x360
    };
    packet mv = servo.make_sync_write_packet(ss);
    pprint(mv);
    serial.write(mv);

    // srand(time(0));
    // uint16_t count = rand() % 1023;
    // cout << ">> Moving: " << count << endl;
    // packet mv = servo.make_move_packet(1, count);
    // pprint(mv);
    // printf(">> wrote: %d\n", serial.write(mv));
    // msleep(100);
    // printf(">> available: %d\n", serial.available());
    // packet p = serial.read();
    // pprint(p);
    // printf(">> Error %u\n", servo.status_error(p));

    // vector<uint8_t> ids {1,2,3,4};
    // packet rd = servo.make_sync_read_packet(22, 4, ids);
    // pprint(rd);
    //
    // cout << termcolor::green << "SUCCESS!" << termcolor::reset << endl;
    // cout << termcolor::color<213> << "color?" << termcolor::reset << endl;
    // cout << "color?" << endl;

    return 0;
}
