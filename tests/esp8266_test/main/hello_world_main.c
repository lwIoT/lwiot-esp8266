/*
 * Basic ESP8266 test application.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"


extern void main_start();
extern void lwiot_init();

void app_main()
{
	printf("Application booted!\n");
	main_start();
}
