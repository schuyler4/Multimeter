#include <stdio.h>

#include "pico/stdlib.h"

#include "main.h"
#include "display_driver.h"

static void clock_in_bit(uint8_t bit)
{
    if(bit == 1 || bit == 0)
    {
        gpio_put(SHIFT_REGISTER_DATA_PIN, bit); 
        sleep_us(CLOCK_DELAY_TIME_US);
        gpio_put(SHIFT_REGISTER_CLOCK_PIN, 1);
        sleep_us(CLOCK_DELAY_TIME_US);
        gpio_put(SHIFT_REGISTER_CLOCK_PIN, 0);
    }
}

void write_digit(uint8_t number, uint8_t decimal_point)
{ 
    uint8_t pattern = DIGITS[number];
    if(decimal_point)
    {
        pattern |= DECIMAL_MASK;
    }
    uint8_t i;
    for(i = 0; i < SEGMENT_COUNT; i++)
    {
        uint8_t bit = ((1 << i) & pattern) != 0;
        printf("%d\n", bit);
        clock_in_bit(bit);
    }
}

static void turn_off_all_digits(void)
{
    gpio_put(DIGIT1_PIN, 0);
    gpio_put(DIGIT2_PIN, 0);
    gpio_put(DIGIT3_PIN, 0);
    gpio_put(DIGIT4_PIN, 0);
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

void display_double(double number)
{
    char double_string[DIGIT_COUNT+1];
    snprintf(double_string, DIGIT_COUNT+1, "4%f", number);        
    uint8_t digit = 1;
    uint8_t i;
    for(i = 0; i < DIGIT_COUNT+1; i++)
    {
        if(double_string[i] == '.')
        {
            continue;
        }
        uint8_t digit_integer = double_string[i] - '0';
        printf("%d\n", digit_integer);
        if(double_string[i+1] == '.')
        {
            write_digit(1, 1); 
        }    
        else
        {
            write_digit(1, 0);
        }
        turn_on_digit(digit);
        sleep_ms(1);
        digit++;
    }
}
