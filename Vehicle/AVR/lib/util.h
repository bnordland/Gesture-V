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

typedef int bool;
enum { false = 0, true = 1 };

// Helper functions
// for toggling a bit
#define bitToggle(port,bit) port ^= ( 1 << bit )

// for holding bit high
#define bitOn(port,bit) port |= (1 << bit)

// for holding bit low
#define bitOff(port,bit) port &= ~(1 << bit)

// for setting a bit to a particular value
#define bitSet(port,bit,on) \
 ({ \
 	if(on) \
 	{ \
 		bitOn(port,bit); \
 	} else { \
 		bitOff(port,bit); \
 	} \
 }) \

#include <avr/io.h>

#define DDR_INPUT 0
#define DDR_OUTPUT 1

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
