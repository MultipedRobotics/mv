/******************************************************************************
MIT License

Copyright (c) 2019 Kevin J. Walchko

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#pragma once


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

static void delay(uint16_t current, uint16_t last){
    // int delay = 0;
    // delay = abs(3-4);
    // if (current >= last) delay = int(1000.0*double(current-last)/1207.14);
    // else delay = int(1000.0*double(last-current)/1207.14);
    // // cout << ">> delay: " << delay << endl;
    // msleep(delay);
    msleep(int(1000.0*double(abs(current-last))/1207.14));
}
