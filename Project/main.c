
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
#include "music.h"

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

//Calibrated to Bot 1
int main(void)
{
    lcd_init();
    uart_init();
    servo_init();
    adc_init();
    ping_init();

    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

    while(1) {
        char data[21];
        int data_index = 0;

        while(data_index < 20){

            data[data_index] = uart_receive();

            if(data[data_index] == '\r' ){
                data[data_index] = '\0';
                break;
            }
            else{

                data_index++;
            }
        }
        data[20] = '\0';

        char command[20], dtm[100];
        int parameter;

        strcpy(dtm, data);
        sscanf(dtm, "%s %d", command, &parameter);


        //Movement control with WASD (num)
        if(strcmp(command, "w") == 0){
            int arr2[7];
            move_forward_safely(sensor_data, parameter, arr2);
            char toPrint[50];
            sprintf(toPrint, "BL:%d BR:%d L:%d FL:%d FR:%d R:%d Read:%d", arr2[0], arr2[1], arr2[2], arr2[3], arr2[4], arr2[5], arr2[6]);
            uart_sendString(toPrint);
            uart_sendChar('\r');
            uart_sendChar('\n');
        }
        else if(strcmp(command, "s") == 0){
            move_backward(sensor_data,  parameter);
            uart_sendString("Backward");
            uart_sendChar('\r');
            uart_sendChar('\n');
        }
        else if(strcmp(command, "d") == 0){
            turn_right(sensor_data,  parameter);
            uart_sendString("Right");
            uart_sendChar('\r');
            uart_sendChar('\n');
        }
        else if(strcmp(command, "a") == 0){
            turn_left(sensor_data,  parameter);
            uart_sendString("Left");
            uart_sendChar('\r');
            uart_sendChar('\n');
        }

        else if(strcmp(command, "t") == 0) {
            turn_to(sensor_data, parameter);
            uart_sendString("Turn");
            uart_sendChar('\r');
            uart_sendChar('\n');
        }

        //Scan with s
        else if(strcmp(command, "p") == 0) {
            struct reading reading_array[180];
            take_reading(reading_array);
        }
        //oi_free end program
        else if(strcmp(command, "stop") ==0) {
            oi_free(sensor_data);
            return 0;
        }
        //               else if(strcmp(command, "destFound") == 0) {
        //                   lcd_printf("Preparing to \napproach green");
        //               }
        else if(strcmp(command, "horses") == 0){
            music_init1();
            music_playSong(0);
            timer_waitMillis(4500);
            music_init2();
            music_playSong(0);
        }

    }

}
