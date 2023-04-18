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

  vector<uint16_t> steps{0, 511, 1023, 511};

  for (const auto &p : steps) {
    packet mv = servo.make_move_packet(1, p);
    // pprint(mv);
    printf(">> Wrote: %d ", serial.write(mv));
    // pprint(mv);
    delay(1000);
  }

  return 0;
}
