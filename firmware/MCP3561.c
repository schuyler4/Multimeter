//
// FILENAME: MCP3561.c 
// 
// DESCRIPTION: MCP3561 24 bit ADC driver.
//
// WRITTEN BY: Marek Newton
//

#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/spi.h"

#include "main.h"
#include "MCP3561.h"

static void chip_select_high(void)
{
    gpio_put(CS_PIN, 1);    
}

static void chip_select_low(void)
{
    gpio_put(CS_PIN, 0);
}

static void MCP3561_write(uint8_t *command, uint8_t *data)
{
    chip_select_low();
    spi_write_blocking(spi0, command, 1);
    spi_write_blocking(spi0, data, 1); 
    sleep_us(3);
    chip_select_high();
}

void dump_registers(void)
{
    uint8_t i;
    for(i = 0; i < 10; i++)
    {
        uint8_t read_result;
        uint8_t read_command = READ_COMMAND | (i << COMMAND_ADDRESS_POSITION);
        chip_select_low();
        spi_write_read_blocking(spi0, &read_command, &read_result, 1); 
        spi_read_blocking(spi0, read_command, &read_result, 1);    
        chip_select_high();   
        printf("%d ", i, read_result);
        int8_t j;
        for(j = 7; j >= 0; j--)
        {
            printf("%d", (read_result & (1 << j)) != 0);       
        }
        printf("\n");
    }
}

void setup_MCP3561(void) 
{ 
    uint8_t command = CONFIG0_WRITE;
    // Put the ADC into conversion mode
    // Enable the internal reference and clock, and put the ADC into conversion mode.
    uint8_t register_data = (3 << ADC_MODE) | (2 << CLK_SEL) | (1 << VREF_SEL);
    MCP3561_write(&command, &register_data);

    uint8_t read_command = CONFIG1_READ;
    uint8_t read_result; 
    chip_select_low();
    spi_write_read_blocking(spi0, &read_command, &read_result, 1); 
    spi_read_blocking(spi0, read_command, &read_result, 1);
    chip_select_high();
    
    command = CONFIG1_WRITE;
    register_data = (CLOCK_PRESCALLER_VALUE << CLOCK_PRESCALLER_POSITION) | read_result; 
    MCP3561_write(&command, &register_data);
    
    read_command = IRQ_READ_COMMAND;
    chip_select_low();
    spi_write_read_blocking(spi0, &read_command, &read_result, 1); 
    spi_read_blocking(spi0, read_command, &read_result, 1);    
    chip_select_high();
    
    command = IRQ_WRITE;
    register_data = (IRQ_MODE << IRQ_MODE_OFFSET) | read_result;
    MCP3561_write(&command, &register_data);

    sleep_us(10);
    command = CONFIG3_WRITE;
    register_data = (3 << CONV_MODE);
    register_data |= (1 << EN_OFFCAL);
    MCP3561_write(&command, &register_data);

    command = CONFIG2_WRITE;
    register_data |= (1 << AZ_MUX);
    register_data &= ~(3 << BOOST);
    register_data |= (1 << GAIN);
    MCP3561_write(&command, &register_data); 
    
    dump_registers();
}

void sample_MCP3561(void)
{
    uint8_t command = SAMPLE_COMMAND;
    uint8_t zero = 0x00;
    MCP3561_write(&command, &zero);
}

static void MCP3561_arm(void)
{
    uint8_t command = CONFIG0_WRITE;
    // Put the ADC into conversion mode
    uint8_t register_data = (ADC_MODE << 3);
    // Select the internal clock without outputting it to the clock pin
    register_data = (CLOCK_SELECTION << CLOCK_SELECTION_POSITION);
    register_data |= (REFERENCE_ENABLE << REFERENCE_OFFSET);
    MCP3561_write(&command, &register_data);      
}

uint32_t MCP3561_read_code(void)
{
    uint8_t command[ADC_DATA_BYTE_COUNT + 1] = {ADC_DATA_READ_COMMAND, 0, 0, 0};    
    uint8_t adc_data[ADC_DATA_BYTE_COUNT + 1];
    uint32_t combined_data = 0;
    chip_select_low();
    spi_write_read_blocking(spi0, command, adc_data, ADC_DATA_BYTE_COUNT+1); 
    chip_select_high();
    return (adc_data[1] << 16) | (adc_data[2] << 8) | adc_data[3]; 
}

uint8_t MCP3561_read_register(void)
{
    uint8_t command = CONFIG0_READ;
    uint8_t result;

    chip_select_low();
    spi_write_read_blocking(spi0, &command, &result, 1); 
    spi_read_blocking(spi0, command, &result, 1);    
    chip_select_high();

    command = IRQ_READ_COMMAND;
    chip_select_low();
    spi_write_read_blocking(spi0, &command, &result, 1); 
    spi_read_blocking(spi0, command, &result, 1);    
    chip_select_high();
    
    return result;
}
