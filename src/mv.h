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

#include <stdint.h>
#include <vector>

typedef std::vector<uint8_t> Packet_t;
typedef std::vector<Packet_t> PktArray_t;

struct ServoMoveSpeed_t {
  uint8_t id;
  uint16_t count; // 0-300 deg or 0x0000-0x1023 counts
  uint16_t speed;
};

typedef std::vector<ServoMoveSpeed_t> SyncVec_t;

struct ReadStatus_t {
  bool ok;
  PktArray_t pkts;
};

union {
  struct {
    uint8_t hi, lo;
  };
  uint16_t u16;
} Memory_t;

#if defined(ARDUINO)
  #include <Arduino.h>
  #include <mv/arduino_port.hpp>
#else
  #include <unistd.h> // usleep
inline void delay(unsigned int msec) { usleep(1000 * msec); }
  #include <mv/unix_port.hpp>
#endif

#include <mv/ax12.hpp>
#include <mv/protocol1.hpp>
#include <mv/protocol2.hpp>