#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#define CLOCK_DELAY_TIME_US 10
#define DIGIT_COUNT 4
#define SEGMENT_COUNT 8
#define DECIMAL_MASK 0x80

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
void negative_sign(uint8_t enabled);
void low_ohm(uint8_t low_ohm_detected);

#endif
