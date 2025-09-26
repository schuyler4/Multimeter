#ifndef MAIN_H
#define MAIN_H

#include "calcs.h"

#define REVISION 1

#define CS_PIN 5
#define SCK_PIN 2
#define MOSI_PIN 3 // MASTER OUT SLAVE IN 
#define MISO_PIN 4 // MASTER IN SLAVE OUT
#define CHIP_SELECT_PIN 5

#define DATA_INTERUPT_PIN 6

#define DIGIT1_PIN 13
#define DIGIT2_PIN 14
#define DIGIT3_PIN 15
#define DIGIT4_PIN 16

#define SEGMENT_A_PIN 28 

#define SEGMENT_B_PIN 27
#define SEGMENT_C_PIN 26
#define SEGMENT_D_PIN 22
#define SEGMENT_E_PIN 21
#define SEGMENT_F_PIN 20
#define SEGMENT_G_PIN 19
#define SEGMENT_DP_PIN 18

#define CURRENT_RANGE_COUNT 4
#define CURRENT_RANGE1_PIN 1
#define CURRENT_RANGE2_PIN 8
#define CURRENT_RANGE3_PIN 9
#define CURRENT_RANGE4_PIN 10

#define MODE_SWITCH_PIN 7
#define MODE_BUTTON_PIN 25
#define COMPONENT_MODE_PIN 1
#define RANGE_PIN 8

#define CONTINUITY_PIN 0

#define VOLTAGE_POSITIVE_CALIBRATION_OFFSET 0.00
#define VOLTAGE_NEGATIVE_CALIBRATION_OFFSET 0.00

#define NANO_PIN 11
#define MICRO_PIN 17
#define LOW_OHM_AND_NEGATIVE_PIN 12

#define SPI_SCK_FREQ 2000000

#define AVERAGE_READING_COUNT 200

#define RANGE_OVER_VOLTAGE 1.6
#define RANGE_UNDER_VOLTAGE 0.09

#define BUTTON_PIN 25

#define OVERLOAD_RESISTANCE 2100000

typedef enum
{
    Voltage,
    Resistance, 
    Diode
} Mode;

typedef struct 
{
    double magnitude;
    uint8_t sign;
} Signed_Voltage;

void setup_SPI(void);
void setup_IO(void);

void sample_resistance(void);
void sample_voltage(void);
void sample_capacitance(void);

void display_reading(void);

static uint8_t CURRENT_RANGE_PINS[] = {CURRENT_RANGE1_PIN, CURRENT_RANGE2_PIN, CURRENT_RANGE3_PIN, CURRENT_RANGE4_PIN};

#endif