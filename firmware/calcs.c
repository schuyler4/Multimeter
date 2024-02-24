#include <math.h>
#include "calcs.h"

static float get_adc_diff_voltage(int adc_code)
{
    float LSB = VOLTAGE_REFERANCE/pow(2, ADC_RESOLUTION);
    return LSB*adc_code;
}

