#include "IIR_filter.h"

void IIR_filter_init(IIR_Filter *filter, float alpha)
{
    filter->alpha = alpha;
    filter->out = 0;
    filter->reading = 0;
    filter->window_samples = 0;
}

float IIR_filter_sample(IIR_Filter *filter, float sample)
{
    filter->window_samples += 1;
    filter->out = (1.0 - filter->alpha)*sample + filter->alpha*filter->out;
    if(DISPLAY_WINDOW_SAMPLES == filter->window_samples)
    {
        filter->window_samples = 0;
        filter->reading = filter->out;
    }
    return filter->reading;
}