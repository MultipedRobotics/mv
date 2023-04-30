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
            // {3, v, 0}
        };
        Packet_t mv = servo.makeMovePacket(ss);

        sp.write(mv);
        get_response();

        delay(1000);
    }
    // Serial.print("servo angle: ");
    // Serial.println(angle);

    // Packet_t mv = servo.makeMovePacket(2, angle);
    // sp.write(mv);

    Packet_t ping = servo.makePingPacket();
    sp.write(ping);
    // sp.write(ping);
    delay(100);
    get_response();

    delay(500);

}
