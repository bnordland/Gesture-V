/************************************************************************
* FILENAME: kill.h														*
*																		*
* DESCRIPTION: Reset Board Functions									*
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

#ifndef _kill_H_
#define _kill_H_

/***************************************************************************************
 * Function Definition: kill(int code)
 * 
 * Description: Used to reset the board, for example, an irrecoverable error
 *				Will turn on yellow, green, red for 5 seconds, then off for two seconds
 *				Then will blink all three leds for <code> # of times
 *
 * Parameters: code - the number of times the LEDs will blink to indicate the reason
****************************************************************************************/
void kill(int code);

#endif