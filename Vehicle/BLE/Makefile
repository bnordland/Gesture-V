##############################################################################
# FILENAME: Makefile                                                         #
#                                                                            #
# DESCRIPTION: Makefile for the A* Controller on the Vehicle                 #
#                                                                            #
# LICENSE: The MIT License (MIT)                                             #
#          Copyright (c) 2017 Brian Nordland                                 #
#                                                                            #
#  ------------------------------------------------------------------------  #
# | Change  | Date     |            |                                      | #
# | Flag    | (DDMYY)  | Author     | Description                          | #
# |---------|----------|------------|--------------------------------------  #
# | None    | 01Apr17  | BNordland  | Initial creation                     | #
# | None    | 16Apr17  | BNordland  | Adding Implementation                | #
#  ------------------------------------------------------------------------  #
##############################################################################

ifndef BUILD_BASE_PATH
$(error BUILD_BASE_PATH must be set to base path of build.)
endif

ifndef NRF5_SDK_PATH
$(error NRF5_SDK_PATH must be set to base path of NRF5_SDK.)
endif

ifndef NRF5_SOFTDEVICE_FILE
$(error NRF5_SOFTDEVICE_FILE must be set to the soft device file.)
endif

OUTPUT_DIRECTORY := $(BUILD_BASE_PATH)/Vehicle_BLE

TARGETS := nrf51422_xxac

$(OUTPUT_DIRECTORY)/nrf51422_xxac.out: \
  LINKER_SCRIPT  := ble_app_gcc_nrf51.ld
  
# Source files for our system
SRC_FILES += \
  main.c \
  Client/Client_Glove.c

# Include folders for our system
INC_FOLDERS += \
  . \
  Config \
  Client \

# Source files for NRF SDK
SRC_FILES += \
  $(NRF5_SDK_PATH)/components/libraries/log/src/nrf_log_backend_serial.c \
  $(NRF5_SDK_PATH)/components/libraries/log/src/nrf_log_backend_serial.c \
  $(NRF5_SDK_PATH)/components/libraries/log/src/nrf_log_frontend.c \
  $(NRF5_SDK_PATH)/components/libraries/button/app_button.c \
  $(NRF5_SDK_PATH)/components/libraries/util/app_error.c \
  $(NRF5_SDK_PATH)/components/libraries/util/app_error_weak.c \
  $(NRF5_SDK_PATH)/components/libraries/fifo/app_fifo.c \
  $(NRF5_SDK_PATH)/components/libraries/timer/app_timer.c \
  $(NRF5_SDK_PATH)/components/libraries/util/app_util_platform.c \
  $(NRF5_SDK_PATH)/components/libraries/hardfault/hardfault_implementation.c \
  $(NRF5_SDK_PATH)/components/libraries/util/nrf_assert.c \
  $(NRF5_SDK_PATH)/components/libraries/util/sdk_errors.c \
  $(NRF5_SDK_PATH)/components/boards/boards.c \
  $(NRF5_SDK_PATH)/components/drivers_nrf/clock/nrf_drv_clock.c \
  $(NRF5_SDK_PATH)/components/drivers_nrf/common/nrf_drv_common.c \
  $(NRF5_SDK_PATH)/components/drivers_nrf/gpiote/nrf_drv_gpiote.c \
  $(NRF5_SDK_PATH)/components/ble/common/ble_advdata.c \
  $(NRF5_SDK_PATH)/components/ble/common/ble_conn_params.c \
  $(NRF5_SDK_PATH)/components/ble/ble_db_discovery/ble_db_discovery.c \
  $(NRF5_SDK_PATH)/components/ble/common/ble_srv_common.c \
  $(NRF5_SDK_PATH)/components/toolchain/gcc/gcc_startup_nrf51.S \
  $(NRF5_SDK_PATH)/components/toolchain/system_nrf51.c \
  $(NRF5_SDK_PATH)/components/ble/ble_services/ble_nus_c/ble_nus_c.c \
  $(NRF5_SDK_PATH)/components/softdevice/common/softdevice_handler/softdevice_handler.c \
  $(NRF5_SDK_PATH)/components/drivers_nrf/spi_slave/nrf_drv_spis.c \

