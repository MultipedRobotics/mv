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
#include <mv/common.hpp>
#include <mv/protocol1.hpp>
#include <mv/protocol2.hpp>
#include <mv/ax12.hpp>

#if defined(ARDUINO)
  #include <Arduino.h>

  constexpr bool DD_WRITE = true;  // false
  constexpr bool DD_READ = !DD_WRITE;

  struct SerialPort {
    void begin(Stream *ser, int pin) {
      serial = ser;
      dd_pin = pin;
      pinMode(dd_pin, OUTPUT);
    }

    int write(const packet& pkt){
      set_dir(DD_WRITE);
      // int num = 0;
      // if ((num = available()) > 0) printf("** Data in input: %d\n", num);
      // flush_input();
      int ret = serial->write(pkt.data(), pkt.size());
      // for (const auto& p: pkt) ::write(fd, (void*)&p, 1);
      // int ret = pkt.size();
      // flush_output();

      // KEEP this delay ... not sure what the minimum should be
      // int min_time = 100;
      // int delay = int(1e6*double(pkt.size())/1e6);
      // delay = delay > min_time ? delay : min_time;
      // // printf(">> delay: %d\n", delay);
      // usleep(delay);
      delay(10);
      set_dir(DD_READ);
      return ret;
    }

    int read(){
      int num = 0;
      uint8_t b[128];
      uint8_t *p = b;
      num = 0;

      Protocol1 proto;

      set_dir(DD_READ);
      delay(10);
      num = serial->available();

      if (num == 0) return 0;

      num = serial->readBytes(p, num);
      set_dir(DD_WRITE);

      return num;
    }

    void set_dir(bool val) {
      digitalWrite(dd_pin, val ? 1 : 0);
    }

    Stream *serial;
    int dd_pin;
  };

#else
  #include <unix/SerialPort.hpp>
#endif