# Gesture-V

**Description:** Gesture-V is a Hand Gesture Controlled Bluetooth "Vehicle".

The basis for this project is for a final project for the *SENG 5831: Software Development for Real-Time and Embedded Systems* course for my [Master's in Software Engineering from the University of Minnesota](https://www.msse.umn.edu).

## Hardware

The primary hardware platform for the vehicle is a Pololu A-Star 32U4 Robot Controller (8-bit AVR Microcontroller) with a BLE Nano (ARM Based) for bluetooth communication to the glove which uses a BLE Nano (ARM based).

### Glove Hardware Components

| Quantity | Description          |
| :-------:|:---------------------|
| 1  | Glove |
| 1  | BLE Nano ([Redbear Lab](http://redbearlab.com/blenano)|
| 1  | SM6DS33 Accelerometer & Gyroscope ([Pololu Product #2736](https://www.pololu.com/product/2736))|
| 2  | 10k Ohm Resistors |
| 2  | 4.5" Flex Sensors ([Sparkfun Product #8606](https://www.sparkfun.com/products/8606)) |
| 1  | Battery Holder for 2x CR2032 batteries |
| 2  | CR2032 Batteries |
| Multiple | Wires, as required to connect components as per *glove hardware wiring diagram below* |

### Vehicle Hardware Components

| Quantity | Description          |
| :-------:|:---------------------|
| 1  | Pololu A-Star 32U4 Robot Controller LV ([Pololu Product #3117](https://www.pololu.com/product/3117)) |
| 1  | BLE Nano ([Redbear Lab](http://redbearlab.com/blenano) |
| 1  | Acrylic Sheet 12x12 (Vehicle Frame) |
| 2  | 25Dx52L (47:1) 6V Gearmotor with 48 CPR Encoder ([Pololu Product #2676](https://www.pololu.com/product/2676)|
| 1  | 25D Metal Gearmotor Bracket Pair ([Pololu Product #2676](https://www.pololu.com/product/2676))|
| 2  | Wheels |
| 1  | Swivel caster wheel ([Adafruit Product #2942](https://www.adafruit.com/product/2942))|
| 1  | Ultrasonic Sensor HC-SR04 ([Sparkfun Product #13959](https://www.sparkfun.com/products/13959)) |
| 1  | Power source capable of powering A\*, BLE Nano, and the Motors |
| Multiple | Wires, as required to connect components as per *vehicle hardware wiring diagram below* |
| Multiple | Screws and Nuts as required to connect components |

## Information Architecture/Flow View
![Image of Information Flow](https://raw.githubusercontent.com/bnordland/Gesture-V/master/Documentation/InformationFlow.png)

## Hardware Wiring
### Glove Hardware Wiring
![Image of Glove Hardware](https://github.com/bnordland/Gesture-V/raw/master/Documentation/GloveHardware.png)

### Vehicle Hardware Wiring
![Image of Vehicle Hardware](https://raw.githubusercontent.com/bnordland/Gesture-V/master/Documentation/VehicleHardware.png)