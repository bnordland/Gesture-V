/************************************************************************
* FILENAME: motor.c														*
*																		*
* DESCRIPTION: Motor Functions	- Implementation of motor.h				*
*																		*
* LICENSE: The MIT License (MIT)										*
*          Copyright (c) 2017 Brian Nordland       						*
* 																		*
* AUTHOR:  Brian Nordland												*
*																		*
* --------------------------------------------------------------------  *
* | Change  | Date     |            |								  | *
* | Flag    | (DDMYY)  | Author     | Description					  |	*
* |---------|----------|------------|---------------------------------	*
* | None    | 11Feb17  | BNordland  | Initial creation				  | *
* | @01     | 2Mar17   | BNordland  | Update motor count to be 32bit  | *
*  -------------------------------------------------------------------	*
*************************************************************************/

// Implementation for
#include "motor.h"

// Our library includes
#include "kill.h"
#include "util.h"
#include "timer.h"

// Standard Includes
#include <stdint.h> // integer types

// AVR includes
#include <avr/interrupt.h>

// For setting frequency of timer. Freq calculation based on TOP+1
#define TOP_4kHz 3999

// Motor 2 PWM signal generated on B6
#define motor2PWMPort	PORTB
#define motor2PWMPin	PORTB6
#define motor2PWMDDR	DDRB
#define motor2PWMDDRPin	DDB6

// Motor 2 direction set by Port E pin 2
#define motor2DirectionPort PORTE
#define motor2DirectionPin	PORTE2
#define motor2DirectionDDR DDRE
#define motor2DirectionDDRPin DDE2

// Motor 1 PWM signal generated on B6
#define motor1PWMPort   PORTB
#define motor1PWMPin    PORTB5
#define motor1PWMDDR    DDRB
#define motor1PWMDDRPin DDB5

// Motor 1 direction set by Port E pin 2
#define motor1DirectionPort PORTD
#define motor1DirectionPin  PORTD6
#define motor1DirectionDDR DDRD
#define motor1DirectionDDRPin DDD6

// Global Variables
volatile int32_t pMotor2Count = 0; // @01c - update to be 32bit
volatile int16_t pMotor2ErrCount = 0;
volatile char    pMotor2ForwardDirectionSetting = 0; // bit setting for moving the motor forward
volatile uint8_t pMotor2ChannelALast = 0;
volatile uint8_t pMotor2ChannelBLast = 0;

volatile int32_t pMotor1Count = 0; // @01c - update to be 32bit
volatile int16_t pMotor1ErrCount = 0;
volatile char    pMotor1ForwardDirectionSetting = 0; // bit setting for moving the motor forward
volatile uint8_t pMotor1ChannelALast = 0;
volatile uint8_t pMotor1ChannelBLast = 0;

void setupMotor2()
{
	// first verify that all motor2 variables are defined
	// if not, reset the board
	#if defined motor2Error
	kill(10);
	#endif

	setMotor2Off(); // turn off the motor so it doesn't start going as we set it up

	// set motor direction DDR to output
	setDDR(motor2DirectionDDR,motor2DirectionDDRPin,DDR_OUTPUT);

	// assume that forward is 0. Calibration will correct this.
	pMotor2ForwardDirectionSetting = 0;

	setMotor2Forward();

	// Motor2 is connected to timer1, channel B.
	// use Fast PWM Mode 14
	TimerWGM timer1Mode;
	timer1Mode.value = 14;
	setTimer1WGMMode(timer1Mode);
	setTimer1CompareOutputMode(ChannelB,COMFastPWMClearCM); // clear on match to make higher OCR make higher duty
	setTimer1ClockSelect(CS1);
	ICR1 = TOP_4kHz;

	// set up encoder

	// set encoder channels as input
	setDDR(motor2EncoderChannelADDR,motor2EncoderChannelAPIN, DDR_INPUT);
	setDDR(motor2EncoderChannelBDDR,motor2EncoderChannelBPIN, DDR_INPUT);

	// Turn on encoder power
	bitOn(motor2EncoderPowerDDR, motor2EncoderPowerPin);
	bitOn(motor2EncoderPowerPort , motor2EncoderPowerPin);
}

