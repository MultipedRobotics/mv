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

int main() {

    AX12 servo;
    Serial serial;
    // string port = "/dev/tty.usbserial-A904MISU";
    string port = "/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A904MISU-if00-port0";
    serial.open(port);

    vector<uint16_t> angles {0,233,765,457,986,234,511};
    uint16_t last = 511;

    for (const auto& v: angles){
        vector<ServoMove_t> ss {
            {1, v},
            {2, 100},
            {3, 100},
            {4, 100}
        };
        packet mv = servo.make_sync_write_packet(ss);
        pprint(mv);
        // serial.write(mv);

        int delay = 0;
        if (v >= last) delay = int(1000.0*double(v-last)/1207.14);
        else delay = int(1000.0*double(last-v)/1207.14);
        // cout << ">> delay: " << delay << endl;
        msleep(delay);
        last = v;
    }

    return 0;
}
