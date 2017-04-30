# Gesture-V

**Description:** Gesture-V is a Hand Gesture Controlled Bluetooth "Vehicle".

The basis for this project is for a final project for the *SENG 5831: Software Development for Real-Time and Embedded Systems* course for my [Master's in Software Engineering from the University of Minnesota](https://www.msse.umn.edu).

## Hardware

The primary hardware platform for the vehicle is an Atmel AVR 8-bit microcontroller.

### Glove Hardware Components

* 4.5" Flex Sensor x 2
* 10k Ohm Resistor x 2
* BLE Nano
* LSM6DS33 (Accelerometer & Gyroscope)
* Battery Holder for 2xCR2032 Batteries
* Wires as needed to attach components as per wiring diagram *below*.

### Vehicle Hardware Components

* A* Microcontroller
* Motors x 2
* BLE Nano
* Ultrasonic Sensor
* Wires as needed to attach components as per wiring diagram *below*.

## Information Architecture View
![Image of Information Flow](https://raw.githubusercontent.com/bnordland/Gesture-V/master/Documentation/InformationFlow.png)

## Hardware Wiring
### Glove Hardware Wiring
![Image of Glove Hardware](https://github.com/bnordland/Gesture-V/raw/master/Documentation/GloveHardware.png)

### Vehicle Hardware Wiring
![Image of Vehicle Hardware](https://raw.githubusercontent.com/bnordland/Gesture-V/master/Documentation/VehicleHardware.png)