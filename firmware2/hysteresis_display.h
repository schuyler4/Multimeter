#ifndef HYSTERESIS_DISPLAY_H
#define HYSTERESIS_DISPLAY_H

#include <stdint.h>

#define HYSTERESIS_VOLTAGE 0.00125

double display_hysteresis_voltage(double new_value);
uint8_t display_hysteresis_sign_voltage(void);
double display_hysteresis_resistance(double new_value, uint8_t range_index);

#endif