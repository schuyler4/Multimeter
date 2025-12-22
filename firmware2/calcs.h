#ifndef CALCS_H
#define CALCS_H

#include <stdint.h>

#include "main.h"

//#define SAMPLE_PERIOD 0.00177
#define SAMPLE_PERIOD 0.00256

#define VOLTAGE_REFERENCE 2.326
#define MEASUREMENT_BIAS 1.162
#define DIVIDER_LOWER_RESISTOR 100000
#define COMPONENT_VOLTAGE_REFERENCE 2.492
#define RANGE_SERIES_RESISTOR 649

#define ADC_BITS 23
#define ADC_STEPS 8388608 

#define DIVIDER_UPPER_RESISTOR 5110000

#define SIGN_MASK 0x800000
#define BIT_MASK_24 0xFFFFFF

#define RESISTANCE_KILO_SCALE 1000
#define RESISTANCE_MEGA_SCALE 1000000

#define RESISTANCE_KILO_THRESHOLD 9999
#define RESISTANCE_MEGA_THRESHOLD 999999 

double get_measurement_voltage(uint32_t adc_code, uint8_t gain);
double get_adc_diff_voltage(uint32_t adc_code);

double get_diode_voltage(uint32_t adc_code);
double get_capacitance(double *voltage_points, uint8_t range);
double scale_capacitance(double capacitance_reading);

double get_resistance(uint32_t adc_code);
double scale_resistance(double resistance_reading);
uint8_t out_of_range_low_condition_resistance(double resistance, uint8_t range);

uint8_t out_of_range_low_condition_capacitance(double capacitance);

static double RANGE_CURRENTS[] = {0.00000099, 0.0000099, 0.000099, 0.00099};

#endif