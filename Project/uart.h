/*
 * @file uart.h
 *
 * @brief Header file contains the declarations for uart functions
 *
 * @date Feb 21, 2019
 * @author Axel Zumwalt, Allan Juarez
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <inc/tm4c123gh6pm.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"


void uart_init(void);

void uart_sendChar(char data);

void uart_sendString(char string[]);

char uart_receive(void);

void init_uart_interrupts();

void UART1_handler();


#endif /* UART_H_ */
