#ifndef ADJUSTMENT_H
#define ADJUSTMENT_H 

#include "main.h"

#define VOLTAGE_OFFSET 0.00565 
#define VOLTAGE_MULTIPLIER 1.01054
#define DIODE_VOLTAGE_MULTIPLIER 0.976

double voltage_adjustment(double voltage);
double diode_voltage_adjustment(double diode_voltage);
double voltage_adjustment_signed(double voltage);
double resistance_adjustment(double resistance, uint8_t range_index);

#endif
