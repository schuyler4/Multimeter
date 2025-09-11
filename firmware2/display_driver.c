#include <stdio.h>
#include <math.h>

#include "pico/stdlib.h"

#include "main.h"
#include "display_driver.h"
#include "calcs.h"

static void turn_off_all_digits(void)
{
    gpio_put(DIGIT1_PIN, 0);
    gpio_put(DIGIT2_PIN, 0);
    gpio_put(DIGIT3_PIN, 0);
    gpio_put(DIGIT4_PIN, 0);
}

void zero_segments(void)
{
    gpio_put(SEGMENT_DP_PIN, 0);
    uint8_t i;
    for(i = 0; i < SEGMENT_COUNT - 1; i++)
    {
        gpio_put(SEGMENT_ARRAY[i], 0);
    }
}

static void write_character(uint8_t character_encoding)
{
    uint8_t i;
    for(i = 0; i < SEGMENT_COUNT - 1; i++)
    {
        gpio_put(SEGMENT_ARRAY[i], (character_encoding >> i) & FIRST_BIT_MASK);
    }
}

static void write_digit(uint8_t number, uint8_t decimal_point)
{ 
    zero_segments();    

    if(decimal_point)
    {
        gpio_put(SEGMENT_DP_PIN, 1);
    }

    write_character(DIGITS[number]);
}

static void turn_on_digit(uint8_t digit)
{
    turn_off_all_digits();
    switch(digit)
    {
        case 1:
            gpio_put(DIGIT1_PIN, 1);
            break;
        case 2:
            gpio_put(DIGIT2_PIN, 1);
            break;
        case 3:
            gpio_put(DIGIT3_PIN, 1);
            break;
        case 4:     
            gpio_put(DIGIT4_PIN, 1);
            break;
        default:
            break;
    }
}

void display_double(double number)
{
    // snprintf stores a null terminator, so an extra space is
    // needed in the array, also a space for the decimal point.
    char double_string[DIGIT_COUNT+2];
    snprintf(double_string, DIGIT_COUNT+2, "%f", number);        
    uint8_t digit = 1;
    uint8_t i;
    for(i = 0; i < DIGIT_COUNT+2; i++)
    {
        if(double_string[i] == '.')
        {
            continue;
        }
        uint8_t digit_integer = double_string[i] - '0';
        if(double_string[i+1] == '.')
        {
            write_digit(digit_integer, 1); 
        }    
        else
        {
            write_digit(digit_integer, 0);
        }
        turn_on_digit(digit);
        sleep_ms(DIGIT_DELAY_MS);
        digit++;
    }
}

void display_open_circuit(void)
{
    zero_segments();
    uint8_t digit;
    for(digit = 1; digit <= DIGIT_COUNT; digit++)
    {
        if(digit == FIRST_MIDDLE_DIGIT || digit == SECOND_MIDDLE_DIGIT)
        {
            write_character(OPEN_CIRCUIT[digit-FIRST_MIDDLE_DIGIT]);
            turn_on_digit(digit);
            sleep_ms(DIGIT_DELAY_MS);
        }
    }
}

void display_short_circuit(void)
{
    zero_segments();
    uint8_t digit;
    for(digit = 1; digit <= DIGIT_COUNT; digit++)
    {
        write_character(DASH_CHARACTER);
        turn_on_digit(digit);
        sleep_ms(DIGIT_DELAY_MS);
    }
}

void negative_sign(uint8_t enabled)
{
    gpio_put(LOW_OHM_AND_NEGATIVE_PIN, enabled);
}

void disable_negative_sign(void)
{
    negative_sign(0);
}

void low_ohm(uint8_t low_ohm_detected)
{
    gpio_put(LOW_OHM_AND_NEGATIVE_PIN, low_ohm_detected);
}

void cap_trigger_indicator(void)
{
    gpio_put(CAP_TRIGGER_PIN, 1);
}

void disable_aux_indicators(void)
{
    gpio_put(MICRO_PIN, 0);
    gpio_put(NANO_PIN, 0);
    gpio_put(CAP_TRIGGER_PIN, 0);
    gpio_put(LOW_OHM_AND_NEGATIVE_PIN, 0);
}

void disable_prefix_indicators(void)
{
    gpio_put(MICRO_PIN, 0);
    gpio_put(NANO_PIN, 0);
    gpio_put(CAP_TRIGGER_PIN, 0);
}

void display_unit_prefix_resistance(double resistance_reading)
{
    if(resistance_reading > RESISTANCE_MEGA_THRESHOLD)
    {
        gpio_put(NANO_PIN, 0);
        gpio_put(MICRO_PIN, 1);
    }
    else if(resistance_reading > RESISTANCE_KILO_THRESHOLD)
    {
        gpio_put(NANO_PIN , 1); 
        gpio_put(MICRO_PIN, 0);
    }
    else
    {
        gpio_put(NANO_PIN, 0);
        gpio_put(MICRO_PIN, 0);
    }
}

void display_unit_prefix_capacitance(double capacitance_reading)
{
    if(capacitance_reading > CAPACITANCE_MICRO_THRESHOLD)
    {
        gpio_put(MICRO_PIN, 1);
        gpio_put(NANO_PIN, 0);
    }
    else
    {
        gpio_put(MICRO_PIN, 0);
        gpio_put(NANO_PIN, 1);
    }
}
