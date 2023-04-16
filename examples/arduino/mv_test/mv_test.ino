#include <mv.h>
#include <vector>
// using namespace std;


AX12 servo;
std::vector<uint16_t> angles {0,200,400,600,800,1000};
SerialPort sp;

constexpr int dd_pin = 2;

void setup() {
  Serial.begin(1000000);
  Serial1.begin(1000000);
  Serial1.setTimeout(100);
  sp.begin(&Serial1, dd_pin);
}

void loop() {
  
    for (const auto& v: angles){
        std::vector<ServoMoveSpeed_t> ss {
            {1, v, 0},
            // {2, v, 0},
            // {3, v, 0}
        };
        packet mv = servo.make_sync_move_speed_packet(ss);

        sp.write(mv);

        delay(1000);
    }

    packet ping = servo.make_ping_packet(1);
    sp.write(ping);
    sp.write(ping);
    delay(100);
    int num = sp.read();
    Serial.println(num);

    delay(500);

}
