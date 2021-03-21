#ifndef PERIPHERALPINS_H
#define PERIPHERALPINS_H

#include "pinmap.h"

extern const PinMap PinMap_UART_TX[];
extern const PinMap PinMap_UART_RX[];

extern const PinMap PinMap_SPI_MOSI[];
extern const PinMap PinMap_SPI_MISO[];
extern const PinMap PinMap_SPI_SCLK[];
extern const PinMap PinMap_SPI_SSEL[];

extern const PinMap PinMap_I2C_SDA[];
extern const PinMap PinMap_I2C_SCL[];

extern const PinMap PinMap_ADC[];

#define LED0    GPIO10
#define LED1    GPIO30
#define LED2    GPIO15
#define LED3    GPIO14
#define LED4    GPIO17

#define BUTTON0 GPIO16
#define BUTTON1 GPIO18
#define BUTTON2 GPIO19

#endif /* PERIPHERALPINS_H */
