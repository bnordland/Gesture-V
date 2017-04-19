##############################################################################
# FILENAME: Makefile                                                         #
#                                                                            #
# DESCRIPTION: Makefile for the Vehicle Bluetooth Low Energy Device on the   #
#              vehicle.                                                      #
#                                                                            #
# LICENSE: The MIT License (MIT)                                             #
#          Copyright (c) 2017 Brian Nordland                                 #
#                                                                            #
#  ------------------------------------------------------------------------  #
# | Change  | Date     |            |                                      | #
# | Flag    | (DDMYY)  | Author     | Description                          | #
# |---------|----------|------------|--------------------------------------  #
# | None    | 01Apr17  | BNordland  | Initial creation                     | #
#  ------------------------------------------------------------------------  #
##############################################################################

PORT=/dev/tty.usbmodemFD131

ifndef BUILD_BASE_PATH
$(error BUILD_BASE_PATH must be set to base path of build.)
endif

ifndef VIRTUAL_SERIAL_PATH
$(warning VIRTUAL_SERIAL_PATH must be set to use usb virtual serial - setting to makefilepath)
VIRTUAL_SERIAL_PATH=/Users/nordland/Documents/MSSE/Spring2017/Embedded/lib/VirtualSerial/
else
CFLAGS+= -I$(VIRTUAL_SERIAL_PATH) -DVIRTUAL_SERIAL
LDFLAGS+=  -L$(VIRTUAL_SERIAL_PATH) -lVirtualSerial
endif

ifneq ($(filter program,$(MAKECMDGOALS)),$())
ifndef PORT
$(error PORT must be set to program device)
endif
endif

MCU=atmega32u4
CFLAGS+=-g -Wall -mcall-prologues -mmcu=$(MCU) -Os
LDFLAGS+=-Wl,-gc-sections -Wl,-relax
CC=avr-gcc
TARGET=main
LIB_FILES = $(wildcard lib/*.c)
OBJECT_FILES=main.o $(addprefix lib/,$(notdir $(LIB_FILES:.c=.o)))

OUTPUT_DIRECTORY := $(BUILD_BASE_PATH)/Vehicle_AVR

all: $(TARGET).hex

clean:  ;
	rm -f *.o *.hex *.obj *.hex
	rm -rf $(OUTPUT_DIRECTORY)

%.hex: %.obj
	mkdir -p $(OUTPUT_DIRECTORY)
	avr-objcopy -R .eeprom -O ihex $< $(OUTPUT_DIRECTORY)/$@

%.obj: $(OBJECT_FILES)
	$(CC) $(CFLAGS) $(OBJECT_FILES) $(LDFLAGS) -o $@

program: $(TARGET).hex
	avrdude -p $(MCU) -c avr109 -P $(PORT) -U flash:w:$(OUTPUT_DIRECTORY)/$(TARGET).hex