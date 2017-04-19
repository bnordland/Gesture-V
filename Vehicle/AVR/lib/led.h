/************************************************************************
* FILENAME: led.h														*
*																		*
* DESCRIPTION: LED defines and macros									*
* 			   This contains some useful LED definitions, functions	    *
*              and macros. Macros are used where possible to prevent    *
*			   extra function calls										*
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
* | None    | 22Jan17  | BNordland  | Initial creation				  | *
*  -------------------------------------------------------------------	*
*************************************************************************/


#ifndef _led_H_
#define _led_H_

#include <avr/io.h>

// Define the Data Direction Registers
#define YELLOW_DDR		DDRC
#define YELLOW_DDRBIT 	DDC7
#define GREEN_DDR		DDRD
#define GREEN_DDRBIT	DDD5
#define RED_DDR			DDRB
#define RED_DDRBIT		DDB0

// Define PORT and Bit mappings on a PORT
// for the various LEDs
#define YELLOW_LED_PORT 	PORTC
#define YELLOW_LED_PORTBIT	PORTC7
#define GREEN_LED_PORT		PORTD
#define GREEN_LED_PORTBIT	PORTD5
#define RED_LED_PORT		PORTB
#define RED_LED_PORTBIT		PORTB0

/***************************************************************************************
 * Macro Definition: yellowToggle(), greenToggle(), redToggle()
 * 
 * Description: Toggle an LED to the opposite state it was in previously
 *
 * Parameters: none
****************************************************************************************/	
#define yellowToggle() 	YELLOW_LED_PORT ^= ( 1 << YELLOW_LED_PORTBIT );
#define greenToggle() 	GREEN_LED_PORT 	^= ( 1 << GREEN_LED_PORTBIT );
#define redToggle() 	RED_LED_PORT 	^= ( 1 << RED_LED_PORTBIT );

/**************************************************
 * Macro Definition: yellow(on)
 * 
 * Description: Turning on and off the yellow LED
 *
 * Parameters:
 * 		on		If 0 LED is OFF, ON otherwise
***************************************************/	
#define yellow(on) \
 ({ \
 	if(on) \
 	{ \
 		YELLOW_LED_PORT |= ( 1 << YELLOW_LED_PORTBIT ); \
 	} else { \
 		YELLOW_LED_PORT &= ~( 1 << YELLOW_LED_PORTBIT ); \
 	} \
 }) \
 
 /**************************************************
 * Macro Definition: green(on)
 * 
 * Description: Turning on and off the green LED
 *
 * Parameters:
 * 		on		If 0 LED is OFF, ON otherwise
***************************************************/
 #define green(on) \
 ({ \
 	if(on) \
 	{ \
 		GREEN_LED_PORT &= ~( 1 << GREEN_LED_PORTBIT ); \
 	} else { \
 		GREEN_LED_PORT |= ( 1 << GREEN_LED_PORTBIT ); \
 	} \
 }) \
 

 /**************************************************
 * Macro Definition: red(on)
 * 
 * Description: Turning on and off the red LED
 *
 * Parameters:
 * 		on		If 0 LED is OFF, ON otherwise
***************************************************/
#define red(on) \
 ({ \
 	if(on) \
 	{ \
 		RED_LED_PORT &= ~( 1 << RED_LED_PORTBIT ); \
 	} else { \
 		RED_LED_PORT |= ( 1 << RED_LED_PORTBIT ); \
 	} \
 }) \
 
 
 #endif /* _led_H_ */