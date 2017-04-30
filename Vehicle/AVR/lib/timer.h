/************************************************************************
* FILENAME: timer.h														*
*																		*
* DESCRIPTION: Timer Functions											*
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

#ifndef _timer_H_
#define _timer_H_

// integer types
#include <stdint.h>

/*****************************************************************************
 * Description: The valid values for the clock select mode of the timer      *
 *                                                                           *
 *              CS0 - No clock source - timer stopped                        *
 *              CS1 - No prescaling                                          *
 *              CS8 - Clock/8                                                *
 *              CS64 - Clock/64                                              *
 *              CS256 - Clock/256                                            *
 *                                                                           *
 *              See: pg. 108 (8-bit timers), or pg. 134 (16-bit timers)      *
 *                                                                           *
 *****************************************************************************/
typedef enum {CS0, CS1, CS8, CS64, CS256, CS1024 } TimerClockSelect;

/*****************************************************************************
 * Description: PWM Compare Output Modes:                                    *
 *                                                                           *
 *              COMDisconnected - PWM pins disconnected                      *
 *              COMFastPWMClearCM - In FastPWM mode, clear pwm bit on match  *
 *              COMFastPWMSetCM   - In FastPWM mode, set pwm bit on match    *
 *                                                                           *
 *              See: datasheet pg 107 (8-bit timers), pg 132 (16-bit timers) *
 *                                                                           *
 *****************************************************************************/
typedef enum {COMDisconnected,COMFastPWMClearCM, COMFastPWMSetCM} TimerCompareOutputMode;

/*****************************************************************************
 * Description: Timer Channels. Each timer has multiple channels. To         *
 *              compare/match on. (ChannelA, ChannelB, or ChannelC)          *
 *                                                                           *
 *              Note: Not all timers have all 3 channels. See the datasheet  *
 *                    for information on which channels are available to     *
 *                    a particular timer.                                    *
 *                                                                           *
 *****************************************************************************/
typedef enum {ChannelA, ChannelB, ChannelC} TimerChannel;

/*****************************************************************************
 * Description: Structure for setting the waveform generation mode (WGM)     *
 *              of the timer.                                                *
 *                                                                           *
 *              See the appropriate datasheet pages.                         *
 *                                                                           *
 *****************************************************************************/
typedef struct
{
	union 
	{
		uint8_t value;
		struct {
			uint8_t WGMn0:1;
			uint8_t WGMn1:1;
			uint8_t WGMn2:1;
			uint8_t WGMn3:1;
			uint8_t reserved:4;
		} bits;
	};
} TimerWGM;

/*****************************************************************************
 * Description: Sets the clock select mode for timer0 to the specified value *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: cs - Clock select value (see definition for TimerClockSelect) *
 *                                                                           *
 *****************************************************************************/
void setTimer0ClockSelect(TimerClockSelect cs);

/*****************************************************************************
 * Description: Sets the Waveform generation mode for timer0                 *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: mode - WGM mode (see definition for TimerWGM structure)       *
 *                                                                           *
 *****************************************************************************/
void setTimer0WGMMode(TimerWGM mode);

/*****************************************************************************
 * Description: Sets the compare output mode (COM) for timer0                *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: channel - which channel (A, B) to set the compare output      *
 *                       mode for.                                           *
 *             com     - what to set the compare output mode for the channel *
 *                       to. (see definition for TimerCompareOutputMode      *
 *                       structure for more information)                     *
 *                                                                           *
 *****************************************************************************/
void setTimer0CompareOutputMode(TimerChannel channel, TimerCompareOutputMode com);

/*****************************************************************************
 * Description: Sets the clock select mode for timer1 to the specified value *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: cs - Clock select value (see definition for TimerClockSelect) *
 *                                                                           *
 *****************************************************************************/
void setTimer1ClockSelect(TimerClockSelect cs);

/*****************************************************************************
 * Description: Sets the Waveform generation mode for timer1                 *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: mode - WGM mode (see definition for TimerWGM structure)       *
 *                                                                           *
 *****************************************************************************/
void setTimer1WGMMode(TimerWGM mode);

/*****************************************************************************
 * Description: Sets the compare output mode (COM) for timer1                *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: channel - which channel (A, B, C) to set the compare output   *
 *                       mode for.                                           *
 *             com     - what to set the compare output mode for the channel *
 *                       to. (see definition for TimerCompareOutputMode      *
 *                       structure for more information)                     *
 *                                                                           *
 *****************************************************************************/
void setTimer1CompareOutputMode(TimerChannel channel, TimerCompareOutputMode com);

/*****************************************************************************
 * Description: Sets the clock select mode for timer3 to the specified value *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: cs - Clock select value (see definition for TimerClockSelect) *
 *                                                                           *
 *****************************************************************************/
void setTimer3ClockSelect(TimerClockSelect cs);

/*****************************************************************************
 * Description: Sets the Waveform generation mode for timer3                 *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: mode - WGM mode (see definition for TimerWGM structure)       *
 *                                                                           *
 *****************************************************************************/
void setTimer3WGMMode(TimerWGM mode);

/*****************************************************************************
 * Description: Sets the compare output mode (COM) for timer3                *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: channel - which channel (A, B, C) to set the compare output   *
 *                       mode for.                                           *
 *             com     - what to set the compare output mode for the channel *
 *                       to. (see definition for TimerCompareOutputMode      *
 *                       structure for more information)                     *
 *                                                                           *
 *****************************************************************************/
void setTimer3CompareOutputMode(TimerChannel channel, TimerCompareOutputMode com);

#endif /* _timer_H_ */
