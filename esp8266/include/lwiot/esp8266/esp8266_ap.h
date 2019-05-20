/*
 * ESP8266 WiFi AP.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot/log.h>
#include <lwiot/types.h>

#include <lwiot/network/ipaddress.h>
#include <lwiot/network/wifiaccesspoint.h>

namespace lwiot
{
	namespace esp8266
	{
		class WifiAccessPoint : public lwiot::WifiAccessPoint {
		public:
			static WifiAccessPoint& instance()
			{
				static WifiAccessPoint ap;
				return ap;
			}

			void start() override;

			void begin(const String& ssid, const String& pass, int chan, bool hidden, int max) override;
			void config(const IPAddress& local, const IPAddress& gw, const IPAddress& subnet) override;
			explicit operator bool() const override;
			void stop() override;
			String mac() const override;

		private:
			explicit WifiAccessPoint();
			virtual ~WifiAccessPoint() = default;
		};
	}
}
