/*
 * @file ping.h
 *
 * @brief Header file for ping functions
 *
 * @author Axel Zumwalt, Allan Juarez
 * @date 3/14/2019
 */

#ifndef PING_H_
#define PING_H_

#include <stdint.h>
#include <inc/tm4c123gh6pm.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"
//
//volatile int state = 0;
//volatile int time_first = 0;
//volatile int time_last = 0;

void TIMER3B_Handler(void);

void ping_init(void);

double ping_read(void);

void switch_function(void);

void ping_send(void);

#endif /* PING_H_ */
