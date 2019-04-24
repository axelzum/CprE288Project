/*
 * @file adc.c
 *
 * @brief Contains the functions called by main to enable and use adc.
 *
 * @author Axel Zumwalt, Allan Juarez
 * @date 3/7/2019
 */

#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80

#include "adc.h"


/**
 * Initilize ADC.
 *
 * @author Axel Zumwalt, Allan Juarez
 *
 * @date 3/7/19
 *
 */
void adc_init(void) {
//Setup GPIO
    //Turn on clock for GPIO Port B
    SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R | 0b000010;

    //Enable alternate function NOT
   // GPIO_PORTB_AFSEL_R |= 0b00010100; //ADC channels 1&9 on pins 2&4

    //Configure pins for input and analog mode
    GPIO_PORTB_DIR_R &= 0b11101111; //Pin 4 set to input
    GPIO_PORTB_DEN_R &= 0b11101111; //Disable digital mode pin 4
    GPIO_PORTB_AMSEL_R |= 0b00010000; //Enable analog mode pin 4

    //Use default sample trigger source
    // By default, the processor (your program) is selected as the
    // trigger source and is initiated by the sample sequencer (SS)
    GPIO_PORTB_ADCCTL_R = 0x0; //Trigger in software

//Setup ADC
    // A) Configure ADC (choose ADC0 or ADC1)
    SYSCTL_RCGCADC_R |= 0x1; // Enable ADC0 clock
    //ADC0_ADCCC = 0x0; //Use SysClk as ADC clk. Default, OK if omitted
    GPIO_PORTB_ADCCTL_R |= 0b00010000;

    // Choose a sample sequencer (SS0, SS1, SS2 or SS3)
    // Disable ADC sample sequencer SS0 while configuring
    // OK to use SS1 or SS2 (cannot use SS3 for multiple samples)
    ADC0_ACTSS_R &= 0xFFFE; //Disable ADC0 SS0 by clearing bit 0
    // Set ADC SS0 functionality
    ADC0_EMUX_R &= 0xFFF0; //Set SS0 to trigger using ADCPSSI reg.
    ADC0_SSMUX0_R |= 0x0000000A; // Sample channel 10
    // Configure which sample is end of sequence (bit 1 of 4-bit
    // field), after which sample the RIS flag is asserted (bit 2)
    // Use first sample (channel 1 is first and last)
    ADC0_SSCTL0_R = 0x00000000; // First clear register to 0
    ADC0_SSCTL0_R |= 0x00000006; // Set bits 1 and 2 for first sample

    ADC0_SAC_R = 0x6;

    ADC0_ACTSS_R |= 0x1;

}

/**
 * Read distance from the IR Sensor
 *
 * @author Axel Zumwalt, Allan Juarez
 *
 * @date 3/7/19
 *
 */
void adc_read(double *distance) {
    // Initiate SS0 conversion sequence
    ADC0_PSSI_R = 0x1;
    // Wait for SS0 ADC conversions to complete
    while((ADC0_RIS_R & 0x1) == 0) // or (~ADC0_RIS_R & 0x1)
    {
    //wait
    }
    // Clear raw interrupt status flag for SS0
    ADC0_ISC_R = 0x01;

    // Get converted results from SS0 FIFO
    *distance = ADC0_SSFIFO0_R; // Channel 1

}


