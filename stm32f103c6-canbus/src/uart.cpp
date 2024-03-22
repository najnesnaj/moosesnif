#include <errno.h>
#include <stdarg.h>
#include <unistd.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>

#include <printf.h>

#include <queue.hpp>

namespace Uart {

namespace {
Queue<char, 256> _tx_buffer;
Queue<char, 256> _rx_buffer;
}

void init() {
  // For the peripheral to work, we need to enable it's clock
  rcc_periph_clock_enable(RCC_USART2);
  // From the datasheet for the STM32F0 series of chips (Page 30, Table 11)
  // we know that the USART1 peripheral has it's TX line connected as
  // alternate function 1 on port A pin 9.
  // In order to use this pin for the alternate function, we need to set the
  // mode to GPIO_MODE_AF (alternate function). We also do not need a pullup
  // or pulldown resistor on this pin, since the peripheral will handle
  // keeping the line high when nothing is being transmitted.
 
//difference F0 F1 
  //gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9 | GPIO10);
  //
  //
    // Configure GPIO pins for USART1 transmit (PA9) and receive (PA10)

    // Set the USART1 alternate function for the TX and RX pins

  //
  // Now that we have put the pin into alternate function mode, we need to
  // select which alternate function to use. PA9 can be used for several
  // alternate functions - Timer 15, USART1 TX, Timer 1, and on some devices
  // I2C. Here, we want alternate function 1 (USART1_TX)
//  gpio_set_af(GPIOA, GPIO_AF1, GPIO9 | GPIO10);
  // Now that the pins are configured, we can configure the USART itself.
  // First, let's set the baud rate at 115200
 // usart_set_baudrate(USART1, 115200);
  // Each datum is 8 bits
  //usart_set_databits(USART1, 8);
  // No parity bit
  //usart_set_parity(USART1, USART_PARITY_NONE);
  // One stop bit
  //usart_set_stopbits(USART1, USART_CR2_STOPBITS_1);
  // For a debug console, we only need unidirectional transmit
 // usart_set_mode(USART1, USART_MODE_TX_RX);
  // No flow control
 // usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
  // Enable the peripheral

        /* Setup GPIO pin GPIO_USART1_TX. */
        gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
                      GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART2_TX);
        /* Setup GPIO pin GPIO_USART1_RX. */
        gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
                      GPIO_CNF_INPUT_FLOAT, GPIO_USART2_RX);

        /* Setup UART parameters. */
        usart_set_baudrate(USART2, 115200);
        usart_set_databits(USART2, 8);
        usart_set_stopbits(USART2, USART_STOPBITS_1);
        usart_set_mode(USART2, USART_MODE_TX_RX);
        usart_set_parity(USART2, USART_PARITY_NONE);
        usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

        /* Finally enable the USART. */
        usart_enable(USART2);






  // Enable interrupts for tx/rx
  nvic_enable_irq(NVIC_USART2_IRQ);
  usart_enable_rx_interrupt(USART2);
  usart_enable_tx_interrupt(USART2);
}

void put(char c) {
  _tx_buffer.push(c);
  usart_enable_tx_interrupt(USART2);
}

bool get(char *c) { return _rx_buffer.pop(*c); }

void flush() {
  while (_tx_buffer.available()) {
  }
}

void isr() {
  // Check if interrupt is signalling TX ready
  if (usart_get_flag(USART2, USART_FLAG_TXE)) {
    // Try and pop a byte from the tx buffer
    char c;
    if (_tx_buffer.pop(c)) {
      // Move the next byte of data to the tx buffer
      usart_send(USART2, c);
    } else {
      // If there's no more data to be sent, disable this interrupt
      usart_disable_tx_interrupt(USART2);
    }
  }

  // Check if data has been received (RX Not Empty)
  if (usart_get_flag(USART2, USART_FLAG_RXNE)) {
    const uint16_t c = usart_recv(USART2);
    // Might fail if buffer is full!
    _rx_buffer.push(c);
  }
}

} // namespace Uart

void _putchar(char c) {
  if (c == '\n') {
    Uart::put('\r');
  }
  Uart::put(c);
}

void usart2_isr(void) { Uart::isr(); }

