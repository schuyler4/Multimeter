#ifndef MAIN_H
#define MAIN_H

#define SCK_PIN 2
#define MOSI_PIN 3 // MASTER OUT SLAVE IN 
#define MISO_PIN 4// MASTER IN SLAVE OUT
#define CHIP_SELECT_PIN 5

#define SPI_SCK_FREQ 2000000

void setup_SPI(void);
void setup_IO(void);

#endif
