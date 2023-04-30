#include <gtest/gtest.h> // googletest header file
#include <mv.h>
#include <vector>

using namespace std;
using namespace Protocol1;

void compare_packets(const Packet_t &a, const Packet_t &b) {
  EXPECT_TRUE(a.size() == b.size());
  for (int i = 0; i < a.size(); ++i)
    EXPECT_TRUE(a[i] == b[i]);
}

// move to 150 deg or 511 counts
TEST(ax12, move) {
  AX12 servo;

  Packet_t pkt = servo.makeMovePacket(1, 511);
  Packet_t ans{255, 255, 1, 5, 3, 30, 255, 1, 216};

  compare_packets(pkt, ans);
}

TEST(ax12, syncMove) {
  AX12 servo;

  vector<ServoMoveSpeed_t> s{
      {1, 100, 0}, {2, 200, 0}, {3, 300, 0}, {4, 400, 0}};

  Packet_t pkt = servo.makeMovePacket(s);
  Packet_t ans{255, 255, 254, 16, 131, 30, 2, 1,   100, 0,
             2,   200, 0,   3,  44,  1,  4, 144, 1,   90};

  compare_packets(pkt, ans);
}

TEST(ax12, syncMoveSpeed) {
  AX12 servo;

  vector<ServoMoveSpeed_t> s{
      {0, 16, 336}, // id:0, goal: 0x10, speed: 0x150
      {1, 544, 864} // 1, 0x220, 0x360
  };

  Packet_t pkt = servo.makeMovePacket(s);
  Packet_t ans{255, 255, 254, 14, 131, 30, 4,  0, 16,
             0,   80,  1,   1,  32,  2,  96, 3, 103};

  compare_packets(pkt, ans);
}

TEST(ax12, ping) {
  AX12 servo;
  Packet_t pkt = servo.makePingPacket(1);
  Packet_t ans{0xFF, 0xFF, 0x01, 0x02, 0x01, 0xFB};
  compare_packets(pkt, ans);
}

TEST(ax12, write) {
  AX12 servo;
  Packet_t pkt = servo.makeWritePacket(0XFE, AX::ID_REG, (uint8_t)1);
  Packet_t ans{0xFF, 0xFF, 0xFE, 0x04, 0x03, 0x03, 0x01, 0xF6};
  compare_packets(pkt, ans);

  Packet_t pkt2 = servo.makeWritePacket(1, AX::GOAL_POSITION_REG, (uint16_t)511);
  Packet_t ans2{255, 255, 1, 5, 3, 30, 255, 1, 216};
  compare_packets(pkt2, ans2);

  Packet_t pkt3 = servo.makeWritePacket(1, AX::GOAL_POSITION_REG, 512, 300);
  Packet_t ans3{0xff, 0xff, 0x01, 0x07, 0x03, 0x1e, 0x00, 0x02, 0x2c, 0x01, 0xa7};
  compare_packets(pkt3, ans3);
}
