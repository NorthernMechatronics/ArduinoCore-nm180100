#ifndef UART_H
#define UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_mcu_apollo.h"

#ifdef __cplusplus
}
#endif

#include "HardwareSerial.h"

#define UART_BUFFER_SIZE 1024

class Uart : public HardwareSerial
{
public:
	Uart();

public:
	void begin(unsigned long);
	void begin(unsigned long baudrate, uint16_t config);
	void end(void);
	int available(void);
	int peek(void);
	int read(void);
	void flush(void);
	size_t write(uint8_t);
	size_t write(const uint8_t* buffer, size_t size);
	operator bool(void)
	{
		return true;
	}

private:
	void uart_init(void);
	
	uint8_t tx_buffer[UART_BUFFER_SIZE];
	uint8_t rx_buffer[UART_BUFFER_SIZE];
	
	am_hal_uart_config_t uart_config;

	void* uart_handle;
};

#endif