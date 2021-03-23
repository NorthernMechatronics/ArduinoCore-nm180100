/*
  main.cpp - Main loop for Arduino sketches
  Copyright (c) 2005-2013 Arduino Team.  All right reserved.
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
*/
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <am_mcu_apollo.h>

#ifdef __cplusplus
}
#endif

#define ARDUINO_MAIN
#include "Arduino.h"
#include "timermap.h"
#include "system_config.h"

// Weak empty variant initialization function.
// May be redefined by variant files.
void initVariant() __attribute__((weak));

void system_timer_init(void)
{
    // microsecond counter
    am_hal_ctimer_clear(
        SYSTEM_TIMER_SRC_US_NUM,
        SYSTEM_TIMER_SRC_US_SEG);
    
    am_hal_ctimer_config_single(
        SYSTEM_TIMER_SRC_US_NUM,
        SYSTEM_TIMER_SRC_US_SEG,
        (AM_HAL_CTIMER_FN_REPEAT |
         AM_HAL_CTIMER_HFRC_12MHZ));
    
    am_hal_ctimer_period_set(
        SYSTEM_TIMER_SRC_US_NUM,
        SYSTEM_TIMER_SRC_US_SEG, 12, 1);
    
    am_hal_ctimer_start(
        SYSTEM_TIMER_SRC_US_NUM,
        SYSTEM_TIMER_SRC_US_SEG);

    am_hal_stimer_config(AM_HAL_STIMER_CFG_CLEAR | AM_HAL_STIMER_CFG_FREEZE);
    am_hal_stimer_config(AM_HAL_STIMER_HFRC_CTIMER0A |
        AM_HAL_STIMER_CFG_COMPARE_A_ENABLE |
        AM_HAL_STIMER_CFG_RUN);

    // trigger source for millisecond counter
    am_hal_ctimer_clear(
        SYSTEM_TIMER_SRC_MS_NUM,
        SYSTEM_TIMER_SRC_MS_SEG);
    
    am_hal_ctimer_config_single(
        SYSTEM_TIMER_SRC_MS_NUM,
        SYSTEM_TIMER_SRC_MS_SEG,
        (AM_HAL_CTIMER_FN_REPEAT |
         AM_HAL_CTIMER_HFRC_12KHZ));
    
    am_hal_ctimer_period_set(
        SYSTEM_TIMER_SRC_MS_NUM,
        SYSTEM_TIMER_SRC_MS_SEG, 12, 1);
    
    am_hal_ctimer_start(
        SYSTEM_TIMER_SRC_MS_NUM,
        SYSTEM_TIMER_SRC_MS_SEG);

    // millisecond counter
    am_hal_ctimer_clear(
        SYSTEM_TIMER_COUNTER_MS_NUM,
        SYSTEM_TIMER_COUNTER_MS_SEG);
    
    am_hal_ctimer_config_single(
        SYSTEM_TIMER_COUNTER_MS_NUM,
        SYSTEM_TIMER_COUNTER_MS_SEG,
        (AM_HAL_CTIMER_FN_REPEAT |
         _VAL2FLD(CTIMER_CTRL0_TMRA0CLK, 0x16)));
    
    am_hal_ctimer_start(
        SYSTEM_TIMER_COUNTER_MS_NUM,
        SYSTEM_TIMER_COUNTER_MS_SEG);

    timermap_ct_assign(0, 0, 0xFF);
    timermap_ct_assign(1, 0, 0xFF);
    timermap_ct_assign(0, 1, 0xFF);
    timermap_ct_assign(1, 1, 0xFF);
    timermap_ct_assign(0, 3, 0xFF);
}

void init (void)
{
    //
    // Set the clock frequency.
    //
    am_hal_clkgen_control(AM_HAL_CLKGEN_CONTROL_SYSCLK_MAX, 0);
//    am_hal_clkgen_control(AM_HAL_CLKGEN_CONTROL_LFRC_START, 0);

    //
    // Set the default cache configuration
    //
    am_hal_cachectrl_config(&am_hal_cachectrl_defaults);
    am_hal_cachectrl_enable();

    am_hal_sysctrl_fpu_enable();
    am_hal_sysctrl_fpu_stacking_enable(true);

    am_hal_interrupt_master_enable();

    timermap_ct_init();
    system_timer_init();
}

/*
 * \brief Main entry point of Arduino application
 */
int main( void )
{
    // Initialize watchdog
    //watchdogSetup();

    init();
    initVariant();

#if defined(USBCON)
    //USBDevice.attach();
#endif

    setup();

    for (;;)
    {
        loop();
        if (serialEventRun) serialEventRun();
    }

    return 0;
}
