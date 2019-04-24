/*
 * @file analysis.c
 *
 * @brief Contains the functions called by main to analyze sensor data
 *
 * @author Axel Zumwalt, Allan Juarez
 * @date 4/4/2019
 */

#include "analysis.h"
#include "ping.h"
#include "adc.h"
#include "timer.h"
#include "servo.h"
#include "lcd.h"
#include "uart.h"
#include <math.h>


/**
 * Finds and indexs all objects in the dataset
 *
 * @author Axel Zumwalt, Allan Juarez
 * @param
 *   reading_array: Array containing dataset to detect
 *   object_array: Array that detected objects will be written to
 * @return total number of objects detected
 *
 * @date 4/4/19
 *
 */
int detect_objects(struct reading *reading_array, struct object *object_array) {

    int i;
    int object_array_index = 0;
    int object_detected = 0;

    //size_t reading_size = sizeof(reading_array) / sizeof(reading_array[0]);
    for (i = 0; i < 175; i++) {

        //Take a running average of the distance
        double ping_average = 0;
        int j;
        for (j = i; j < (i + 5); j++) {
            ping_average += reading_array[j].sonar_distance;
        }
        ping_average /= 5;

        //Start Object detection if, no current object and distance is less than 70
        if (ping_average < 70 && object_detected == 0) {

            object_detected = 1;
            object_array[object_array_index].degree_start = reading_array[i+5].degrees;
            //object_array[object_array_index].ir_start = reading_array[i].ir_distance;
            //object_array[object_array_index].sonar_start = reading_array[i].sonar_distance;

        }
        //End objects if distance is greater than 70.
        else if (ping_average > 70 && object_detected == 1) {

            //end object
            object_detected = 0;
            object_array[object_array_index].degree_stop = reading_array[i].degrees;
            //object_array[object_array_index].ir_stop = reading_array[i].ir_distance;
            //object_array[object_array_index].sonar_stop = reading_array[i].sonar_distance;
            object_array_index++;
        }
        if (object_array_index > 10) {
            break;
        }
    }
    //If the sweep ends on an object, end the object there
    if (object_detected == 1) {
        object_detected = 0;
        object_array[object_array_index].degree_stop = reading_array[i].degrees;
        //object_array[object_array_index].ir_stop = reading_array[i].ir_distance;
        //object_array[object_array_index].sonar_stop = reading_array[i].sonar_distance;
        object_array_index++;
    }

    return object_array_index;
}

/**
 * Calculates size of objects in an array and displays the smallest
 *
 * @author Axel Zumwalt, Allan Juarez
 * @param
 *   reading_array: Data array to pull distance from
 *   object_array: Array of objects to analyze
 *   num_objects: the size of object_array
 *
 * @return
 *   Returns a 0 if the smallest object was greater than 3in. Returns a 1 if the smallest object is smaller or a goal marker
 *
 * @date 4/4/19
 *
 */
int find_smallest(struct reading *reading_array, struct object *object_array, int num_objects) {
    int index;
    int smallest_index = 0;
    double smallest_width = 500;
    int smallest_location;
    double smallest_average_distance;

    //Test size of each object in array
    for(index = 0; index < num_objects; index++) {
        int i;
        double average_distance = 0;
        int radial_size = (object_array[index].degree_stop - object_array[index].degree_start);
        //If the object is really small its probably not real
        if (radial_size < 10) {
            continue;
        }

        for (i = object_array[index].degree_start; i < object_array[index].degree_stop; i++) {
            average_distance += reading_array[i].sonar_distance;
        }
        average_distance /= radial_size;

        double width = tan((radial_size * 3.1415) / (2 * 180)) * 2 * average_distance /2;

        if (width < smallest_width) {
            smallest_index = index;
            smallest_width = width;
            smallest_location = (object_array[index].degree_stop + object_array[index].degree_start) / 2;
            smallest_average_distance = average_distance;
        }

    }

    lcd_printf("Index:%d\nDist:%.3f\nLoc:%d\nWidth:%.3f", smallest_index+1, smallest_average_distance, smallest_location, smallest_width);

    servo_position = servo_move(0);
    servo_position = servo_move(smallest_location);

    if (smallest_width < 4) {
        return 1;
    }
    else {
        return 0;
    }
}
/**
 * Calculates the biggest size of the gap
 *  @param
 *   reading_array: Data array to pull distance from
 *   object_array: Array of objects to analyze
 *
 * @author Axel Zumwalt, Allan Juarez
 * @date 04/22/2019
 */
