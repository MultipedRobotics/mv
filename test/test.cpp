#include <gtest/gtest.h> // googletest header file
#include <mv/mv.h>
#include <vector>

using namespace std;


void compare_packets(const packet& a, const packet& b){
    EXPECT_TRUE(a.size() == b.size());
    for (int i=0; i < a.size(); ++i) EXPECT_TRUE(a[i] == b[i]);
}

// move to 150 deg or 511 counts
TEST(AX12, move) {
    AX12 servo;

    packet pkt = servo.make_move_packet(1, 511);
    packet ans {255, 255, 1, 5, 3, 30, 255, 1, 216};

    compare_packets(pkt, ans);
}

TEST(Ax12, sync_move){
    AX12 servo;

    vector<ServoMoveSpeed_t> s {
        {0, 16, 336}, // id:0, goal: 0x10, speed: 0x150
        {1, 544, 864} // 1, 0x220, 0x360
    };

    packet pkt = servo.make_sync_write_packet(s);
    packet ans {255,255,254,14,131,30,4,0,16,0,80,1,1,32,2,96,3,103};

    compare_packets(pkt, ans);
}
