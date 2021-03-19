#include "Uart.h"

static uint8_t tx_buffer[UART_BUFFER_SIZE];
static uint8_t rx_buffer[UART_BUFFER_SIZE];

static am_hal_uart_config_t uart_config;

static void* uart_handle;

Uart::Uart()
{

}

void Uart::begin(unsigned long)
{
    uart_init();
}

void Uart::begin(unsigned long, uint16_t config)
{
    uart_init();
}

void Uart::uart_init(void)
{
	const am_hal_gpio_pincfg_t tx = 
    {
    .uFuncSel            = AM_HAL_PIN_22_UART0TX,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
    };

	const am_hal_gpio_pincfg_t rx = 
    {
    .uFuncSel            = AM_HAL_PIN_23_UART0RX,
    };

    //
// Standard UART settings: 115200-8-N-1
//
    uart_config.ui32BaudRate = 115200;
    uart_config.ui32DataBits = AM_HAL_UART_DATA_BITS_8;
    uart_config.ui32Parity = AM_HAL_UART_PARITY_NONE;
    uart_config.ui32StopBits = AM_HAL_UART_ONE_STOP_BIT;
    uart_config.ui32FlowControl = AM_HAL_UART_FLOW_CTRL_NONE;

    //
    // Set TX and RX FIFOs to interrupt at three-quarters full.
    //
    uart_config.ui32FifoLevels = (AM_HAL_UART_TX_FIFO_1_2 |
        AM_HAL_UART_RX_FIFO_1_2);

    //
    // This code will use the standard interrupt handling for UART TX, but
    // we will have a custom routine for UART RX.
    //
    uart_config.pui8TxBuffer = tx_buffer;
    uart_config.ui32TxBufferSize = sizeof(tx_buffer);
    uart_config.pui8RxBuffer = rx_buffer;
    uart_config.ui32RxBufferSize = sizeof(rx_buffer);

	am_hal_uart_initialize(0, &uart_handle);
	am_hal_uart_power_control(uart_handle, AM_HAL_SYSCTRL_WAKE, false);
	am_hal_uart_configure(uart_handle, &uart_config);

	am_hal_gpio_pinconfig(22, tx);
	am_hal_gpio_pinconfig(23, rx);
/*
    am_hal_uart_interrupt_enable(uart_handle, AM_HAL_UART_INT_RX);
    NVIC_EnableIRQ((IRQn_Type)(UART0_IRQn));
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
    int c = -1;
    uint32_t ui32BytesWritten = 0;

    const am_hal_uart_transfer_t sUartWrite =
    {
        .ui32Direction = AM_HAL_UART_READ,
        .pui8Data = (uint8_t *)&c,
        .ui32NumBytes = 1,
        .ui32TimeoutMs = 1000,
        .pui32BytesTransferred = &ui32BytesWritten,
    };

    am_hal_uart_transfer(uart_handle, &sUartWrite);

    if (ui32BytesWritten)
    {
	    return c;
    }

    return -1;
}

void Uart::flush(void)
{
	am_hal_uart_tx_flush(uart_handle);
}

size_t Uart::write(uint8_t c)
{
    uint32_t ui32BytesWritten = 0;

    const am_hal_uart_transfer_t sUartWrite =
    {
        .ui32Direction = AM_HAL_UART_WRITE,
        .pui8Data = &c,
        .ui32NumBytes = 1,
        .ui32TimeoutMs = AM_HAL_UART_WAIT_FOREVER,
        .pui32BytesTransferred = &ui32BytesWritten,
    };

    am_hal_uart_transfer(uart_handle, &sUartWrite);
	return ui32BytesWritten;
}

size_t Uart::write(const uint8_t* buffer, size_t size)
{
    uint32_t ui32BytesWritten = 0;
    const am_hal_uart_transfer_t sUartWrite =
    {
        .ui32Direction = AM_HAL_UART_WRITE,
        .pui8Data = (uint8_t *)buffer,
        .ui32NumBytes = size,
        .ui32TimeoutMs = AM_HAL_UART_WAIT_FOREVER,
        .pui32BytesTransferred = &ui32BytesWritten,
    };

    if (uart_handle)
    {
        am_hal_uart_transfer(uart_handle, &sUartWrite);
    }

    return ui32BytesWritten;
}

void am_uart_isr(void)
{
    uint32_t ui32Status, ui32Idle;
    am_hal_uart_interrupt_status_get(uart_handle, &ui32Status, true);
    am_hal_uart_interrupt_clear(uart_handle, ui32Status);
    am_hal_uart_interrupt_service(uart_handle, ui32Status, &ui32Idle);

    am_hal_gpio_state_write(17, AM_HAL_GPIO_OUTPUT_TOGGLE);
}