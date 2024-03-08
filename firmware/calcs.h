//
// FILENAME: calcs.h
//
// description: This file contains constants and constructs that
// are used in the calculations. Also function prototypes. 
//
// Written by Marek Newton
//

#ifndef CALCS_H
#define CALCS_H

#define SAMPLE_PERIOD 0.00177

#define VOLTAGE_REFERANCE 2.316
#define MEASUREMENT_BIAS 1.158 
#define ADC_BITS 23
#define ADC_STEPS 8388608 

#define DIVIDER_UPPER_RESISTOR 511000
#define DIVIDER_LOWER_RESISTOR 9980

#define COMPONENT_SERIES_RESISTOR 10000
#define COMPONENT_VOLTAGE_REFERANCE 2.49

#define SIGN_MASK 0x800000
#define BIT_MASK_24 0xFFFFFF

#define LOW_OHM_THRESHOLD 10
#define OUT_OF_RANGE_THRESHOLD 50000

#define CAPACITANCE_SAMPLE_COUNT 2
#define CAP_VS 1.36
#define CAP_RS COMPONENT_SERIES_RESISTOR

typedef struct 
{
    double magnitude;
    uint8_t sign;
} Signed_Voltage;

Signed_Voltage get_measurement_voltage(uint32_t adc_code);
double get_resistance(uint32_t adc_code);
double get_capacitor_voltage(uint32_t adc_code);
double get_capacitance(double *voltage_points);

uint8_t low_ohm_condition(double resistance);
uint8_t out_of_range_condition(double resistance);

#endif
