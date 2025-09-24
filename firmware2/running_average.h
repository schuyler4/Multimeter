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

#define MAX_FILTER_LENGTH 30

// struct used to implement a running average filter
typedef struct running_average_filter
{
    uint16_t length;
    uint16_t data[MAX_FILTER_LENGTH];
    uint16_t data_length;
    uint16_t sample;
} Running_Average_Filter;

int running_average_filter_sample(Running_Average_Filter *filter);

#endif