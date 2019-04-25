/*
 * ESP8266 watchdog timer definition.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

extern "C" {
#include <esp_task_wdt.h>
}

#include <lwiot/io/watchdog.h>
#include <lwiot/esp8266/esp8266watchdog.h>

namespace lwiot { namespace esp8266
{
	Watchdog::Watchdog() : lwiot::Watchdog()
	{ }

	bool Watchdog::enable(uint32_t tmo)
	{
		lwiot::Watchdog::enable(tmo);
		return esp_task_wdt_init() == -ESP_OK;
	}

	bool Watchdog::disable()
	{
		return false;
	}

	void Watchdog::reset()
	{
		if(!this->enabled())
			return;

		esp_task_wdt_reset();
	}
}
}

lwiot::Watchdog& wdt = lwiot::esp8266::Watchdog::instance();
