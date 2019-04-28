/*
 * @file movement.h
 *
 * @brief Header file contains the declarations of all the movement functions
 *
 * @date Feb 7, 2019
 * @author Axel Zumwalt, Allan Juarez, Harrison Majerus, Adam Ford
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "open_interface.h"

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
void move_forward(oi_t *sensor, int centimeters);
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
void move_backward(oi_t *sensor, int centimeters);
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
void turn_right(oi_t *sensor, double degrees);
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
void turn_left(oi_t *sensor, double degrees);
/**
 * Turns the robot left the number of degrees specified.
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
void turn_to(oi_t *sensor, int degree);
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
void move_forward_safely(oi_t *sensor, int centimeters, int *output);
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
void collision(oi_t *sensor);

//void userMovement(int direction,oi_t *sensor);
#endif /* MOVEMENT_H_ */
