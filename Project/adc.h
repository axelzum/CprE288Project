/*
 * @file adc.h
 *
 * @brief Header file contains the declarations for adc functions
 *
 * @date 3/7/2019
 * @author Axel Zumwalt, Allan Juarez
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>
#include <inc/tm4c123gh6pm.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"

void adc_init(void);

void adc_read(double *distance);

#endif /* ADC_H_ */
