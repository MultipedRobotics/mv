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

  // vector<uint16_t> path {511, 0, 1023, 0, 1023, 0, 1023, 511};
  vector<uint16_t> path{511, 34, 100, 300, 564, 758, 1000,
                        700, 12, 50,  450, 890, 511};
  uint16_t last = 511;

  for (const auto &p : path) {
    packet mv = servo.make_move_packet(1, p);
    serial.write(mv);

    /*
       rev   min    360 deg
    59 --- * ---- * ------- = 354 deg/sec
       min   60 s    rev

        deg   1023 cnts
    354 --- * --------- = 1207.14 cnts/sec
        sec    300 deg
    */
    int d = 0;
    if (p >= last) d = int(1000.0 * (p - last) / 1207.14);
    else d = int(1000.0 * (last - p) / 1207.14);
    delay(d);
    last = p;
  }

  return 0;
}
