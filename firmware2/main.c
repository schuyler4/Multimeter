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
#include "hysteresis_display.h"

#include "IIR_filter.h"

static double average_resistance_reading = 0;
static double resistance_reading = 0;
static uint8_t resistance_reading_count = 0;

static volatile double average_voltage_reading = 0;
static volatile Signed_Voltage voltage_reading;
static volatile uint8_t voltage_sign = 0;
static volatile uint16_t voltage_reading_count = 0;

static volatile double average_diode_voltage_reading = 0;
static volatile double diode_voltage_reading = 0;
static volatile uint8_t diode_voltage_reading_count = 0;

IIR_Filter voltage_iir_filter;
IIR_Filter resistance_iir_filter;

static Mode past_mode;
static Mode mode;

static uint8_t range = 0;
static uint8_t past_range;

static uint32_t code;

static uint8_t button_filter = 0;
static uint8_t button_pressed = 0;

static float display_value_voltage = 0;

static void mode_change(void)
{
    disable_prefix_indicators();
    zero_segments();
}

static void check_mode_change(void)
{
    if(gpio_get(MODE_SWITCH_PIN)) mode = Voltage;
    else
    {
        if(mode == Voltage)
            mode = Resistance;
        button_filter <<= 1;
        button_filter |= gpio_get(BUTTON_PIN);
        if(button_filter == BUTTON_HISTORY_MASK) button_pressed = 0;
        else if(button_filter == 0)
        {
            if(!button_pressed)
            {
                reset_indicators();
                if(mode == Resistance)
                {
                    mode = Continuity;
                    range = CURRENT_RANGE_COUNT-1;
                    for(uint8_t i=0; i < CURRENT_RANGE_COUNT; i++) 
                        gpio_put(CURRENT_RANGE_PINS[i], i == CURRENT_RANGE_COUNT-1);
                }
                else if(mode == Continuity)
                    mode = Diode;
                else if(mode == Diode)
                    mode = Resistance;
            }
            button_pressed = 1;
        }
    }
}

static void find_range_resistance(void)
{
    if(resistance_reading > RANGE_OVER_VOLTAGE && range > 0) range--;
    if(resistance_reading < RANGE_UNDER_VOLTAGE && range < CURRENT_RANGE_COUNT-1) range++;
    for(uint8_t i=0; i < CURRENT_RANGE_COUNT; i++) gpio_put(CURRENT_RANGE_PINS[i], i == range);
}

static void find_range_voltage(void)
{

}

static void display_resistance(void)
{
    
    if(resistance_reading/RANGE_CURRENTS[range] > OVERLOAD_RESISTANCE)
    {
        display_open_circuit();
        disable_aux_indicators();
    }
    else
    {
        double adjusted_resistance = resistance_adjustment(resistance_reading/RANGE_CURRENTS[range], range);
        display_double(scale_resistance(adjusted_resistance));
        display_unit_prefix_resistance(adjusted_resistance);
    }
}

static void display_diode(void)
{
    if(diode_voltage_reading > OVERLOAD_VOLTAGE) display_open_circuit();
    else display_double(diode_voltage_reading);
    display_diode_mode_indicator();
}

static void display_continuity(void)
{
    if(gpio_get(CONTINUITY_PIN)) display_short_circuit();
    else if(resistance_reading/RANGE_CURRENTS[range] <= OVERLOAD_VOLTAGE/RANGE_CURRENTS[range])
        display_double(scale_resistance(resistance_adjustment(resistance_reading/RANGE_CURRENTS[range], range)));
    else display_open_circuit();
    display_continuity_mode_indicator();
}

int main(void)
{
    stdio_init_all();
    
    setup_IO();
    setup_SPI();
    setup_MCP3561();

    IIR_filter_init(&voltage_iir_filter, 0.993);
    IIR_filter_init(&resistance_iir_filter, 0.993);

    voltage_reading.magnitude = 0;
    voltage_reading.sign = 0;

    mode = Voltage; 

    while(1)
    {
        check_mode_change();
        if(mode == Diode)
        {
            gpio_put(CURRENT_RANGE4_PIN, 1);
            display_diode();
        }
        else if(mode == Resistance)
        {
            disable_negative_sign();
            display_resistance();
        }
        else if(mode == Voltage)
        {
            display_double(display_hysteresis_voltage(voltage_reading.magnitude));
            if(display_hysteresis_sign_voltage()) gpio_put(LOW_OHM_AND_NEGATIVE_PIN, 1);
            else gpio_put(LOW_OHM_AND_NEGATIVE_PIN, 0);
            disable_prefix_indicators();
        }
        else if(mode == Continuity)
            display_continuity();
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
    if(mode == Diode) sample_diode();
    else if(mode == Resistance || mode == Continuity) sample_resistance();        
    else if(mode == Voltage) sample_voltage();    
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
    resistance_reading = IIR_filter_sample(&resistance_iir_filter, get_resistance(code));
    resistance_reading_count += 1;
    // Need to wait for the IIR filter for a little before auto range
    if(resistance_reading_count == AVERAGE_READING_COUNT)
    {
        printf("%d\n", range);
        find_range_resistance();
        resistance_reading_count = 0;
    }
}

void sample_diode(void)
{
    for(uint8_t i=0; i < CURRENT_RANGE_COUNT; i++) 
        gpio_put(CURRENT_RANGE_PINS[i], i == CURRENT_RANGE_COUNT-1);
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
    double measurement_voltage = get_measurement_voltage(code, 1);
    voltage_reading.magnitude = voltage_adjustment(IIR_filter_sample(&voltage_iir_filter, measurement_voltage));
    voltage_reading.sign = voltage_adjustment_signed(voltage_reading.magnitude) < 0.0; 
}