# Include folders common to all targets
INC_FOLDERS += \
  $(NRF5_SDK_PATH)/components/drivers_nrf/comp \
  $(NRF5_SDK_PATH)/components/drivers_nrf/twi_master \
  $(NRF5_SDK_PATH)/components/ble/ble_services/ble_ancs_c \
  $(NRF5_SDK_PATH)/components/ble/ble_services/ble_ias_c \
  $(NRF5_SDK_PATH)/components/softdevice/s130/headers \
  $(NRF5_SDK_PATH)/components/libraries/pwm \
  $(NRF5_SDK_PATH)/components/libraries/usbd/class/cdc/acm \
  $(NRF5_SDK_PATH)/components/libraries/usbd/class/hid/generic \
  $(NRF5_SDK_PATH)/components/libraries/usbd/class/msc \
  $(NRF5_SDK_PATH)/components/libraries/usbd/class/hid \
  $(NRF5_SDK_PATH)/components/libraries/log \
  $(NRF5_SDK_PATH)/components/ble/ble_services/ble_gls \
  $(NRF5_SDK_PATH)/components/libraries/fstorage \
  $(NRF5_SDK_PATH)/components/drivers_nrf/i2s \
  $(NRF5_SDK_PATH)/components/libraries/gpiote \
  $(NRF5_SDK_PATH)/components/drivers_nrf/gpiote \
  $(NRF5_SDK_PATH)/components/libraries/fifo \
  $(NRF5_SDK_PATH)/components/boards \
  $(NRF5_SDK_PATH)/components/drivers_nrf/common \
  $(NRF5_SDK_PATH)/components/ble/ble_advertising \
  $(NRF5_SDK_PATH)/components/drivers_nrf/adc \
  $(NRF5_SDK_PATH)/components/softdevice/s130/headers/nrf51 \
  $(NRF5_SDK_PATH)/components/ble/ble_services/ble_bas_c \
  $(NRF5_SDK_PATH)/components/ble/ble_services/ble_hrs_c \
  $(NRF5_SDK_PATH)/components/libraries/queue \
  $(NRF5_SDK_PATH)/components/ble/ble_dtm \
  $(NRF5_SDK_PATH)/components/toolchain/cmsis/include \
  $(NRF5_SDK_PATH)/components/ble/ble_services/ble_rscs_c \
  $(NRF5_SDK_PATH)/components/ble/common \
  $(NRF5_SDK_PATH)/components/ble/ble_services/ble_lls \
  $(NRF5_SDK_PATH)/components/drivers_nrf/wdt \
  $(NRF5_SDK_PATH)/components/ble/ble_db_discovery \
  $(NRF5_SDK_PATH)/components/ble/ble_services/ble_bas \
  $(NRF5_SDK_PATH)/components/libraries/experimental_section_vars \
  $(NRF5_SDK_PATH)/components/ble/ble_services/ble_ans_c \
  $(NRF5_SDK_PATH)/components/libraries/slip \
  $(NRF5_SDK_PATH)/components/libraries/mem_manager \
  $(NRF5_SDK_PATH)/components/libraries/usbd/class/cdc \
  $(NRF5_SDK_PATH)/components/drivers_nrf/hal \
  $(NRF5_SDK_PATH)/components/ble/ble_services/ble_nus_c \
  $(NRF5_SDK_PATH)/components/drivers_nrf/rtc \
  $(NRF5_SDK_PATH)/components/ble/ble_services/ble_ias \
  $(NRF5_SDK_PATH)/components/libraries/usbd/class/hid/mouse \
  $(NRF5_SDK_PATH)/components/drivers_nrf/ppi \
  $(NRF5_SDK_PATH)/components/ble/ble_services/ble_dfu \
  $(NRF5_SDK_PATH)/components/drivers_nrf/twis_slave \
  $(NRF5_SDK_PATH)/components \
  $(NRF5_SDK_PATH)/components/libraries/scheduler \
  $(NRF5_SDK_PATH)/components/ble/ble_services/ble_lbs \
  $(NRF5_SDK_PATH)/components/ble/ble_services/ble_hts \
  $(NRF5_SDK_PATH)/components/drivers_nrf/delay \
  $(NRF5_SDK_PATH)/components/libraries/crc16 \
  $(NRF5_SDK_PATH)/components/drivers_nrf/timer \
  $(NRF5_SDK_PATH)/components/libraries/util \
  $(NRF5_SDK_PATH)/components/drivers_nrf/pwm \
  $(NRF5_SDK_PATH)/components/libraries/csense_drv \
  $(NRF5_SDK_PATH)/components/libraries/csense \
  $(NRF5_SDK_PATH)/components/drivers_nrf/rng \
  $(NRF5_SDK_PATH)/components/libraries/low_power_pwm \
  $(NRF5_SDK_PATH)/components/libraries/hardfault \
  $(NRF5_SDK_PATH)/components/ble/ble_services/ble_cscs \
  $(NRF5_SDK_PATH)/components/libraries/hci \
  $(NRF5_SDK_PATH)/components/libraries/usbd/class/hid/kbd \
  $(NRF5_SDK_PATH)/components/drivers_nrf/spi_slave \
  $(NRF5_SDK_PATH)/components/drivers_nrf/lpcomp \
  $(NRF5_SDK_PATH)/components/libraries/timer \
  $(NRF5_SDK_PATH)/components/drivers_nrf/power \
  $(NRF5_SDK_PATH)/components/libraries/usbd/config \
  $(NRF5_SDK_PATH)/components/toolchain \
  $(NRF5_SDK_PATH)/components/libraries/led_softblink \
  $(NRF5_SDK_PATH)/components/drivers_nrf/qdec \
  $(NRF5_SDK_PATH)/components/ble/ble_services/ble_cts_c \
  $(NRF5_SDK_PATH)/components/drivers_nrf/spi_master \
  $(NRF5_SDK_PATH)/components/ble/ble_services/ble_nus \
  $(NRF5_SDK_PATH)/components/ble/ble_services/ble_hids \
  $(NRF5_SDK_PATH)/components/drivers_nrf/pdm \
  $(NRF5_SDK_PATH)/components/libraries/crc32 \
  $(NRF5_SDK_PATH)/components/libraries/usbd/class/audio \
  $(NRF5_SDK_PATH)/components/ble/peer_manager \
  $(NRF5_SDK_PATH)/components/drivers_nrf/swi \
  $(NRF5_SDK_PATH)/components/ble/ble_services/ble_tps \
  $(NRF5_SDK_PATH)/components/ble/ble_services/ble_dis \
  $(NRF5_SDK_PATH)/components/device \
  $(NRF5_SDK_PATH)/components/ble/nrf_ble_qwr \
  $(NRF5_SDK_PATH)/components/libraries/button \
  $(NRF5_SDK_PATH)/components/libraries/usbd \
  $(NRF5_SDK_PATH)/components/drivers_nrf/saadc \
  $(NRF5_SDK_PATH)/components/ble/ble_services/ble_lbs_c \
  $(NRF5_SDK_PATH)/components/ble/ble_racp \
  $(NRF5_SDK_PATH)/components/toolchain/gcc \
  $(NRF5_SDK_PATH)/components/libraries/fds \
  $(NRF5_SDK_PATH)/components/libraries/twi \
  $(NRF5_SDK_PATH)/components/drivers_nrf/clock \
  $(NRF5_SDK_PATH)/components/ble/ble_services/ble_rscs \
  $(NRF5_SDK_PATH)/components/drivers_nrf/usbd \
  $(NRF5_SDK_PATH)/components/softdevice/common/softdevice_handler \
  $(NRF5_SDK_PATH)/components/ble/ble_services/ble_hrs \
  $(NRF5_SDK_PATH)/components/libraries/log/src \
  
