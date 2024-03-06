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

typedef struct 
{
    double magnitude;
    uint8_t sign;
} Signed_Voltage;

Signed_Voltage get_measurement_voltage(uint32_t adc_code);
double get_resistance(uint32_t adc_code);

uint8_t low_ohm_condition(double resistance);
uint8_t out_of_range_condition(double resistance);

#endif
