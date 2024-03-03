#include <math.h>
#include <stdio.h>

#include "calcs.h"

static double get_adc_diff_voltage(int adc_code)
{
    return (adc_code*VOLTAGE_REFERANCE)/ADC_STEPS;
}

Signed_Voltage get_measurement_voltage(int adc_code)
{
    Signed_Voltage voltage;
    int magnitude_adc_code = adc_code & MAGNITUDE_MASK;
    double diff_voltage = get_adc_diff_voltage(magnitude_adc_code); 
    double I = diff_voltage/DIVIDER_LOWER_RESISTOR;
    voltage.magnitude = I*(DIVIDER_UPPER_RESISTOR+DIVIDER_LOWER_RESISTOR);
    voltage.sign = (uint8_t)(adc_code & SIGN_MASK); 
    return voltage;
}

double get_resistance(int adc_code)
{
    int magnitude_adc_code = adc_code & MAGNITUDE_MASK;
    double diff_voltage = get_adc_diff_voltage(magnitude_adc_code);
    double series_resistor_volt = COMPONENT_VOLTAGE_REFERANCE - diff_voltage - MEASUREMENT_BIAS;
    double I = series_resistor_volt/COMPONENT_SERIES_RESISTOR;
    return diff_voltage/I;
}
