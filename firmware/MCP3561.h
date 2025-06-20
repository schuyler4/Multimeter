//
// FILENAME: MCP3561.h
//
// DESCRIPTION: definitions for MCP3561 24 bit ADC driver.
//
// WRITTEN BY: Marek Newton
//

#ifndef MCP3561_H
#define MCP3561_H

#define ADC_DATA_BYTE_COUNT 3

#define WRITE_COMMAND_MASK 0x02
#define READ_COMMAND_MASK 0x01

#define ADCDATA_REGISTER 0x0
#define CONFIG0_REGISTER_ADDRESS 0x1
#define CONFIG1_REGISTER_ADDRESS 0x2
#define CONFIG2_REGISTER_ADDRESS 0x3
#define CONFIG3_REGISTER_ADDRESS 0x4
#define INTERRUPT_REQUEST_REGISTER 0x5
#define MULTIPLEXER_REGISTER 0x6
#define SCAN_MODE_SETTINGS_REGISTER 0x7
#define TIMER_DELAY_VALUE_REGISTER 0x8
#define OFFSET_CALIBRATION_REGISTER 0x9
#define GAINCAL_REGISTER 0xA

#define DEVICE_ADDRESS_READ 0x01
#define DEVICE_ADDRESS_WRITE 0x01
#define DEVICE_ADDRESS_MASK_READ (DEVICE_ADDRESS_READ << 6)
#define DEVICE_ADDRESS_MASK_WRITE (DEVICE_ADDRESS_WRITE << 6)

#define COMMAND_ADDRESS_POSITION 2 
#define WRITE_COMMAND DEVICE_ADDRESS_MASK_WRITE | WRITE_COMMAND_MASK
#define READ_COMMAND DEVICE_ADDRESS_MASK_READ | READ_COMMAND_MASK 
#define SAMPLE_COMMAND 0x0A

#define CONFIG0_WRITE WRITE_COMMAND | (CONFIG0_REGISTER_ADDRESS << COMMAND_ADDRESS_POSITION)  
#define CONFIG1_WRITE WRITE_COMMAND | (CONFIG1_REGISTER_ADDRESS << COMMAND_ADDRESS_POSITION)
#define CONFIG2_WRITE WRITE_COMMAND | (CONFIG2_REGISTER_ADDRESS << COMMAND_ADDRESS_POSITION)
#define CONFIG3_WRITE WRITE_COMMAND | (CONFIG3_REGISTER_ADDRESS << COMMAND_ADDRESS_POSITION)
#define IRQ_WRITE WRITE_COMMAND | (INTERRUPT_REQUEST_REGISTER << COMMAND_ADDRESS_POSITION)

// CONFIG 0 Register Offsets
#define VREF_SEL 7 
#define CONFIG0 6
#define CLK_SEL 4
#define CS_SEL 2
#define ADC_MODE 0

// CONFIG 1 Register Offsets
#define PRE 6
#define OSR 2

// CONFIG 2 Register Offsets
#define BOOST 6
#define GAIN 3
#define AZ_MUX 2
#define AZ_REF 1

// CONFIG 3 Register Offsets
#define CONV_MODE 6
#define DATA_FORMAT 4
#define CRC_FORMAT 3
#define EN_CRCCOM 2
#define EN_OFFCAL 1
#define EN_GAINCAL 0

// IRQ Register Offsets
#define DR_STATUS 6
#define CRCCFG_STATUS 5
#define POR_STATUS 4
#define IRQ_MODE 2
#define EN_FASTCMD 1
#define EN_STP 0

#define CONFIG0_READ READ_COMMAND | (CONFIG0_REGISTER_ADDRESS << COMMAND_ADDRESS_POSITION)
#define CONFIG1_READ READ_COMMAND | (CONFIG1_REGISTER_ADDRESS << COMMAND_ADDRESS_POSITION)
#define CONFIG2_READ READ_COMMAND | (CONFIG2_REGISTER_ADDRESS << COMMAND_ADDRESS_POSITION)
#define CONFIG3_READ READ_COMMAND | (CONFIG3_REGISTER_ADDRESS << COMMAND_ADDRESS_POSITION)
#define ADC_DATA_READ_COMMAND READ_COMMAND | (ADCDATA_REGISTER << COMMAND_ADDRESS_POSITION)
#define IRQ_READ_COMMAND READ_COMMAND | (INTERRUPT_REQUEST_REGISTER << COMMAND_ADDRESS_POSITION)

void setup_MCP3561(void);
uint32_t MCP3561_read_code(void);
uint8_t MCP3561_read_register(void);
void sample_MCP3561(void);
void dump_registers(void);

#endif
