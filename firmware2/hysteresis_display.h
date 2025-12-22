#ifndef HYSTERESIS_DISPLAY_H
#define HYSTERESIS_DISPLAY_H

#include <stdint.h>

#define HYSTERESIS 0.001

double display_hysteresis(double new_value);
uint8_t display_hysteresis_sign(void);

#endif