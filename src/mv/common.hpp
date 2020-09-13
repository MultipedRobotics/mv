#pragma once


// #include <array>
#include <vector>
#include <stdint.h>
#include <unistd.h> // sleep

// typedef uint8_t byte;
typedef std::vector<uint8_t> packet;
typedef std::vector<packet> packets;
typedef std::vector<uint8_t> parameters;

typedef struct {
    uint8_t id;
    uint16_t count;
} ServoMove_t;

typedef struct {
    uint8_t id;
    uint16_t count;
    uint16_t speed;
} ServoMoveSpeed_t;

// typedef struct {
//     uint8_t id;
// } ServoInfo_t;

// typedef std::vector<ServoInfo_t> PingResponse;
// typedef std::vector<ServoMove_t> SyncMoveAngles;
// typedef std::vector<ServoMove_t> SyncMoveAngles;

static void msleep(unsigned int msec){
    usleep(1000*msec);
}

static void calc_delay(uint16_t current, uint16_t last){
    // int delay = 0;
    // delay = abs(3-4);
    // if (current >= last) delay = int(1000.0*double(current-last)/1207.14);
    // else delay = int(1000.0*double(last-current)/1207.14);
    // // cout << ">> delay: " << delay << endl;
    // msleep(delay);
    msleep(int(1000.0*double(abs(current-last))/1207.14));
}
