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
#ifdef __cplusplus
extern "C" {
#endif

#include "am_mcu_apollo.h"

#ifdef __cplusplus
}
#endif

void *uart_handle[AM_REG_UART_NUM_MODULES] = { 0 };

void am_gpio_isr(void)
{
	uint64_t status;
	am_hal_gpio_interrupt_status_get(true, &status);
	am_hal_gpio_interrupt_clear(status);
	am_hal_gpio_interrupt_service(status);
}

void interrupts()
{
	am_hal_interrupt_master_enable();
}

void noInterrupts()
{
	am_hal_interrupt_master_disable();
}

void am_uart_isr(void)
{
    uint32_t ui32Status;
    uint32_t ui32Idle;

    if (uart_handle[0])
    {
        am_hal_uart_interrupt_status_get(uart_handle[0], &ui32Status, true);
        am_hal_uart_interrupt_clear(uart_handle[0], ui32Status);
        am_hal_uart_interrupt_service(uart_handle[0], ui32Status, &ui32Idle);
    }
}

void am_uart1_isr(void)
{
    uint32_t ui32Status;
    uint32_t ui32Idle;

    if (uart_handle[1])
    {
        am_hal_uart_interrupt_status_get(uart_handle[1], &ui32Status, true);
        am_hal_uart_interrupt_clear(uart_handle[1], ui32Status);
        am_hal_uart_interrupt_service(uart_handle[1], ui32Status, &ui32Idle);
    }
}
