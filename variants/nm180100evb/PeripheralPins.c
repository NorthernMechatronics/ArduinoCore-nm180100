#include "am_mcu_apollo.h"

#include "PinNames.h"
#include "PeripheralPins.h"
#include "PeripheralNames.h"

const PinMap PinMap_UART_TX[] =
{
    { GPIO22, UART_0, AM_HAL_PIN_22_UART0TX }
};

const PinMap PinMap_UART_RX[] =
{
    { GPIO23, UART_0, AM_HAL_PIN_23_UART0RX }
};

const PinMap PinMap_SPI_MOSI[] =
{
};

const PinMap PinMap_SPI_MISO[] =
{
};

const PinMap PinMap_SPI_SCLK[] =
{
};

const PinMap PinMap_SPI_SSEL[] =
{
};

const PinMap PinMap_I2C_SDA[] =
{
};

const PinMap PinMap_I2C_SCL[] =
{
};
