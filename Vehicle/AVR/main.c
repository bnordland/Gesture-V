/************************************************************************
* FILENAME: main.c                                                      *
*                                                                       *
* DESCRIPTION: Gesture-V main                                           *
*              This is the entry point into the program and contains    *
*              the necessary functionality of Gesture-V A* Controller   *
*                                                                       *
* LICENSE: The MIT License (MIT)                                        *
*          Copyright (c) 2017 Brian Nordland                            *
*                                                                       *
* AUTHOR:  Brian Nordland                                               *
*                                                                       *
* CREDITS: The following tutorials were used as starting points:        *
*   AVR SPI Tutorial:                                                   *
*       Link: http://maxembedded.com/2013/11/the-spi-of-the-avr/        *
*       Notes: For Information on SPI programming with the AVR          *
*                                                                       *
* --------------------------------------------------------------------  *
* | Change  | Date     |            |                                 | *
* | Flag    | (DDMYY)  | Author     | Description                     | *
* |---------|----------|------------|---------------------------------  *
* | None    | 18Apr17  | BNordland  | Initial creation                | *
*  -------------------------------------------------------------------  *
*************************************************************************/


#define F_CPU 16000000

// AVR Includes
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Our Library
#include "lib/led.h" // LED utilities
#include "lib/util.h" // bit and DDR Utilities
#include "lib/timer.h" // Timer utilities
#include "lib/kill.h" // kill board
#include "lib/motor.h" // motor utilities

// Hardware Definitions
#include "hardware.h"

// Standard Includes
#include <stdint.h> // integer types
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Virtual Serial Includes
#ifdef VIRTUAL_SERIAL
#include <VirtualSerial.h>
#else
#error VirtualSerial not defined, USB IO will not work
#endif

// Global Constants
#define DIRECTION_BACKWARD 0
#define DIRECTION_FORWARD  1

// Internal function definitions
void pSetup();
void pStartupFlashLEDs();
void pRetrieveGloveValues();
void pCalculateDuty();
bool pIsDirectionChanging();
uint8_t pSpiTransmit(uint8_t data);

// Global Variables
volatile int16_t    mAnglePitch; // Typically between -90 and 90
volatile bool       mVehicleDirection; // use DIRECTION_FOWARD, and DIRECTION_BACKWARD
volatile uint8_t    mThrottle; // a percentage of throttle from 0-100%
volatile int16_t    mLeftMotorDuty; // computed duty cycle of the drivers side motor
volatile int16_t    mRightMotorDuty; // computed duty cycle of the passenger side motor

volatile bool       mPreviousDirection; // the direction we were going last time
volatile uint16_t   mDirectionChangeCount; // how long we have been waiting for a direction change

int main(void)
{
    pSetup();

    pStartupFlashLEDs(); // Do our quick sanity check

    sei(); //Enables interrupts

    calibrateMotor1();
    setMotor1DutyCycle(0);
    setMotor1Forward();

    calibrateMotor2();
    setMotor2DutyCycle(0);
    setMotor2Forward();

    mPreviousDirection = mVehicleDirection;
    while(1)
    {
        pRetrieveGloveValues();
        pCalculateDuty();

        // If we are not currently changing the direction
        // of travel, then update our duty cycle.
        if(!pIsDirectionChanging())
        {
            if(mVehicleDirection)
            {
                setMotor1DutyCycle((uint8_t)mLeftMotorDuty);
                setMotor2DutyCycle((uint8_t)mRightMotorDuty);
            }
            else
            {

                // Ironically we flip the duty cycles here.
                // This is to make it so that when going backwards the direction
                // we head is intuitive to the tilt of the hand.
                setMotor1DutyCycle((uint8_t)mLeftMotorDuty);
                setMotor2DutyCycle((uint8_t)mRightMotorDuty);
            }
        }

        _delay_ms(10);
    }
}

void pCalculateDuty()
{
    if(mAnglePitch > -10 && mAnglePitch < 10)
    {
        // we are within 10 degree of same, use same throttle for each.
        mAnglePitch = 0;
    }
    else if(mAnglePitch > 80)
    {
        // We are almost at the max value of 90, set the angle pitch to 100
        // We set to 100 here because we want the other motor to turn off
        // in this case
       mAnglePitch = 100;
    }
    else if(mAnglePitch < -80)
    {
        mAnglePitch = -100;
    }

    // Throttle limiter (we want at most 50%)
    if(mThrottle > 0)
    {
        mThrottle = mThrottle / 2;
    }

    // Adjust for turning
    if(mAnglePitch < 0)
    {
        // Turn left
        mRightMotorDuty = mThrottle;
        // Although the next statement may seem odd, since below we set to the throttle if we are above it
        // because pitch angle could be negative, this takes care of that case of subtracing when it is negative.
        mLeftMotorDuty = mThrottle + mAnglePitch;
    }
    else if(mAnglePitch > 0)
    {
        // Turn right
        mLeftMotorDuty = mThrottle;
        mRightMotorDuty = mThrottle - mAnglePitch;
    }

    // normal range adjustment
    // pitch angle between -90 and 90
    if(mRightMotorDuty > mThrottle)
    {
        mRightMotorDuty = mThrottle;
    }
    else if(mRightMotorDuty < 0)
    {
        mRightMotorDuty = 0;
    }
    if(mLeftMotorDuty > mThrottle)
    {
        mLeftMotorDuty = mThrottle;
    }
    else if(mLeftMotorDuty < 0)
    {
        mLeftMotorDuty = 0;
    }

}

