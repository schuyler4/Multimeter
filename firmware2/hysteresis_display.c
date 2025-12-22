#include "hysteresis_display.h"

#include <math.h>

static double last_display_value = 0;
static double RESISTANCE_HYSTERESIS[] = {0, 0, 0.1, 0.01};

double display_hysteresis_voltage(double new_value)
{
    if(fabs(new_value - last_display_value) > HYSTERESIS_VOLTAGE) last_display_value = new_value;
    return fabs(last_display_value);
}

uint8_t display_hysteresis_sign_voltage(void)
{
    return last_display_value < 0;
}

double display_hysteresis_resistance(double new_value, uint8_t range_index)
{
    if(fabs(new_value - last_display_value) > RESISTANCE_HYSTERESIS[range_index]) last_display_value = new_value;
    return fabs(last_display_value);
}