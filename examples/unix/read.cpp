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

  // AX12 servo;
  ServoPort serial;
  // // string port = "/dev/tty.usbserial-A904MISU";
  string port =
      "/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A904MISU-if00-port0";
  serial.init(port);

  if (0) {
    srand(time(0));
    mvpkt_t mv = AX12::makeMovePacket(1, rand() % 1023);
    serial.write(mv);
    delay(500);
  }
  // Protocol1 pc;
  // mvpkt_t p = pc.make_read_packet(1, 32, 2);
  // pprint(p);
  // return 0;

  // mvpkt_t pkt = servo.read_goal_speed_packet(1);
  mvpkt_t pkt = AX12::makeReadPacket(1, AX::GOAL_SPEED, 2);
  // pprint(pkt);
  int num = serial.write(pkt);
  cout << ">> sent: " << num << endl;
  usleep(8 * 8 + 500 + 6 * 8);
  int ret = serial.read();
  printf(">> read: %d\n", ret);
  // mvpkt_t ans = serial.buffer2packet(ret);
  // pprint(ans);

  return 0;
}
