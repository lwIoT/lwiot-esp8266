#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := esp32-test

PORT_PATH = $(PWD)/../../esp8266/include
BUILD_DIR = $(PWD)/../../build/debug

EXTRA_CFLAGS = -I$(BUILD_DIR) -I$(LWIOT_PATH)/include -I$(PORT_PATH)
EXTRA_CXXFLAGS = $(EXTRA_CFLAGS) -std=gnu++14

IDF_PATH := $(ESP8266_IDF_PATH)
export IDF_PATH

include $(IDF_PATH)/make/project.mk

