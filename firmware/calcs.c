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
    return (adc_code*VOLTAGE_REFERENCE)/ADC_STEPS;
}

double get_capacitor_voltage(uint32_t adc_code)
{
    uint32_t adc_magnitude_code = get_adc_code_magnitude(adc_code);
    return get_adc_diff_voltage(adc_magnitude_code);
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
        return voltage*-1;
    } 
    else 
    {
        return voltage;
    } 
}

static double parallel_resistance(double r1, double r2)
{
    return 1/((1/r1)+(1/r2));
}

double get_component_series_resistor(uint8_t range)
{
    if(range)
    {
        return parallel_resistance(COMPONENT_SERIES_RESISTOR_RANGE1, DIVIDER_UPPER_RESISTOR);
    }
    else 
    {
        return parallel_resistance(COMPONENT_SERIES_RESISTOR_RANGE2, DIVIDER_UPPER_RESISTOR);
    }
}

double get_resistance(uint32_t adc_code, uint8_t range)
{
    uint32_t magnitude_adc_code = get_adc_code_magnitude(adc_code);
    double diff_voltage = get_adc_diff_voltage(magnitude_adc_code);
    double series_resistor_volt = COMPONENT_VOLTAGE_REFERENCE - diff_voltage - MEASUREMENT_BIAS;
    double Rs = get_component_series_resistor(range);
    double I = series_resistor_volt/Rs;
    double calculated_resistance = (diff_voltage*Rs)/series_resistor_volt; 
    return calculated_resistance + RESISTANCE_CALIBRATION_OFFSET;
}

// v(t) = Vs*(1-e^(-t/T))
// v(t)/Vs = 1-e^(-t/T) 
// -v(t)/Vs + 1 = e^(-t/T) 
// ln(-v(t)/Vs + 1) = -t/T
// -T*ln(((-v(t)/Vs) + 1) = t
// -T*ln((-v2/Vs) + 1) + T*ln((-v1/Vs) + 1) = t2 - t1
double get_capacitance(double *voltage_points, uint8_t range)
{
    double v0 = *(voltage_points + 0);
    double v1 = *(voltage_points + 1);
    double Rs = get_component_series_resistor(range);
    printf("%f\n", v0);
    printf("%f\n", v1);
    return (1000*SAMPLE_PERIOD)/((log((-v1/CAP_VS)+1) - log((-v0/CAP_VS)+1))*-1*Rs);
}

uint8_t low_ohm_condition(double resistance)
{
    return resistance < LOW_OHM_THRESHOLD; 
}

uint8_t out_of_range_condition(double resistance)
{
    return resistance > OUT_OF_RANGE_THRESHOLD;
}
