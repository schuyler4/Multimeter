#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "pico/stdlib.h"
#include "pico/time.h"

#include "hardware/spi.h"

#include "main.h"
#include "MCP3561.h"
#include "calcs.h"
#include "display_driver.h"
#include "adjustment.h"

#include "running_average.h"

static double average_resistance_reading = 0;
static double resistance_reading = 0;
static uint8_t resistance_reading_count = 0;

static volatile double average_voltage_reading = 0;
static volatile Signed_Voltage voltage_reading;
static volatile uint8_t voltage_sign = 0;
static volatile uint8_t voltage_reading_count = 0;

static volatile double average_diode_voltage_reading = 0;
static volatile double diode_voltage_reading = 0;
static volatile uint8_t diode_voltage_reading_count = 0;

static Mode past_mode;
static Mode mode;

static uint8_t range = 0;
static uint8_t past_range;

static volatile double zero_voltage = 0;
static volatile uint8_t zero_voltage_sample = 0;

static uint32_t code;
Running_Average_Filter voltage_filter; 

static void mode_change(void)
{
    disable_prefix_indicators();
    zero_segments();
}

static void check_mode_change(void)
{
    if(gpio_get(MODE_SWITCH_PIN))
    {
        mode = Voltage;
    }
    else
    {
        mode = Resistance;
    }
}

static void check_range_change(void)
{
    if(gpio_get(RANGE_PIN) != past_range)
    {
        disable_prefix_indicators();
        zero_segments();
        past_range = gpio_get(RANGE_PIN);
    }
}

static void find_range(void)
{
    
    if(resistance_reading > RANGE_OVER_VOLTAGE && range > 0) 
    {
        range--;
    }
    if(resistance_reading < RANGE_UNDER_VOLTAGE && range < CURRENT_RANGE_COUNT-1) 
    {
        range++;
    }
    printf("%f\n", resistance_reading);
    printf("setting range %d\n", range);
    uint8_t i;
    for(i=0; i < CURRENT_RANGE_COUNT; i++)
    {
        printf("%d\n", i == range);
        gpio_put(CURRENT_RANGE_PINS[i], i == range);
    }
}

static void display_resistance(void)
{
    /*
    double adjusted_resistance = resistance_adjustment(resistance_reading, gpio_get(RANGE_PIN));
    if(out_of_range_high_condition_resistance(adjusted_resistance, gpio_get(RANGE_PIN))) 
    {
        display_open_circuit();     
        disable_aux_indicators();
    }
    else if(out_of_range_low_condition_resistance(adjusted_resistance, gpio_get(RANGE_PIN)))
    {
        display_short_circuit();
        disable_aux_indicators();
    }
    else
    {
        display_double(scale_resistance(adjusted_resistance));
        display_unit_prefix_resistance(resistance_reading);
    }
    */
    if(gpio_get(CONTINUITY_PIN)) 
    {
        display_short_circuit();
        disable_aux_indicators();
    }
    else if(resistance_reading/RANGE_CURRENTS[range] > OVERLOAD_RESISTANCE)
    {
        display_open_circuit();
        disable_aux_indicators();
    }
    else
    {
        display_double(scale_resistance(resistance_reading/RANGE_CURRENTS[range]));
        display_unit_prefix_resistance(resistance_reading/RANGE_CURRENTS[range]);
    }
}

