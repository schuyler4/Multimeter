#include <math.h>
#include <stdio.h>

#include "calcs.h"

static double get_adc_diff_voltage(int adc_code)
{
    //double LSB = VOLTAGE_REFERANCE/pow(2, ADC_RESOLUTION);
    //return LSB*adc_code;
    return (adc_code*VOLTAGE_REFERANCE)/ADC_STEPS;
}

double get_measurement_voltage(int adc_code)
{
    double diff_voltage = get_adc_diff_voltage(adc_code); 
    double I = diff_voltage/DIVIDER_LOWER_RESISTOR;
    return I*(DIVIDER_UPPER_RESISTOR+DIVIDER_LOWER_RESISTOR);
}