# Libraries common to all targets
LIB_FILES += \
 
# C flags common to all targets
CFLAGS += -DBOARD_PCA10028
CFLAGS += -DSOFTDEVICE_PRESENT
CFLAGS += -DSWI_DISABLE0
CFLAGS += -D__HEAP_SIZE=0
CFLAGS += -DNRF51
CFLAGS += -DS130
CFLAGS += -DBLE_STACK_SUPPORT_REQD
CFLAGS += -DNRF51422
CFLAGS += -DNRF_SD_BLE_API_VERSION=2
CFLAGS += -mcpu=cortex-m0
CFLAGS += -mthumb -mabi=aapcs
CFLAGS +=  -Wall -Werror -O3 -g3
CFLAGS += -mfloat-abi=soft
# keep every function in separate section, this allows linker to discard unused ones
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin --short-enums 

# C++ flags common to all targets
CXXFLAGS += \

# Assembler flags common to all targets
ASMFLAGS += -x assembler-with-cpp
ASMFLAGS += -DBOARD_PCA10028
ASMFLAGS += -DSOFTDEVICE_PRESENT
ASMFLAGS += -DSWI_DISABLE0
ASMFLAGS += -D__HEAP_SIZE=0
ASMFLAGS += -DNRF51
ASMFLAGS += -DS130
ASMFLAGS += -DBLE_STACK_SUPPORT_REQD
ASMFLAGS += -DNRF51422
ASMFLAGS += -DNRF_SD_BLE_API_VERSION=2

# Linker flags
LDFLAGS += -mthumb -mabi=aapcs -L $(TEMPLATE_PATH) -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m0
# let linker to dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs -lc -lnosys


.PHONY: $(TARGETS) default all clean help

# Default target - first one defined
default: hex

# Print all targets that can be built
help:
	@echo following targets are available:
	@echo 	nrf51422_xxac

TEMPLATE_PATH := $(NRF5_SDK_PATH)/components/toolchain/gcc

include $(TEMPLATE_PATH)/Makefile.common

$(foreach target, $(TARGETS), $(call define_target, $(target)))

hex: nrf51422_xxac
	rm -rf 	$(BUILD_BASE_PATH)/Vehicle_BLE/*.o
	rm -rf  $(BUILD_BASE_PATH)/Vehicle_BLE/*.d
	rm -rf  $(BUILD_BASE_PATH)/Vehicle_BLE/*.in
	rm -rf  $(BUILD_BASE_PATH)/Vehicle_BLE/*.map
	rm -rf  $(BUILD_BASE_PATH)/Vehicle_BLE/*.out
	rm -rf  $(BUILD_BASE_PATH)/Vehicle_BLE/*.bin
	srec_cat $(NRF5_SOFTDEVICE_FILE) -intel $(BUILD_BASE_PATH)/Vehicle_BLE/nrf51422_xxac.hex -intel -o $(BUILD_BASE_PATH)/Vehicle_BLE/Vehicleble.hex -intel -line-length=44
	rm -rf  $(BUILD_BASE_PATH)/Vehicle_BLE/nrf51422_xxac.hex

flash:
	cp $(BUILD_BASE_PATH)/Vehicle_BLE/Vehicleble.hex /Volumes/DAPLINK