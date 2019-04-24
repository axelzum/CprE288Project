/*
 * @file movement.h
 *
 * @brief Header file contains the declarations of all the movement functions
 *
 * @date Feb 7, 2019
 * @author Axel Zumwalt, Allan Juarez
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "open_interface.h"


void move_forward(oi_t *sensor, int centimeters);

void move_backward(oi_t *sensor, int centimeters);

void turn_right(oi_t *sensor, double degrees);

void turn_left(oi_t *sensor, double degrees);

int * move_forward_safely(oi_t *sensor, int centimeters);

void collision(oi_t *sensor);

void userMovement(int direction,oi_t *sensor);
#endif /* MOVEMENT_H_ */