void calibrateMotor2()
{
	setMotor2Forward();
  	setMotor2DutyCycle(15);

  	int16_t count = getMotor2Count();
  	while(count < 10 && count > -10)
  	{
  		count = getMotor2Count();
  	}


  	setMotor2DutyCycle(0);
  	if(getMotor2Count() < 0)
  	{
  		// direction is backwards
  		if(pMotor2ForwardDirectionSetting == 0)
  		{
  			pMotor2ForwardDirectionSetting = 1;
  		}
  		else
  		{
  			pMotor2ForwardDirectionSetting = 0;
  		}
  	}

  	returnMotor2ToRefPosition();
}

void returnMotor2ToRefPosition()
{
	int16_t count = getMotor2Count();
	while(count != 0)
	{
		if(count > 0)
		{
			setMotor2Backward();
			setMotor2DutyCycle(15);
		}
		else if(count < 0)
		{
			setMotor2Forward();
			setMotor2DutyCycle(15);
		}
		count = getMotor2Count();
	}
	setMotor2Forward();
	setMotor2DutyCycle(0);
}

void handleMotor2Interrupt()
{
	// Make a copy of the current reading from the encoders
	uint8_t tmpB = PIND;

	// Get value of each channel, making it either a 0 or 1 valued integer
	uint8_t m2a_val = (tmpB & (1 << motor2EncoderChannelAPIN )) >> motor2EncoderChannelAPIN;
	uint8_t m2b_val = (tmpB & (1 << motor2EncoderChannelBPIN )) >> motor2EncoderChannelBPIN;

	// Adding or subtracting counts is determined by how these change between interrupts
	int8_t plus_m2 = m2a_val ^ pMotor2ChannelBLast;
	int8_t minus_m2 = m2b_val ^ pMotor2ChannelALast;

	// Add or subtract encoder count as appropriate
	if(plus_m2) { pMotor2Count += 1; }
	if(minus_m2) { pMotor2Count -= 1; }

	// If both values changed, something went wrong - probably missed a reading
	if(m2a_val != pMotor2ChannelALast && m2b_val != pMotor2ChannelBLast) {
		pMotor2ErrCount++;
	}

	// Save for next interrupt
	pMotor2ChannelALast = m2a_val;
	pMotor2ChannelBLast = m2b_val;
}

void setMotor2DutyCycle(uint16_t dutyCycle)
{
	if (dutyCycle == 0) {
		OCR1B = 0;
		setMotor2Off();
		return;
	}
	else if (dutyCycle > 100) {
		dutyCycle = 100;
	}
	OCR1B = (uint16_t) ((TOP_4kHz + 1.0) * (dutyCycle / 100.0 )) - 1;
	setMotor2On();
}

void setMotor2Off()
{
	// set to off by turning PWM to input
	setDDR(motor2PWMDDR, motor2PWMDDRPin, DDR_INPUT);
}

void setMotor2On()
{
	// set to on by turning PWM to output
	setDDR(motor2PWMDDR, motor2PWMDDRPin, DDR_OUTPUT);
}

void setMotor2Forward()
{
	bitSet(motor2DirectionPort, motor2DirectionPin, pMotor2ForwardDirectionSetting);
}

void setMotor2Backward()
{
	bitSet(motor2DirectionPort, motor2DirectionPin, !pMotor2ForwardDirectionSetting);
}

void resetMotor2Count()
{
	pMotor2Count = 0;
}

// @01c - update to be 32bit
int32_t getMotor2Count()
{
	return pMotor2Count;
}






void setupMotor1()
{
    // first verify that all motor1 variables are defined
    // if not, reset the board
    #if defined motor1Error
    kill(10);
    #endif

    setMotor1Off(); // turn off the motor so it doesn't start going as we set it up

    // set motor direction DDR to output
    setDDR(motor1DirectionDDR,motor1DirectionDDRPin,DDR_OUTPUT);

    // assume that forward is 0. Calibration will correct this.
    pMotor1ForwardDirectionSetting = 0;

    setMotor1Forward();

    // Motor1 is connected to timer1, channel A.
    // use Fast PWM Mode 14
    TimerWGM timer1Mode;
    timer1Mode.value = 14;
    setTimer1WGMMode(timer1Mode);
    setTimer1CompareOutputMode(ChannelA,COMFastPWMClearCM); // clear on match to make higher OCR make higher duty
    setTimer1ClockSelect(CS1);
    ICR1 = TOP_4kHz;

    // set up encoder

    // set encoder channels as input
    setDDR(motor1EncoderChannelADDR,motor1EncoderChannelAPIN, DDR_INPUT);
    setDDR(motor1EncoderChannelBDDR,motor1EncoderChannelBPIN, DDR_INPUT);

    // Turn on encoder power
    bitOn(motor1EncoderPowerDDR, motor1EncoderPowerPin);
    bitOn(motor1EncoderPowerPort , motor1EncoderPowerPin);
}

