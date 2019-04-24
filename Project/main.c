
#include "timer.h"
#include "open_interface.h"
#include "movement.h"
#include "servo.h"
#include "button.h"
#include "adc.h"
#include "uart.h"
#include "ping.h"
#include "analysis.h"
#include <math.h>

/**
 * @file main.c
 *
 * @brief Main for final project
 *
 * @author Axel Zumwalt, Allan Juarez, Adam Ford, Harrison Majerus
 *
 * @date 4/11/2019
 *
 */

//TODO before demo calibrate servo, ir sensor.
int main(void)
{
    lcd_init();
    uart_init();
    uart_interrupt_init();
    while(1){

    }
}
