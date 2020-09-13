#include <gtest/gtest.h> // googletest header file
#include <mv/mv.h>
#include <vector>

using namespace std;


void compare_packets(const packet& a, const packet& b){
    EXPECT_TRUE(a.size() == b.size());
    for (int i=0; i < a.size(); ++i) EXPECT_TRUE(a[i] == b[i]);
}

// move to 150 deg or 511 counts
TEST(protocol_1, move) {
    AX12 servo;

    packet pkt = servo.make_move_packet(1, 511);
    packet ans {255, 255, 1, 5, 3, 30, 255, 1, 216};

    compare_packets(pkt, ans);
}

TEST(protocol_1, sync_move){
    AX12 servo;

    vector<ServoMoveSpeed_t> s {
        {0, 16, 336}, // id:0, goal: 0x10, speed: 0x150
        {1, 544, 864} // 1, 0x220, 0x360
    };

    packet pkt = servo.make_sync_move_speed_packet(s);
    packet ans {255,255,254,14,131,30,4,0,16,0,80,1,1,32,2,96,3,103};

    compare_packets(pkt, ans);
}

TEST(protocol_1, ping){
    AX12 servo;
    packet pkt = servo.make_ping_packet(1);
    packet ans {0xFF,0xFF,0x01,0x02,0x01,0xFB};
    compare_packets(pkt, ans);
}

TEST(protocol_1, write){
    AX12 servo;
    packet pkt = servo.make_write8_packet(AX12::BROADCAST_ID, AX::ID, 1);
    packet ans {0xFF,0xFF,0xFE,0x04,0x03,0x03,0x01,0xF6};
    compare_packets(pkt, ans);

    packet pkt2 = servo.make_write16_packet(1, AX::GOAL_POSITION, 511);
    packet ans2 {255, 255, 1, 5, 3, 30, 255, 1, 216};
    compare_packets(pkt2, ans2);

    packet pkt3 = servo.make_write_packet(1, AX::GOAL_POSITION, 512, 300);
    packet ans3 {0xff,0xff,0x01,0x07,0x03,0x1e,0x00,0x02,0x2c,0x01,0xa7};
    compare_packets(pkt3, ans3);
}
