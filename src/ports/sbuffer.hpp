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
#include "mv/structs.hpp" // mvpkt_t


template <uint16_t BUFFER_SIZE> class SBuffer {
  uint8_t buffer[BUFFER_SIZE];
  uint16_t tail{0}; // write / push / newest data
  uint32_t overruns{0}; // how many times we couldn't read all data?
  uint16_t max_usage{0}; // how full does the buffer ever get?

public:

  SBuffer() {}

  inline const uint16_t size() const { return tail; }
  const uint16_t buffer_size{BUFFER_SIZE};
  const uint16_t max_buffer_usage() { return max_usage; }

  inline
  bool push(const uint8_t b) volatile {
    if (tail == BUFFER_SIZE) {
      overruns += 1;
      return false;
    }
    buffer[tail] = b;
    tail += 1;
    max_usage = tail > max_usage ? tail : max_usage;
    return true;
  }

  const uint32_t get_overruns() volatile { return overruns; }

  // calling function turns off interrupt so this doesn't
  // get interrupted
  mvpkt_t get() volatile {
    const uint16_t size = tail;
    
    mvpkt_t pkt(size, 0);
    memcpy(pkt.data(), (const void *)buffer, size);
    // memset((void*)buffer,0,size);

    printf("buffer max usage: %d\n", (int)max_usage);
    
    tail = 0;
    return pkt; // RVO or vector move used, don't call std::move()
  }
};















/**
 This is a FIFO, but with a fixed size that will over write old
 data. It is designed to be simple and fast with low overhead.
*/
// template <uint16_t BUFFER_SIZE> class Fifo {
//   // public:
//   uint8_t buffer[BUFFER_SIZE];
//   uint16_t head{0}; // read / pop / oldest data
//   uint16_t tail{0}; // write / push / newest data
//   uint16_t numElem{0};

// public:
//   Fifo() {}

//   inline const uint16_t size() const volatile { return numElem; }
//   inline bool isFull() volatile { return numElem >= BUFFER_SIZE; }
//   inline bool isEmpty() volatile { return numElem == 0; }
//   inline uint16_t nextPos(const size_t pos) volatile {
//     return (pos + 1) % BUFFER_SIZE;
//   }

//   void clear() volatile {
//     head    = 0;
//     tail    = 0;
//     numElem = 0;
//   }

//   void push(const uint8_t b) volatile {
//     if (isFull()) {
//       head    = nextPos(head); // drop oldest
//       numElem = numElem - 1;
//     }
//     buffer[tail] = b;
//     tail         = nextPos(tail);
//     numElem += 1;
//   }

//   int16_t pop() volatile {
//     if (isEmpty()) return -1;
//     uint8_t ret = buffer[head];
//     head        = nextPos(head);
//     numElem     = numElem - 1;
//     if (numElem == 0) clear();
//     return ret;
//   }

//   // uint16_t copy(uint8_t *p, const uint16_t size) volatile {
//   //   memcpy(p, (const void *)buffer, size);
//   //   return size;
//   // }

//   // uint16_t copy(uint8_t *p, const uint16_t size) volatile {
//   //   if (tail > head) {
//   //     uint16_t n = tail - head;
//   //     n = n > size ? size : n;
//   //     memcpy(p, &buffer[head], n);
//   //     head += n;
//   //     return n;
//   //   }
//   //   else {
//   //     uint16_t n = BUFFER_SIZE - head;
//   //     n = n > size ? size : n;
//   //     uint16_t save = n;
//   //     memcpy(p, &buffer[head], n);
//   //     if (n == size) {
//   //       head += n;
//   //       return save;
//   //     }
//   //     n = size - n;
//   //     n = n > tail ? tail : n;
//   //     memcpy(&p[save], &buffer[n], n);
//   //     head = n;
//   //     return save + n;
//   //   }
//   // }
// };