void calibrateMotor1()
{
    setMotor1Forward();
    setMotor1DutyCycle(15);

    int16_t count = getMotor1Count();
    while(count < 10 && count > -10)
    {
        count = getMotor1Count();
    }


    setMotor1DutyCycle(0);
    if(getMotor1Count() < 0)
    {
        // direction is backwards
        if(pMotor1ForwardDirectionSetting == 0)
        {
            pMotor1ForwardDirectionSetting = 1;
        }
        else
        {
            pMotor1ForwardDirectionSetting = 0;
        }
    }

    returnMotor1ToRefPosition();
}

void handleMotor1Interrupt()
{
    // Make a copy of the current reading from the encoders
    uint8_t tmpB = PINB;

    // Get value of each channel, making it either a 0 or 1 valued integer
    uint8_t m2a_val = (tmpB & (1 << motor1EncoderChannelAPIN )) >> motor1EncoderChannelAPIN;
    uint8_t m2b_val = (tmpB & (1 << motor1EncoderChannelBPIN )) >> motor1EncoderChannelBPIN;

    // Adding or subtracting counts is determined by how these change between interrupts
    int8_t plus_m2 = m2a_val ^ pMotor1ChannelBLast;
    int8_t minus_m2 = m2b_val ^ pMotor1ChannelALast;

    // Add or subtract encoder count as appropriate
    if(plus_m2) { pMotor1Count += 1; }
    if(minus_m2) { pMotor1Count -= 1; }

    // If both values changed, something went wrong - probably missed a reading
    if(m2a_val != pMotor1ChannelALast && m2b_val != pMotor1ChannelBLast) {
        pMotor1ErrCount++;
    }

    // Save for next interrupt
    pMotor1ChannelALast = m2a_val;
    pMotor1ChannelBLast = m2b_val;
}

void setMotor1On()
{
    // set to on by turning PWM to output
    setDDR(motor1PWMDDR, motor1PWMDDRPin, DDR_OUTPUT);
}
void setMotor1Off()
{
    // set to off by turning PWM to input
    setDDR(motor1PWMDDR, motor1PWMDDRPin, DDR_INPUT);
}

void setMotor1DutyCycle(uint16_t dutyCycle)
{
    if (dutyCycle == 0) {
        OCR1A = 0;
        setMotor1Off();
        return;
    }
    else if (dutyCycle > 100) {
        dutyCycle = 100;
    }
    OCR1A = (uint16_t) ((TOP_4kHz + 1.0) * (dutyCycle / 100.0 )) - 1;
    setMotor1On();
}

void setMotor1Forward()
{
    bitSet(motor1DirectionPort, motor1DirectionPin, pMotor1ForwardDirectionSetting);
}

void setMotor1Backward()
{
    bitSet(motor1DirectionPort, motor1DirectionPin, !pMotor1ForwardDirectionSetting);
}

void returnMotor1ToRefPosition()
{
    int16_t count = getMotor1Count();
    while(count != 0)
    {
        if(count > 0)
        {
            setMotor1Backward();
            setMotor1DutyCycle(15);
        }
        else if(count < 0)
        {
            setMotor1Forward();
            setMotor1DutyCycle(15);
        }
        count = getMotor1Count();
    }
    setMotor1Forward();
    setMotor1DutyCycle(0);
}

/**************************************************
 * Function Definition: resetMotor1Count()
 *
 * Description: Resets the counter for motor1
 *
 * Parameters: None
 *
 * Returns: None
***************************************************/
void resetMotor1Count()
{
    pMotor1Count = 0;
}

/**************************************************
 * Function Definition: getMotor1Count()
 *
 * Description: Gets the current counter for motor1
 *
 * Parameters: None
 *
 * Returns: The current encoder count.
 *          @01c - update to be 32bit instead of 16
***************************************************/
int32_t getMotor1Count()
{
    return pMotor1Count;
}
