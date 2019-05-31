/*
 * GPIO strategy for the ESP8266.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot.h>
#include <lwiot/io/gpiochip.h>

#include <driver/gpio.h>

#ifdef PINS
#undef PINS
#endif

#define PINS 16

#ifdef CXX
namespace lwiot { namespace esp8266
{
	class GpioChip : public lwiot::GpioChip {
	public:
		explicit GpioChip();
		virtual ~GpioChip() = default;

		void mode(int pin, const PinMode& mode) override;
		void write(int pin, bool value) override;
		bool read(int pin) const override;

		void setOpenDrain(int pin) override;
		void odWrite(int pin, bool value) override;
		void attachIrqHandler(int pin, irq_handler_t handler, IrqEdge edge) override;
		void detachIrqHandler(int pin) override ;

	private:
		void mode16(const PinMode& mode);
		void setPullup(int pin, bool enabled);
		gpio_int_type_t mapIrqEdge(const IrqEdge& edge) const;
	};
}
}
#endif
