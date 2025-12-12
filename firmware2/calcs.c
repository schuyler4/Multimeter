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
    //return ((adc_code*VOLTAGE_REFERENCE)/ADC_STEPS)/8;
    return ((adc_code*VOLTAGE_REFERENCE)/ADC_STEPS);
}

double get_measurement_voltage(uint32_t adc_code)
{
    uint32_t magnitude_adc_code = get_adc_code_magnitude(adc_code);
    double diff_voltage = get_adc_diff_voltage(magnitude_adc_code);
    double I = diff_voltage/DIVIDER_LOWER_RESISTOR;
    double voltage = I*(DIVIDER_UPPER_RESISTOR+DIVIDER_LOWER_RESISTOR);
    uint8_t sign = (uint8_t)((adc_code & SIGN_MASK) >> ADC_BITS);
    if(sign)
    {
        return (voltage*-1);
    } 
    else 
    {
        return (voltage);
    } 
}

double get_diode_voltage(uint32_t adc_code)
{
    uint32_t magnitude_adc_code = get_adc_code_magnitude(adc_code);
    return get_adc_diff_voltage(magnitude_adc_code);
}

static double parallel_resistance(double r1, double r2)
{
    return 1/((1/r1)+(1/r2));
}

static double get_range_resistor(uint8_t range)
{
    if(range)
    {
        return COMPONENT_SERIES_RESISTOR_RANGE1;
    }
    else
    {
        return COMPONENT_SERIES_RESISTOR_RANGE2;
    }
}

double get_resistance(uint32_t adc_code)
{
    uint32_t magnitude_adc_code = get_adc_code_magnitude(adc_code);
    return get_adc_diff_voltage(magnitude_adc_code);
}

double scale_resistance(double resistance_reading)
{
    if(resistance_reading > RESISTANCE_MEGA_THRESHOLD)
    {
        return resistance_reading/RESISTANCE_MEGA_SCALE;
    }
    else if(resistance_reading > RESISTANCE_KILO_THRESHOLD)
    {
        return resistance_reading/RESISTANCE_KILO_SCALE;
    }
    else
    {
        return resistance_reading;
    }
}

uint8_t out_of_range_low_condition_resistance(double resistance, uint8_t range)
{
    if(range)
    {
        return resistance < OUT_OF_RANGE_LOW_THRESHOLD_RANGE1_RESISTANCE;
    }
    else
    {
        return resistance < OUT_OF_RANGE_LOW_THRESHOLD_RANGE2_RESISTANCE;
    }
}