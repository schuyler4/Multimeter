#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "pico/stdlib.h"

#include "hardware/spi.h"

#include "main.h"
#include "MCP3561.h"
#include "calcs.h"
#include "display_driver.h"
#include "adjustment.h"

static double average_resistance_reading = 0;
static double resistance_reading = 0;
static uint8_t resistance_reading_count = 0;

static volatile double average_voltage_reading = 0;
static volatile Signed_Voltage voltage_reading;
static volatile uint8_t voltage_sign = 0;
static volatile uint8_t voltage_reading_count = 0;

static Mode past_mode;
static Mode mode;

static uint8_t past_range;

static uint32_t code;

static void mode_change(void)
{
    disable_prefix_indicators();
    zero_segments();
    cap_measurement_triggered = 0;
    cap_measurement_recorded = 0;
    capacitance_reading_count = 0;
}

static void check_mode_change(void)
{

}

static void check_range_change(void)
{
#if REVISION == 2
    if(gpio_get(RANGE_PIN) != past_range)
    {
        disable_prefix_indicators();
        zero_segments();
        cap_measurement_triggered = 0;
        cap_measurement_recorded = 0;  
        capacitance_reading_count = 0;
        past_range = gpio_get(RANGE_PIN);
    }
#endif
}

int main(void)
{
    stdio_init_all();
    
    setup_IO();
    setup_SPI();
    setup_MCP3561();

    voltage_reading.magnitude = 0;
    voltage_reading.sign = 0;

    //gpio_init(25);
    //gpio_set_dir(25, GPIO_OUT);
    gpio_put(CURRENT_RANGE4_PIN, 1);

    while(1)
    {
        printf("%f\n", resistance_reading);
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
    sample_resistance();
    /*
    if(mode == Resistance)
    {
        sample_resistance();        
    }
    else if(mode == Voltage)
    {
        sample_voltage();    
    }
    else if(mode == Capacitance)
    {
        sample_capacitance();        
    }
    */
}

void setup_IO(void)
{
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_init(CS_PIN);
    
    gpio_init(DIGIT1_PIN);
    gpio_init(DIGIT2_PIN);
    gpio_init(DIGIT3_PIN);
    gpio_init(DIGIT4_PIN);

    gpio_init(CAP_TRIGGER_PIN);
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

    gpio_set_dir(CS_PIN, GPIO_OUT);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    gpio_set_dir(DIGIT1_PIN, GPIO_OUT);
    gpio_set_dir(DIGIT2_PIN, GPIO_OUT);
    gpio_set_dir(DIGIT3_PIN, GPIO_OUT);
    gpio_set_dir(DIGIT4_PIN, GPIO_OUT);

    gpio_set_dir(CAP_TRIGGER_PIN, GPIO_OUT);
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

    gpio_set_drive_strength(SEGMENT_A_PIN, GPIO_DRIVE_STRENGTH_8MA); 
    gpio_set_drive_strength(SEGMENT_B_PIN, GPIO_DRIVE_STRENGTH_8MA); 
    gpio_set_drive_strength(SEGMENT_C_PIN, GPIO_DRIVE_STRENGTH_8MA);
    gpio_set_drive_strength(SEGMENT_D_PIN, GPIO_DRIVE_STRENGTH_8MA); 
    gpio_set_drive_strength(SEGMENT_E_PIN, GPIO_DRIVE_STRENGTH_8MA);
    gpio_set_drive_strength(SEGMENT_F_PIN, GPIO_DRIVE_STRENGTH_8MA);
    gpio_set_drive_strength(SEGMENT_G_PIN, GPIO_DRIVE_STRENGTH_8MA); 
    gpio_set_drive_strength(SEGMENT_DP_PIN, GPIO_DRIVE_STRENGTH_8MA);
     
    gpio_put(CS_PIN, 1);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);    
    
    gpio_put(DIGIT1_PIN, 0);
    gpio_put(DIGIT2_PIN, 0);
    gpio_put(DIGIT3_PIN, 0);
    gpio_put(DIGIT4_PIN, 0);
    
    gpio_put(CAP_TRIGGER_PIN, 0);
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
    }
}

void sample_voltage(void)
{
    average_voltage_reading += get_measurement_voltage(code); 
    voltage_reading_count++;
    if(voltage_reading_count == AVERAGE_READING_COUNT)
    {
        voltage_reading.magnitude = average_voltage_reading;
        voltage_reading.magnitude /= AVERAGE_READING_COUNT;
        voltage_reading.sign = voltage_adjustment_signed(voltage_reading.magnitude) < 0.0; 
        voltage_reading.magnitude = voltage_adjustment(voltage_reading.magnitude);
        voltage_reading_count = 0;
        average_voltage_reading = 0;
    }
}

void sample_capacitance(void)
{
    double voltage = get_capacitor_voltage(code);
    if(voltage < CAPACITANCE_VOLTAGE_THRESHOLD_HIGH && voltage > 
    CAPACITANCE_VOLTAGE_THRESHOLD_LOW) 
    {
        cap_measurement_triggered = 1;
        cap_measurement_recorded = 1;
        printf("Sampled Capacitance %f\n", voltage);
        printf("%d\n", cap_measurement_recorded);
    } 
    if(cap_measurement_triggered)
    {
        capacitance_samples[capacitance_reading_count] = voltage;                
        capacitance_reading_count++;  
        if(capacitance_reading_count >= CAPACITANCE_SAMPLE_COUNT)
        {
            cap_measurement_triggered = 0;
            capacitance_reading_count = 0;
        }
    }
}

static void display_resistance(void)
{
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
}

static void display_capacitance(void)
{
#if REVISION == 2
    cap_trigger_indicator();     
    double capacitance_reading = get_capacitance(capacitance_samples, gpio_get(RANGE_PIN));
    if(out_of_range_low_condition_capacitance(capacitance_reading))
    {
        display_short_circuit();
    }
    else
    {
        display_unit_prefix_capacitance(capacitance_reading); 
        display_double(scale_capacitance(capacitance_reading));
    }
#endif
}

void display_reading(void)
{
    if(mode == Voltage)
    { 
        display_double(voltage_reading.magnitude);
        negative_sign(voltage_reading.sign);
        disable_prefix_indicators();
    }
    else if(mode == Resistance)
    {
        disable_negative_sign();
        display_resistance(); 
    }
    else if(mode == Capacitance && cap_measurement_recorded)
    {
        disable_negative_sign();
        display_capacitance(); 
    }
    else
    {
        disable_negative_sign();
    }
}
