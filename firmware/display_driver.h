#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#include "main.h"

#define DIGIT_COUNT 4
#define SEGMENT_COUNT 8

#define FIRST_BIT_MASK 0x01

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

void write_digit(uint8_t number, uint8_t decimal_point);
void turn_on_digit(uint8_t digit);
void display_double(double number);
void display_integer(uint16_t integer);

void negative_sign(uint8_t enabled);
void low_ohm(uint8_t low_ohm_detected);
void cap_triggered(void);

#endif