int find_gap(struct reading *reading_array, struct object *object_array, int num_objects){

    //If the goal has been found.
    if (find_smallest(reading_array, object_array, num_objects)) {
        return 500;
    }
    int robot_gap = 38;  //Threshhold of the robot size
    int i, location, biggest_gap_location, average_distance;
    int biggest_gap = 0;
    int radial_size = object_array[0].degree_start;


    //first case from 0 to first object
    average_distance = 0;
    for (i =0; i < object_array[0].degree_start; i++) {
        average_distance += reading_array[i].sonar_distance;
    }
    double width = tan((radial_size * 3.1415) / (2 * 180)) * 2 * average_distance /2;
    if(width > robot_gap){

        biggest_gap = width;
        biggest_gap_location = radial_size /2;
    }

    for(i =0; i<number_objects-1; i++){

        average_distance = 0;
        location = (object_array[i+1].degree_start + object_array[i].degree_stop) /2;
        radial_size = (object_array[i+1].degree_start + object_array[i].degree_stop);

        for (i =object_array[i].degree_stop; i < object_array[i].degree_stop; i++) {
            average_distance += reading_array[i].sonar_distance;
        }
        width = tan((radial_size * 3.1415) / (2 * 180)) * 2 * average_distance /2;

        if(width> robot_gap ){

            if(width > biggest_gap){
                biggest_gap = width;
                biggest_gap_location = location;
            }
        }
    }

    //last case from last object to 180
    if(180 - object_array[number_objects].degree_stop > biggest_gap){
        biggest_gap = 180 - object_array[number_objects].degree_stop;
    }

    servo_position = servo_move(0);
    servo_position = servo_move(biggest_gap_location);

    //TODO if no gap return -1
    return biggest_gap_location;
}

/**
 * Performs a scan and saves the data in reading_array
 *  @param
 *   reading_array: Data array to pull distance from
 *   object_array: Array of objects to analyze
 *
 * @author Axel Zumwalt, Allan Juarez
 * @date 04/24/2019
 */
void take_reading(struct reading *reading_array) {

    double ir_raw, ir_distance; //adc IR variables
    double ir_average;

    double ping_average;

    int i;

    int reading_index = 0;

    while (servo_position < 180) {

        //IR
        ir_average = 0;
        for (i = 0; i < 5; i++) {
            adc_read(&ir_raw);
            ir_distance = 81189*(pow(ir_raw, -1.132)); //TODO calibrate

            ir_average += ir_distance;
        }
        ir_average /= 5;

//        char ir_char[20];
//        sprintf(ir_char, "%f", ir_average);


        //PING)))
        ping_average = 0;
        for (i = 0; i < 5; i++) {
            switch_function();

            ping_send();

            switch_function();

            timer_waitMillis(20);

            ping_average += ping_read();
        }
        ping_average /= 5;

//        char ping_char[20];
//        sprintf(ping_char, "%f", ping_average);


        //Degrees
//        char degrees[5];
//        sprintf(degrees, "%d", servo_position);


//        //Print to UART
//        uart_sendString(degrees);
//        uart_sendChar('\t');
//        uart_sendString(ir_char);
//        uart_sendChar('\t');
//        uart_sendString(ping_char);
//        uart_sendChar('\r');
//        uart_sendChar('\n');

        reading_array[reading_index].degrees = servo_position;
        reading_array[reading_index].ir_distance = ir_average;
        reading_array[reading_index].sonar_distance = ping_average;
        reading_index++;

        //Move servo
        servo_position = servo_move(1);
    }

//    struct object object_array[10];
//    int num_objects = detect_objects(reading_array, object_array);

//    //Send all objects to UART
//    for (i = 0; i < num_objects; i++) {
//        char objects[20];
//        sprintf(objects, "%d", object_array[i].degree_start);
//        uart_sendString(objects);
//        uart_sendChar('\t');
//        sprintf(objects, "%d", object_array[i].degree_stop);
//        uart_sendString(objects);
//        uart_sendChar('\r');
//        uart_sendChar('\n');
//
//    }

//    find_smallest(reading_array, object_array, num_objects);

    return 0;
}
}


