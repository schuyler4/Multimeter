//
// FILENAME: calcs.c
//
// DESCRIPTION: This file contains all the calculations necissary to resolve a voltage
// or resistance reading from an ADC code.
//
// Written by Marek Newton
//

#include <math.h>
#include <stdio.h>
#include <stdint.h>

#include "calcs.h"

static uint32_t get_adc_code_magnitude(uint32_t adc_code)
{
    if((adc_code & SIGN_MASK) >> ADC_BITS)
    {
        // Negate the two's complement encoding
        return ((~adc_code) & BIT_MASK_24)+1;    
    }
    else
    {
        return adc_code;
    }
}

static double get_adc_diff_voltage(uint32_t adc_code)
{
    return (adc_code*VOLTAGE_REFERANCE)/ADC_STEPS;
}

Signed_Voltage get_measurement_voltage(uint32_t adc_code)
{
    Signed_Voltage voltage;
    voltage.sign = (uint8_t)((adc_code & SIGN_MASK) >> ADC_BITS); 
    uint32_t magnitude_adc_code = get_adc_code_magnitude(adc_code);
    double diff_voltage = get_adc_diff_voltage(magnitude_adc_code); 
    double I = diff_voltage/DIVIDER_LOWER_RESISTOR;
    voltage.magnitude = I*(DIVIDER_UPPER_RESISTOR+DIVIDER_LOWER_RESISTOR);
    printf("%f\n", voltage.magnitude);
    return voltage;
}

double get_resistance(uint32_t adc_code)
{
    uint32_t magnitude_adc_code = get_adc_code_magnitude(adc_code);
    double diff_voltage = get_adc_diff_voltage(magnitude_adc_code);
    double series_resistor_volt = COMPONENT_VOLTAGE_REFERANCE - diff_voltage - MEASUREMENT_BIAS;
    double I = series_resistor_volt/COMPONENT_SERIES_RESISTOR;
    return (diff_voltage*COMPONENT_SERIES_RESISTOR)/series_resistor_volt;
}

uint8_t low_ohm_condition(double resistance)
{
    return resistance < LOW_OHM_THRESHOLD; 
}

uint8_t out_of_range_condition(double resistance)
{
    return resistance > OUT_OF_RANGE_THRESHOLD;
}

