
// #include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "tusb.h"
#include <mv.h>
#include <stdio.h>

#include "pico/cyw43_arch.h"

constexpr uint32_t DD0_PIN  = 2;
constexpr uint32_t DD1_PIN  = 3;
constexpr uart_pins_t UART0PINS = uart0_pins[1];
constexpr uart_pins_t UART1PINS = uart1_pins[0];

// UART -----------------------------------------------------------
pico_uart_set(UART0PINS);
pico_uart_set(UART1PINS);
bi_decl(bi_1pin_with_func(DD0_PIN, GPIO_FUNC_SIO));

int main() {
  // Enable UART so we can print status output
  stdio_init_all();
  // stdio_usb_init(); // do this in cmake right now
  // pico_stdio_uart(); // do this in cmake right now

  // wait for usb serial connection
  // while (!tud_cdc_connected()) {
  //   sleep_ms(500);
  // }

  cyw43_arch_init();

  // Protocol1::PacketDecoder dec;

  ServoPort s;
  uint32_t baud = s.open(UART0PINS,DD0_PIN);

  ServoPort ss;
  uint32_t baud2 = ss.open(UART1PINS,DD1_PIN);

  mvpkt_t serv = AX12::makeMovePacketDeg(1,140.0f);
  s.write(serv);

  while (1) {
    printf("ServoPort at: %d  %d\n", (int)baud, (int)baud2);
    
    mvpkt_t outpkt = Protocol1::makePingPacket();

    s.write(outpkt);
    s.write(outpkt);
    s.write(outpkt);
    s.write(outpkt);
    s.write(outpkt);

    s.write(outpkt);
    s.write(outpkt);
    s.write(outpkt);
    s.write(outpkt);
    s.write(outpkt);

    mvpkt_t inpkt = ss.read();
    if (inpkt.size() > 0) {
      // printf("inpkt size: %d\n", (int)inpkt.size());
      // for (const uint8_t &b: inpkt) printf("%d,", (int)b);
      // printf("\n");
      mvpkts_t recv;
      bool ok = Protocol1::parsePacket(inpkt, recv);
      printf("messages found UART 1: %s %d\n", ok ? "ok" : "bad", (int)recv.size());
    }
    else printf("*** inpkt no data on UART 1 ***\n");


    mvpkt_t inpkt2 = s.read();
    if (inpkt2.size() > 0) {
      // printf("inpkt2 size: %d\n", (int)inpkt2.size());
      // for (const uint8_t &b: inpkt2) printf("%d,", (int)b);
      // printf("\n");
      mvpkts_t recv2;
      bool ok = Protocol1::parsePacket(inpkt2, recv2);
      printf("messages found UART 0: %s %d\n", ok ? "ok" : "bad", (int)recv2.size());
    }
    else printf("*** inpkt2 no data on UART 0 ***\n");

    sleep_ms(1000);
  }
}