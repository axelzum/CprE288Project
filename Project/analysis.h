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

struct reading {
    int degrees;
    float ir_distance;
    float sonar_distance;
};

struct object {
    int degree_start;
    int degree_stop;
    /*
    float ir_start;
    float ir_stop;
    float sonar_start;
    float sonar_stop;
    */
};

int detect_objects(struct reading *reading_array, struct object *object_array);

void find_smallest(struct reading *reading_array, struct object *object_array, int num_objects);

int find_gap(struct reading *reading_array, struct object *object_array);

void take_reading(struct reading *reading_array);

#endif /* ANALYSIS_H_ */
