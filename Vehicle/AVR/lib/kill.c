/************************************************************************
* FILENAME: kill.c														*
*																		*
* DESCRIPTION: Reset Board Functions Implementation						*
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
*  -------------------------------------------------------------------	*
*************************************************************************/

#ifndef F_CPU
#define F_CPU 16000000
#endif

#include "kill.h"
#include "util.h"
#include "led.h" 

// AVR Includes
#include <avr/wdt.h>
#include <util/delay.h>
#include <avr/interrupt.h>


void wdt_init(void) __attribute__((naked)) __attribute__((section(".init3")));

#define soft_reset()        \
do                          \
{                           \
    wdt_enable(WDTO_15MS);  \
    for(;;)                 \
    {                       \
    }                       \
} while(0)


// Function Implementation
void wdt_init(void)
{
    MCUSR = 0;
    wdt_disable();

    return;
}


void kill(int code)
{
	cli();
	
	// turn off timers
	TCCR0A = 0;
	TCCR0B = 0; // sets clock source to 0 for stopped
	TCCR1A = 0;
	TCCR1B = 0; // sets clock source to 0 for stopped
	TCCR3A = 0;
	TCCR3B = 0; // sets clock source to 0 for stopped
	
	// Set DDR for LEDs which needs to be output
	setDDR(YELLOW_DDR,YELLOW_DDRBIT,DDR_OUTPUT);
	setDDR(GREEN_DDR,GREEN_DDRBIT,DDR_OUTPUT);
	setDDR(RED_DDR,RED_DDRBIT,DDR_OUTPUT);
	
	yellow(1);
    green(1);
    red(1);
    _delay_ms(5000);
    yellow(0);
    green(0);
    red(0);
    _delay_ms(2000);
    
	int i;
	for(i = 0; i < code; i++)
	{
		yellow(1);
    	green(1);
    	red(1);
    	_delay_ms(250);
    	yellow(0);
    	green(0);
    	red(0);
    	_delay_ms(250);
    }
    soft_reset();
}