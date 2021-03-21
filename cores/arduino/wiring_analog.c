/*
  wiring_digital.c - digital input and output functions
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  Modified 28 September 2010 by Mark Sproul
*/
#include <string.h>
#include "am_mcu_apollo.h"
#include "am_util.h"

#include "system_config.h"
#include "Common.h"
#include "pinmap.h"
#include "PeripheralPins.h"

#define ARDUINO_MAIN

static void *adc_handle;
static am_hal_adc_refsel_e adc_reference = AM_HAL_ADC_REFSEL_INT_2P0;
static int8_t adc_read_resolution = ADC_MAX_RESOLUTION;
static volatile uint32_t adc_cnv_complete;

static void* adc_initialize(pin_size_t pinNumber)
{
    am_hal_gpio_pincfg_t pincfg;
    memset(&pincfg, 0, sizeof(am_hal_gpio_pincfg_t));

    uint32_t adc_ch = pinmap_peripheral(pinNumber, PinMap_ADC);
    uint32_t func   = pinmap_function(pinNumber, PinMap_ADC);

    pincfg.uFuncSel = func;
    am_hal_gpio_pinconfig(pinNumber, pincfg);


    am_hal_adc_config_t adc_config;
    am_hal_adc_slot_config_t adc_slot_config;
    void *handle;

    adc_config.eClock     = AM_HAL_ADC_CLKSEL_HFRC_DIV2;
    adc_config.ePolarity  = AM_HAL_ADC_TRIGPOL_RISING;
    adc_config.eTrigger   = AM_HAL_ADC_TRIGSEL_SOFTWARE;
    adc_config.eReference = adc_reference;
    adc_config.eClockMode = AM_HAL_ADC_CLKMODE_LOW_LATENCY;
    adc_config.ePowerMode = AM_HAL_ADC_LPMODE0;
    adc_config.eRepeat    = AM_HAL_ADC_REPEATING_SCAN;

    adc_slot_config.eMeasToAvg     = AM_HAL_ADC_SLOT_AVG_4;
    adc_slot_config.ePrecisionMode = AM_HAL_ADC_SLOT_14BIT;
    adc_slot_config.eChannel       = adc_ch;
    adc_slot_config.bWindowCompare = false;
    adc_slot_config.bEnabled       = true;

    am_hal_adc_initialize(0, &handle);
    am_hal_adc_power_control(handle, AM_HAL_SYSCTRL_WAKE, false);
    am_hal_adc_configure(handle, &adc_config);
    am_hal_adc_disable(handle);
    am_hal_adc_configure_slot(handle, 0, &adc_slot_config);
    am_hal_adc_enable(handle);

    am_hal_adc_interrupt_enable(handle, AM_HAL_ADC_INT_CNVCMP);
    NVIC_EnableIRQ(ADC_IRQn);

    adc_cnv_complete = 0;

    return handle;
}

static void adc_uninitialize(void *handle)
{
    NVIC_DisableIRQ(ADC_IRQn);
    am_hal_adc_interrupt_disable(handle, AM_HAL_ADC_INT_CNVCMP);
    am_hal_adc_disable(handle);
    am_hal_pwrctrl_periph_disable(AM_HAL_PWRCTRL_PERIPH_ADC);
    am_hal_adc_deinitialize(handle);
}

static void adc_timer_initialize(void)
{
    am_hal_ctimer_config_single(ADC_TIMER_NUM, ADC_TIMER_SEG,
            AM_HAL_CTIMER_HFRC_47KHZ |
            AM_HAL_CTIMER_FN_REPEAT |
            AM_HAL_CTIMER_ADC_TRIG);

    am_hal_ctimer_period_set(ADC_TIMER_NUM, ADC_TIMER_SEG, 3, 1);

    //
    // Start the timer.
    //
    am_hal_ctimer_start(ADC_TIMER_NUM, ADC_TIMER_SEG);
    am_hal_ctimer_adc_trigger_enable();
}

static void adc_timer_uninitialize(void)
{
    am_hal_ctimer_adc_trigger_disable();
    am_hal_ctimer_stop(ADC_TIMER_NUM, ADC_TIMER_SEG);
}

int analogRead(pin_size_t pinNumber)
{
    adc_handle = adc_initialize(pinNumber);
    am_hal_adc_sw_trigger(adc_handle);

    adc_timer_initialize();
    while (adc_cnv_complete < 4)
    {
        // FIXME: change to yield for multi-threaded implementation.
        am_hal_sysctrl_sleep(AM_HAL_SYSCTRL_SLEEP_DEEP);
    }
    adc_timer_uninitialize();


    uint32_t sample_count = 1;
    uint32_t sample_code = 0;
    uint32_t scaled_code = 0;
    am_hal_adc_sample_t sample;

    am_hal_adc_samples_read(adc_handle, false, NULL, &sample_count, &sample);
    sample_code = AM_HAL_ADC_FIFO_FULL_SAMPLE(sample.ui32Sample);
    adc_uninitialize(adc_handle);

    scaled_code = sample_code;
    int8_t dres = adc_read_resolution - ADC_MAX_RESOLUTION;

    if (dres > 0)
    {
        scaled_code <<= dres;
    }
    else if (dres < 0)
    {
        scaled_code >>= -dres;
    }

    return scaled_code;
}

void analogWrite(pin_size_t pinNumber, int value)
{

}

void analogReference(am_hal_adc_refsel_e ref)
{
    switch(ref)
    {
    case AM_HAL_ADC_REFSEL_INT_2P0:
    case AM_HAL_ADC_REFSEL_INT_1P5:
        adc_reference = ref;
        break;

    case AM_HAL_ADC_REFSEL_EXT_2P0:
    case AM_HAL_ADC_REFSEL_EXT_1P5:
    default:
        adc_reference = AM_HAL_ADC_REFSEL_INT_2P0;
        break;
    }
}

void analogReadResolution(uint8_t bits)
{
    if (bits > ADC_MAX_RESOLUTION)
    {
        bits = ADC_MAX_RESOLUTION;
    }
    if (bits < ADC_MIN_RESOLUTION)
    {
        bits = ADC_MIN_RESOLUTION;
    }

    adc_read_resolution = bits;
}

void analogWriteResolution(uint8_t bits)
{

}

void am_adc_isr(void)
{
    uint32_t ui32IntMask;

    am_hal_adc_interrupt_status(adc_handle, &ui32IntMask, false);
    am_hal_adc_interrupt_clear(adc_handle, ui32IntMask);

    if (ui32IntMask & AM_HAL_ADC_INT_CNVCMP)
    {
        adc_cnv_complete++;
    }
}
