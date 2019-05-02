/**
 * @file uart.h
 * @brief Class to receive uart interrupts and handle them
 * @author Adam Ford, Harrison Majerus
 * @date 02/28/19
 */

#ifndef UART_H_
#define UART_H_

/**
* Initializes the uart1
* @author Harrison Majerus, Adam Ford
* @date 2/28/19
*/
void uart_init();

/**
* Sends char data to Putty
* @author Harrison Majerus, Adam Ford
* @param data Char to be sent
* @date 2/28/19
*/
void uart_sendChar(char data);

/**Blocking call to receive over UART1
* returns char with data
* @author Harrison Majerus, Adam Ford
* @date 2/28/19
*/
char uart_receive(void);

/**
* Utilizes sendChar to print a full string of characters
* @param String to be printed onto putty
* @author: Adam Ford, Harrison Majerus
* @date: 2/28/19
**/
void uart_sendString(char* str);


#endif /* UART_H_ */
