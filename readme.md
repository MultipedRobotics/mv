# mv

![Cpp](https://github.com/MultipedRobotics/mv/workflows/Cpp/badge.svg)

**Under Heavey Development, only minimal AX12 working**

Yet another Dynamexl library for smart servos. This is call `mv` after the
unix move command.

## Issues

**Only works on linux**, macOS serial port in C++ can't achieve 1000000 baud. I can
do it in python, but not C++.

## Cool

- [termcolor](https://github.com/ikalnytskyi/termcolor)

## Examples

```cpp
#include <iostream>
#include <mv/mv.h>

using namespace std;

int main() {

    AX12 servo;
    Serial serial;
    string port = "/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A904MISU-if00-port0";
    serial.open(port);

    packet t = servo.make_torque_packet(1, true);
    serial.write(t);

    vector<uint16_t> angles {0,233,765,457,986,234,511};
    uint16_t last = 511;

    // by setting speed to zero, it just takes the current default
    // and doesn't send uncessary data down the wire.
    for (const auto& v: angles){
        vector<ServoMoveSpeed_t> ss {
            {1, v, 0},
            {2, 100, 0},
            {3, 100, 0},
            {4, 100, 0}
        };
        packet mv = servo.make_sync_move_speed_packet(ss);
        pprint(mv);
        serial.write(mv);

        AX::delay(v, last);
        last = v;
    }

    return 0;
}
```
