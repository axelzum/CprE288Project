/*
* @file analysis.h
*
* @brief Header file for analysis.c
*
* @author Axel Zumwalt, Allan Juarez
* @date 4/4/2019
*/

#ifndef ANALYSIS_H_
#define ANALYSIS_H_
/**
 * Struct that holds information about a scan
 */
struct reading {
    int degrees;
    float ir_distance;
    float sonar_distance;
};
/**
 * Struct that holds at what degrees an object starts and whenit stops
 */
struct object {
    int degree_start;
    int degree_stop;
};
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
int detect_objects(struct reading *reading_array, struct object *object_array);
/**
 * Calculates size of objects in an array and displays the smallest
 *
 * @author Axel Zumwalt, Allan Juarez
 * @param
 *   reading_array: Data array to pull distance from
 *   object_array: Array of objects to analyze
 *   num_objects: the size of object_array
 *
 *
 * @date 4/4/19
 *
 */
void find_smallest(struct reading *reading_array, struct object *object_array, int num_objects);

//int find_gap(struct reading *reading_array, struct object *object_array);
/**
 * Performs a scan and saves the data in reading_array
 *  @param
 *   reading_array: Data array to pull distance from
 *   object_array: Array of objects to analyze
 *
 * @author Axel Zumwalt, Allan Juarez
 * @date 04/24/2019
 */
void take_reading(struct reading *reading_array);

#endif /* ANALYSIS_H_ */
