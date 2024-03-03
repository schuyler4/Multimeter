#ifndef CALCS_H
#define CALCS_H

#define VOLTAGE_REFERANCE 2.316
#define MEASUREMENT_BIAS VOLTAGE_REFERANCE/2
#define ADC_STEPS 8388608 

#define DIVIDER_UPPER_RESISTOR 511000
#define DIVIDER_LOWER_RESISTOR 10000

#define COMPONENT_SERIES_RESISTOR 1500000
#define COMPONENT_VOLTAGE_REFERANCE 2.5

#define SIGN_MASK 0x800000
#define MAGNITUDE_MASK 0x7FFFFF

typedef struct 
{
    double magnitude;
    uint8_t sign;
} Signed_Voltage;

Signed_Voltage get_measurement_voltage(int adc_code);
double get_resistance(int adc_code);

#endif
