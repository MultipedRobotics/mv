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
  // // string port = "/dev/tty.usbserial-A904MISU";
  string port =
      "/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A904MISU-if00-port0";
  serial.begin(port);

  // vector<uint16_t> steps {0,511};
  //
  // for (const auto& p: steps){
  //     mvpkt_t mv = servo.make_move_packet(1, p);
  //     pprint(mv);
  //     serial.write(mv);
  //     msleep(500);
  // }

  // typedef struct {
  //     int bob = 5;
  //     // int tom = 55;
  // } reg_t;
  //
  //
  // class A {
  // public:
  //     A(ret_t& dd): d(dd) {}
  //     const reg_t d;
  // }

  // typedef struct {
  //     uint8_t id;
  //     array<uint16_t,2> params;
  // } stuff_t;
  //
  // stuff_t aa {10,{600, 200}};
  // stuff_t bb {12,{954,0}};
  //
  // for (const auto& p: aa.params) cout << p << endl;
  // cout << sizeof(aa) << "  " << sizeof(bb) << endl;
  //
  //
  // typedef struct {
  //     uint8_t id;
  //     // uint8_t id2;
  //     uint16_t params;
  //     uint16_t params2;
  // } stuff2_t;
  // stuff2_t cc {12,34,954};
  // cout << sizeof(cc) << endl;

  // vector<uint16_t> angles {0,233,765,457,986,234,511};
  // uint16_t last = 511;
  // uint16_t s = 0;
  //
  // for (const auto& v: angles){
  //     vector<ServoMoveSpeed_t> ss {
  //         {1, v,s},
  //         {2, 100,s},
  //         {3, 100,s},
  //         {4, 100,s}
  //     };
  //     mvpkt_t mv = servo.make_sync_move_packet(ss);
  //     pprint(mv);
  //     // serial.write(mv);
  //
  //     // delay(v, last);
  //     // last = v;
  // }

  // vector<ServoMove_t> s {
  //     {1, 100},
  //     {2, 200},
  //     {3, 300},
  //     {4, 400}
  // };

  // vector<ServoMoveSpeed_t> ss {
  //     {1, 100,0},
  //     {2, 200,0},
  //     {3, 300,0},
  //     {4, 400,0}
  // };
  //
  // vector<ServoMoveSpeed_t> sss {
  //     {1, 100,300},
  //     {2, 200,300},
  //     {3, 300,300},
  //     {4, 400,300}
  // };
  //
  // // mvpkt_t mv = servo.make_sync_move_packet(s);
  // // pprint(mv);
  //
  // mvpkt_t mv2 = servo.make_sync_move_speed_packet(ss);
  // pprint(mv2);
  //
  // cout << "-------------------------------"<< endl;
  //
  // // mvpkt_t mv3 = servo.make_sync_move_speed_packet(sss);
  // // pprint(mv3);
  //
  // mvpkt_t mv4 = servo.make_sync_move_speed_packet(sss);
  // pprint(mv4);

  if (1) {
    srand(time(0));
    mvpkt_t mv = servo.makeMovePacket(1, rand() % 1023);
    serial.write(mv);
    delay(500);
  }
  // Protocol1 pc;
  // mvpkt_t p = pc.make_read_packet(1, 32, 2);
  // pprint(p);
  // return 0;

  // mvpkt_t pkt = servo.read_goal_speed_packet(1);
  mvpkt_t pkt = servo.makePingPacket(1);
  // pprint(pkt);
  int num = serial.write(pkt);
  cout << ">> sent: " << num << endl;
  delay(500);
  int ret = serial.read();
  printf(">> read: %d\n", ret);
  // mvpkt_t ans = serial.buffer2packet(ret);
  // pprint(ans);

  return 0;
}
