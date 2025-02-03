
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "tusb.h"
#include <mv.h>


int main() {
  // Enable UART so we can print status output
  stdio_init_all();

  while (!tud_cdc_connected()) {
    printf("w");
    sleep_ms(500);
  }

  ServoPort s;

  uint8_t data[] = {1,2,3,4,5,6,7,8,9,0};
  s.write(data, 10);
  s.read(data, 10);
  s.write(data, 10);
  s.read(data, 10);
  s.write(data, 10);
  s.read(data, 10);
  s.write(data, 10);
  s.read(data, 10);
  s.write(data, 10);
  s.read(data, 10);
  s.write(data, 10);
  s.read(data, 10);

  while (1) {
    printf(".");
    sleep_ms(1000);
  }
}