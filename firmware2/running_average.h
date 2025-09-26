//
//  FILENAME: running_average.h
// 
// description: Header file for running_average.c
//
// Written by Marek Newton
//

#ifndef RUNNING_AVERAGE
#define RUNNING_AVERAGE

#include <stdint.h>

#define MAX_FILTER_LENGTH 1000

// struct used to implement a running average filter
typedef struct running_average_filter
{
    uint16_t length;
    double data[MAX_FILTER_LENGTH];
    uint16_t data_length;
    double sample;
} Running_Average_Filter;

double running_average_filter_sample(Running_Average_Filter *filter);

#endif