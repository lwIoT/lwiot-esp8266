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
#include <lwiot/network/wifistation.h>

namespace lwiot
{
	namespace esp8266
	{
		class WifiStation : public lwiot::WifiStation {
		public:
			static WifiStation& instance()
			{
				static WifiStation sta;
				return sta;
			}

			WifiStation(const WifiStation&) = delete;
			void operator =(const WifiStation&) = delete;

			void connectTo(const String& ssid) override;
			void connectTo(const String& ssid, const String& password) override;
			void disconnect() override;

			explicit operator bool () const override;

		private:
			explicit WifiStation();
			virtual ~WifiStation() = default;
		};
	}
}
