#ifndef UART_H
#define UART_H

#include <zephyr/kernel.h>



int uart_init(void);
int uart_send(uint8_t *data, uint16_t len);
int uart_printf(const char *format, ...);

#endif // UART_H