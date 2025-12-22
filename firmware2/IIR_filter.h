#ifndef IIR_FILTER_H
#define IIR_FILTER_H

#include <stdint.h>

#define DISPLAY_WINDOW_SAMPLES 50

typedef struct {
    float alpha;
    float out;
    float reading;
    uint16_t window_samples;
} IIR_Filter;

void IIR_filter_init(IIR_Filter *filter, float alpha);
float IIR_filter_sample(IIR_Filter *filter, float sample);

#endif