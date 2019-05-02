/*
 * @file movement.c
 *
 * @brief Contains the functions called by main to move the robot.
 *
 * @author Axel Zumwalt, Allan Juarez, Adam Ford, Harrison Majerus
 * @date 2/7/2019
 */

#include "uart.h"
#include "movement.h"
#include "analysis.h"

/**
 * Moves the robot forward the number of centimeters specified.
 *
 * @author Axel Zumwalt, Allan Juarez
 * @param
 *   sensor: struct of type oi_t from open_interface
 *   centimeters: Number of centimeters to move forward.
 *
 * @date 2/7/19
 *
 */
void move_forward(oi_t *sensor, int centimeters) {

    int sum = 0;

    oi_setWheels(400, 400);

    while (sum < centimeters * 10) {
        oi_update(sensor);
        sum += sensor -> distance;
    }

    oi_setWheels(0,0);

    return;
}
/**
 * Moves the robot backward the number of centimeters specified.
 *
 * @author Axel Zumwalt, Allan Juarez
 * @param
 *   sensor: struct of type oi_t from open_interface
 *   centimeters: Number of centimeters to backward.
 *
 * @date 2/7/19
 *
 */
void move_backward(oi_t *sensor, int centimeters) {

    int sum = 0;

    oi_setWheels(-200, -200);

    while (sum > -1* centimeters * 10) {
        oi_update(sensor);
        sum += sensor -> distance;
    }

    oi_setWheels(0,0);

    return;
}
/**
 * Turns the robot right the number of degrees specified.
 *
 * @author Axel Zumwalt, Allan Juarez
 * @param
 *   sensor: struct of type oi_t from open_interface
 *   centimeters: Number of degrees to turn right.
 *
 * @date 2/7/19
 *
 */
void turn_right(oi_t *sensor, double degrees) {
    double sum = 0;

    degrees = degrees;

    oi_setWheels(-200, 200);

    while (sum > -1 * degrees) {
        oi_update(sensor);
        sum += sensor -> angle;
    }

    oi_setWheels(0,0);

    return;
}
/**
 * Turns the robot left the number of degrees specified.
 *
 * @author Axel Zumwalt, Allan Juarez
 * @param
 *   sensor: struct of type oi_t from open_interface
 *   degrees: Number of degrees to turn left.
 *
 * @date 2/7/19
 *
 */
void turn_left(oi_t *sensor, double degrees) {
    double sum = 0;

    degrees = degrees;

    oi_setWheels(200, -200);

    while (sum < degrees) {
        oi_update(sensor);
        sum += sensor -> angle;
    }

    oi_setWheels(0,0);

    return;
}

/**
 * Turns the robot to the degree specified
 *
 * @author Axel Zumwalt, Allan Juarez
 * @param
 *   sensor: struct of type oi_t from open_interface
 *    degrees: Degree to turn to.
 *
 *
 * @date 2/7/19
 *
 */
void turn_to(oi_t *sensor, int degree) {
    if (degree > 90) { //turn left
        turn_left(sensor, degree - 90);
    }
    else if (degree < 90) { //turn right
        turn_right(sensor, 90 - degree);
    }
}

/**
 * Moves the bot forward and stops if it collides with an object, reaches an edge, or arrives at a barrier.
 *
 * @author: Adam Ford, Harrison Majerus
 *
 * @param
 *    sensor: struct of type oi_t from open_interface
 *    centimeters: distance to move
 *    output: array to hold collision data
 *
 * @date: 4/25/19
 */
void move_forward_safely(oi_t *sensor, int centimeters, int *output){
    int sum = 0;
    oi_setWheels(200, 200);
    int bump = 0;
    int left = 0;
    int frontleft = 0;
    int right = 0;
    int frontright = 0;
    int reading = 0;
    while( sum < (centimeters*10)){
        oi_update(sensor);
        sum += sensor -> distance;
        //char hello[50];

        //Conditions to check for bumps, edge, or cliff
        if( sensor -> bumpLeft | sensor -> bumpRight){
            bump = 1;
        }
        if( sensor -> cliffLeft | ((sensor -> cliffLeftSignal > 2700) & (sensor -> cliffLeftSignal < 2800))){
            left = 1;
            if(sensor -> cliffLeftSignal != 0){ reading = sensor -> cliffLeftSignal;}
        }
        if( sensor -> cliffFrontLeft |  ((sensor -> cliffFrontLeftSignal > 2700) & (sensor -> cliffFrontLeftSignal < 2800))){
            frontleft = 1;
            if(sensor -> cliffFrontLeftSignal != 0){ reading = sensor -> cliffFrontLeftSignal;}
        }
        if( sensor -> cliffRight | ((sensor -> cliffRightSignal > 2700) & (sensor -> cliffRightSignal < 2800))){
            right = 1;
            if(sensor -> cliffRightSignal != 0){ reading = sensor -> cliffRightSignal;}
        }
        if( sensor -> cliffFrontRight | ((sensor -> cliffFrontRightSignal > 2700) & (sensor -> cliffFrontRightSignal < 2800))){
            frontright = 1;
            if(sensor -> cliffFrontRightSignal != 0){ reading = sensor -> cliffFrontRightSignal;}
        }
        if(bump + left + frontleft + right + frontright){
            break;
        }
    }


    oi_setWheels(0,0);

    char sum_char[20];
    sprintf(sum_char, "Distance Traveled: %dcm\r\n", sum/10);
    uart_sendString(sum_char);
    //0 = Clean exit
    //1 = Bump
    //2 = left
    //3 = frontleft
    //4 = frontright
    //5 = right
    output[0] = sensor -> bumpLeft;
    output[1] = sensor -> bumpRight;
    output[2] = left;
    output[3] = frontleft;
    output[4] = frontright;
    output[5] = right;
    output[6] = reading;
}
