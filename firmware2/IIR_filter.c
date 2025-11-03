#include "IIR_filter.h"

void IIR_filter_init(IIR_Filter *filter, float alpha)
{
    filter->alpha = alpha;
    filter->out = 0;
}

float IIR_filter_sample(IIR_Filter *filter, float sample)
{
    filter->out = (1.0 - filter->alpha)*sample + filter->alpha*filter->out;
    return filter->out;
}