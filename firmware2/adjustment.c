#include <stdint.h>
#include <math.h>

#include "adjustment.h"
#include "main.h"

double voltage_adjustment(double voltage)
{
    return fabs(voltage*VOLTAGE_MULTIPLIER + VOLTAGE_OFFSET); 
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
    return fabs(resistance + RESISTANCE_RANGE2_OFFSET);
}
