
include(${PROJECT_SOURCE_DIR}/freertos/freertos.cmake)

SET(PORT_SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/esp8266/esp8266gpiochip.cpp
    ${PROJECT_SOURCE_DIR}/esp8266/esp8266adcchip.cpp
    ${PROJECT_SOURCE_DIR}/esp8266/esp8266uart.cpp
    ${PROJECT_SOURCE_DIR}/esp8266/esp8266system.cpp
    ${PROJECT_SOURCE_DIR}/esp8266/esp8266watchdog.cpp
    ${PROJECT_SOURCE_DIR}/esp8266/esp8266_station.cpp
	${PROJECT_SOURCE_DIR}/esp8266/esp8266_ap.cpp

	${PROJECT_SOURCE_DIR}/esp8266/soc/wifi.c
	${PROJECT_SOURCE_DIR}/esp8266/soc/esp8266.c
	${PROJECT_SOURCE_DIR}/lwip/lwip.c
	${PROJECT_SOURCE_DIR}/mbedtls/mbedtls.c
	${FREERTOS_SOURCE_FILES}
)

add_library(lwiot-platform ${PORT_SOURCE_FILES})

INSTALL(TARGETS lwiot-platform
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/esp8266/include/ DESTINATION include)

target_include_directories(lwiot-platform PUBLIC ${PROJECT_SOURCE_DIR}/lwiot-core/include
	${CMAKE_BINARY_DIR} ${LWIOT_CORE_INCLUDE_DIRECTORIES}
	${PORT_INCLUDE_DIR})
