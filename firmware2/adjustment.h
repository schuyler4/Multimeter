#ifndef ADJUSTMENT_H
#define ADJUSTMENT_H 

#include "main.h"

#if REVISION == 1

#define VOLTAGE_OFFSET -0.008
#define VOLTAGE_MULTIPLIER 1.014
#define RESISTANCE_RANGE1_OFFSET 0
#define RESISTANCE_RANGE2_OFFSET 0

#else 

#define VOLTAGE_OFFSET 0
#define VOLTAGE_MULTIPLIER 0.976
#define RESISTANCE_RANGE1_MULTIPLIER 0.9263   
#define RESISTANCE_RANGE2_MULTIPLIER 0.981
#define RESISTANCE_RANGE1_OFFSET -28.1
#define RESISTANCE_RANGE2_OFFSET -10 

#endif

double voltage_adjustment(double voltage);
double voltage_adjustment_signed(double voltage);
double resistance_adjustment(double resistance, uint8_t range);

#endif
