#include <stdio.h>
#include <math.h>

#include "pico/stdlib.h"

#include "main.h"
#include "display_driver.h"

static void turn_off_all_digits(void)
{
    gpio_put(DIGIT1_PIN, 0);
    gpio_put(DIGIT2_PIN, 0);
    gpio_put(DIGIT3_PIN, 0);
    gpio_put(DIGIT4_PIN, 0);
}

static void zero_segments(void)
{
    gpio_put(SEGMENT_DP_PIN, 0);
    uint8_t i;
    for(i = 0; i < SEGMENT_COUNT - 1; i++)
    {
        gpio_put(SEGMENT_ARRAY[i], 0);
    }
}

void write_digit(uint8_t number, uint8_t decimal_point)
{ 
    zero_segments();    

    if(decimal_point)
    {
        gpio_put(SEGMENT_DP_PIN, 1);
    }

    uint8_t number_code = DIGITS[number];
    uint8_t i;    
    for(i = 0; i < SEGMENT_COUNT - 1; i++)
    {
        gpio_put(SEGMENT_ARRAY[i], (number_code >> i) & FIRST_BIT_MASK);
    }
}

void turn_on_digit(uint8_t digit)
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

void display_integer(uint16_t integer)
{   
    uint8_t i; 
    for(i = DIGIT_COUNT; i > 0; i--)
    {
        int divisor = (int)pow(10, i - 1);
        int digit = integer/divisor;
        if(digit != 0)
        {
            turn_on_digit(i);
            write_digit(digit, 0);
        }
        integer = integer % divisor;
        sleep_ms(2);
    }
}

void display_double(double number)
{
    // snprintf stores a null terminator, so an extra space is
    // needed in the array, also a space for decimal point.
    char double_string[DIGIT_COUNT+2];
    snprintf(double_string, DIGIT_COUNT+2, "%f", number);        
    uint8_t digit = 1;
    uint8_t i;
    printf("%s %f\n", double_string, number);
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
        sleep_ms(1);
        digit++;
    }
}

void negative_sign(uint8_t enabled)
{
    gpio_put(LOW_OHM_AND_NEGATIVE_PIN, enabled);
}

void low_ohm(uint8_t low_ohm_detected)
{
    gpio_put(LOW_OHM_AND_NEGATIVE_PIN, low_ohm_detected);
}

void cap_triggered(void)
{
    gpio_put(LOW_OHM_AND_NEGATIVE_PIN, 1);
}
