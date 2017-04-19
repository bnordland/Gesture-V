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

typedef enum {CS0, CS1, CS8, CS64, CS256, CS1024 } TimerClockSelect;
typedef enum {COMDisconnected,COMFastPWMClearCM, COMFastPWMSetCM} TimerCompareOutputMode;
typedef enum {ChannelA, ChannelB, ChannelC} TimerChannel;

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

void setTimer3ClockSelect(TimerClockSelect cs);
void setTimer3WGMMode(TimerWGM mode);
void setTimer3CompareOutputMode(TimerChannel channel, TimerCompareOutputMode com);

void setTimer1ClockSelect(TimerClockSelect cs);
void setTimer1WGMMode(TimerWGM mode);
void setTimer1CompareOutputMode(TimerChannel channel, TimerCompareOutputMode com);


void setTimer0ClockSelect(TimerClockSelect cs);
void setTimer0WGMMode(TimerWGM mode);
void setTimer0CompareOutputMode(TimerChannel channel, TimerCompareOutputMode com);

#endif /* _timer_H_ */
