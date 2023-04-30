# mv

![Cpp](https://github.com/MultipedRobotics/mv/workflows/Cpp/badge.svg)

**Under Heavey Development, only minimal AX12 working**

Yet another Dynamexl library for smart servos. This is call `mv` after the
unix move command.

## Cool

- [termcolor](https://github.com/ikalnytskyi/termcolor)

## Arduino Example

```cpp
#include <mv.h>
#include <vector>

AX12 servo;
std::vector<uint16_t> angles {0,200,400,600,800,1000};
SerialPort sp;
uint16_t angle;
constexpr int dd_pin = 2;

void setup() {
  Serial.begin(1000000);
  Serial1.begin(1000000);
  Serial1.setTimeout(10);

  sp.begin(&Serial1, dd_pin);

  randomSeed(analogRead(0));
  angle = random(0,1023);
}

void get_response() {
  ReadStatus_t rs = sp.read();
  if (rs.ok) {
    for (Packet_t const &p: rs.pkts) {
      for (uint8_t const &b: p) {
        Serial.print((int) b);
        Serial.print(",");
      }
      Serial.println(" ");
    }
  }
}

void loop() {

    for (const auto& v: angles){
        std::vector<ServoMoveSpeed_t> ss {
            {1, v, 0},
            {2, v, 0},
            {3, v, 0}
        };
        Packet_t mv = servo.makeMovePacket(ss);

        sp.write(mv);
        get_response();

        delay(1000);
    }

    Packet_t ping = servo.makePingPacket();
    sp.write(ping);
    delay(100);
    get_response();

    delay(500);
}
```

## References

- [AX-12 e-manual](https://emanual.robotis.com/docs/en/dxl/ax/ax-12a/)
- [Protocol 1 e-manual](https://emanual.robotis.com/docs/en/dxl/protocol1/)

## Todo

- [x] support Protocol 1
- [ ] support Protocol 2
- [ ] support XL430 maybe
- [ ] support XL320 probably not

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
