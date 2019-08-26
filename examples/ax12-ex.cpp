#include <iostream>
#include <string>
#include <unistd.h>

#include "ax12.hpp"

using namespace std;

int main() {

    AX12 servo;
    // servo.open("/dev/ttyS0", 17);

    while (true){
        int pos = rand()%250;
        cout << pos << endl;
        sleep(1);
    }
}
