/**
 * @file uart.c
 * @brief Class to receive uart interrupts and handle them
 * @author Adam Ford, Harrison Majerus
 * @date 02/28/19
 */

#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"
#include "uart.h"
#include "movement.h"
#include "open_interface.h"
#include <stdlib.h>
#include <stdio.h>

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
    uart_sendChar('\r');
    uart_sendChar('\n');
}

/**
* Moves the Roomba if a specific command has been entered
* @author Harrison Majerus, Adam Ford
* @param word Pointer to string of current words
* @date 2/28/19
*/
void moveWithWords(char *word){

    if(initialized == 0){
        sensor_data = oi_alloc();
        oi_init(sensor_data);
        initialized++;
    }
    char first = word[0];
    char num[4] = {word[2],word[3],word[4],'\0'};
    int result = atoi(num);

    if(first == 'f'){
        int arr2[5] = move_forward_safely(sensor_data,  result);
        char toPrint[30];
        sprintf(toPrint, "B:%d L:%d FL:%d FR:%d R:%d", arr2[0], arr2[1], arr2[2], arr2[4], arr2[5]);
        uart_sendString(toPrint);
    }
    else if(first == 'b' ){
        move_backward(sensor_data,  result);
        uart_sendString("Backward");
    }
    else if(first == 'r' ){
        turn_right(sensor_data,  result);
        uart_sendString("Right");
    }
    else if(first == 'l' ){
        turn_left(sensor_data,  result);
        uart_sendString("Left");
    }

}

/**
* Handles the UART interrupts
* @author Harrison Majerus
* @date 2/28/19
*/
void uart_handler(){
    //clear interrupt status
    UART1_ICR_R |= 0x00000010;
    char charlie = uart_receive();
    if(charlie == '\r'){
        uart_sendChar('\r');
        uart_sendChar('\n');

        lcd_printf(arr);
        moveWithWords(arr);
        count=0;
    }
    else{
    uart_sendChar(charlie);
    arr[count] = charlie;
    count++;
    arr[count] = '\0';
        if(count == 20){
            uart_sendChar('\r');
            uart_sendChar('\n');
            lcd_printf(arr);
            count=0;
        }
    }
}

/**
* Initializes the uart interrupts
* @author Harrison Majerus, Adam Ford
* @date 2/28/19
*/
void uart_interrupt_init(){
        // Unmask
        UART1_ICR_R |= 0x00000010;
        UART1_IM_R |= 0x00000010;

        // Enable interrupt for UART1
        NVIC_PRI1_R |= 0x00020000;
        NVIC_EN0_R |= 0x00000040;

        // Bind the interrupt to the handler.
        IntRegister(INT_UART1, uart_handler);
}
