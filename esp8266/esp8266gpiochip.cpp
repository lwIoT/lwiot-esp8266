/*
 * GPIO strategy for the ESP8266.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>
#include <esp_attr.h>
#include <math.h>

#include <lwiot/io/gpiochip.h>
#include <lwiot/log.h>
#include <lwiot/esp8266/esp8266gpiochip.h>

#include <driver/gpio.h>
#include <rom/ets_sys.h>
#include <sys/reent.h>

#include "esp8266gpio.h"

#ifdef PINS
#undef PINS
#endif

#define PINS 16

#ifndef HIGH
#define HIGH 1
#define LOW  0
#endif

static irq_handler_t gpio_isr_handlers[PINS] = {nullptr};
static gpio_config_t configs[PINS];

static void IRAM_ATTR gpio_external_isr(void *arg)
{
	int bits;
	auto config = static_cast<gpio_config_t*>(arg);

	bits = static_cast<int>(config->pin_bit_mask);
	auto pin = int(log2(bits & -bits) + 1);
	irq_handler_t handler = gpio_isr_handlers[pin];

	handler();
}

namespace lwiot { namespace esp8266
{
	GpioChip::GpioChip() : lwiot::GpioChip(PINS)
	{ }

	void GpioChip::mode(int pin, const PinMode& mode)
	{
		auto _pin = (gpio_num_t)pin;

		switch(mode) {
		case INPUT_PULLUP:
			gpio_set_direction(_pin, GPIO_MODE_INPUT);
			gpio_pullup_en(_pin);
			break;

		case INPUT_NOPULLUP:
			gpio_set_direction(_pin, GPIO_MODE_INPUT);
			gpio_pullup_dis(_pin);
			break;

		case INPUT:
			gpio_set_direction(_pin, GPIO_MODE_INPUT);
			gpio_set_pull_mode(_pin, GPIO_FLOATING);
			break;

		case OUTPUT:
			gpio_set_direction(_pin, GPIO_MODE_OUTPUT);
			gpio_pullup_dis(_pin);
			gpio_pulldown_dis(_pin);
			break;

		case OUTPUT_OPEN_DRAIN:
			gpio_set_direction(_pin, GPIO_MODE_OUTPUT_OD);
			gpio_set_pull_mode(_pin, GPIO_FLOATING);
			break;

		default:
			print_dbg("Attempting to set invalid GPIO mode!\n");
			break;
		}
	}

	void IRAM_ATTR GpioChip::write(int pin, bool value)
	{
		uint32_t level = value ? HIGH : LOW;
		gpio_set_level((gpio_num_t)pin, level);
	}

	bool IRAM_ATTR GpioChip::read(int pin) const
	{
		return gpio_get_level((gpio_num_t)pin) == 1U;
	}

	void GpioChip::setOpenDrain(int pin)
	{
		this->mode(pin, OUTPUT_OPEN_DRAIN);
		this->write(pin, true);
	}

	void IRAM_ATTR GpioChip::odWrite(int pin, bool value)
	{
		uint32_t level = value ? HIGH : LOW;
		gpio_set_level((gpio_num_t)pin, level);
	}

	void GpioChip::attachIrqHandler(int pin, irq_handler_t handler, IrqEdge edge)
	{
		static bool interrupt_initialized = false;
		gpio_config_t *config = &configs[pin];

		if(!interrupt_initialized) {
			gpio_install_isr_service(0);
			interrupt_initialized = true;
		}

		if(config->intr_type != GPIO_INTR_DISABLE)
			gpio_isr_handler_remove(static_cast<gpio_num_t>(pin));

		config->intr_type = this->mapIrqEdge(edge);
		config->mode = GPIO_MODE_INPUT;
		config->pin_bit_mask = 1UL << pin;
		config->pull_down_en = GPIO_PULLDOWN_DISABLE;
		config->pull_up_en = GPIO_PULLUP_DISABLE;
		gpio_config(config);

		gpio_isr_handlers[pin] = handler;
		gpio_isr_handler_add(static_cast<gpio_num_t>(pin), gpio_external_isr, config);
	}

	gpio_int_type_t GpioChip::mapIrqEdge(const lwiot::IrqEdge &edge) const
	{
		switch(edge) {
		case IrqRising:
			return GPIO_INTR_POSEDGE;

		case IrqFalling:
			return GPIO_INTR_NEGEDGE;

		case IrqRisingFalling:
			return GPIO_INTR_ANYEDGE;

		default:
		case IrqNone:
			return GPIO_INTR_DISABLE;
		}
	}
}

extern "C" void pinMode(int pin, int mode)
{
	switch(mode) {
	default:
	case RAW_INPUT:
		gpio.mode(pin, lwiot::PinMode::INPUT);
		break;

	case RAW_OUTPUT:
		gpio.mode(pin, lwiot::PinMode::OUTPUT);
		break;

	case RAW_OUTPUT_OPENDRAIN:
		gpio.mode(pin, lwiot::PinMode::OUTPUT_OPEN_DRAIN);
		break;
	}
}
}

static lwiot::esp8266::GpioChip esp_gpio;
lwiot::GpioChip& gpio = esp_gpio;
