/*
 * @file ping.h
 *
 * @brief Header file for servo functions
 *
 * @author Axel Zumwalt, Allan Juarez
 * @date 3/28/2019
 */

#ifndef SERVO_H_
#define SERVO_H_

#include <stdint.h>
#include <inc/tm4c123gh6pm.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"

extern volatile int servo_position;
extern volatile int movement_direction;

void servo_init(void);

int servo_move(float degrees);


#endif /* SERVO_H_ */
