#include <stdint.h>
#include <math.h>

#include "adjustment.h"
#include "main.h"

double voltage_adjustment(double voltage)
{
    return fabs(voltage*VOLTAGE_MULTIPLIER + VOLTAGE_OFFSET); 
}

double voltage_adjustment_signed(double voltage)
{
    return voltage*VOLTAGE_MULTIPLIER + VOLTAGE_OFFSET;
}

double resistance_adjustment(double resistance, uint8_t range)
{
#if REVISION == 2
    if(range)
    {
        return fabs(resistance*RESISTANCE_RANGE1_MULTIPLIER + RESISTANCE_RANGE1_OFFSET);
    }
    else
    {
        return fabs(resistance + RESISTANCE_RANGE2_OFFSET);
    }
#endif
}
