PROJECT_ROOT := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

BUILD_MODE = debug
BUILD_DIR = build

PROJECT = RTOS_template

DEVICE = stm32f401ret6
OPENCM3_DEFS = -DSTM32F4
OPENCM3_DIR = $(PROJECT_ROOT)/external/libopencm3
OPENCM3_LIB = opencm3_stm32f4
OOCD_FILE = board/stm32f401ret6.cfg 

PORT_NAME = GCC/ARM_CM4F
FREERTOS_DIR = external/FreeRTOS
FREERTOS_SRC_DIR = $(FREERTOS_DIR)/Source
FREERTOS_INCLUDES=-I$(FREERTOS_SRC_DIR)/include -I$(FREERTOS_SRC_DIR)/portable/$(PORT_NAME)

FREERTOS_SRC = $(FREERTOS_SRC_DIR)/tasks.c $(FREERTOS_SRC_DIR)/queue.c $(FREERTOS_SRC_DIR)/list.c \
				$(FREERTOS_SRC_DIR)/timers.c $(FREERTOS_SRC_DIR)/event_groups.c $(FREERTOS_SRC_DIR)/portable/$(PORT_NAME)/port.c \
				$(FREERTOS_SRC_DIR)/portable/MemMang/heap_1.c
				


CFILES = src/main.c $(FREERTOS_SRC) src/usbd.c src/opencm3.c
INCLUDES = -I$(PROJECT_ROOT)/include $(FREERTOS_INCLUDES)
LDSCRIPT = $(PROJECT_ROOT)/generated.stm32f401ret6.ld 

include $(OPENCM3_DIR)/mk/genlink-config.mk
include rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk



