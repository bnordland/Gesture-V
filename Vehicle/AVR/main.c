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
*   Ultrasonic Tutorial:                                                *
*       Link: http://www.embedds.com/interfacing-ultrasonic-            *
*                   rangefinder-with-avr/                               *
*       Notes: Used as a basis for ultrasonic sensor.                   *
*                                                                       *
* --------------------------------------------------------------------  *
* | Change  | Date     |            |                                 | *
* | Flag    | (DDMYY)  | Author     | Description                     | *
* |---------|----------|------------|---------------------------------  *
* | None    | 18Apr17  | BNordland  | Initial creation                | *
* | @01     | 30Apr17  | BNordland  | Adding ultrasonic sensor        | *
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

// @01a Ultrasonic Sensor Constants
#define ULTRASONIC_STATE_OFF 0
#define ULTRASONIC_STATE_PULSING   1
#define ULTRASONIC_STATE_MEASURING 2
#define ULTRASONIC_STATE_AVAILABLE 3
#define ULTRASONIC_MAX_RSP_TIME_MS  200
#define ULTRASONIC_INSTR_PER_MS     (F_CPU / 1000)
#define ULTRASONIC_INSTR_PER_US     (ULTRASONIC_INSTR_PER_MS / 1000)
#define ULTRASONIC_MAX_TICKS        (uint32_t)ULTRASONIC_MAX_RSP_TIME_MS * ULTRASONIC_INSTR_PER_MS

// Internal function definitions
void pSetup();
void pStartupFlashLEDs();
void pRetrieveGloveValues();
void pCalculateDuty();
bool pIsDirectionChanging();
uint8_t pSpiTransmit(uint8_t data);
void pTriggerSonar(); // @01a start the ultrasonic detection

// Global Variables
volatile int16_t    mAnglePitch; // Typically between -90 and 90
volatile bool       mVehicleDirection; // use DIRECTION_FOWARD, and DIRECTION_BACKWARD
volatile uint8_t    mThrottle; // a percentage of throttle from 0-100%
volatile int16_t    mLeftMotorDuty; // computed duty cycle of the drivers side motor
volatile int16_t    mRightMotorDuty; // computed duty cycle of the passenger side motor

volatile bool       mPreviousDirection; // the direction we were going last time
volatile uint16_t   mDirectionChangeCount; // how long we have been waiting for a direction change

// Global Variables for ultrasonic @01a
volatile uint8_t    mUltrasonicState;
volatile uint32_t   mUltrasonicTimerOverflowCount;
volatile float      mUltrasonicResult;


int main(void)
{
    pSetup();

    pStartupFlashLEDs(); // Do our quick sanity check

    sei(); //Enables interrupts

    uint8_t    ultrasonicDelayCount = 0; // @01a used to delay ultrasonic readings
    uint32_t    collisionDistanceFront = 0; // @01a start off assuming we are going to hit something

    // Have the motors figure out which direction
    // is considered forward by calibrating them.
    calibrateMotor1();
    setMotor1DutyCycle(0);
    setMotor1Forward();

    calibrateMotor2();
    setMotor2DutyCycle(0);
    setMotor2Forward();

    mPreviousDirection = mVehicleDirection; // set our direction to be whatever it may be (by default 0 for backward)

    while(1)
    {
        pRetrieveGloveValues();
        pCalculateDuty();

        // Start @01a - Check for collisions
        if(mUltrasonicState == ULTRASONIC_STATE_OFF)
        {
            // If the ultrasonic sensor is off,
            // check to see if it has been long enough since
            // the last reading, and try again.
            if(ultrasonicDelayCount >= 5)
            {
                ultrasonicDelayCount = 0;
                pTriggerSonar();
            }
            else
            {
                ultrasonicDelayCount++;
            }
        }
        else if(mUltrasonicState == ULTRASONIC_STATE_AVAILABLE)
        {
            mUltrasonicState = ULTRASONIC_STATE_OFF;
            if(mUltrasonicResult != -1)
            {
                collisionDistanceFront = mUltrasonicResult;
            }
            else
            {
                collisionDistanceFront = 1000; // assume big, we didn't get a response
            }
        }

        // If we are closer than 15cm, let's stop
        if(mVehicleDirection && collisionDistanceFront < 15)
        {
            mLeftMotorDuty = 0;
            mRightMotorDuty = 0;
            yellow(1); // indicate close collision with yellow LED
        }
        else
        {
            yellow(0); // Not close to hitting anything, or we are going backwards
        }
        // end @01a

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

    // Throttle limiter (we want at most 75%)
    if(mThrottle > 0)
    {
        mThrottle = mThrottle / 1.5;
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

/*****************************************************************************
 *                                                                      @01a *
 *                                                                           *
 * Description: Starts the triggering of an ultrasonic distance detection    *
 *                                                                           *
 *              Performance Note: This has 12us of delays in it.             *
 *                               (almost ~200 instructions)                  *
 *                                                                           *
 *              Note: This will set trigger high, then lower it. We then     *
 *                    wait for the echo line to come high, and measure       *
 *                    how long the line stays high. The waiting for echo     *
 *                    and measuring is all done via interrupts.              *
 *                    See: ISR(INT0_vect) and ISR(TIMER0_OVF_vect)           *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: None                                                          *
 *                                                                           *
 *****************************************************************************/
void pTriggerSonar()
{
    if(mUltrasonicState == ULTRASONIC_STATE_OFF)
    {
        // generate the pulse for the trigger
        // TODO: use hardware.h defines
        bitOff(PORTE, PORTE6);
        _delay_us(2); // delay for 1us
        bitOn(PORTE, PORTE6);
        mUltrasonicState = ULTRASONIC_STATE_PULSING;
        _delay_us(10); // delay with high for 10us
        bitOff(PORTE, PORTE6);
    }
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

/*****************************************************************************
 *                                                                      @01a *
 *                                                                           *
 * Description: Timer overflow interrupt. Used to count how many times the   *
 *              timer has overflowed since an ultrasonic reading was taken.  *
 *              This will also calculate if things have gone on too long     *
 *              (over 200ms) and will then cancel the ultrasonic reading     *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: TIMER0_OVF_vect - Timer overflow interrupt vector             *
 *                                                                           *
 *****************************************************************************/
ISR(TIMER0_OVF_vect)
{
        if (mUltrasonicState == ULTRASONIC_STATE_MEASURING)
        {
            // currently measuring the distance (voltage rise happened before)
            mUltrasonicTimerOverflowCount++;

            // check to see if we have gone over maximum response time
            uint32_t ticks = (mUltrasonicTimerOverflowCount * 256) + TCNT0;
            if (ticks > ULTRASONIC_MAX_TICKS)
            {
                // timeout in distance measurement
                mUltrasonicState = ULTRASONIC_STATE_AVAILABLE; // set to off, as no distance is available.
                mUltrasonicResult = -1; // TODO: return max distance
            }
        }
}

/*****************************************************************************
 *                                                                      @01a *
 *                                                                           *
 * Description: INT0 interrupt. Used for ultrasonic pulsing. Starts timer    *
 *              counting when pin goes high, and captures measurement time   *
 *              when pin goes low.                                           *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: INT0_vect - INT0 interrupt vector                             *
 *                                                                           *
 *****************************************************************************/
ISR(INT0_vect)
{
    // TODO: we should probably do extra checking in here to make sure
    //       the pin is actually in the state we want as well as the measurement
    //       state. This way we don't get false readings somehow.
    if (mUltrasonicState == ULTRASONIC_STATE_PULSING)
    {
        // voltage rise, we can start the measurement
        mUltrasonicState = ULTRASONIC_STATE_MEASURING;

        // reset counts
        mUltrasonicTimerOverflowCount = 0;
        TCNT0 = 0;
    }
    else if(mUltrasonicState == ULTRASONIC_STATE_MEASURING)
    {
        // voltage drop, stop the measurement
        mUltrasonicState = ULTRASONIC_STATE_AVAILABLE;

        mUltrasonicResult = (mUltrasonicTimerOverflowCount * 256 + TCNT0)/58.0/ULTRASONIC_INSTR_PER_US;
    }
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
    SPCR=(1<<SPE)|(1<<MSTR)|(1<<SPR0);
    bitOn(PORTB, PORTB0); // Turn off slave select (which is done by holding high)

    setupMotor1(); // This sets up the motor 1
    setupMotor2(); // This sets up the motor 2

    // Enable interrupts required for motor encoders:
    // Enable the interrupts for the motor 1 encoder channels (uses pcint interrupts)
    bitOn(PCMSK0, motor1EncoderChannelAINT);
    bitOn(PCMSK0, motor1EncoderChannelBINT);
    bitOn(PCICR, PCIE0);// enable PCINT interrupts

    // Enable interrupts for motor 2 encoders, which is done via standard INTx PINs
    // Set INT1 to trigger on rising and falling edge (mode: 1,0)
    bitOn(EICRA,ISC10);
    bitOff(EICRA, ISC11);
    // Set INT3 to trigger on rising and falling edge (mode: 1,0)
    bitOn(EICRA,ISC30);
    bitOff(EICRA, ISC31);
    // Set the interrupt mask for INT1 and INT3
    bitOn(EIMSK, INT1);
    bitOn(EIMSK, INT3);

    // start @01a - setup ultrasonic sensor
    // TODO: we should move to hardware.h for sonar
    setDDR(DDRE,DDE6, DDR_OUTPUT); // trigger (transmit)
    setDDR(DDRD,DDD0, DDR_INPUT); // echo pin (recieve)
    bitOff(PORTE, PORTE6); // start with trigger off

    // Turn on ultrasonic sensor power
    setDDR(DDRF,DDF1, DDR_OUTPUT);
    bitOn(PORTF, PORTF1);

    // enable ultrasonic sensor interrupts (INT0)
    // Set INT0 to trigger on rising and falling edge (mode: 1,0)
    bitOn(EICRA,ISC00);
    bitOff(EICRA, ISC01);
    bitOn(EIMSK, INT0);

    // Enable timer0
    // Timer 0 - used for distance
    TimerWGM timer0Mode;
    timer0Mode.value = 0;
    setTimer0WGMMode(timer0Mode);
    setTimer0ClockSelect(CS1); // No prescaling
    // TODO: this should probably be made easier and implemented in the timer library
    // enable timer 0 interrupts
    bitOn(TIMSK0,TOIE0); // TIMSK0
    // end @01a - end of setup of ultrasonic sensor

    SetupHardware(); //This setups the USB hardware and stdio
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
