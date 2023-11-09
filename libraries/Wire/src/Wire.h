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
#ifndef _WIRE_H_
#define _WIRE_H_

#include <stdint.h>

#include <am_hal_queue.h>

#include <FreeRTOS.h>
#include <semphr.h>

#include "HardwareI2C.h"

#define IOM_MAX_BUFFER_SIZE (255)

namespace arduino
{

struct I2CPinMap;

class nmI2C : public HardwareI2C
{
public:
    nmI2C(uint32_t module, I2CPinMap *pinMap);

    virtual void begin();
    virtual void begin(uint8_t address);
    virtual void end();

    virtual void setClock(uint32_t freq);

    virtual void beginTransmission(uint8_t address);
    virtual uint8_t endTransmission(bool stopBit);
    virtual uint8_t endTransmission(void);

    virtual size_t requestFrom(uint8_t address, size_t len, bool stopBit);
    virtual size_t requestFrom(uint8_t address, size_t len);

    virtual void onReceive(void (*)(int));
    virtual void onRequest(void (*)(void));

    virtual int available();
    virtual int read();
    virtual int read(uint8_t *buffer, size_t size);
    virtual int peek();

    virtual size_t write(uint8_t n);
    virtual size_t write(const uint8_t *buffer, size_t size);

    using Print::write;

    virtual void isr(void);

protected:
    uint8_t mTxBuffer[IOM_MAX_BUFFER_SIZE];
    uint8_t mRxBuffer[IOM_MAX_BUFFER_SIZE];
    uint8_t mTransferBuffer[IOM_MAX_BUFFER_SIZE];

    am_hal_queue_t mTxQueue;
    am_hal_queue_t mRxQueue;

private:
    uint32_t mModule;
    I2CPinMap *mPinMap;
    void *mIomHandle;
    void *mIosHandle;
    uint32_t mAddress;
    uint32_t mFrequency;
    SemaphoreHandle_t mMutex;
    void (*mReceiveHandler)(int);
    void (*mRequestHandler)(void);
};

} // namespace arduino

typedef arduino::nmI2C TwoWire;

#endif