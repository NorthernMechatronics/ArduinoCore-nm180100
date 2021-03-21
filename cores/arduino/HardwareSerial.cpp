/*
 Stream.cpp - adds parsing methods to Stream class
 Copyright (c) 2008 David A. Mellis.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

 Created July 2011
 parsing functions based on TextFinder library by Michael Margolis

 findMulti/findUntil routines written by Jim Leonard/Xuth
 */
#include "pinmap.h"
#include "PeripheralPins.h"
#include "Common.h"
#include "HardwareSerial.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include "am_mcu_apollo.h"

typedef struct
{
    bool bValid;
    uint32_t regILPR;
    uint32_t regIBRD;
    uint32_t regFBRD;
    uint32_t regLCRH;
    uint32_t regCR;
    uint32_t regIFLS;
    uint32_t regIER;
}
am_hal_uart_register_state_t;

typedef struct
{
    am_hal_handle_prefix_t prefix;
    am_hal_uart_register_state_t sRegState;

    uint32_t ui32Module;

    bool bEnableTxQueue;
    am_hal_queue_t sTxQueue;

    bool bEnableRxQueue;
    am_hal_queue_t sRxQueue;

    uint32_t ui32BaudRate;
}
am_hal_uart_state_t;

#ifdef __cplusplus
}
#endif


using namespace arduino;

extern void *uart_handle[AM_REG_UART_NUM_MODULES];

HardwareSerial::HardwareSerial()
 : handle(0)
{
    txPin = GPIO22;
    rxPin = GPIO23;
}

HardwareSerial::HardwareSerial(PinName tx, PinName rx)
 : handle(0), txPin(tx), rxPin(rx)
{
}

HardwareSerial::~HardwareSerial()
{

}

void HardwareSerial::begin(unsigned long baudrate)
{
    initialize(
        baudrate,
        AM_HAL_UART_DATA_BITS_8,
        AM_HAL_UART_PARITY_NONE,
        AM_HAL_UART_ONE_STOP_BIT,
        AM_HAL_UART_FLOW_CTRL_NONE);
}

void HardwareSerial::begin(unsigned long baudrate, uint16_t config)
{
    uint32_t databits;
    uint32_t parity;
    uint32_t stopbit;

    switch (config & SERIAL_DATA_MASK)
    {
    case SERIAL_DATA_5:
        databits = AM_HAL_UART_DATA_BITS_5;
        break;
    case SERIAL_DATA_6:
        databits = AM_HAL_UART_DATA_BITS_6;
        break;
    case SERIAL_DATA_7:
        databits = AM_HAL_UART_DATA_BITS_7;
        break;
    case SERIAL_DATA_8:
    default:
        databits = AM_HAL_UART_DATA_BITS_8;
        break;
    }

    switch (config & SERIAL_PARITY_MASK)
    {
    case SERIAL_PARITY_EVEN:
        parity = AM_HAL_UART_PARITY_EVEN;
        break;
    case SERIAL_PARITY_ODD:
        parity = AM_HAL_UART_PARITY_ODD;
        break;
    case SERIAL_PARITY_NONE:
    default:
        parity = AM_HAL_UART_PARITY_NONE;
        break;
    }

    switch (config & SERIAL_STOP_BIT_MASK)
    {
    case SERIAL_STOP_BIT_2:
        stopbit = AM_HAL_UART_TWO_STOP_BITS;
        break;
    case SERIAL_STOP_BIT_1:
    default:
        stopbit = AM_HAL_UART_ONE_STOP_BIT;
        break;
    }

    initialize(
        baudrate,
        databits,
        parity,
        stopbit,
        AM_HAL_UART_FLOW_CTRL_NONE);
}

void HardwareSerial::end()
{
    deinitialize();
}

int HardwareSerial::available()
{
    am_hal_uart_state_t *state = (am_hal_uart_state_t *)handle;
    am_hal_queue_t *queue = &state->sRxQueue;

    return (!am_hal_queue_empty(queue));
}

int HardwareSerial::availableForWrite()
{
    am_hal_uart_state_t *state = (am_hal_uart_state_t *)handle;
    am_hal_queue_t *queue = &state->sTxQueue;

    return (!am_hal_queue_full(queue));
}

