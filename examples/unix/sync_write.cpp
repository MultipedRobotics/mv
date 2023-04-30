#include <array>
#include <cstdlib> // rand, srand
#include <iostream>
#include <stdint.h>
#include <string>
#include <time.h> // seed srand
#include <unistd.h>
#include <vector>

#include <mv.h>

using namespace std;

int main() {

  AX12 servo;
  SerialPort serial;
  // string port = "/dev/tty.usbserial-A904MISU";
  string port =
      "/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A904MISU-if00-port0";
  serial.begin(port);

  packet t = servo.make_torque_packet(1, false);
  serial.write(t);

  vector<uint16_t> angles{0, 233, 765, 457, 986, 234, 511};
  uint16_t last = 511;

  for (const auto &v : angles) {
    vector<ServoMoveSpeed_t> ss{
        {1, v, 0}, {2, 100, 0}, {3, 100, 0}, {4, 100, 0}};
    packet mv = servo.makeSyncMoveSpeedPacket(ss);
    // pprint(mv);
    serial.write(mv);

    // AX::delay(v, last);
    last = v;
  }

  return 0;
}
