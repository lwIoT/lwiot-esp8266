/*
 * SoC system interface class definition.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <esp_sleep.h>
#include <esp_system.h>

#include <lwiot/lwiot.h>
#include <lwiot/types.h>
#include <lwiot/log.h>
#include <lwiot/esp8266/esp8266system.h>

namespace lwiot { namespace esp8266
{
	System::System(SleepState state) : lwiot::System(state)
	{ }

	void System::sleep(const uint32_t& ms) const
	{
		uint32_t us;

		switch(this->sleepState()) {
		case SLEEP_DEEP:
		default:
			us = ms * 1000UL;
			esp_deep_sleep(us);
			break;
		}
	}

	void System::restart() const
	{
		esp_restart();
	}
}
}
