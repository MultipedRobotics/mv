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
  Serial serial;
  // // string port = "/dev/tty.usbserial-A904MISU";
  string port =
      "/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A904MISU-if00-port0";
  serial.open(port);

  // vector<uint16_t> steps {0,511};
  //
  // for (const auto& p: steps){
  //     packet mv = servo.make_move_packet(1, p);
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
  //     packet mv = servo.make_sync_move_packet(ss);
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
  // // packet mv = servo.make_sync_move_packet(s);
  // // pprint(mv);
  //
  // packet mv2 = servo.make_sync_move_speed_packet(ss);
  // pprint(mv2);
  //
  // cout << "-------------------------------"<< endl;
  //
  // // packet mv3 = servo.make_sync_move_speed_packet(sss);
  // // pprint(mv3);
  //
  // packet mv4 = servo.make_sync_move_speed_packet(sss);
  // pprint(mv4);

  if (1) {
    srand(time(0));
    packet mv = servo.make_move_packet(1, rand() % 1023);
    serial.write(mv);
    msleep(500);
  }
  // Protocol1 pc;
  // packet p = pc.make_read_packet(1, 32, 2);
  // pprint(p);
  // return 0;

  // packet pkt = servo.read_goal_speed_packet(1);
  packet pkt = servo.make_ping_packet(1);
  pprint(pkt);
  int num = serial.write(pkt);
  cout << ">> sent: " << num << endl;
  msleep(500);
  int ret = serial.read();
  printf(">> read: %d\n", ret);
  packet ans = serial.buffer2packet(ret);
  // pprint(ans);

  return 0;
}
