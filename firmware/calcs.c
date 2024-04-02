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

// Figure Out The Resistance 
// I = Vref-Vbias/(Rseries + Rrange + Rmeasure)
// I = Vmeasure/(Rmeasure + Rseries)
// Vmeasure/(Rmeasure + Rseries) = (Vref - Vbias)/(Rseries + Rrange + Rmeasure)
// Python Code to Solve
/***************************
import sympy as sp
from sympy.solvers import solve
 
Rm, Rs, Rr, Vr, Vb, Vm = sp.symbols('Rm, Rs, Rr, Vr, Vb, Vm')
solution = solve((Vm/(Rs + Rm)) - ((Vr - Vb)/(Rm + Rs +Rr)), Rm)  
print(solution[0])
****************************/
// Solution: (-Rr*Vm - Rs*Vb - Rs*Vm + Rs*Vr)/(Vb + Vm - Vr)  
double get_resistance(uint32_t adc_code, uint8_t range)
{
    double Rrange = get_range_resistor(range);
    double Rseries = parallel_resistance(DIVIDER_UPPER_RESISTOR, RANGE_SERIES_RESISTOR);
    printf("Rseries %f\n", Rseries);
    uint32_t magnitude_adc_code = get_adc_code_magnitude(adc_code);
    double Vmeasure = get_adc_diff_voltage(magnitude_adc_code);
    double Vbias = MEASUREMENT_BIAS;
    double Vref = COMPONENT_VOLTAGE_REFERENCE;
    double numerator = -Rrange*Vmeasure - Rseries*Vbias - Rseries*Vmeasure + Rseries*Vref;
    double denominator = Vbias - Vmeasure - Vref;
    return fabs(numerator/denominator);
}

double scale_resistance(double resistance_reading)
{
    if(resistance_reading > RESISTANCE_KILO_THRESHOLD)
    {
        return resistance_reading/RESISTANCE_KILO_SCALE;
    }
    else if(resistance_reading > RESISTANCE_MEGA_THRESHOLD)
    {
        return resistance_reading/RESISTANCE_MEGA_SCALE;
    }
    else
    {
        return resistance_reading;
    }
}

uint8_t out_of_range_low_condition(double resistance, uint8_t range)
{
    if(range)
    {
        return resistance < OUT_OF_RANGE_LOW_THRESHOLD_RANGE1;
    }
    else
    {
        return resistance < OUT_OF_RANGE_LOW_THRESHOLD_RANGE2;
    }
}

uint8_t out_of_range_high_condition(double resistance, uint8_t range)
{
    if(range)
    {
        return resistance > OUT_OF_RANGE_HIGH_THRESHOLD_RANGE1;
    }
    else
    {
        return resistance > OUT_OF_RANGE_HIGH_THRESHOLD_RANGE2;
    }
}

// v(t) = Vs*(1-e^(-t/T))
// v(t)/Vs = 1-e^(-t/T) 
// -v(t)/Vs + 1 = e^(-t/T) 
// ln(-v(t)/Vs + 1) = -t/T
// -T*ln(((-v(t)/Vs) + 1) = t
// -T*ln((-v2/Vs) + 1) + T*ln((-v1/Vs) + 1) = t2 - t1
double get_capacitance(double *voltage_points, uint8_t range)
{
    /*
    double v0 = *(voltage_points + 0);
    double v1 = *(voltage_points + 1);
    double Rs = get_component_series_resistor(range);
    printf("%f\n", v0);
    printf("%f\n", v1);
    return (1000*SAMPLE_PERIOD)/((log((-v1/CAP_VS)+1) - log((-v0/CAP_VS)+1))*-1*Rs);
    */
}


