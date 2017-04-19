/************************************************************************
* FILENAME: timer.c														*
*																		*
* DESCRIPTION: Timer Functions Implementation							*
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
* | None    | 4Feb17   | BNordland  | Initial creation				  | *
* | $01     | 19Mar17  | BNordland  | Add ability to turn timer off   | *
*  -------------------------------------------------------------------	*
*************************************************************************/

#include "timer.h"
#include "util.h"
#include "kill.h"

// AVR Includes
#include <avr/io.h>

// integer types
#include <stdint.h>

void setTimer0ClockSelect(TimerClockSelect cs)
{
	switch(cs)
	{
		// cs02, cs01, cs00
	    case CS0:  // @01a - allow turning timer off
	        bitOff(TCCR0B, CS00);
	        bitOff(TCCR0B, CS01);
	        bitOff(TCCR0B, CS02);
	        break;
		case CS1:
			bitOn(TCCR0B, CS00);
			bitOff(TCCR0B, CS01);
			bitOff(TCCR0B, CS02);
			break;
		case CS8:
			bitOff(TCCR0B, CS00);
			bitOn(TCCR0B, CS01);
			bitOff(TCCR0B, CS02);
			break;
		case CS64:
			bitOn(TCCR0B, CS00);
			bitOn(TCCR0B, CS01);
			bitOff(TCCR0B, CS02);
			break;
		case CS256:
			bitOff(TCCR0B, CS00);
			bitOff(TCCR0B, CS01);
			bitOn(TCCR0B, CS02);
			break;
		case CS1024:
			bitOn(TCCR0B, CS00);
			bitOff(TCCR0B, CS01);
			bitOn(TCCR0B, CS02);
			break;
		default:
			// uh oh, reset the board
			kill(1);
			break;
	}
}

void setTimer1ClockSelect(TimerClockSelect cs)
{
	switch(cs)
	{
	    case CS0:  // @01a - allow turning timer off
	        bitOff(TCCR1B, CS10);
	        bitOff(TCCR1B, CS11);
	        bitOff(TCCR1B, CS12);
	        break;
		case CS1:
			bitOn(TCCR1B, CS10);
			bitOff(TCCR1B, CS11);
			bitOff(TCCR1B, CS12);
			break;
		case CS8:
			bitOff(TCCR1B, CS10);
			bitOn(TCCR1B, CS11);
			bitOff(TCCR1B, CS12);
			break;
		case CS64:
			bitOn(TCCR1B, CS10);
			bitOn(TCCR1B, CS11);
			bitOff(TCCR1B, CS12);
			break;
		case CS256:
			bitOff(TCCR1B, CS10);
			bitOff(TCCR1B, CS11);
			bitOn(TCCR1B, CS12);
			break;
		case CS1024:
			bitOn(TCCR1B, CS10);
			bitOff(TCCR1B, CS11);
			bitOn(TCCR1B, CS12);
			break;
		default:
			// uh oh, reset the board
			kill(1);
			break;
	}	
}

void setTimer3ClockSelect(TimerClockSelect cs)
{
	switch(cs)
	{
	    case CS0: // @01a - allow turning timer off
	        bitOff(TCCR3B, CS30);
	        bitOff(TCCR3B, CS31);
	        bitOff(TCCR3B, CS32);
	        break;
		case CS1:
			bitOn(TCCR3B, CS30);
			bitOff(TCCR3B, CS31);
			bitOff(TCCR3B, CS32);
			break;
		case CS8:
			bitOff(TCCR3B, CS30);
			bitOn(TCCR3B, CS31);
			bitOff(TCCR3B, CS32);
			break;
		case CS64:
			bitOn(TCCR3B, CS30);
			bitOn(TCCR3B, CS31);
			bitOff(TCCR3B, CS32);
			break;
		case CS256:
			bitOff(TCCR3B, CS30);
			bitOff(TCCR3B, CS31);
			bitOn(TCCR3B, CS32);
			break;
		case CS1024:
			bitOn(TCCR3B, CS30);
			bitOff(TCCR3B, CS31);
			bitOn(TCCR3B, CS32);
			break;
		default:
			// uh oh, reset the board
			kill(1);
			break;
	}	
}


void setTimer0WGMMode(TimerWGM mode)
{
	if(mode.bits.WGMn0)
	{
		bitOn(TCCR0A,WGM00);
	}
	else
	{
		bitOff(TCCR0A,WGM00);
	}
	
	if(mode.bits.WGMn1)
	{
		bitOn(TCCR0A,WGM01);
	}
	else
	{
		bitOff(TCCR0A,WGM01);
	}
	
	if(mode.bits.WGMn2)
	{
		bitOn(TCCR0B,WGM02);
	}
	else
	{
		bitOff(TCCR0B,WGM02);
	}
	
	// TODO, should probably verify that WGMn3 is not set
	// TODO: should probably verify reserved are 0
}

