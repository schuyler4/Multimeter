#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#include "main.h"

#define DIGIT_COUNT 4
#define SEGMENT_COUNT 8

#define FIRST_BIT_MASK 0x01

#define FIRST_MIDDLE_DIGIT 2
#define SECOND_MIDDLE_DIGIT 3

#define DASH_CHARACTER 0x40
#define DIGIT_DELAY_MS 1

static uint8_t SEGMENT_ARRAY[] = {
    SEGMENT_A_PIN,
    SEGMENT_B_PIN,
    SEGMENT_C_PIN, 
    SEGMENT_D_PIN,
    SEGMENT_E_PIN,
    SEGMENT_F_PIN,
    SEGMENT_G_PIN
};

static uint8_t DIGITS[] = {
    0x3F,
    0x06,
    0x5B,
    0x4F,
    0x66,
    0x6D,
    0x7D,
    0x07,
    0x7F,
    0x6F
};

static uint8_t OPEN_CIRCUIT[] = {0x3F, 0x38};

void display_double(double number);
void display_open_circuit(void);
void display_short_circuit(void);
void display_unit_prefix_resistance(double resistance_reading);
void display_unit_prefix_capacitance(double capacitance_reading);
void zero_segments(void);

void negative_sign(uint8_t enabled);
void disable_negative_sign(void);
void low_ohm(uint8_t low_ohm_detected);
void cap_trigger_indicator(void);

void disable_prefix_indicators(void);
void disable_aux_indicators(void);

#endif
