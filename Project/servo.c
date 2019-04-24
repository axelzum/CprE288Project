/*
 * @file servo.c
 *
 * @brief Contains the functions called by main to enable and use the servo motor.
 *
 * @author Axel Zumwalt, Allan Juarez
 * @date 3/28/2019
 */

#include "servo.h"
#include <math.h>

volatile int servo_position = 0;
volatile int movement_direction = 1; //1 == counterclockwise, c == clockwise

/**
 * Initilize servo motor with timer in PWM mode
 *
 * @author Axel Zumwalt, Allan Juarez
 *
 * @date 3/28/19
 *
 */
void servo_init(void) {
    //Setup GPIO
    SYSCTL_RCGCGPIO_R |= 0b10; //Enable clock on Port B
    SYSCTL_RCGCTIMER_R |= 0b10; //Enable timer 1 clock

    GPIO_PORTB_DIR_R |= 0x20; //Set pin 5 output
    GPIO_PORTB_DEN_R |= 0x20; //set pin 5 as digital mode

    GPIO_PORTB_AFSEL_R |= 0b00100000; //Use alternate function on port 5
    GPIO_PORTB_PCTL_R |= 0x00700000; //Use function T1CCP1


    //Setup Timer 1B
    TIMER1_CTL_R &= 0xFFFFFEFF; //Disable timer while we set it up
    //DISABLE TB INSTEA DOF TA

    TIMER1_CFG_R = 0x4; //Set to 16-bit mode

    TIMER1_TBMR_R |= 0b1010; //PWM, edge count, periodic timer //0b00000001010
    TIMER1_TBMR_R &= 0xFFFFFFFA;

    //  TIMER1_TBPR_R = 0x0005; //Use prescaler extension to 24 bits
    //TIMER1_TBILR_R = 0x3FC0; //Load max 24-bit value

    TIMER1_TBPR_R = 0x0004; //Use prescaler extension to 24 bits
    TIMER1_TBILR_R = 0xE200; //Load max 24-bit value 320000

    //Init to 0 degrees
    TIMER1_TBPMR_R = 0x0004; //use prescaler extension to 24 bits
    TIMER1_TBMATCHR_R = 0xC2C0; //load match value

    TIMER1_CTL_R &= 0x0D00;
    TIMER1_CTL_R |= 0x0D00; //enable //0b0000110100000000 //both edges, enable timer

}

/**
 * Move servo to specified angle
 *
 * @author Axel Zumwalt, Allan Juarez
 *
 * @date 3/28/19
 *
 */
int servo_move(float degrees) {
    if (movement_direction == 1) {
        servo_position += degrees;
    }
    else {
        servo_position -= degrees;
    }

    int match = (-160)*servo_position + 312000;
    int match_r = match & 0xFFFF;
    int match_prescale = match >> 16;

    if (degrees == 180) {
        //move to 180 degrees'

        TIMER1_TBPMR_R = 0x0004;
        TIMER1_TBMATCHR_R = 0x5240; // (.002*16000000) *2
        return 180;
    }
    else if (degrees == 0) {
        TIMER1_TBPMR_R = 0x0004;
        TIMER1_TBMATCHR_R =  0xC2C0; // (.001*16000000) /2
        return 0;
    }
    else {
        if(servo_position >= 180){
            TIMER1_TBPMR_R = 0x0004;
            TIMER1_TBMATCHR_R = 0x5240;
            return 180;
        }
        if(servo_position <= 0){
            TIMER1_TBPMR_R = 0x0004;
            TIMER1_TBMATCHR_R =  0xC2C0;
            return 0;
        }
        //move degrees
        TIMER1_TBPMR_R = match_prescale;
        TIMER1_TBMATCHR_R = match_r;
        return servo_position;
    }



}


