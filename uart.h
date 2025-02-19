/*
 * File:        uart.h
 * Purpose:     UART function prototypes and constants
 *
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include "Common.h"

typedef unsigned char BYTE;

// Function prototypes
void uart0_init(void);
BYTE uart0_getchar(void);
void uart0_putchar(char ch);
void uart0_put(char *ptr_str);
BOOLEAN uart0_dataAvailable(void);

void uart2_init(void);
BYTE uart2_getchar(void);
void uart2_putchar(char ch);
void uart2_put(char *ptr_str);
BOOLEAN uart2_dataAvailable(void);
#endif /* UART_H_ */