int main(void)
{
    stdio_init_all();
    
    setup_IO();
    setup_SPI();
    setup_MCP3561();

    voltage_reading.magnitude = 0;
    voltage_reading.sign = 0;

    mode = Voltage;

    while(1)
    {
        //check_mode_change();
        //printf("%f %f %d\n", resistance_reading/RANGE_CURRENTS[range], resistance_reading, range); 
        if(mode == Diode)
        {
            gpio_put(CURRENT_RANGE4_PIN, 1);
            display_double(diode_voltage_reading);
        }
        if(mode == Resistance)
        {
            display_resistance();
        }
        else if(mode == Voltage)
        {
            display_double(voltage_reading.magnitude);
            disable_prefix_indicators();
        }
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

static void adc_data_callback(uint gpio, uint32_t events)
{
    code = MCP3561_read_code();
    if(mode == Diode)
    {
        sample_diode();
    }
    else if(mode == Resistance)
    {
        sample_resistance();        
    }
    else if(mode == Voltage)
    {
        sample_voltage();    
    }
}

void setup_IO(void)
{
    gpio_init(CS_PIN);
    
    gpio_init(DIGIT1_PIN);
    gpio_init(DIGIT2_PIN);
    gpio_init(DIGIT3_PIN);
    gpio_init(DIGIT4_PIN);

    gpio_init(NANO_PIN);
    gpio_init(MICRO_PIN);
    gpio_init(LOW_OHM_AND_NEGATIVE_PIN);

    gpio_init(DATA_INTERUPT_PIN);

    gpio_init(SEGMENT_A_PIN);
    gpio_init(SEGMENT_B_PIN);
    gpio_init(SEGMENT_C_PIN);
    gpio_init(SEGMENT_D_PIN);
    gpio_init(SEGMENT_E_PIN);
    gpio_init(SEGMENT_F_PIN);
    gpio_init(SEGMENT_G_PIN);
    gpio_init(SEGMENT_DP_PIN);

    gpio_init(CURRENT_RANGE1_PIN);
    gpio_init(CURRENT_RANGE2_PIN);
    gpio_init(CURRENT_RANGE3_PIN);
    gpio_init(CURRENT_RANGE4_PIN);

    gpio_init(MODE_SWITCH_PIN);
    gpio_init(BUTTON_PIN);

    gpio_init(CONTINUITY_PIN);

    gpio_init(PS_NOISE_SET_PIN);

    gpio_set_dir(CS_PIN, GPIO_OUT);

    gpio_set_dir(DIGIT1_PIN, GPIO_OUT);
    gpio_set_dir(DIGIT2_PIN, GPIO_OUT);
    gpio_set_dir(DIGIT3_PIN, GPIO_OUT);
    gpio_set_dir(DIGIT4_PIN, GPIO_OUT);

    gpio_set_dir(NANO_PIN, GPIO_OUT);
    gpio_set_dir(MICRO_PIN, GPIO_OUT);
    gpio_set_dir(LOW_OHM_AND_NEGATIVE_PIN, GPIO_OUT);

    gpio_set_dir(DATA_INTERUPT_PIN, GPIO_IN);

    gpio_set_dir(SEGMENT_A_PIN, GPIO_OUT);
    gpio_set_dir(SEGMENT_B_PIN, GPIO_OUT);
    gpio_set_dir(SEGMENT_C_PIN, GPIO_OUT);
    gpio_set_dir(SEGMENT_D_PIN, GPIO_OUT);
    gpio_set_dir(SEGMENT_E_PIN, GPIO_OUT);
    gpio_set_dir(SEGMENT_F_PIN, GPIO_OUT);
    gpio_set_dir(SEGMENT_G_PIN, GPIO_OUT);
    gpio_set_dir(SEGMENT_DP_PIN, GPIO_OUT);

    gpio_set_dir(CURRENT_RANGE1_PIN, GPIO_OUT);
    gpio_set_dir(CURRENT_RANGE2_PIN, GPIO_OUT);
    gpio_set_dir(CURRENT_RANGE3_PIN, GPIO_OUT);
    gpio_set_dir(CURRENT_RANGE4_PIN, GPIO_OUT);

    gpio_set_dir(MODE_SWITCH_PIN, GPIO_IN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);

    gpio_set_dir(CONTINUITY_PIN, GPIO_IN);

    gpio_set_dir(PS_NOISE_SET_PIN, GPIO_OUT);

    gpio_set_drive_strength(SEGMENT_A_PIN, GPIO_DRIVE_STRENGTH_8MA); 
    gpio_set_drive_strength(SEGMENT_B_PIN, GPIO_DRIVE_STRENGTH_8MA); 
    gpio_set_drive_strength(SEGMENT_C_PIN, GPIO_DRIVE_STRENGTH_8MA);
    gpio_set_drive_strength(SEGMENT_D_PIN, GPIO_DRIVE_STRENGTH_8MA); 
    gpio_set_drive_strength(SEGMENT_E_PIN, GPIO_DRIVE_STRENGTH_8MA);
    gpio_set_drive_strength(SEGMENT_F_PIN, GPIO_DRIVE_STRENGTH_8MA);
    gpio_set_drive_strength(SEGMENT_G_PIN, GPIO_DRIVE_STRENGTH_8MA); 
    gpio_set_drive_strength(SEGMENT_DP_PIN, GPIO_DRIVE_STRENGTH_8MA);
     
    gpio_put(PS_NOISE_SET_PIN, 1);

    gpio_put(CS_PIN, 1);
    
    gpio_put(DIGIT1_PIN, 0);
    gpio_put(DIGIT2_PIN, 0);
    gpio_put(DIGIT3_PIN, 0);
    gpio_put(DIGIT4_PIN, 0);
    
    gpio_put(NANO_PIN, 0);
    gpio_put(MICRO_PIN, 0);
    gpio_put(LOW_OHM_AND_NEGATIVE_PIN, 0);
    
    gpio_put(SEGMENT_A_PIN, 0);
    gpio_put(SEGMENT_B_PIN, 0);
    gpio_put(SEGMENT_C_PIN, 0);
    gpio_put(SEGMENT_D_PIN, 0);
    gpio_put(SEGMENT_E_PIN, 0);
    gpio_put(SEGMENT_F_PIN, 0);
    gpio_put(SEGMENT_G_PIN, 0);
    gpio_put(SEGMENT_DP_PIN, 0);

    gpio_put(CURRENT_RANGE1_PIN, 0);
    gpio_put(CURRENT_RANGE2_PIN, 0);
    gpio_put(CURRENT_RANGE3_PIN, 0);
    gpio_put(CURRENT_RANGE4_PIN, 0);

    gpio_set_irq_enabled_with_callback(DATA_INTERUPT_PIN, GPIO_IRQ_EDGE_FALL, true, *adc_data_callback);
}

void sample_resistance(void)
{
    average_resistance_reading += get_resistance(code);
    resistance_reading_count += 1;
    if(resistance_reading_count == AVERAGE_READING_COUNT)
    {
        resistance_reading = average_resistance_reading/AVERAGE_READING_COUNT;
        resistance_reading_count = 0;    
        average_resistance_reading = 0;
        find_range();
    }
}

void sample_diode(void)
{
    average_diode_voltage_reading += get_diode_voltage(code);
    diode_voltage_reading_count++;
    if(diode_voltage_reading_count == AVERAGE_READING_COUNT)
    {
        diode_voltage_reading = diode_voltage_adjustment(average_diode_voltage_reading/AVERAGE_READING_COUNT);
        diode_voltage_reading_count = 0;  
        average_diode_voltage_reading = 0;
    }
}

void sample_voltage(void)
{
    if(zero_voltage_sample)
    {
        zero_voltage = get_measurement_voltage(code); 
        printf("zero voltage %f\n", zero_voltage);
        gpio_put(LOW_OHM_AND_NEGATIVE_PIN, 0);
    }
    else 
    {
        average_voltage_reading += get_measurement_voltage(code); 
        
        voltage_reading_count++;
        if(voltage_reading_count == AVERAGE_READING_COUNT)
        {
            voltage_reading.magnitude = average_voltage_reading;
            voltage_reading.magnitude /= AVERAGE_READING_COUNT;
            voltage_reading.sign = voltage_adjustment_signed(voltage_reading.magnitude) < 0.0; 
            voltage_reading.magnitude = voltage_adjustment(voltage_reading.magnitude);
            printf("read voltage %f", voltage_reading.magnitude);
            voltage_reading_count = 0;
            average_voltage_reading = 0;
        }
        gpio_put(LOW_OHM_AND_NEGATIVE_PIN, 0);
    }
    zero_voltage_sample = !zero_voltage_sample;
}

void display_reading(void)
{
    if(mode == Voltage)
    { 
        display_double(voltage_reading.magnitude);
        //negative_sign(voltage_reading.sign);
        disable_prefix_indicators();
    }
    else if(mode == Resistance)
    {
        disable_negative_sign();
        display_resistance(); 
    }
    else
    {
        disable_negative_sign();
    }
}
