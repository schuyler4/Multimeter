#include <stdint.h>
#include <math.h>

#include "adjustment.h"
#include "main.h"

static double RESISTANCE_MULTIPLIERS[] = {1, 1, 1, 1};
static double RESISTANCE_OFFSET[] = {700, 68, 10.8, 0.28};

double voltage_adjustment(double voltage)
{
    return (voltage + VOLTAGE_OFFSET)*VOLTAGE_MULTIPLIER; 
}

double diode_voltage_adjustment(double diode_voltage)
{
    return fabs(diode_voltage*DIODE_VOLTAGE_MULTIPLIER);
}

double voltage_adjustment_signed(double voltage)
{
    return voltage*VOLTAGE_MULTIPLIER + VOLTAGE_OFFSET;
}

double resistance_adjustment(double resistance, uint8_t range_index)
{
    return (resistance+RESISTANCE_OFFSET[range_index])*RESISTANCE_MULTIPLIERS[range_index];
}
