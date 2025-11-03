#ifndef IIR_FILTER_H
#define IIR_FILTER_H

typedef struct {
    float alpha;
    float out;
} IIR_Filter;

void IIR_filter_init(IIR_Filter *filter, float alpha);
float IIR_filter_sample(IIR_Filter *filter, float sample);

#endif