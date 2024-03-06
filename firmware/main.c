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

static double average_resistance_reading = 0;
static double resistance_reading = 0;
static uint8_t resistance_reading_count = 0;

static double average_voltage_reading = 0;
static double voltage_reading = 0;
static uint8_t voltage_reading_count = 0;

static Mode mode = Voltage;

static uint32_t code;

int main(void)
{
    stdio_init_all();
    
    setup_SPI();
    setup_IO();
    
    setup_MCP3561();
    
    while(1)
    {
        if(mode == Voltage)
        { 
            Signed_Voltage voltage = get_measurement_voltage(code); 
            printf("Voltage: %f\n", voltage.magnitude);
        }
        else if(mode == Resistance)
        {
            if(out_of_range_condition(resistance_reading)) 
            {
                printf("OL\n");
            }
            else
            {
                printf("%f\n", resistance_reading);
            }
            low_ohm(low_ohm_condition(resistance_reading));
        }
    }

    return 1;
}

void adc_data_callback(uint gpio, uint32_t events)
{
    code = MCP3561_read_code();
    if(mode == Resistance)
    {
        average_resistance_reading += get_resistance(code); 
        resistance_reading_count++; 
        if(resistance_reading_count == AVERAGE_READING_COUNT)
        {
            resistance_reading = average_resistance_reading/AVERAGE_READING_COUNT;
            resistance_reading_count = 0;    
            average_resistance_reading = 0;
        }
    }
    else if(mode == Voltage)
    {
        Signed_Voltage voltage = get_measurement_voltage(code); 
        if(voltage.sign)
        {
            average_voltage_reading -= voltage.magnitude;
        }
        else
        {
            average_voltage_reading += voltage.magnitude;
        }
        voltage_reading_count++;
        if(voltage_reading_count == AVERAGE_READING_COUNT)
        {
            voltage_reading = average_voltage_reading/AVERAGE_READING_COUNT;
            voltage_reading_count = 0;
            average_voltage_reading = 0;
        }
    }
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

    gpio_init(DATA_INTERUPT_PIN);
    
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

    gpio_set_dir(DATA_INTERUPT_PIN, GPIO_IN);
    
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

    gpio_set_irq_enabled_with_callback(
        DATA_INTERUPT_PIN, 
        GPIO_IRQ_EDGE_FALL, 
        true, 
        *adc_data_callback);
}


