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

// Useful
// https://github.com/raspberrypi/pico-sdk/blob/master/src/rp2040/hardware_structs/include/hardware/structs/uart.h
// https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_uart
//
// uart_inst_t
// _REG_(UART_UARTDR_OFFSET) // UART_UARTDR
// Data Register, UARTDR
// 0x00000800 [11]    OE           (-) Overrun error
// 0x00000400 [10]    BE           (-) Break error
// 0x00000200 [9]     PE           (-) Parity error
// 0x00000100 [8]     FE           (-) Framing error
// 0x000000ff [7:0]   DATA         (-) Receive (read) data character
// io_rw_32 dr;

// _REG_(UART_UARTFR_OFFSET) // UART_UARTFR
// Flag Register, UARTFR
// 0x00000100 [8]     RI           (-) Ring indicator
// 0x00000080 [7]     TXFE         (1) Transmit FIFO empty
// 0x00000040 [6]     RXFF         (0) Receive FIFO full
// 0x00000020 [5]     TXFF         (0) Transmit FIFO full
// 0x00000010 [4]     RXFE         (1) Receive FIFO empty
// 0x00000008 [3]     BUSY         (0) UART busy
// 0x00000004 [2]     DCD          (-) Data carrier detect
// 0x00000002 [1]     DSR          (-) Data set ready
// 0x00000001 [0]     CTS          (-) Clear to send
// io_ro_32 fr;


// _REG_(UART_UARTIFLS_OFFSET) // UART_UARTIFLS
// Interrupt FIFO Level Select Register, UARTIFLS
// 0x00000038 [5:3]   RXIFLSEL     (0x2) Receive interrupt FIFO level select
// 0x00000007 [2:0]   TXIFLSEL     (0x2) Transmit interrupt FIFO level select
// io_rw_32 ifls;
//
// RXIFLSEL: Receive interrupt FIFO level select. The trigger points for the receive
// interrupt are as follows for 32 byte buffer: 
// 0 b000 = Receive FIFO becomes >= 1 / 8 full 
// 1 b001 = Receive FIFO becomes >= 1 / 4 full 
// 2 b010 = Receive FIFO becomes >= 1 / 2 full
// 3 b011 = Receive FIFO becomes >= 3 / 4 full 
// 4 b100 = Receive FIFO becomes >= 7 / 8 full 
// 5-7 b101-b111 = reserved.


#pragma once

#include <hardware/gpio.h>
#include <hardware/irq.h>
#include <hardware/uart.h>
#include <stdint.h>

#include "sbuffer.hpp"
#include "mv/structs.hpp"
#include "pico/cyw43_arch.h"

#define pico_uart_set(u) bi_decl(bi_2pins_with_func(u.rx, u.tx, GPIO_FUNC_UART));

typedef struct {
  uint32_t port, tx, rx;
} uart_pins_t;

// valid UART port and pin combinations
constexpr uart_pins_t uart0_pins[3]{{0, 0, 1}, {0, 12, 13}, {0, 16, 17}};
constexpr uart_pins_t uart1_pins[2]{{1, 4, 5}, {1, 8, 9}};

///////////////////////////////////////////////////

constexpr uint16_t UART_BUFFER_SIZE = 128;
static volatile SBuffer<UART_BUFFER_SIZE> rx_buffer0;
static volatile SBuffer<UART_BUFFER_SIZE> rx_buffer1;

static
void fast_read(const uint32_t uart_num) {

  uart_inst_t *uart = uart_num == 0? uart0 : uart1;
  volatile SBuffer<UART_BUFFER_SIZE> *buffer = uart_num == 0? &rx_buffer0 : &rx_buffer1;
  uart_hw_t *hw = uart_get_hw(uart);

  while (uart_is_readable(uart)) {
    buffer->push((uint8_t)hw->dr);
  }
}

static 
void uart0_irq_func(void) {
  irq_set_enabled(UART0_IRQ, false);
  fast_read(0);
  irq_set_enabled(UART0_IRQ, true);
}

static 
void uart1_irq_func(void) {
  irq_set_enabled(UART1_IRQ, false);
  fast_read(1);
  irq_set_enabled(UART1_IRQ, true);
}

static
void uart_set_fifo_level(uart_inst_t *uart, uint32_t level) {
  level = level > 4 ? 4 : level;
  uart_hw_t *hw = uart_get_hw(uart);
  hw->ifls = (level << 3) | (hw->ifls & 7); 
}


class ServoPort {
  static constexpr uint32_t DD_WRITE = 1;
  static constexpr uint32_t DD_READ  = 0;
  uart_inst_t *uart{nullptr};
  uint32_t dd_pin{0};
  volatile SBuffer<UART_BUFFER_SIZE> *buffer{nullptr};
  
public:
  ServoPort() {}
  ~ServoPort() { uart_deinit(uart); }

  uint32_t open(uart_pins_t uartpins, uint32_t pin_dd) {

    // IRQ will fire when 4 bytes are in FIFO RX buffer
    uint32_t baudrate = 1000000;
    const uint32_t port = uartpins.port;
    const uint32_t rx = uartpins.rx;
    const uint32_t tx = uartpins.tx;
    void (*irq_func)(void) = nullptr;
    uint32_t my_irq = 0;

    if (port == 0) {
      irq_func = uart0_irq_func;
      uart = uart0;
      buffer = &rx_buffer0;
      my_irq = UART0_IRQ;
    }
    else {
      irq_func = uart1_irq_func;
      uart = uart1;
      buffer = &rx_buffer1;
      my_irq = UART1_IRQ;
    }

    // UART
    baudrate = uart_init(uart, baudrate);
    gpio_set_function(tx, GPIO_FUNC_UART);
    gpio_set_function(rx, GPIO_FUNC_UART);

    // IRQ
    irq_set_exclusive_handler(my_irq, irq_func);
    irq_set_enabled(my_irq, true);
    uart_set_irq_enables(uart, true, false); // uart, rx, tx

    // FIFO
    uart_set_fifo_enabled(uart, true); // fifo is 32B long 
    uart_set_fifo_level(uart, 3);

    uart_set_hw_flow(uart, false, false); // cts, rts
    uart_set_format(uart, 8, 1, UART_PARITY_NONE); // AX-12
    // uart_set_translate_crlf(uart, false);

    // Data Direction Pin
    dd_pin = pin_dd;
    gpio_init(dd_pin);
    gpio_set_dir(dd_pin, GPIO_OUT);
    set_dir(DD_READ);

    return baudrate;
  }

  inline 
  void set_dir(uint32_t val) { 
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, val); // led
    gpio_put(dd_pin, val); 
  }

  inline
  mvpkt_t read() { return buffer->get(); }

  void write(const mvpkt_t &pkt) {
    set_dir(DD_WRITE);
    uart_write_blocking(uart, pkt.data(), pkt.size());
    // uart_tx_wait_blocking(uart);
    sleep_ms(1);
    set_dir(DD_READ);
    sleep_ms(2);
  }

  mvpkt_t write_read(const mvpkt_t &pkt) {
    write(pkt);
    return read(); // RVO or vector move used, don't call std::move()
  }

  int32_t get_dr_errors() { return (int32_t) (uart_get_hw(uart)->dr >> 8); }
  int32_t get_irq_level() { return (int32_t) (uart_get_hw(uart)->ifls & 0x00000038) >> 3; }

};