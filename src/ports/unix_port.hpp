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

#include <array>   // buffer
#include <fcntl.h> // open
#include <mv.h>
#include <string>
#include <sys/ioctl.h> // dtr or rts
#include <termios.h>   // serial
#include <vector>

constexpr bool DD_WRITE = true; // false
constexpr bool DD_READ  = !DD_WRITE;

// macos is broken!!!
#ifndef B1000000
  #define B1000000 0010010
#endif

// #include "unix_termcolor.hpp"
//
// static void pprint(const packet& pkt, bool hex){
//     if ( hex ) std::cout << std::hex;

//     std::cout << '[';
//     // for (auto const& v: pkt) cout << int(v) << ",";
//     for (int i=0; i < pkt.size(); ++i){
//         int v = int(pkt[i]);
//         if (i == 0 || i == 1) std::cout << termcolor::yellow << v <<
//         termcolor::reset << ','; else if (i == 2) std::cout <<
//         termcolor::blue << v << termcolor::reset << ','; else if (i == 3)
//         std::cout << termcolor::green << v << termcolor::reset << ','; else
//         if (i == 4) std::cout << termcolor::cyan << v << termcolor::reset <<
//         ','; else if (i == 5) std::cout << termcolor::magenta << v <<
//         termcolor::reset << ','; else if (i == pkt.size()-1) std::cout <<
//         termcolor::yellow << v << termcolor::reset << ','; else std::cout <<
//         v << ',';
//     }
//     std::cout << ']' << std::endl;
//     if ( hex ) std::cout << std::dec;
// }

class SerialPort {
  int fd;
  // std::array<std::uint8_t, 512> buffer;
  void set_dir(bool enabled) {
    int dd_pin = TIOCM_DTR;
    int value  = enabled ? TIOCMBIS : TIOCMBIC;
    ioctl(fd, value, &dd_pin);
  }

public:
  SerialPort() {}
  ~SerialPort() {}

  void begin(const std::string &port) {
    int speed = B1000000;
    struct termios t;

    // O_RDWR means that the port is opened for both reading and writing.
    // O_NOCTTY means that no terminal will control the process opening the
    // serial port. fd = ::open(port.c_str(), O_RDWR|O_NOCTTY|O_NONBLOCK);
    fd = ::open(port.c_str(), O_RDWR | O_NOCTTY);
    if (fd < 0) {
      perror("Error opening serial port");
      return;
    }

    memset(&t, 0, sizeof(t)); // clear struct for new port settings

    t.c_cflag = speed | CS8 | CLOCAL | CREAD;
    t.c_iflag = IGNPAR;
    t.c_oflag = 0;
    t.c_lflag = 0;
    t.c_cc[VTIME] =
        0; // 10th of second, 1 = 0.1 sec, time to block before return
    t.c_cc[VMIN] = 0; // min number of characters before return

    // clean the buffer and activate the settings for the port
    if (tcflush(fd, TCIFLUSH) < 0) perror("*** Couldn't flush input buffer");
    if (tcsetattr(fd, TCSANOW, &t) < 0)
      perror("*** Couldn't set port attribute");

    set_dir(DD_READ);
    sleep_ms(100);
    set_dir(DD_WRITE);
  }

  // bool open(const std::string& port, int speed=B1000000);
  // void close();
  int write(const Packet_t &pkt) {
    set_dir(DD_WRITE);
    // int num = 0;
    // if ((num = available()) > 0) printf("** Data in input: %d\n", num);
    // flush_input();
    int ret = ::write(fd, pkt.data(), pkt.size());
    // for (const auto& p: pkt) ::write(fd, (void*)&p, 1);
    // int ret = pkt.size();
    // flush_output();

    // KEEP this delay ... not sure what the minimum should be
    // int min_time = 100;
    // int delay    = int(1e6 * double(pkt.size()) / 1e6);
    // delay        = delay > min_time ? delay : min_time;
    // printf(">> delay: %d\n", delay);
    usleep(500);
    set_dir(DD_READ);
    return ret;
  }

  int read() {
    int num = 0;
    // set_dir(DD_READ);
    // smallest packet is 6 bytes
    // int num = available();
    // printf(">> available: %d\n", num);
    // if (num < 6) return 0;

    // num = (num > 0) ? num : 10;
    //
    // buffer.fill(0);
    // size_t remain = num;
    // while(remain > 0){
    //     remain -= ::read(fd, buffer.data() + remain, remain);
    // }

    // num = ::read(fd, buffer.data(), buffer.size());
    uint8_t b[128];
    uint8_t *p = b;
    num        = 0;
    // num = ::read(fd, b, 10);
    for (int i = 0; i < 10; ++i) {
      num += ::read(fd, p, 1);
      if (*p < 0) printf("oops %d", *p);
      p++;
    }
    // num = ::read(fd, buffer.data(), num);

    // set_dir(DD_WRITE);
    // TODO: check error code and return it if error
    return num;
  }
  // packet buffer2packet(int num, int offset=0);
  // status_t decode();
  // void flush_input();
  // void flush_output();
  // void flush();
  int available() {
    int bytes_available;
    ioctl(fd, FIONREAD, &bytes_available);
    return bytes_available;
  }
};
