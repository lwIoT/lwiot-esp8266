/*
 * ESP32 device test.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <esp_attr.h>
#include <esp_heap_caps.h>
#include <lwiot.h>

#include <lwiot/kernel/thread.h>
#include <lwiot/log.h>
#include <lwiot/stl/string.h>
#include <lwiot/io/watchdog.h>
#include <lwiot/util/application.h>

#include <lwiot/network/socketudpserver.h>
#include <lwiot/network/udpserver.h>
#include <lwiot/network/wifiaccesspoint.h>
#include <lwiot/network/captiveportal.h>
#include <lwiot/network/ipaddress.h>

#include <lwiot/esp8266/esp8266adcchip.h>
#include <lwiot/esp8266/esp8266watchdog.h>
#include <lwiot/esp8266/esp8266_ap.h>

class App : public lwiot::Functor {
protected:

	void startAP(const lwiot::String& ssid, const lwiot::String& passw)
	{
		auto& ap = lwiot::esp8266::WifiAccessPoint::instance();
		lwiot::IPAddress local(192, 168, 1, 1);
		lwiot::IPAddress subnet(255, 255, 255, 0);
		lwiot::IPAddress gw(192, 168, 1, 1);

		ap.start();
		ap.config(local, gw, subnet);
		ap.begin(ssid, passw, 4, false, 4);
	}

	void run() override
	{
		lwiot_sleep(1000);
		print_dbg("Creating AP...\n");
		this->startAP("esp8266", "testAP1234");

		while(true) {
			printf("[%li] PING!\n", lwiot_tick_ms());
			lwiot_sleep(1000);
		}
	}
};

extern "C" void main_start(void)
{
	App runner;

	lwiot::Application app(runner);
	app.start();
}
