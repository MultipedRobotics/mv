#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <array>
#include <stdint.h>
#include <cstdlib> // rand, srand
#include <time.h> // seed srand

// #include "mv/ax12.hpp"
#include <mv/mv.h>

using namespace std;




int main() {

    AX12 servo;
    // Serial serial;
    // // string port = "/dev/tty.usbserial-A904MISU";
    // string port = "/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A904MISU-if00-port0";
    // serial.open(port);

    // vector<uint16_t> steps {0,511};
    //
    // for (const auto& p: steps){
    //     packet mv = servo.make_move_packet(1, p);
    //     pprint(mv);
    //     serial.write(mv);
    //     msleep(500);
    // }


    // typedef struct {
    //     int bob = 5;
    //     // int tom = 55;
    // } reg_t;
    //
    //
    // class A {
    // public:
    //     A(ret_t& dd): d(dd) {}
    //     const reg_t d;
    // }





    // const ax12_reg_t bob;
    // bob.bob = 35;
    // bob.tom = 25;

    // cout << bob.bob << endl;
    // cout << &bob << endl;
    // cout << sizeof(bob) << endl;

    return 0;
}
