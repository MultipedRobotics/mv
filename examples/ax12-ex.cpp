#include <iostream>
#include <string>
#include <unistd.h>

#include "ax12.hpp"

using namespace std;

int main() {

    AX12 servo;
    // servo.open("/dev/ttyS0", 17);
    packet ping = servo.make_ping_packet(BROADCAST_ADDR);
    servo.write(ping);
    packets ans = servo.read();

// for (auto const& s: ans) cout << s.id << endl;

    while (true){
        int pos = rand()%250;
        cout << pos << endl;
        sleep(1);
    }
}
