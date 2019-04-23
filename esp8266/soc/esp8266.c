/*
 * ESP8266 SoC definitions.
 */

#include <stdlib.h>
#include <lwiot.h>

#include <FreeRTOS.h>
#include <task.h>

#include <rom/ets_sys.h>

void RAM_ATTR lwiot_udelay(uint32_t delay)
{
	os_delay_us((uint16_t)delay);
}

void enter_critical()
{
	taskENTER_CRITICAL();
}

void exit_critical()
{
	taskEXIT_CRITICAL();
}
