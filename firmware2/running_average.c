//
// FILENAME: running_average.c
//
// description: Implements a causal running average filter. The number
// of points is defined with the filter length variable
//
// Written by Marek Newton
//

#include <stdio.h>

#include "running_average.h"

double running_average_filter_sample(Running_Average_Filter *filter)
{
    if(filter->data_length < filter->length)
    {
        filter->data_length++;
    }

    filter->data[0] = filter->sample;

    int i;
    for(i = filter->length - 1; i >= 0; i--)
    {
        if(i != 0)
        {
            filter->data[i] = filter->data[i-1];
        }
    }

    double sum = 0;

    for(i = 0; i < filter->data_length; i++)
    {
        sum += filter->data[i];
    }

    printf("total %f\n", sum);

    return (double)(sum/filter->data_length);
}