void setTimer1WGMMode(TimerWGM mode)
{
	if(mode.bits.WGMn0)
	{
		bitOn(TCCR1A,WGM10);
	}
	else
	{
		bitOff(TCCR1A,WGM10);
	}
	
	if(mode.bits.WGMn1)
	{
		bitOn(TCCR1A,WGM11);
	}
	else
	{
		bitOff(TCCR1A,WGM11);
	}
	
	if(mode.bits.WGMn2)
	{
		bitOn(TCCR1B,WGM12);
	}
	else
	{
		bitOff(TCCR1B,WGM12);
	}	
	
	if(mode.bits.WGMn3)
	{
		bitOn(TCCR1B,WGM13);
	}
	else
	{
		bitOff(TCCR1B,WGM13);
	}
	
	// TODO: should probably verify reserved are 0
}

void setTimer3WGMMode(TimerWGM mode)
{
	if(mode.bits.WGMn0)
	{
		bitOn(TCCR3A,WGM10);
	}
	else
	{
		bitOff(TCCR3A,WGM10);
	}
	
	if(mode.bits.WGMn1)
	{
		bitOn(TCCR3A,WGM11);
	}
	else
	{
		bitOff(TCCR3A,WGM11);
	}
	
	if(mode.bits.WGMn2)
	{
		bitOn(TCCR3B,WGM12);
	}
	else
	{
		bitOff(TCCR3B,WGM12);
	}	
	
	if(mode.bits.WGMn3)
	{
		bitOn(TCCR3B,WGM13);
	}
	else
	{
		bitOff(TCCR3B,WGM13);
	}
	
	// TODO: should probably verify reserved are 0
}


void setTimer0CompareOutputMode(TimerChannel channel, TimerCompareOutputMode com)
{
	uint8_t bit1 = 0;
	uint8_t bit0 = 0;
	switch(channel)
	{
		case ChannelA:
			bit1 = COM0A1;
			bit0 = COM0A0;
			break;
		case ChannelB:
			bit1 = COM0B1;
			bit0 = COM0B0;
			break;
		default:
			// uh oh, reset the board
			kill(2);
			break;
	}
	
	switch(com)
	{
		case COMDisconnected:
			bitOff(TCCR0A,bit1);
			bitOff(TCCR0A,bit0);			
			break;
		case COMFastPWMClearCM:
			bitOn(TCCR0A,bit1);
			bitOff(TCCR0A,bit0);
			break;
		case COMFastPWMSetCM:
			bitOn(TCCR0A,bit1);
			bitOn(TCCR0A,bit0);
			break;
		default:
			// uh oh, reset the board
			kill(3);
			break;
	}
}

void setTimer1CompareOutputMode(TimerChannel channel, TimerCompareOutputMode com)
{
	uint8_t bit1 = 0;
	uint8_t bit0 = 0;
	switch(channel)
	{
		case ChannelA:
			bit1 = COM1A1;
			bit0 = COM1A0;
			break;
		case ChannelB:
			bit1 = COM1B1;
			bit0 = COM1B0;
			break;
		case ChannelC:
			bit1 = COM1C1;
			bit0 = COM1C0;
			break;
		default:
			// uh oh, reset the board
			kill(2);
			break;
	}
	
	switch(com)
	{
		case COMDisconnected:
			bitOff(TCCR1A,bit1);
			bitOff(TCCR1A,bit0);
			break;
		case COMFastPWMClearCM:
			bitOn(TCCR1A,bit1);
			bitOff(TCCR1A,bit0);
			break;
		case COMFastPWMSetCM:
			bitOn(TCCR1A,bit1);
			bitOn(TCCR1A,bit0);
			break;
		default:
			// uh oh, reset the board
			kill(3);
			break;
	}
}

void setTimer3CompareOutputMode(TimerChannel channel, TimerCompareOutputMode com)
{
	uint8_t bit1 = 0;
	uint8_t bit0 = 0;
	switch(channel)
	{
		case ChannelA:
			bit1 = COM3A1;
			bit0 = COM3A0;
			break;
		default:
			// uh oh, reset the board
			kill(2);
			break;
	}
	
	switch(com)
	{
		case COMDisconnected:
			bitOff(TCCR3A,bit1);
			bitOff(TCCR3A,bit0);
			break;
		case COMFastPWMClearCM:
			bitOn(TCCR3A,bit1);
			bitOff(TCCR3A,bit0);
			break;
		case COMFastPWMSetCM:
			bitOn(TCCR3A,bit1);
			bitOn(TCCR3A,bit0);
			break;
		default:
			// uh oh, reset the board
			kill(3);
			break;
	}
}
