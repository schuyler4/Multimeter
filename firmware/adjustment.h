#ifndef ADJUSTMENT_H
#define ADJUSTMENT_H 

#include "main.h"

#if REVISION == 1

#define VOLTAGE_OFFSET 0
#define VOLTAGE_MULTIPLIER 0.976
#define RESISTANCE_RANGE1_OFFSET 0
#define RESISTANCE_RANGE2_OFFSET 0

#else 

#define VOLTAGE_OFFSET 0
#define VOLTAGE_MULTIPLIER 0.976
#define RESISTANCE_RANGE1_OFFSET -420.9
#define RESISTANCE_RANGE2_OFFSET 0 

#endif

double voltage_adjustment(double voltage);
double resistance_adjustment(double resistance, uint8_t range);

#endif
