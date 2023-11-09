/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2023, Northern Mechatronics, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ArduinoAPI.h"
#include "uart.h"
#include "uart_pinmap.h"

using namespace arduino;

///////////////////////////////////////////////////////////////////////////////
//
// Redefinition of the private UART HAL state information from am_hal_uart.c
//
///////////////////////////////////////////////////////////////////////////////
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
} am_hal_uart_register_state_t;

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

    am_hal_uart_clock_speed_e  eUartClockSpeed ;
}
am_hal_uart_state_t;

///////////////////////////////////////////////////////////////////////////////

Uart::Uart(uint32_t module, UartPinMap *pinMap) : mModule(module), mPinMap(pinMap)
{
    mUartHandle = NULL;
}

void Uart::begin(unsigned long baudrate)
{
    begin(baudrate, SERIAL_8N1);
}

void Uart::begin(unsigned long baudrate, uint16_t config)
{
    am_hal_uart_config_t uart_config;
    uint32_t data, parity, stop;
    data   = config & SERIAL_DATA_MASK;
    parity = config & SERIAL_PARITY_MASK;
    stop   = config & SERIAL_STOP_BIT_MASK;

    uart_config.ui32BaudRate = baudrate;
    switch (data)
    {
    case SERIAL_DATA_5:
        uart_config.ui32DataBits = AM_HAL_UART_DATA_BITS_5;
        break;
    case SERIAL_DATA_6:
        uart_config.ui32DataBits = AM_HAL_UART_DATA_BITS_6;
        break;
    case SERIAL_DATA_7:
        uart_config.ui32DataBits = AM_HAL_UART_DATA_BITS_7;
        break;
    case SERIAL_DATA_8:
    default:
        uart_config.ui32DataBits = AM_HAL_UART_DATA_BITS_8;
        break;
    }

    switch (parity)
    {
    case SERIAL_PARITY_EVEN:
        uart_config.ui32Parity = AM_HAL_UART_PARITY_EVEN;
        break;
    case SERIAL_PARITY_ODD:
        uart_config.ui32Parity = AM_HAL_UART_PARITY_ODD;
        break;
    case SERIAL_PARITY_NONE:
    default:
        uart_config.ui32Parity = AM_HAL_UART_PARITY_NONE;
        break;
    }

    switch (stop)
    {
    case SERIAL_STOP_BIT_2:
        uart_config.ui32StopBits = AM_HAL_UART_TWO_STOP_BITS;
        break;
    case SERIAL_STOP_BIT_1:
    default:
        uart_config.ui32StopBits = AM_HAL_UART_ONE_STOP_BIT;
        break;
    }

    if (mPinMap->cts_pin && mPinMap->rts_pin)
    {
        uart_config.ui32FlowControl = AM_HAL_UART_FLOW_CTRL_RTS_CTS;
    }
    else if (mPinMap->cts_pin)
    {
        uart_config.ui32FlowControl = AM_HAL_UART_FLOW_CTRL_CTS_ONLY;
    }
    else if (mPinMap->rts_pin)
    {
        uart_config.ui32FlowControl = AM_HAL_UART_FLOW_CTRL_RTS_ONLY;
    }
    else
    {
        uart_config.ui32FlowControl = AM_HAL_UART_FLOW_CTRL_NONE;
    }

    uart_config.ui32FifoLevels = AM_HAL_UART_TX_FIFO_3_4 | AM_HAL_UART_RX_FIFO_3_4;
    uart_config.pui8TxBuffer = mTxBuffer;
    uart_config.ui32TxBufferSize = sizeof(mTxBuffer);
    uart_config.pui8RxBuffer = mRxBuffer;
    uart_config.ui32RxBufferSize = sizeof(mRxBuffer);

    am_hal_uart_initialize(mModule, &mUartHandle);
    am_hal_uart_power_control(mUartHandle, AM_HAL_SYSCTRL_WAKE, false);
    am_hal_uart_configure(mUartHandle, &uart_config);

    am_hal_uart_interrupt_enable(mUartHandle, AM_HAL_UART_INT_RX);
    NVIC_EnableIRQ((IRQn_Type)(UART0_IRQn + mModule));
    NVIC_SetPriority((IRQn_Type)(UART0_IRQn + mModule), NVIC_configKERNEL_INTERRUPT_PRIORITY);

    am_hal_gpio_pinconfig(mPinMap->tx_pin, mPinMap->tx_pincfg);
    am_hal_gpio_pinconfig(mPinMap->rx_pin, mPinMap->rx_pincfg);
    if (mPinMap->cts_pin)
    {
        am_hal_gpio_pinconfig(mPinMap->cts_pin, mPinMap->cts_pincfg);
    }
    if (mPinMap->rts_pin)
    {
        am_hal_gpio_pinconfig(mPinMap->rts_pin, mPinMap->rts_pincfg);
    }
}

