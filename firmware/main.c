//
// FILENAME: main.c
//
// DESCRIPTION: This is main file for Multimeter firmware. It contains the 
// logic for voltage and resistance measurement at a high level. Additionaly, 
// it contains peripheral setup.
//
// Written by Marek Newton
// 

#include <stdio.h>
#include <stdint.h>

#include "pico/stdlib.h"

#include "hardware/spi.h"

#include "main.h"
#include "MCP3561.h"
#include "calcs.h"
#include "display_driver.h"

int main(void)
{
    stdio_init_all();
    
    setup_SPI();
    setup_IO();
    
    setup_MCP3561();
    
    while(1)
    {
        /*
        Signed_Voltage reading = average_voltage_reading(); 
        negative_sign(reading.sign); 
        printf("%f\n", reading.magnitude);
        */

        double resistance_reading = average_resistance_reading();
        printf("%f\n", resistance_reading);
        sleep_ms(500);
    }

    return 1;
}

void setup_SPI(void)
{
    spi_init(spi0, SPI_SCK_FREQ);
    gpio_set_function(MOSI_PIN, GPIO_FUNC_SPI);
    gpio_set_function(MISO_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SCK_PIN, GPIO_FUNC_SPI);
}

void setup_IO(void)
{
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_init(CS_PIN);
    
    gpio_init(DIGIT1_PIN);
    gpio_init(DIGIT2_PIN);
    gpio_init(DIGIT3_PIN);
    gpio_init(DIGIT4_PIN);

    gpio_init(SHIFT_REGISTER_CLOCK_PIN);
    gpio_init(SHIFT_REGISTER_DATA_PIN);

    gpio_init(PICO_PIN);
    gpio_init(NANO_PIN);
    gpio_init(MICRO_PIN);
    gpio_init(LOW_OHM_AND_NEGATIVE_PIN);
    
    gpio_set_dir(CS_PIN, GPIO_OUT);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    gpio_set_dir(DIGIT1_PIN, GPIO_OUT);
    gpio_set_dir(DIGIT2_PIN, GPIO_OUT);
    gpio_set_dir(DIGIT3_PIN, GPIO_OUT);
    gpio_set_dir(DIGIT4_PIN, GPIO_OUT);
    
    gpio_set_dir(SHIFT_REGISTER_CLOCK_PIN, GPIO_OUT);
    gpio_set_dir(SHIFT_REGISTER_DATA_PIN, GPIO_OUT);

    gpio_set_dir(PICO_PIN, GPIO_OUT);
    gpio_set_dir(NANO_PIN, GPIO_OUT);
    gpio_set_dir(MICRO_PIN, GPIO_OUT);
    gpio_set_dir(LOW_OHM_AND_NEGATIVE_PIN, GPIO_OUT);
    
    gpio_put(CS_PIN, 1);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);    
    
    gpio_put(DIGIT1_PIN, 0);
    gpio_put(DIGIT2_PIN, 0);
    gpio_put(DIGIT3_PIN, 0);
    gpio_put(DIGIT4_PIN, 0);

    gpio_put(SHIFT_REGISTER_CLOCK_PIN, 0);
    gpio_put(SHIFT_REGISTER_DATA_PIN, 0);
    
    gpio_put(PICO_PIN, 0);
    gpio_put(NANO_PIN, 0);
    gpio_put(MICRO_PIN, 0);
    gpio_put(LOW_OHM_AND_NEGATIVE_PIN, 0);
}

Signed_Voltage average_voltage_reading(void)
{
    double average_voltage_magnitude = 0;
    uint8_t i;
    for(i = 0; i < AVERAGE_READING_COUNT; i++)
    {
        uint32_t code = MCP3561_read_code();
        Signed_Voltage voltage = get_measurement_voltage(code);
        if(voltage.sign)
        {
            average_voltage_magnitude -= voltage.magnitude; 
        }
        else
        {
            average_voltage_magnitude += voltage.magnitude;
        }
    }
    average_voltage_magnitude = average_voltage_magnitude / AVERAGE_READING_COUNT;
    Signed_Voltage average_voltage; 
    if(average_voltage_magnitude < 0)
    {
        average_voltage.sign = 1;
    }
    else
    {
        average_voltage.sign = 0;
    }
    average_voltage.magnitude = average_voltage_magnitude;
    return average_voltage;
}

double average_resistance_reading(void)
{
    double resistance_reading = 0;
    uint8_t i;
    for(i = 0; i < AVERAGE_READING_COUNT; i++)
    {
        uint32_t code = MCP3561_read_code();  
        resistance_reading += get_resistance(code); 
    }  
    return resistance_reading/AVERAGE_READING_COUNT;
}
