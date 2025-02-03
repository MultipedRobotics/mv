

#pragma once

#include <hardware/gpio.h>
#include <hardware/irq.h>
#include <hardware/uart.h>
#include <stdint.h>

///////////////////////////////////////////////////////////

/**
 This is a FIFO, but with a fixed size that will over write old
 data. It is designed to be simple and fast with low overhead.
*/
template <uint16_t BUFFER_SIZE> class Fifo {
  // public:
  uint8_t buffer[BUFFER_SIZE];
  uint16_t head{0}; // read / pop / oldest data
  uint16_t tail{0}; // write / push / newest data
  uint16_t numElem{0};

public:
  Fifo() {}

  inline const uint16_t size() const volatile { return numElem; }
  inline bool isFull() volatile { return numElem >= BUFFER_SIZE; }
  inline bool isEmpty() volatile { return numElem == 0; }
  inline uint16_t nextPos(const size_t pos) volatile {
    return (pos + 1) % BUFFER_SIZE;
  }

  void clear() volatile {
    head    = 0;
    tail    = 0;
    numElem = 0;
  }

  void push(const uint8_t b) volatile {
    if (isFull()) {
      head    = nextPos(head); // drop oldest
      numElem = numElem - 1;
    }
    buffer[tail] = b;
    tail         = nextPos(tail);
    numElem      += 1;
  }

  uint8_t pop() volatile {
    if (isEmpty()) return 0;
    uint8_t ret = buffer[head];
    head        = nextPos(head);
    numElem     = numElem - 1;
    if (numElem == 0) clear();
    return ret;
  }

  // uint16_t copy(uint8_t *p, const uint16_t size) volatile {
  //   if (tail > head) {
  //     uint16_t n = tail - head;
  //     n = n > size ? size : n;
  //     memcpy(p, &buffer[head], n);
  //     head += n;
  //     return n;
  //   }
  //   else {
  //     uint16_t n = BUFFER_SIZE - head;
  //     n = n > size ? size : n;
  //     uint16_t save = n;
  //     memcpy(p, &buffer[head], n);
  //     if (n == size) {
  //       head += n;
  //       return save;
  //     }
  //     n = size - n;
  //     n = n > tail ? tail : n;
  //     memcpy(&p[save], &buffer[n], n);
  //     head = n;
  //     return save + n;
  //   }
  // }
};

///////////////////////////////////////////////////////////

constexpr uint UART0_TX_PIN    = 0;
constexpr uint UART0_RX_PIN    = 1;
constexpr uint UART1_TX_PIN    = 8;
constexpr uint UART1_RX_PIN    = 9;

constexpr uint32_t tx_valid[2] = { // uart0, uart1
    (1 << 0) | (1 << 12) | (1 << 16) | (1 << 28),
    (1 << 4) | (1 << 8) | (1 << 20) | (1 << 24)};
constexpr uint32_t rx_valid[2] = { // uart0, uart1
    (1 << 1) | (1 << 13) | (1 << 17) | (1 << 29),
    (1 << 5) | (1 << 9) | (1 << 21) | (1 << 25)};

constexpr uint16_t UART_BUFFER_SIZE = 128;
static volatile Fifo<UART_BUFFER_SIZE> buffer_0;
static volatile Fifo<UART_BUFFER_SIZE> buffer_1;

// do I need to return size?
inline void fast_read_to_fifo(uart_inst_t *uart,
                              volatile Fifo<UART_BUFFER_SIZE> *fifo) {
  uart_hw_t *hw = (uart_hw_t *)uart;

  fifo->push((uint8_t)hw->dr);
  fifo->push((uint8_t)hw->dr);
  fifo->push((uint8_t)hw->dr);
  fifo->push((uint8_t)hw->dr);

  // #define UART_UARTFR_RXFE_BITS   _u(0x00000010)
  // If the FIFO is enabled, the RXFE bit is set when the receive FIFO is empty.
  // while ((hw->fr & UART_UARTFR_RXFE_BITS) == 2) {
  // while (!(hw->fr & UART_UARTFR_RXFE_BITS) ) {
  //   fifo->push( (uint8_t) hw->dr );
  // }
}

static void uart0_irq_func(void) {
  irq_set_enabled(UART0_IRQ, false);
  fast_read_to_fifo(uart0, &buffer_0);
  // while (uart_is_readable(uart0)) {
  //   uint8_t b = (uint8_t)uart_getc(uart0);
  //   buffer_0.push(b);
  // }
  irq_set_enabled(UART0_IRQ, true);
}

static void uart1_irq_func(void) {
  irq_set_enabled(UART1_IRQ, false);
  // printf("IRQ enabled: %u", (uint)irq_is_enabled(UART1_IRQ));
  fast_read_to_fifo(uart1, &buffer_1);
  irq_set_enabled(UART1_IRQ, true);
}


class ServoPort {
  uart_inst_t *uart;
  uint32_t dd{0};
  volatile Fifo<UART_BUFFER_SIZE> *buffer{nullptr};

public:
  ServoPort() {}
  ~ServoPort() { uart_deinit(uart); }

  uint init(uint baudrate, uint8_t port, uint8_t pin_tx, uint8_t pin_rx, uint8_t pin_dd) {
    bool valid = (1 << pin_tx) & tx_valid[port];
    if (!valid) return 0;
    valid = (1 << pin_rx) & rx_valid[port];
    if (!valid) return 0;

    // IRQ will fire when 4 bytes are in FIFO RX buffer
    const bool RX_EN = true; // enable IRQ fro RX
    const bool TX_EN = false;
    if (port == 0) {
      uart     = uart0;
      buffer   = &buffer_0;
      baudrate = uart_init(uart0, baudrate);
      gpio_set_function(pin_tx, GPIO_FUNC_UART);
      gpio_set_function(pin_rx, GPIO_FUNC_UART);
      irq_set_exclusive_handler(UART0_IRQ, uart0_irq_func);
      irq_set_enabled(UART0_IRQ, true);
      uart_set_irq_enables(uart0, RX_EN, TX_EN); // uart, rx, tx
    }
    else if (port == 1) {
      uart     = uart1;
      buffer   = &buffer_1;
      baudrate = uart_init(uart1, baudrate);
      gpio_set_function(pin_tx, GPIO_FUNC_UART);
      gpio_set_function(pin_rx, GPIO_FUNC_UART);
      irq_set_exclusive_handler(UART1_IRQ, uart1_irq_func);
      irq_set_enabled(UART1_IRQ, true);
      uart_set_irq_enables(uart1, RX_EN, TX_EN); // uart, rx, tx
    }
    else return 0;

    uart_set_fifo_enabled(uart, true);
    uart_set_translate_crlf(uart, false);

    gpio_init(pin_dd);
    gpio_set_dir(pin_dd, GPIO_OUT);
    dd = pin_dd;
    gpio_put(dd, 0);

    return baudrate;
  }

  inline bool is_enabled() { return uart_is_enabled(uart); }
  inline void flush() { buffer->clear(); }
  inline uint set_baud(uint baud) { return uart_set_baudrate(uart, baud); }
  inline size_t available() { return buffer->size(); }

  inline uint8_t read() { return buffer->pop(); }
  size_t read(uint8_t *data, size_t size) {
    size_t cnt   = 0;
    size_t bsize = buffer->size();
    while ((cnt < bsize) && (cnt < size)) {
      data[cnt++] = buffer->pop();
    }

    return cnt;
    // return buffer->copy(data, size);
  }

  void write(const uint8_t *data, size_t size) {
    gpio_put(dd, 1);
    uart_write_blocking(uart, data, size);
    gpio_put(dd, 0);
  }
};