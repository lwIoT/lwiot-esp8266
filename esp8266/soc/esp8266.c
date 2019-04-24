/*
 * ESP8266 SoC definitions.
 */

#include <stdlib.h>
#include <stdint.h>
#include <lwiot.h>
#include <esp_system.h>

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

#if UINT32_MAX == UINTPTR_MAX
#define STACK_CHK_GUARD 0xe2dee396
#else
#define STACK_CHK_GUARD 0x595e9fbd94fda766
#endif

uintptr_t __weak __stack_chk_guard = STACK_CHK_GUARD;
extern uint32_t **pxCurrentTCB;
extern void __attribute__((noreturn)) panicHandler(void *frame, int wdt);

static inline void* current_frame()
{
	return pxCurrentTCB[0];
}

void __weak __attribute__((noreturn)) __stack_chk_fail(void)
{
#ifdef HAVE_DEBUG
	printf("[%li] Stack protector failed!\n", lwiot_tick_ms());
	panicHandler(current_frame(), 0);
#else
	esp_restart();
	panicHandler(current_frame(), 0);
#endif
}
