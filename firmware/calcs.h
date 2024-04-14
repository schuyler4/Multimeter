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

#include <stdint.h>

#include "main.h"

//#define SAMPLE_PERIOD 0.00177
#define SAMPLE_PERIOD 0.00256

#if REVISION == 1
#define VOLTAGE_REFERENCE 2.316
#define MEASUREMENT_BIAS 1.158 
#define DIVIDER_LOWER_RESISTOR 10001
#define COMPONENT_VOLTAGE_REFERENCE 2.49
#else
#define VOLTAGE_REFERENCE 2.4
#define MEASUREMENT_BIAS 1.162
#define DIVIDER_LOWER_RESISTOR 10000
#define COMPONENT_VOLTAGE_REFERENCE 2.492
#define RANGE_SERIES_RESISTOR 649
#endif

#define ADC_BITS 23
#define ADC_STEPS 8388608 

#define DIVIDER_UPPER_RESISTOR 511000

#define COMPONENT_SERIES_RESISTOR_RANGE1 649
#define COMPONENT_SERIES_RESISTOR_RANGE2 200000

#define SIGN_MASK 0x800000
#define BIT_MASK_24 0xFFFFFF

#define OUT_OF_RANGE_LOW_THRESHOLD_RANGE1_RESISTANCE 7.5
#define OUT_OF_RANGE_HIGH_THRESHOLD_RANGE1_RESISTANCE 20000

#define OUT_OF_RANGE_LOW_THRESHOLD_RANGE2_RESISTANCE 1200
#define OUT_OF_RANGE_HIGH_THRESHOLD_RANGE2_RESISTANCE 5000000

#define CAPACITANCE_SAMPLE_COUNT 100
#define CAP_VS 1.36
#define CAP_RS COMPONENT_SERIES_RESISTOR

#define RESISTANCE_CALIBRATION_OFFSET 14

#define RESISTANCE_KILO_SCALE 1000
#define RESISTANCE_MEGA_SCALE 1000000

#define RESISTANCE_KILO_THRESHOLD 9999
#define RESISTANCE_MEGA_THRESHOLD 999999 

#define CAPACITANCE_MICRO_THRESHOLD 0.0000009999
#define CAPACITANCE_MICRO_SCALE 1000000
#define CAPACITANCE_NANO_SCALE 1000000000

#define OUT_OF_RANGE_LOW_THRESHOLD_CAPACITANCE 0.00000001

double get_measurement_voltage(uint32_t adc_code);

double get_capacitor_voltage(uint32_t adc_code);
double get_capacitance(double *voltage_points, uint8_t range);
double scale_capacitance(double capacitance_reading);

double get_resistance(uint32_t adc_code, uint8_t range);
double scale_resistance(double resistance_reading);
uint8_t out_of_range_low_condition_resistance(double resistance, uint8_t range);
uint8_t out_of_range_high_condition_resistance(double resistance, uint8_t range);

uint8_t out_of_range_low_condition_capacitance(double capacitance);

#endif