bool pIsDirectionChanging()
{
    if(mPreviousDirection != mVehicleDirection)
    {
        // If the direction changed, put in a delay
        // of 25 times our normal setting of the duty
        // cycle. This makes us favor the previous direction
        // for times when we are flipping right on the edge
        // and helps prevent such rapid changes in direction.

        // set the duty cycle to 0
        setMotor1DutyCycle(0);
        setMotor2DutyCycle(0);

        // Determine if we should change the direction
        if(mDirectionChangeCount > 25)
        {
            if(mVehicleDirection)
            {
                // going forward
                setMotor1Forward();
                setMotor2Forward();
            }
            else
            {
                // going backward
                setMotor1Backward();
                setMotor2Backward();
            }
            mDirectionChangeCount = 0;
            mPreviousDirection = mVehicleDirection; // update our previous direction
        }
        else
        {
            mDirectionChangeCount += 1;
        }
    }
    else
    {
        // Directions do equal, set out change count back to 0
        mDirectionChangeCount = 0;
    }

    // If the direction is changing, let's return true
    return (mPreviousDirection != mVehicleDirection);
}

void pRetrieveGloveValues()
{
    bitOff(PORTB, PORTB0); // Slave select on (by turning the bit off)
    _delay_ms(1); // The BLE Nano requires a 7us delay after turning on slave select.
    // Note, since the transmit data is ignored in our implementation
    // we simply transmit 0x00.
    uint8_t anglePitchByteHigh = pSpiTransmit(0x00);
    uint8_t anglePitchByteLow = pSpiTransmit(0x00);
    mAnglePitch = ((int16_t)(anglePitchByteHigh << 8)) | anglePitchByteLow;

    mVehicleDirection = pSpiTransmit(0x00);
    mThrottle = pSpiTransmit(0x00);
    bitOn(PORTB, PORTB0); // Slave select off (by turning the bit on)
}

uint8_t pSpiTransmit(uint8_t data)
{
    // Load data into the buffer
    SPDR = data;

    //Wait until transmission complete
    while(!(SPSR & (1<<SPIF) ));

    // Return received data
    return(SPDR);
}

ISR(PCINT0_vect)
{
    handleMotor1Interrupt();
}

ISR(INT1_vect)
{
    handleMotor2Interrupt();
}

ISR(INT3_vect)
{
    handleMotor2Interrupt();
}

void pSetup()
{
    // This prevents the need to reset after flashing
    USBCON = 0;

    // Set DDR for LEDs which needs to be output
    setDDR(YELLOW_DDR,YELLOW_DDRBIT,DDR_OUTPUT);
    setDDR(GREEN_DDR,GREEN_DDRBIT,DDR_OUTPUT);

    // set DDR for BLE Power to output and turn bit on
    setDDR(EXT_BLE_DDR,EXT_BLE_DDRBIT,DDR_OUTPUT);
    bitOn(EXT_BLE_PORT,EXT_BLE_PORTBIT);

    // Setup SPI
    // Set MOSI, SCK as Output
    DDRB=(1<<DDB1)|(1<<DDB2)|(1<<DDB0); // TODO: make this nicer
    SPCR=(1<<SPE)|(1<<MSTR)|(1<<SPR0); //|(1<<SPIE) < for interrupts
    bitOn(PORTB, PORTB0);

    setupMotor1(); // This sets up the motor 1
    setupMotor2(); // This sets up the motor 2

    // Enable interrupts required for motor encoders:
    // Enable the interrupts for the motor 1 encoder channels (uses pcint interrupts)
    bitOn(PCMSK0, motor1EncoderChannelAINT);
    bitOn(PCMSK0, motor1EncoderChannelBINT);
    bitOn(PCICR, PCIE0);// enable PCINT interrupts

    // Set INT1 to trigger on rising and falling edge (mode: 1,0)
    bitOn(EICRA,ISC10);
    bitOff(EICRA, ISC11);
    // Set INT3 to trigger on rising and falling edge (mode: 1,0)
    bitOn(EICRA,ISC30);
    bitOff(EICRA, ISC31);
    // Set the interrupt mask for INT1 and INT3
    bitOn(EIMSK, INT1);
    bitOn(EIMSK, INT3);

    SetupHardware(); //This setups the USB hardware and stdio // TODO: remove
}

/***************************************************************************************
 * Function Definition: startupFlashLEDs()
 *
 * Description: Sanity check to flash the LEDs in a certain order.
 *                  Turns on yellow, then green
 *                  Flashes all 4 times
 *                  Turns off green, then yellow
 *
 * Parameters: None
****************************************************************************************/
void pStartupFlashLEDs()
{
    yellow(1);
    _delay_ms(250);
    green(1);
    _delay_ms(250);

    int i;
    for(i = 0; i < 2; i++)
    {
        yellow(0);
        green(0);
        _delay_ms(100);
        yellow(1);
        green(1);
        _delay_ms(100);
    }

    green(0);
    _delay_ms(250);
    yellow(0);
}
