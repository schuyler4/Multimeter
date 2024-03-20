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
#include <math.h>

#include "pico/stdlib.h"

#include "hardware/spi.h"

#include "main.h"
#include "MCP3561.h"
#include "calcs.h"
#include "display_driver.h"

static double average_resistance_reading = 0;
static double resistance_reading = 0;
static uint8_t resistance_reading_count = 0;

static volatile double average_voltage_reading = 0;
static volatile double voltage_reading = 0;
static volatile uint8_t voltage_sign = 0;
static volatile uint8_t voltage_reading_count = 0;

static double capacitance_samples[CAPACITANCE_SAMPLE_COUNT];
static uint8_t capacitance_reading_count = 0;
static uint8_t cap_measurement_triggered = 0;
static volatile uint8_t cap_measurement_recorded;

static Mode mode = Voltage;

static uint32_t code;

int main(void)
{
    stdio_init_all();
    
    setup_SPI();
    setup_IO();
    
    setup_MCP3561();

    cap_measurement_recorded = 0;
    
    while(1)
    {
        if(mode == Voltage)
        { 
            printf("Voltage: %f\n", voltage_reading);
            display_double(voltage_reading);
            negative_sign(voltage_sign);
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
                display_integer((int)resistance_reading);
            }
            low_ohm(low_ohm_condition(resistance_reading));
        }
        else if(mode == Capacitance && cap_measurement_recorded)
        {
            cap_triggered();     
            double capacitance = get_capacitance(capacitance_samples);
            printf("%f\n", capacitance); 
            cap_measurement_recorded = 0;
        }
    }

    // The program should never return.
    return 1;
}

void adc_data_callback(uint gpio, uint32_t events)
{
    code = MCP3561_read_code();
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

    gpio_init(PICO_PIN);
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
    
    gpio_set_dir(CS_PIN, GPIO_OUT);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    gpio_set_dir(DIGIT1_PIN, GPIO_OUT);
    gpio_set_dir(DIGIT2_PIN, GPIO_OUT);
    gpio_set_dir(DIGIT3_PIN, GPIO_OUT);
    gpio_set_dir(DIGIT4_PIN, GPIO_OUT);

    gpio_set_dir(PICO_PIN, GPIO_OUT);
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
    
    gpio_put(PICO_PIN, 0);
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

    gpio_set_irq_enabled_with_callback(
        DATA_INTERUPT_PIN, 
        GPIO_IRQ_EDGE_FALL, 
        true, 
        *adc_data_callback);
}

void sample_resistance(void)
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

void sample_voltage(void)
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
        voltage_sign = !(voltage_reading >= 0.0); 
        voltage_reading = fabs(voltage_reading);
        voltage_reading_count = 0;
        average_voltage_reading = 0;
    }
}

void sample_capacitance(void)
{
    double voltage = get_capacitor_voltage(code); 
    if(voltage < CAPACITANCE_VOLTAGE_THRESHOLD)
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
