/*
 * ESP8266 analog to digital converter.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>

#include <lwiot/lwiot.h>
#include <lwiot/io/adcchip.h>
#include <lwiot/esp8266/esp8266adcchip.h>

#include <driver/adc.h>


namespace lwiot { namespace esp8266
{
	AdcChip::AdcChip() : lwiot::AdcChip(1, 3300, 1024)
	{ }

	size_t AdcChip::read(int pin) const
	{
		UNUSED(pin);
		return adc_read();
	}

	void AdcChip::begin()
	{

	}
}
}

static lwiot::esp8266::AdcChip _espadc;
lwiot::AdcChip& adc = _espadc;