int HardwareSerial::peek()
{
    am_hal_uart_state_t *state = (am_hal_uart_state_t *)handle;
    am_hal_queue_t *queue = &state->sRxQueue;

    if (am_hal_queue_empty(queue))
    {
        return -1;
    }

    int *item = (int *)am_hal_queue_peek(queue);
    return *item;
}

int HardwareSerial::read()
{
    uint32_t ui32BytesTransferred = 0;
    uint8_t  buffer;

    const am_hal_uart_transfer_t uart_transfer =
    {
        .ui32Direction = AM_HAL_UART_READ,
        .pui8Data = (uint8_t *)&buffer,
        .ui32NumBytes = 1,
        .ui32TimeoutMs = _timeout,
        .pui32BytesTransferred = &ui32BytesTransferred,
    };

    if (handle)
    {
        am_hal_uart_transfer(handle, &uart_transfer);
    }

    if (ui32BytesTransferred)
    {
        return buffer & 0xFF;
    }

    return -1;
}

void HardwareSerial::flush()
{
    am_hal_uart_tx_flush(handle);
}

size_t HardwareSerial::write(uint8_t ch)
{
    return write(&ch, 1);
}

size_t HardwareSerial::write(const uint8_t *buffer, size_t size)
{
    uint32_t ui32BytesTransferred = 0;

    const am_hal_uart_transfer_t uart_transfer =
    {
        .ui32Direction = AM_HAL_UART_WRITE,
        .pui8Data = (uint8_t *)buffer,
        .ui32NumBytes = size,
        .ui32TimeoutMs = _timeout,
        .pui32BytesTransferred = &ui32BytesTransferred,
    };

    if (handle)
    {
        am_hal_uart_transfer(handle, &uart_transfer);
    }

    return ui32BytesTransferred;
}

HardwareSerial::operator bool()
{
    return true;
}

void HardwareSerial::initialize(
        uint32_t baudrate,
        uint32_t databits,
        uint32_t parity,
        uint32_t stopbits,
        uint32_t flowcontrol
        )
{
    am_hal_gpio_pincfg_t tx_pincfg, rx_pincfg;

    memset(&tx_pincfg, 0, sizeof(am_hal_gpio_pincfg_t));
    memset(&rx_pincfg, 0, sizeof(am_hal_gpio_pincfg_t));

    tx_pincfg.eDriveStrength = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA;

    am_hal_uart_config_t config;
    config.ui32BaudRate    = baudrate;
    config.ui32DataBits    = databits;
    config.ui32Parity      = parity;
    config.ui32StopBits    = stopbits;
    config.ui32FlowControl = flowcontrol;
    config.ui32FifoLevels  = (AM_HAL_UART_TX_FIFO_1_2 | AM_HAL_UART_RX_FIFO_1_2);
    config.pui8TxBuffer     = txbuffer;
    config.ui32TxBufferSize = sizeof(txbuffer);
    config.pui8RxBuffer     = rxbuffer;
    config.ui32RxBufferSize = sizeof(rxbuffer);

    uint32_t uart_module_tx = pinmap_peripheral(txPin, PinMap_UART_TX);
    uint32_t uart_module_rx = pinmap_peripheral(rxPin, PinMap_UART_RX);
    uint32_t uart_module = 0xFFFFFFFF;

    if (uart_module_tx == uart_module_rx)
    {
        uart_module = uart_module_tx;
    }

    am_hal_uart_initialize(uart_module, &handle);
    am_hal_uart_power_control(handle, AM_HAL_SYSCTRL_WAKE, false);
    am_hal_uart_configure(handle, &config);

    tx_pincfg.uFuncSel = pinmap_function(txPin, PinMap_UART_TX);
    rx_pincfg.uFuncSel = pinmap_function(rxPin, PinMap_UART_RX);

    am_hal_gpio_pinconfig(txPin, tx_pincfg);
    am_hal_gpio_pinconfig(rxPin, rx_pincfg);

    am_hal_uart_interrupt_enable(handle, AM_HAL_UART_INT_RX | AM_HAL_UART_INT_TX);
    NVIC_EnableIRQ((IRQn_Type)(UART0_IRQn + uart_module));

    uart_handle[uart_module] = handle;
}

void HardwareSerial::deinitialize()
{
    am_hal_uart_power_control(handle, AM_HAL_SYSCTRL_DEEPSLEEP, true);
    am_hal_uart_deinitialize(handle);
}
