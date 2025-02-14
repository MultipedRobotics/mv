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
using namespace Protocol1;

int main() {

  // AX12 servo;
  ServoPort serial;
  // string port = "/dev/tty.usbserial-A904MISU";
  string port =
      "/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A904MISU-if00-port0";
  serial.init(port);

  mvpkt_t t = AX12::makeTorquePacket(1, false);
  serial.write(t);

  vector<uint16_t> angles{0, 233, 765, 457, 986, 234, 511};
  uint16_t last = 511;

  for (const auto &v : angles) {
    vector<ServoMoveSpeed_t> ss{
        {1, v, 0}, {2, 100, 0}, {3, 100, 0}, {4, 100, 0}};
    mvpkt_t mv = AX12::makeMovePacket(ss);
    // pprint(mv);
    serial.write(mv);

    // AX::delay(v, last);
    last = v;
  }

  return 0;
}
