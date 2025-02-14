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

// mvpkt_t - to/from serial
typedef std::vector<uint8_t> mvpkt_t;
typedef std::vector<mvpkt_t> mvpkts_t;

// union {
//   struct {
//     uint8_t hi, lo;
//   };
//   uint16_t u16;
// } memory_t;

template<float window>
class WindowAve {
  float ave{0.0f};
  float tmp{0.0f};
  uint32_t cnt{static_cast<uint32_t>(window)};
  uint32_t win_len{static_cast<uint32_t>(window)};
  float win{1.0f / window};

public:
  float calc(float val) {
    tmp += val*win;
    if (cnt < win_len) cnt += 1;
    else {
      cnt = 0;
      ave = tmp;
      tmp = 0.0f;
    }
    return ave;
  }
};