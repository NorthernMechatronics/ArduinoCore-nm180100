#include "Uart.h"

void Uart::begin(unsigned long)
{
	init();
}

void Uart::begin(unsigned long, uint16_t config)
{
	init();
}

void Uart::init(void)
{
	am_hal_gpio_pincfg_t tx, rx;
	
	tx.uFuncSel = AM_HAL_PIN_22_UART0TX;
	tx.eDriveStrength = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA;

	rx.uFuncSel = AM_HAL_PIN_23_UART0RX;

	am_hal_gpio_pinconfig(22, tx);
	am_hal_gpio_pinconfig(23, rx);
	/*
	am_hal_uart_initialize(UART0, &uart_handle);
	am_hal_uart_power_control(uart_handle, AM_HAL_SYSCTRL_WAKE, false);
	am_hal_uart_configure(uart_handle, &uart_config);
	*/
}

void Uart::end(void)
{
}

int Uart::available(void)
{
	return 0;
}

int Uart::peek(void)
{
	return 0;
}

int Uart::read(void)
{
	return 0;
}

void Uart::flush(void)
{
	am_hal_uart_tx_flush(uart_handle);
}

size_t Uart::write(uint8_t)
{
	return 0;
}

