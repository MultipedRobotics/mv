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

  // int delay = int(1000.0 * 50.0 / 1207.14);

  /////////////////////////////////
  //  This sucks!!!!!!!!!!!!
  /////////////////////////////////

  // for (int i=0; i<1024; i+=50){
  //     printf(">> Count: %d\n", i);
  //     mvpkt_t mv = servo.make_move_packet(1, i);
  //     if (serial.write(mv) != mv.size()) printf("*** wrote wrong amount!\n");
  //     msleep(delay);
  //
  //     // resp: [s,s,id,len,err,moving,chksum]
  //     uint8_t m = 1;
  //     int cnt = 10;
  //     mvpkt_t chk = servo.make_moving_packet(1);
  //     serial.write(chk);
  //     // pprint(chk);
  //     while (m != 0) {
  //         if (cnt-- == 0) {
  //             serial.write(chk);
  //             cnt = 10;
  //         }
  //         // printf("write\n");
  //         // serial.write(chk);
  //         msleep(100);
  //         // printf(".\n");
  //         // fprintf(stderr,".");
  //         int num = serial.read();
  //         // printf("%d\n", num);
  //
  //         if (num > 0){
  //             status_t ans = serial.decode();
  //             if (ans.error == 0 && ans.params.size() > 0) m = ans.params[0];
  //             // moving mvpkt_t
  //             // printf("m %d", int(m));
  //         }
  //         // else printf("%d\n", num);
  //     }
  //
  //     // printf("DONE\n");
  // }

  mvpkt_t mv = servo.make_move_packet(1, 512);
  serial.write(mv);
  delay(500);

  return 0;
}
