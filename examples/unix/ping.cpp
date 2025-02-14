#include <iostream>
#include <string>
#include <unistd.h> // printf
#include <vector>
// #include <array>
// #include <stdint.h>
#include <cstdlib> // rand, srand
#include <time.h>  // seed srand

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

  mvpkt_t ping = makePingPacket();
  // pprint(ping);
  serial.write(ping);

  int num = serial.read();
  // packet p = serial.buffer2packet(num);
  // pprint(p);
  // printf(">> Error %u\n", servo.status_error(p));

  return 0;
}
