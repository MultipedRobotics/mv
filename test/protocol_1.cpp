#include <gtest/gtest.h> // googletest header file
#include <mv.h>
#include <vector>

using namespace std;
// using namespace Protocol1;

void print(mvpkt_t pkt) {
  printf("pkt[%d]: ", (int)pkt.size());
  for (const uint8_t &b: pkt) printf("%d,", (int)b);
  printf("\n");
}

inline
void compare_packets(const mvpkt_t &a, const mvpkt_t &b) {
  EXPECT_TRUE(a.size() == b.size());
  for (int i = 0; i < a.size(); ++i)
    EXPECT_EQ(a[i], b[i]);
}

// move to 150 deg or 511 counts
TEST(ax12, move) {
  mvpkt_t pkt = AX12::makeMovePacketDeg(1, 150); // degrees
  // print(pkt);
  mvpkt_t ans{255, 255, 1, 5, 3, 30, 255, 1, 216};

  compare_packets(pkt, ans);

  pkt = AX12::makeMovePacket(1, 511); // counts
  // print(pkt);
  compare_packets(pkt, ans);
}

TEST(ax12, syncMove) {
  vector<Protocol1::ServoMoveSpeed_t> s{
      {1, 100, 0}, {2, 200, 0}, {3, 300, 0}, {4, 400, 0}};

  mvpkt_t pkt = AX12::makeMovePacket(s);
  // print(pkt);
  mvpkt_t ans{255, 255, 254, 16, 131, 30, 2, 1,   100, 0,
              2,   200, 0,   3,  44,  1,  4, 144, 1,   90};

  compare_packets(pkt, ans);
}

TEST(ax12, syncMoveSpeed) {
  vector<Protocol1::ServoMoveSpeed_t> s{
      {0, 16, 336}, // id:0, goal: 0x10, speed: 0x150
      {1, 544, 864} // 1, 0x220, 0x360
  };

  mvpkt_t pkt = AX12::makeMovePacket(s);
  mvpkt_t ans{255, 255, 254, 14, 131, 30, 4,  0, 16,
              0,   80,  1,   1,  32,  2,  96, 3, 103};

  compare_packets(pkt, ans);
}

TEST(ax12, ping) {
  mvpkt_t pkt = Protocol1::makePingPacket(1);
  mvpkt_t ans{0xFF, 0xFF, 0x01, 0x02, 0x01, 0xFB};
  compare_packets(pkt, ans);
}

TEST(ax12, write) {
  mvpkt_t pkt = Protocol1::makeWritePacket(0XFE, AX12::ID_REG, (uint8_t)1);
  mvpkt_t ans{0xFF, 0xFF, 0xFE, 0x04, 0x03, 0x03, 0x01, 0xF6};
  compare_packets(pkt, ans);

  mvpkt_t pkt2 = Protocol1::makeWritePacket(1, AX12::GOAL_POSITION_REG, (uint16_t)511);
  mvpkt_t ans2{255, 255, 1, 5, 3, 30, 255, 1, 216};
  compare_packets(pkt2, ans2);

  mvpkt_t pkt3 = Protocol1::makeWritePacket(1, AX12::GOAL_POSITION_REG, 512, 300);
  mvpkt_t ans3{0xff, 0xff, 0x01, 0x07, 0x03, 0x1e, 0x00, 0x02, 0x2c, 0x01, 0xa7};
  compare_packets(pkt3, ans3);
}


TEST(ax12, decode) {
  // 2 messages here
  mvpkt_t in{
    255, 255, 1, 5, 3, 30, 255, 1, 216,
    255, 255, 1, 5, 3, 30, 255, 1, 216
  };
  mvpkts_t out;
  bool ok = Protocol1::parsePacket(in, out);

  EXPECT_TRUE(ok);
  EXPECT_TRUE(out.size() == 2);
}
