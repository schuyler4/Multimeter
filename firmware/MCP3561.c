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

static uint8_t MCP3561_read(uint8_t *command)
{   
    uint8_t result[2];
    chip_select_low(); 
    spi_write_read_blocking(spi0, command, result, 2);  
    chip_select_high();
    return result[1];
}

void setup_MCP3561(void) 
{ 
    uint8_t command = CONFIG0_WRITE;
    uint8_t register_data = (0x03 << 0);
    MCP3561_write(&command, &register_data); 
}

