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

#include "mv/structs.hpp"

// fix time functions
#if defined(__APPLE__) || defined(__linux__) || defined(__unix__)
  #include <unistd.h> // usleep
  inline void sleep_us(unsigned int usec) { usleep(usec); }
  inline void sleep_ms(unsigned int msec) { usleep(1000 * msec); }
#endif

// os specific servo port
#if defined(PICO_BOARD)
  #include <ports/pico_port.hpp>
#elif defined(__APPLE__)
  #include <ports/unix_port.hpp>
#elif defined(__linux__)
  #include <ports/unix_port.hpp>
#endif

#include <mv/protocol1.hpp>
#include <mv/protocol2.hpp>
#include <mv/ax12.hpp>
#include <mv/xl320.hpp>
#include <mv/xl430.hpp>