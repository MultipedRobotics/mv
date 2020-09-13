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

## References

- [AX-12 e-manual](https://emanual.robotis.com/docs/en/dxl/ax/ax-12a/)
- [Protocol 1 e-manual](https://emanual.robotis.com/docs/en/dxl/protocol1/)

# MIT License

**Copyright (c) 2019 Kevin J. Walchko**

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
