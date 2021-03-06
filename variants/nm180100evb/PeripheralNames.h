#ifndef PERIPHERALNAMES_H
#define PERIPHERALNAMES_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UART_0 = 0,
    UART_1 = 1,
    UART_COUNT,
} UARTName;

typedef enum {
	SPI_0 = 0,
	SPI_1 = 1,
	SPI_2 = 2,
	SPI_3 = 3,
	SPI_4 = 4,
	SPI_5 = 5,
    SPI_COUNT,
} SPIName;

typedef enum {
	I2C_0 = 0,
	I2C_1 = 1,
	I2C_2 = 2,
	I2C_3 = 3,
	I2C_4 = 4,
	I2C_5 = 5,
    I2C_COUNT,
} I2CName;

typedef enum {
    ADC0 = 0,
    ADC1 = 1,
    ADC2 = 2,
    ADC3 = 3,
    ADC4 = 4,
    ADC5 = 5,
    ADC6 = 6,
    ADC7 = 7,
    ADC8 = 8,
    ADC9 = 9,
} ADCName;

#ifdef __cplusplus
}
#endif

#endif