void Uart::end()
{
    am_hal_uart_power_control(mUartHandle, AM_HAL_SYSCTRL_DEEPSLEEP, false);
    am_hal_uart_deinitialize(mUartHandle);

    am_hal_gpio_pinconfig(mPinMap->tx_pin, g_AM_HAL_GPIO_DISABLE);
    am_hal_gpio_pinconfig(mPinMap->rx_pin, g_AM_HAL_GPIO_DISABLE);
    if (mPinMap->cts_pin && mPinMap->rts_pin)
    {
        am_hal_gpio_pinconfig(mPinMap->cts_pin, g_AM_HAL_GPIO_DISABLE);
        am_hal_gpio_pinconfig(mPinMap->rts_pin, g_AM_HAL_GPIO_DISABLE);
    }
    else if (mPinMap->cts_pin)
    {
        am_hal_gpio_pinconfig(mPinMap->cts_pin, g_AM_HAL_GPIO_DISABLE);
    }
    else if (mPinMap->rts_pin)
    {
        am_hal_gpio_pinconfig(mPinMap->rts_pin, g_AM_HAL_GPIO_DISABLE);
    }
}

int Uart::available(void)
{
    am_hal_uart_state_t *state = (am_hal_uart_state_t *)mUartHandle;
    am_hal_queue_t *queue = &state->sRxQueue;

    return am_hal_queue_items_left(queue);
}

int Uart::availableForWrite(void)
{
    am_hal_uart_state_t *state = (am_hal_uart_state_t *)mUartHandle;
    am_hal_queue_t *queue = &state->sTxQueue;

    return am_hal_queue_slots_left(queue);
}

int Uart::peek(void)
{
    am_hal_uart_state_t *state = (am_hal_uart_state_t *)mUartHandle;
    am_hal_queue_t *queue = &state->sRxQueue;

    if (am_hal_queue_empty(queue))
    {
        return -1;
    }

    int *item = (int *)am_hal_queue_peek(queue);
    return *item;
}

int Uart::read(void)
{
    uint8_t ui8Byte = 0;
    uint32_t ui32BytesRead = 0;

    const am_hal_uart_transfer_t transfer = {
        .ui32Direction = AM_HAL_UART_READ,
        .pui8Data = (uint8_t *)&ui8Byte,
        .ui32NumBytes = 1,
        .ui32TimeoutMs = _timeout,
        .pui32BytesTransferred = &ui32BytesRead,
    };
    am_hal_uart_transfer(mUartHandle, &transfer);

    if (ui32BytesRead)
    {
        return ui8Byte;
    }

    return -1;
}

void Uart::flush(void)
{
    am_hal_uart_tx_flush(mUartHandle);
}

size_t Uart::write(uint8_t c)
{
    return write(&c, 1);
}

size_t Uart::write(const uint8_t *buffer, size_t size)
{
    uint32_t ui32TotalBytesWritten = 0;
    uint32_t ui32BytesWritten = 0;

    const am_hal_uart_transfer_t transfer = {
        .ui32Direction = AM_HAL_UART_WRITE,
        .pui8Data = (uint8_t *)buffer,
        .ui32NumBytes = size,
        .ui32TimeoutMs = _timeout,
        .pui32BytesTransferred = &ui32BytesWritten,
    };

    while (ui32TotalBytesWritten < size)
    {
        if (AM_HAL_STATUS_SUCCESS != am_hal_uart_transfer(mUartHandle, &transfer))
        {
            return ui32TotalBytesWritten;
        }

        ui32TotalBytesWritten += ui32BytesWritten;
    }

    return ui32TotalBytesWritten;
}

Uart::operator bool()
{
    if (mUartHandle)
        return true;

    return false;
}

void Uart::isr(void)
{
    BaseType_t bContextSwitch;

    uint32_t ui32Status;
    uint32_t ui32UartIdle;

    am_hal_uart_interrupt_status_get(mUartHandle, &ui32Status, true);
    am_hal_uart_interrupt_clear(mUartHandle, ui32Status);
    am_hal_uart_interrupt_service(mUartHandle, ui32Status, &ui32UartIdle);
}