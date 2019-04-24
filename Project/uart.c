/*
* @file uart.c
*
* @brief Contains the functions called by main enable and use uart communication
*
* @author Axel Zumwalt, Allan Juarez
* @date 2/21/2019
*/

#include "uart.h"
#include "lcd.h"

#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80


/**
* Uart initilization functions. Enables rx and tx over UART1 and a uart interrupt.
*
* @author Axel Zumwalt, Allan Juarez
*
* @date 2/21/19
*
*/
void uart_init(void) {

    //Turn on clock for UART1 and Port B
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R1;
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;

    //Enable alternate function for PB0, PB1
    GPIO_PORTB_AFSEL_R |= (BIT1 | BIT0);
    GPIO_PORTB_PCTL_R |= (BIT0 | BIT4);

    GPIO_PORTB_DEN_R |= (BIT1 | BIT0); //Set pins0,1 digital mode
    GPIO_PORTB_DIR_R &= ~BIT0; // Set pin 0 (Rx) to input(0)
    GPIO_PORTB_DIR_R |= BIT1;  // Set pin 1 (Tx) to output(1)

    //Disable UART0 device while we set it up
    UART1_CTL_R &= ~UART_CTL_UARTEN;

    // Set desired UART functionality
    UART1_CTL_R = 0b001100000000;

    //Set baud rate (115200 Baud)
    UART1_IBRD_R = 8; //16,000,000 / (16 * 115200) = 8.44
    UART1_FBRD_R = 44;
    UART1_CC_R = 0; //Use system clock as UART clock source

    //Set frame format: 1 start bit, 8databits, an odd parity bit, and 1 stop bit
    UART1_LCRH_R = (UART1_LCRH_R |0b11110010) & 0xFFFFFFF2;

    //Setup UART1 interrupts
    UART1_ICR_R |= 0b00010000; // Clear RX interrupt status flag
    UART1_IM_R  |= 0b00010000; // Enable RX interrupts

    //Configure NVIC to allow UART interrupts
    NVIC_PRI1_R |= 0x00020000;//Set UART1 pri to 1
    NVIC_EN0_R  |= 0x00000040;//Enable UART1


    IntRegister(INT_UART1, UART1_handler);

    //Re-enable UART1
    UART1_CTL_R = UART1_CTL_R | 0x01;
    IntMasterEnable(); //Globally allows CPU to service interrupts
}

/**
* Handeler for UART1 interrupt.
*
* @author Axel Zumwalt, Allan Juarez
*
* @date 2/21/19
*
*/
void UART1_handler() {

    //TODO there is a delay here.
    //Part 3
//   UART1_ICR_R |= 0b10000;
//   char data = uart_receive();
//   lcd_printf("%c", data);
//   uart_sendChar(data);

}

/**
* Sends a character over UART1
*
* @author Axel Zumwalt, Allan Juarez
* @param
*   data: Character to send.
*
* @date 2/21/19
*
*/
void uart_sendChar(char data) {

    while(UART1_FR_R & 0x20){

    }
    UART1_DR_R  = data;

}

/**
* Sends a string over UART1
*
* @author Axel Zumwalt, Allan Juarez
* @param
*   data: Character array to send.
*
* @date 2/28/19
*
*/
void uart_sendString(char data[]) {

    int i;
    for(i = 0; i < strlen(data); i++) {
        uart_sendChar(data[i]);
    }
}

/**
* Recieves a char over UART1 and returns it.
*
* @author Axel Zumwalt, Allan Juarez
* @return Character recieved over UART1.
*
* @date 2/21/19
*
*/
char uart_receive(void) {
    char data =0;

  while(UART1_FR_R & UART_FR_RXFE){

    }
    data =(char)(UART1_DR_R & 0xFF);
    return data;
}
