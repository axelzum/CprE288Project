/*
 * @file ping.c
 *
 * @brief Contains the functions called by main to enable and use PING))).
 *
 * @author Axel Zumwalt, Allan Juarez
 * @date 3/14/2019
 */

#include "ping.h"
#include "timer.h"
#include <math.h>


volatile int function_state = 0;
volatile int edge_state = 0;
volatile int time_first = 0;
volatile int time_last = 0;
volatile int overflow_count = 0;

/**
 * Initilize PING))) with GPTM
 *
 * @author Axel Zumwalt, Allan Juarez
 *
 * @date 3/14/19
 *
 */
void ping_init(void) {
//Setup GPIO
    SYSCTL_RCGCGPIO_R |= 0b10; //Enable clock on Port B
    SYSCTL_RCGCTIMER_R |= 0x8; //Enable timer 3 clock

    GPIO_PORTB_AFSEL_R |= 0b00001000; //Use alternate function on port 3
    GPIO_PORTB_PCTL_R |= 0x00007000; //Use function T3CCP1

    GPIO_PORTB_DIR_R &= 0xF7; //Set pin 3 input
    GPIO_PORTB_DEN_R |= 0x8; //set pin 3 as digital mode

//Setup Timer 3B
    TIMER3_CTL_R &= 0xFFFFFEFF; //Disable timer while we set it up
    //DISABLE TB INSTEA DOF TA

    TIMER3_CFG_R = 0x4; //Set to 16-bit mode
    TIMER3_TBMR_R = 0b10111; //count up, edge time, capture mode
    TIMER3_CTL_R = 0xC00; //Detect both edge

    TIMER3_TBPR_R = 0xFF; //Use prescaler extension to 24 bits
    TIMER3_TBILR_R = 0xFFFF; //Load max 24-bit value

    TIMER3_ICR_R |= 0b10000000000; //Clear capture event interrupt status
    TIMER3_IMR_R |= 0b10000000000; //Enable capture event interrupts

//NVIC setup
    NVIC_PRI9_R |= 0x20; //Timer 3B pri=1
    NVIC_EN1_R |= 0x10; //Enable interrupt 36

    IntRegister(52, TIMER3B_Handler);

    TIMER3_CTL_R |= 0x00000100; //Reenable Timer3

    IntMasterEnable();
}

void TIMER3B_Handler() {
    if (edge_state == 0) { //state == rising edge

        edge_state = 1;
        time_first = TIMER3_TBR_R;
    }
    else if(edge_state == 1) { //state == falling edge
        edge_state = 0;
        time_last = TIMER3_TBR_R;
    }
    TIMER3_ICR_R |= 0b10000000000; //Clear capture event interrupt status
}

/**
 * Read from PING)))
 *
 * @author Axel Zumwalt, Allan Juarez
 *
 * @return Ping distance
 *
 * @date 3/14/19
 *
 */
double ping_read(void) {
    int pulse_width;
    double pulse_time;
    double distance;
    pulse_width = time_last - time_first;
    if (pulse_width < 0) { //Overflow occured
        pulse_width = time_last + (pow(2, 24) - 1 - time_first);
        overflow_count++;
    }
    pulse_time = (pulse_width *.0625) / 1000;
    distance = (340 * (pulse_time / 1000)) * 100 /2;
    return distance;
}

/**
 * Switch from PB3 output ping to PB3 input timer
 *
 * @author Axel Zumwalt
 *
 * @date 3/25/2019
 */
void switch_function(void) {
    if (function_state == 0) { //state == send pulse
        TIMER3_CTL_R &= 0xFFFFFEFF;
        function_state = 1;
        GPIO_PORTB_AFSEL_R &= 0xF7; //Disable alternate function port 3
        GPIO_PORTB_DIR_R |= 0x8; //set pin 3 as output

    }
    else if (function_state == 1) { //state == recieve pulse
        function_state = 0;
        GPIO_PORTB_AFSEL_R |= 0b00001000; //Enable alternate function port 3
        GPIO_PORTB_DIR_R &= 0xF7; //Set pin 3 input
        TIMER3_CTL_R |= 0x00000100; //Reenable Timer3
    }
}

/**
 * Send a pulse with the PING))) sensor
 *
 * @author Axel Zumwalt
 *
 * @date 3/25/2019
 */
void ping_send(void) {
    GPIO_PORTB_DATA_R = 0x8;
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R = 0x0;
}
