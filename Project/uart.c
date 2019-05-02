/**
 * @file uart.c
 * @brief Class to receive uart interrupts and handle them
 * @author Adam Ford, Harrison Majerus
 * @date 02/28/19
 */

#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"
#include "uart.h"
#include "movement.h"
#include "analysis.h"
#include "open_interface.h"
#include <stdlib.h>
#include <stdio.h>
#include "music.h"
#include "ping.h"

//volatile struct reading reading_array[180];

char arr[21]; //Used to hold current string
int count = 0;  //Current index in char arr
int initialized = 0; //0 if oi has not been initialized
oi_t *sensor_data;  //Holds pointer to sensor data

/**
* Initializes the uart1
* @author Harrison Majerus, Adam Ford
* @date 2/28/19
*/
void uart_init(){
    //enable UART1
    SYSCTL_RCGCUART_R |= 0x02;
    //Enable clock on port B
    SYSCTL_RCGCGPIO_R |= 0x02;
    //Enable ports 1 and 2
    GPIO_PORTB_AFSEL_R |= 0x03;
    //Set muxs of ctrl
    GPIO_PORTB_PCTL_R |= 0x00000011;
    //Digital Enable
    GPIO_PORTB_DEN_R |= 0x03;
    GPIO_PORTB_DIR_R |= 0x2;
    GPIO_PORTB_DIR_R &= 0xFFFFFFFE;
    //Disable UART1 so we can change stuff safely
    UART1_CTL_R &= 0xFFFFFFFE;
    //Baud Rate
    UART1_IBRD_R = 0x8;
    UART1_FBRD_R = 0x2C;
    //Set Serial Parameters
    UART1_LCRH_R = 0x60;
    //Set clock to user systems
    UART1_CC_R = 0;
    //Enable it again
    UART1_CTL_R |= 0x1;
}


/**
* Sends char data to Putty
* @author Harrison Majerus, Adam Ford
* @param data Char to be sent
* @date 2/28/19
*/
void uart_sendChar(char data)
{
    //wait until there is room to send data
    while(UART1_FR_R & 0x20)
    {
    }
    //send data
    UART1_DR_R = data;
}

/**Blocking call to receive over UART1
* returns char with data
* @author Harrison Majerus, Adam Ford
* @date 2/28/19
*/
char uart_receive(){
    char data = 0;
    //wait to receive
    while(UART1_FR_R & UART_FR_RXFE)
    {
    }
    //mask the 4 error bits and grab only 8 data bits
    data = (char)(UART1_DR_R & 0xFF);
    return data;
}
/**
* Utilizes sendChar to print a full string of characters
* @param String to be printed onto putty
* @author: Adam Ford, Harrison Majerus
* @date: 2/28/19
**/
void uart_sendString(char *str){
    while(true){
        if(*str == '\0'){
            break;
        }
        uart_sendChar(*str);
        str++;
    }
}
