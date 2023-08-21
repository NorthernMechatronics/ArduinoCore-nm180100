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
#ifndef _UART_H_
#define _UART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <am_mcu_apollo.h>

#ifdef __cplusplus
}
#endif

#include <FreeRTOS.h>
#include <task.h>

#include "HardwareSerial.h"

#define UART_BUFFER_SIZE    (1024)

struct UartPinMap
{
    uint32_t tx_pin;
    uint32_t rx_pin;
    uint32_t cts_pin;
    uint32_t rts_pin;
    am_hal_gpio_pincfg_t tx_pincfg;
    am_hal_gpio_pincfg_t rx_pincfg;
    am_hal_gpio_pincfg_t cts_pincfg;
    am_hal_gpio_pincfg_t rts_pincfg;
};

class Uart : public arduino::HardwareSerial
{
public:
    Uart(uint32_t module, UartPinMap *pinMap);
    void begin(unsigned long);
    void begin(unsigned long baudrate, uint16_t config);
    void end();
    int available(void);
    int availableForWrite(void);
    int peek(void);
    int read(void);
    void flush(void);
    size_t write(uint8_t c);
    size_t write(const uint8_t *buffer, size_t size);
    operator bool();

    using Print::write; // pull in write(str) and write(buf, size) from Print

    void isr(void);
protected:
    uint8_t mTxBuffer[UART_BUFFER_SIZE];
    uint8_t mRxBuffer[UART_BUFFER_SIZE];

private:
    uint32_t mModule;
    am_hal_uart_config_t mConfig;
    UartPinMap *mPinMap;
    void *mUartHandle;
    TaskHandle_t mTaskHandle;
};

#endif