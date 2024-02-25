#ifndef CALCS_H
#define CALCS_H

#define VOLTAGE_REFERANCE 2.316
#define ADC_STEPS 8388608 

#define DIVIDER_UPPER_RESISTOR 511000
#define DIVIDER_LOWER_RESISTOR 10000

double get_measurement_voltage(int adc_code);

#endif
