
#if defined(__AVR__)
#include "unittest_transport.h"

#include <avr/io.h>
#include <util/delay.h>

#define USART_BAUDRATE 57600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void unittest_uart_begin() {
  /*Set baud rate */
  UBRR0H = (unsigned char)(BAUD_PRESCALE>>8);
  UBRR0L = (unsigned char)BAUD_PRESCALE;
  /* Enable receiver and transmitter */
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);
  /* Set frame format: 1stop bit, 8data */
  UCSR0C = (0<<USBS0)|(3<<UCSZ00);
}

void unittest_uart_putchar(char c) {
  /* Wait for empty transmit buffer */
  while (!(UCSR0A & (1<<UDRE0)));
  /* Put data into buffer, sends the data */
  UDR0 = c;
}

void unittest_uart_flush() {
  /* Wait for empty transmit buffer */
  while (!(UCSR0A & (1<<UDRE0)));
}

void unittest_uart_end() {
}

#endif