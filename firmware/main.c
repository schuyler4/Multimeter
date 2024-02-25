#include <stdio.h>

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
        uint32_t code = MCP3561_read_code();
        printf("%d\n", code);
        float voltage = get_measurement_voltage(code);
        printf("%f\n", voltage);
        turn_on_digit(1);
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
    
    gpio_set_dir(CS_PIN, GPIO_OUT);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    gpio_set_dir(DIGIT1_PIN, GPIO_OUT);
    gpio_set_dir(DIGIT2_PIN, GPIO_OUT);
    gpio_set_dir(DIGIT3_PIN, GPIO_OUT);
    gpio_set_dir(DIGIT4_PIN, GPIO_OUT);
    
    gpio_set_dir(SHIFT_REGISTER_CLOCK_PIN, GPIO_OUT);
    gpio_set_dir(SHIFT_REGISTER_DATA_PIN, GPIO_OUT);
    
    gpio_put(CS_PIN, 1);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);    
    
    gpio_put(DIGIT1_PIN, 0);
    gpio_put(DIGIT2_PIN, 0);
    gpio_put(DIGIT3_PIN, 0);
    gpio_put(DIGIT4_PIN, 0);

    gpio_put(SHIFT_REGISTER_CLOCK_PIN, 0);
    gpio_put(SHIFT_REGISTER_DATA_PIN, 0);
}
