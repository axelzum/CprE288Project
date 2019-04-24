/*
 * @file movement.c
 *
 * @brief Contains the functions called by main to move the robot.
 *
 * @author Axel Zumwalt, Allan Juarez
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

    oi_setWheels(-400, -400);

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

    degrees = degrees + 4;

    oi_setWheels(-400, 400);

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
 *   centimeters: Number of degrees to turn left.
 *
 * @date 2/7/19
 *
 */
void turn_left(oi_t *sensor, double degrees) {
    double sum = 0;

    degrees = degrees + 4;

    oi_setWheels(400, -400);

    while (sum < degrees) {
        oi_update(sensor);
        sum += sensor -> angle;
    }

    oi_setWheels(0,0);

    return;
}

void userMovement(int direction, oi_t *sensor){
    int openAngle = 0;

    if(direction == 0){
        turn_left(sensor, 45);
    }
    if(direction == 1)
    {
        turn_right(sensor, 45);
    }
    if(direction == 2){
        turn_left(sensor, 180);
    }

    struct reading reading_array[180];
    take_reading(reading_array);

    struct object object_array[10];
    int num_objects = detect_objects(reading_array, object_array);
    openAngle = find_gap(reading_array, object_array, num_objects);
    // no open angles
    if(openAngle <0){
        uart_sendString("There were no open angles... turn bot left or right");
        return;
    }
    //when robot finds green
    else if(openAngle ==500){
        uart_sendString("Green is found");
        //greenFunction()
    }
    else{

        while(openAngle < 85 || openAngle >95){
            if(openAngle < 90){
                uart_sendString("Bot is turning Right");
                turn_right(sensor, 90 - openAngle);
                userMovement(3, sensor);
            }
            else if(openAngle > 90){
                uart_sendString("Bot is turning Left");
                turn_left(sensor, openAngle - 90);
                userMovement(3, sensor);
            }
        }
        return;
    }

}
/**
 *
 *
 */
void check_for_tape(oi_t *sensor){

    int check_left_value = sensor->cliffFrontLeftSignal;
    int check_right_value = sensor->cliffFrontRightSignal;

    if(check_left_value> 2000 || check_right_value >2000){
        move_backward(sensor, 15);
        uart_sendString("There is tape ahead dont go back");
    }
}

/**
 * Drives forward two meters, detecting for any bumps and driving around that.
 *
 * @author Axel Zumwalt, Allan Juarez
 * @param
 *   sensor: struct of type oi_t from open_interface
 *
 * @date 2/7/19
 *
 */
void collision(oi_t *sensor) {
    int dist_traveled = 0;

    oi_setWheels(400, 400);

    while (dist_traveled < 2000) {
        oi_update(sensor);
        dist_traveled += sensor -> distance;

        //TODO: check if the actions after bump are adding to total distance traveled.
        if (sensor -> bumpRight == 1) {
            move_backward(sensor, 15);
            turn_left(sensor, 90);
            move_forward(sensor, 25);
            turn_right(sensor, 90);
            oi_setWheels(400, 400);
            //dist_traveled -= sensor -> distance;
        }
        else if (sensor -> bumpLeft == 1) {
            move_backward(sensor, 15);
            turn_right(sensor, 90);
            move_forward(sensor, 25);
            turn_left(sensor, 90);
            oi_setWheels(400, 400);
            //dist_traveled -= sensor -> distance;
        }
    }

    oi_setWheels(0,0);

    return;
}
