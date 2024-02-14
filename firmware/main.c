#include "pico/stdlib.h"

#include "hardware/spi.h"

#include "main.h"
#include "MCP3561.h"

int main(void)
{
    stdio_init_all();
    
    setup_SPI();
    setup_IO();
    
    while(1)
    {
        setup_MCP3561();
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
    
    gpio_set_dir(CS_PIN, GPIO_OUT);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    
    gpio_put(CS_PIN, 1);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);    
}
