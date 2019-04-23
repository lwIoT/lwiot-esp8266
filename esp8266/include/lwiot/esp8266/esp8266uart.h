/*
 * ESP8266 UART implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <freertos/FreeRTOS.h>

#include <lwiot/io/uart.h>
#include <lwiot/stream.h>

#include <driver/uart.h>

namespace lwiot { namespace esp8266
{
	class Uart : public lwiot::Uart {
	public:
		explicit Uart(int num, int rx, int tx, long baud, uint32_t config = SERIAL_8N1);
		virtual ~Uart();

		virtual uint8_t read();
		virtual ssize_t read(uint8_t *buffer, const size_t& length) ;
		using Stream::read;

		bool write(uint8_t byte) override;
		ssize_t write(const void *buffer, const size_t& length) override;
		using Stream::write;

		virtual size_t available() const override;

	private:
		uart_port_t _uart_num;
		bool initialized;

		uart_config_t _setup;
	};
}
}
