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

#include "protocol1.hpp"
#include <Arduino.h>

constexpr int DD_WRITE = HIGH;
constexpr int DD_READ  = LOW;

/*
This is returned for Ping or most other commands to
indicate if an error occurred
*/
struct StatusPacket_t {
  uint8_t id;
  uint8_t err;

#if defined(ARDUINO) // FIXME: separate out
  void print() {
    Serial.print(">> pkt[");
    Serial.print(id);
    Serial.print("], error");

    // see if error bits are set
    if (err == 0) Serial.print(": none ");
    else {
      if (err & Protocol1::INPUT_VOLTAGE_ERR)
        Serial.print(": exceed input voltage ");
      if (err & Protocol1::ANGLE_LIMIT_ERR)
        Serial.print(": exceed angle limit ");
      if (err & Protocol1::OVERHEATING_ERR) Serial.print(": overheating ");
      if (err & Protocol1::RANGE_ERR) Serial.print(": exceed range limit ");
      if (err & Protocol1::CHECKSUM_ERR) Serial.print(": invalid checksum ");
      if (err & Protocol1::OVERLOAD_ERR) Serial.print(": overload ");
      if (err & Protocol1::INSTRUCTION_ERR)
        Serial.print(": invalid instruction ");
    }
  }

  void println() {
    print();
    Serial.println(" ");
  }
#endif
};

/**
  SerialPort
  - `dd_pin` is always in `DD_READ` and only changed to `DD_WRITE` in the write
  function
*/
struct SerialPort {
  SerialPort() : serial(nullptr), dd_pin(0) { set_dir(DD_READ); }

  void begin(Stream *ser, int pin) {
    serial = ser;
    dd_pin = pin;
    pinMode(dd_pin, OUTPUT);
  }

  inline int available() { return serial->available(); }

  // int deg2hex(int deg) {}

  int write(const mvpkt_t &pkt) {
    set_dir(DD_WRITE);

    int num = serial->write(pkt.data(), pkt.size());
    serial->flush(); // wait for outgoing data to complete

    // delay(1); // python: 8*pkt.size()/1E6
    // delayMicroseconds(500);
    set_dir(DD_READ);
    return num;
  }

  // int read() {
  //   // int num = 0;
  //   // uint8_t b[128];
  //   // uint8_t *p = b;
  //   // num        = 0;

  //   // Protocol1 proto;

  //   // set_dir(DD_READ);
  //   // delay(10); // FIXME: too long!
  //   int num = serial->available();
  //   // Serial.print("avail: ");
  //   // Serial.println(num);
  //   if (num == 0) return 0;

  //   // num = serial->readBytes(p, num);
  //   // set_dir(DD_WRITE);

  //   // return num;
  //   // memset((void*)buffer, 0, 128);
  //   num = serial->readBytes(buffer, num);
  //   bool ok = false;
  //   for (int i = 0; i < num; ++i) { // FIXME: not super thrilled with how I
  //   do this
  //     ok = proto.readPacket(buffer[i]);
  //     if (ok) {
  //       return proto.gpkt.size();
  //     }
  //   }
  //   return 0;
  // }

  mvpkt_t get_packet() { return proto.gpkt; }

  Statusmvpkt_t get_status() {
    StatusPacket_t s;
    s.id  = proto.gpkt[2];
    s.err = proto.gpkt[4];
    return s;
  }

  ReadStatus_t read() {
    ReadStatus_t rs;
    rs.ok   = false;

    int num = serial->available();
    bool ok = false;
    while (num) {
      for (int i = 0; i < num;
           ++i) { // FIXME: not super thrilled with how I do this
        ok = proto.readPacket(buffer[i]);
        if (ok) {
          rs.ok = ok;
          rs.pkts.push_back(proto.gpkt);
        }
      }
      num = serial->available();
    }
    return rs;
  }

  inline void set_dir(int val) { digitalWrite(dd_pin, val); }

  Stream *serial;
  int dd_pin; // data direction pin for read/write
  Protocol1::PacketManager proto;
  uint8_t buffer[64]; // why so big?
};