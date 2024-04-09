#include <stdint.h>
#include <math.h>

#include "adjustment.h"

double voltage_adjustment(double voltage)
{
    return fabs(voltage*VOLTAGE_MULTIPLIER+ VOLTAGE_OFFSET); 
}

double resistance_adjustment(double resistance, uint8_t range)
{
    if(range)
    {
        return fabs(resistance + RESISTANCE_RANGE1_OFFSET);
    }
    else
    {
        return fabs(resistance + RESISTANCE_RANGE2_OFFSET);
    }
}
