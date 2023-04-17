#pragma once

#include <Arduino.h>

constexpr bool DD_WRITE = true; // false
constexpr bool DD_READ  = !DD_WRITE;

struct SerialPort {
  void begin(Stream *ser, int pin) {
    serial = ser;
    dd_pin = pin;
    pinMode(dd_pin, OUTPUT);
  }

  int write(const packet &pkt) {
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

  int read() {
    int num = 0;
    uint8_t b[128];
    uint8_t *p = b;
    num        = 0;

    Protocol1 proto;

    set_dir(DD_READ);
    delay(10);
    num = serial->available();

    if (num == 0) return 0;

    num = serial->readBytes(p, num);
    set_dir(DD_WRITE);

    return num;
  }

  void set_dir(bool val) { digitalWrite(dd_pin, val ? 1 : 0); }

  Stream *serial;
  int dd_pin;
};