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

#define SEGMENT_DP_PIN 18
static uint8_t SEGMENT_ARRAY[] = {28, 27, 26, 22, 21, 20, 19, 18};

#define CURRENT_RANGE_COUNT 4


#define MODE_SWITCH_PIN 7
#define MODE_BUTTON_PIN 25
#define COMPONENT_MODE_PIN 1
#define RANGE_PIN 8

#define CONTINUITY_PIN 0

#define PS_NOISE_SET_PIN 23

#define VOLTAGE_POSITIVE_CALIBRATION_OFFSET 0.00
#define VOLTAGE_NEGATIVE_CALIBRATION_OFFSET 0.00

#define MICRO_PIN 11
#define NANO_PIN 17
#define LOW_OHM_AND_NEGATIVE_PIN 12

#define SPI_SCK_FREQ 1000000

#define AVERAGE_READING_COUNT 250

#define DEFAULT_GAIN_SETTING 4
#define RANGE_OVER_VOLTAGE 1.6
#define RANGE_UNDER_VOLTAGE 0.09
#define CONTINUITY_RANGE 3

#define BUTTON_PIN 25

#define OVERLOAD_RESISTANCE 2000000
#define OVERLOAD_VOLTAGE 2.2

#define BUTTON_HISTORY_MASK 0xFF

typedef enum
{
    Voltage,
    Resistance, 
    Diode,
    Continuity
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
void sample_diode(void);

void display_reading(void);

static uint8_t CURRENT_RANGE_PINS[] = {1, 8, 9, 10};

#endif