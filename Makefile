##############################################################################
# FILENAME: Makefile                                                         #
#                                                                            #
# DESCRIPTION: Makefile for Gesture-V. This calls the makefiles for the      #
#              various components. The components are:                       #
#                                                                            #
#                  1. Glove BLE (for BLE Nano)                               #
#                  2. Vehicle BLE (for BLE Nano Central Device)              #
#                  3. Vehicle AVR (For A* Device)                            #
#                                                                            #
# LICENSE: The MIT License (MIT)                                             #
#          Copyright (c) 2017 Brian Nordland                                 #
#                                                                            #
#  ------------------------------------------------------------------------  #
# | Change  | Date     |            |                                      | #
# | Flag    | (DDMYY)  | Author     | Description                          | #
# |---------|----------|------------|--------------------------------------  #
# | None    | 31Mar17  | BNordland  | Initial creation                     | #
#  ------------------------------------------------------------------------  #
##############################################################################

default:
    # 1. Make the Glove BLE module
	cd Glove/BLE && $(MAKE)
	# 2. Make the Vehicle BLE module
	cd Vehicle/BLE && $(MAKE)
	# 3. Make the Vehicle AVR module
	cd Vehicle/AVR && $(MAKE)
	
clean:
	# 1. Clean the Glove BLE module
	cd Glove/BLE && $(MAKE) clean
	# 2. Clean the Vehicle BLE module
	cd Vehicle/BLE && $(MAKE) clean
	# 3. Clean the Vehicle AV module
	cd Vehicle/AVR && $(MAKE) clean

program:
	# Make program only valid for A*
	cd Vehicle/AVR && $(MAKE) program

flashG:
	# Flash the glove
	cd Glove/BLE && $(MAKE) flash