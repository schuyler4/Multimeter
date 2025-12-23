//
// FILENAME: calcs.c
//
// DESCRIPTION: This file contains all the calculations necessary to resolve a voltage
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

double get_adc_diff_voltage(uint32_t adc_code)
{
    return ((adc_code*VOLTAGE_REFERENCE)/ADC_STEPS);
}

double get_measurement_voltage(uint32_t adc_code)
{
    uint32_t magnitude_adc_code = get_adc_code_magnitude(adc_code);
    double diff_voltage = get_adc_diff_voltage(magnitude_adc_code);
    double I = diff_voltage/DIVIDER_LOWER_RESISTOR;
    double voltage = I*(DIVIDER_UPPER_RESISTOR+DIVIDER_LOWER_RESISTOR);
    uint8_t sign = (uint8_t)((adc_code & SIGN_MASK) >> ADC_BITS);
    if(sign) return (voltage*-1);
    else return voltage;
}

double get_diode_voltage(uint32_t adc_code)
{
    uint32_t magnitude_adc_code = get_adc_code_magnitude(adc_code);
    return get_adc_diff_voltage(magnitude_adc_code);
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