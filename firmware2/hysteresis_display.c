#include "hysteresis_display.h"

#include <math.h>

static double last_display_value = 0;

double display_hysteresis(double new_value)
{
    if(fabs(new_value - last_display_value) > HYSTERESIS) last_display_value = new_value;
    return fabs(last_display_value);
}

uint8_t display_hysteresis_sign(void)
{
    return last_display_value < 0;
}