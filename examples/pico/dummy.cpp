
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

  while (1) {
    printf(".");
    sleep_ms(1000);
  }
}