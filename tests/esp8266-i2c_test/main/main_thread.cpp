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
#include <lwiot/kernel/functionalthread.h>

#include <lwiot/log.h>
#include <lwiot/io/gpiochip.h>
#include <lwiot/io/gpiopin.h>
#include <lwiot/io/watchdog.h>
#include <lwiot/io/i2cbus.h>
#include <lwiot/io/hardwarei2calgorithm.h>
#include <lwiot/io/gpioi2calgorithm.h>
#include <lwiot/io/i2cmessage.h>

#include <lwiot/util/datetime.h>
#include <lwiot/util/application.h>

#include <lwiot/esp8266/hardwarei2calgorithm.h>

#include <lwiot/stl/vector.h>
#include <lwiot/stl/string.h>

class App : public lwiot::Functor {
protected:
	void run() override
	{
		lwiot::GpioPin scl(2), sda(14);
		auto algo = new lwiot::esp8266::HardwareI2CAlgorithm(scl, sda, 400000);
		//auto algo = new lwiot::GpioI2CAlgorithm(sda, scl, 100000);
		lwiot::I2CBus bus(algo);

		wdt.enable(3000);

		while(true) {
			wdt.reset();
			testSingle(bus);
			testRead(bus);
		}
	}
	
	void testSingle(lwiot::I2CBus& bus)
	{
		lwiot::I2CMessage wr(1);

		wr.setAddress(0x6B, false, false);
		wr.write(1);
		wr.setRepeatedStart(false);

		if(bus.transfer(wr)) {
			print_dbg("Single test successfull!\n");
		} else {
			print_dbg("Single test failed!\n");
		}

		lwiot_sleep(500);
	}

	void testRead(lwiot::I2CBus& bus)
	{
		lwiot::I2CMessage wr(1), rd(3);
		lwiot::stl::Vector<lwiot::I2CMessage*> msgs;

		wr.setAddress(0x6B, false, false);
		wr.write(1);
		wr.setRepeatedStart(true);

		rd.setAddress(0x6B, false, true);
		rd.setRepeatedStart( false);

		msgs.pushback(&wr);
		msgs.pushback(&rd);

		if(bus.transfer(msgs)) {
			print_dbg("Read test successfull!\n");
		} else {
			print_dbg("Read test failed!\n");
		}

		auto& msg = *msgs[1];
		print_dbg("Read data:\n");
		print_dbg("Read byte: %u\n", msg[0]);
		print_dbg("Read byte: %u\n", msg[1]);
		print_dbg("Read byte: %u\n", msg[2]);

		lwiot_sleep(500);
	}
};

extern "C" void main_start(void)
{
	App runner;

	lwiot::Application app(runner);
	app.start();
}
