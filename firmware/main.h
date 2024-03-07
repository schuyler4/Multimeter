//
// FILENAME: main.h
//
// DESCRIPTION: This is the header file for main.c. Pin Definitions
// and high level measurement function prototypes are included. 
//
// Written by Marek Newton
// 

#ifndef MAIN_H
#define MAIN_H

#include "calcs.h"

#define CS_PIN 5
#define SCK_PIN 2
#define MOSI_PIN 3 // MASTER OUT SLAVE IN 
#define MISO_PIN 4 // MASTER IN SLAVE OUT
#define CHIP_SELECT_PIN 5

#define DATA_INTERUPT_PIN 6

#define SHIFT_REGISTER_CLOCK_PIN 19 
#define SHIFT_REGISTER_DATA_PIN 20 

#define DIGIT1_PIN 21
#define DIGIT2_PIN 22
#define DIGIT3_PIN 26
#define DIGIT4_PIN 27

#define PICO_PIN 12
#define NANO_PIN 11
#define MICRO_PIN 10
#define LOW_OHM_AND_NEGATIVE_PIN 9

#define SPI_SCK_FREQ 2000000

#define AVERAGE_READING_COUNT 100

#define CAPACITANCE_VOLTAGE_THRESHOLD 1

typedef enum
{
    Voltage,
    Resistance, 
    Capacitance
} Mode;

void setup_SPI(void);
void setup_IO(void);

void sample_resistance(void);
void sample_voltage(void);
void sample_capacitance(void);

#endif
