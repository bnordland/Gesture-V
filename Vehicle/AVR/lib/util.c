/************************************************************************
* FILENAME: util.c														*
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
* | None    | 2Mar17   | BNordland  | Initial creation				  | *
*  -------------------------------------------------------------------	*
*************************************************************************/

#include "util.h"

/************************************************************************************
 * Function Definition: isInt(char c)
 *
 * Description: Indicates if a character is an integer or not
 *
 * Parameters:
 * 		c	the character to check
 *
 * 	Returns:
 * 		true if the character is an integer, or false otherwise.
 ************************************************************************************/
bool isInt(char c)
{
	int num = c - '0';
	return (num >= 0 && num <= 9);
}

/************************************************************************************
 * Function Definition: charToInt(char c)
 *
 * Description: Converts a character to an integer (use isInt before for safety)
 *
 * Parameters:
 * 		c	the character to convert.
 *
 * 	Returns:
 * 		an integer representation of the character
 ************************************************************************************/
inline int charToInt(char c)
{
	return c - '0';
}
