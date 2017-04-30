/************************************************************************
* FILENAME: util.h														*
*																		*
* DESCRIPTION: Utility Functions										*
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
* | @01     | 2Mar17   | BNordland  | Added isInt and charToInt       | *
*  -------------------------------------------------------------------	*
*************************************************************************/


#ifndef _util_H_
#define _util_H_

#include <avr/io.h>

#define DDR_INPUT 0
#define DDR_OUTPUT 1

// Define the boolean type
typedef int bool;
enum { false = 0, true = 1 }; // Define the boolean values

/**************************************************************************************
* Macro Definition: bitToggle(port,bit)
*
* Description: For toggling the value of a bit. If the bit is currently on, it'll be
*              set to off. If the bit is currently off, it'll be set to on.
*
* Parameters:
*      port    The port the bit is on
*      bit     The bit (pin) on the port to toggle
***************************************************************************************/
#define bitToggle(port,bit) port ^= ( 1 << bit )

/**************************************************************************************
* Macro Definition: bitOff(port,bit)
*
* Description: For holding a bit high
*
* Parameters:
*      port    The port the bit is on
*      bit     The bit (pin) on the port to turn on
***************************************************************************************/
#define bitOn(port,bit) port |= (1 << bit)

/**************************************************************************************
* Macro Definition: bitOff(port,bit)
*
* Description: For holding a bit low
*
* Parameters:
*      port    The port the bit is on
*      bit     The bit (pin) on the port to turn off
***************************************************************************************/
#define bitOff(port,bit) port &= ~(1 << bit)

/**************************************************************************************
* Macro Definition: bitSet(port,bit,on)
*
* Description: For setting a bit in a register to a particular value
*
* Parameters:
*      port    The port the bit is on
*      bit     The bit (pin) on the port
*      on      either 1/true (on) or 0/false (off), for the value of the bit.
***************************************************************************************/
#define bitSet(port,bit,on) \
 ({ \
 	if(on) \
 	{ \
 		bitOn(port,bit); \
 	} else { \
 		bitOff(port,bit); \
 	} \
 }) \

 /**************************************************************************************
 * Macro Definition: setDDR(ddr,ddrbit,func)
 * 
 * Description: Set Data Direction Register (DDR) for either input or output
 *
 * Parameters:
 * 		ddr		The data direction register to set
 *      ddrbit	The bit (pin) in the data direction register to set
 *		func	either DDR_INPUT or DDR_OUTPUT, to set the DDR.
***************************************************************************************/
 #define setDDR(ddr,ddrbit,func) \
 ({ \
 	if(func == DDR_INPUT) \
 	{ \
 		ddr &= ~(1 << ddrbit); \
 	} else if (func ==  DDR_OUTPUT) { \
 		ddr |= (1 << ddrbit); \
 	} \
 }) 
 
/************************************************************************************
 * Function Definition: isInt(char c)										    @01a
 *
 * Description: Indicates if a character is an integer or not
 *
 * Parameters:
 * 		c	the character to check
 *
 * 	Returns:
 * 		true if the character is an integer, or false otherwise.
 ************************************************************************************/
bool isInt(char c);

/************************************************************************************
 * Function Definition: charToInt(char c)										@01a
 *
 * Description: Converts a character to an integer (use isInt before for safety)
 *
 * Parameters:
 * 		c	the character to convert.
 *
 * 	Returns:
 * 		an integer representation of the character
 ************************************************************************************/
inline int charToInt(char c);

 #endif /* _util_H_ */
