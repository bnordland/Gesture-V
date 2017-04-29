/************************************************************************
* FILENAME: motor.h														*
*																		*
* DESCRIPTION: Motor Functions											*
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
* | None    | 11Feb17   | BNordland  | Initial creation				  | *
* | @01     | 2Mar17   | BNordland  | Update motor count to be 32bit  | *
*  -------------------------------------------------------------------	*
*************************************************************************/

#ifndef _motor_H_
#define _motor_H_

#include <stdint.h> // integer types

/************************************************************************
 * IMPORTANT USAGE INSTRUCTIONS:										*
 * 	Setting up motors requires certain definitions, failure to	    	*
 * 	set these will result in runtime crashes when calling motor setup   *
 * 	routine (i.e. setupMotor2()).										*
 * 																		*
 * 	The following definitions are required for motor 2:					*
 * 		#define motor2EncoderChannelADDR	<DDR (i.e. DDRB>			*
 * 		#define	motor2EncoderChannelAPIN	<DDRPIN (i.e. DDB4>			*
 *		#define motor2EncoderChannelAINT	<INTPIN (i.e. PCINT4>		*
 * 		#define motor2EncoderChannelBDDR	<DDR (i.e. DDRB>			*
 * 		#define	motor2EncoderChannelBPIN	<DDRPIN (i.e. DDB5>			*
 *		#define	motor2EncoderChannelBINT	<INTPIN (i.e. PCINT5>		*
 *		#define motor2EncoderPowerDDR		<DDR (i.e. DDRD>			*
 *		#define motor2EncoderPowerPin		<DDRPIN (i.e. DDD1>			*
 *		#define motor2EncoderPowerPort		<PORT (i.e. PORTD>			*
 *	The following definitions are required for motor 1:       	        *
 *		#define motor1EncoderChannelADDR    <DDR (i.e. DDRB>            *
 *      #define motor1EncoderChannelAPIN    <DDRPIN (i.e. DDB4>         *
 *      #define motor1EncoderChannelAINT    <INTPIN (i.e. PCINT4>       *
 *      #define motor1EncoderChannelBDDR    <DDR (i.e. DDRB>            *
 *      #define motor1EncoderChannelBPIN    <DDRPIN (i.e. DDB5>         *
 *      #define motor1EncoderChannelBINT    <INTPIN (i.e. PCINT5>       *
 *      #define motor1EncoderPowerDDR       <DDR (i.e. DDRD>            *
 *      #define motor1EncoderPowerPin       <DDRPIN (i.e. DDD1>         *
 *      #define motor1EncoderPowerPort      <PORT (i.e. PORTD>          *
 *      // PCIR, PCIE0, PCMSK0											*
 * 																		*
 * The below verifies that the defines have all been completed before   *
 * this header was included. 											*
 ************************************************************************/
#include "../hardware.h" // hardware definitions for project
#if defined motor2EncoderChannelADDR && defined motor2EncoderChannelAPIN \
 && defined motor2EncoderChannelAINT && defined motor2EncoderChannelBDDR \
 && defined motor2EncoderChannelBPIN && defined motor2EncoderChannelBINT \
 && defined motor2EncoderPowerDDR && defined motor2EncoderPowerPin \
 && defined motor2EncoderPowerPort
	// All motor 2 defines are okay
#else
	// one or more motor 2 defines are missing, throw a warning.
	#warning Motor2 variables not all defined. Usage of motor 2 will fail.
	#define motor2Error
	#ifndef motor2EncoderChannelADDR
		#define motor2EncoderChannelADDR DDRB
	#endif
	#ifndef motor2EncoderChannelAPIN
		#define motor2EncoderChannelAPIN 0
	#endif
	#ifndef motor2EncoderChannelAINT
		#define motor2EncoderChannelAINT 0
	#endif
	#ifndef motor2EncoderChannelBDDR
		#define motor2EncoderChannelBDDR DDRB
	#endif
	#ifndef motor2EncoderChannelBPIN
		#define motor2EncoderChannelBPIN 0
	#endif
	#ifndef motor2EncoderChannelBINT
		#define motor2EncoderChannelBINT 0
	#endif
 	#ifndef motor2EncoderPowerDDR
		#define motor2EncoderPowerDDR DDRD
	#endif
 	#ifndef motor2EncoderPowerPin
		#define motor2EncoderPowerPin 0
	#endif
 	#ifndef motor2EncoderPowerPort
		#define motor2EncoderPowerPort PORTD
	#endif
#endif

#if defined motor1EncoderChannelADDR && defined motor1EncoderChannelAPIN \
 && defined motor1EncoderChannelAINT && defined motor1EncoderChannelBDDR \
 && defined motor1EncoderChannelBPIN && defined motor1EncoderChannelBINT \
 && defined motor1EncoderPowerDDR && defined motor1EncoderPowerPin \
 && defined motor1EncoderPowerPort
    // All motor 1 defines are okay
#else
    // one or more motor 1 defines are missing, throw a warning.
    #warning Motor1 variables not all defined. Usage of motor 2 will fail.
    #define motor1Error
    #ifndef motor1EncoderChannelADDR
        #define motor1EncoderChannelADDR DDRB
    #endif
    #ifndef motor1EncoderChannelAPIN
        #define motor1EncoderChannelAPIN 0
    #endif
    #ifndef motor1EncoderChannelAINT
        #define motor1EncoderChannelAINT 0
    #endif
    #ifndef motor1EncoderChannelBDDR
        #define motor1EncoderChannelBDDR DDRB
    #endif
    #ifndef motor1EncoderChannelBPIN
        #define motor1EncoderChannelBPIN 0
    #endif
    #ifndef motor1EncoderChannelBINT
        #define motor1EncoderChannelBINT 0
    #endif
    #ifndef motor1EncoderPowerDDR
        #define motor1EncoderPowerDDR DDRD
    #endif
    #ifndef motor1EncoderPowerPin
        #define motor1EncoderPowerPin 0
    #endif
    #ifndef motor1EncoderPowerPort
        #define motor1EncoderPowerPort PORTD
    #endif
#endif




// Definitions useful for callers of the motor
#define MOTOR_COUNTSPERREV 2248.86 // the number of counts per revolution
#define MOTOR_COUNTSPERDEGREE 6.24683 // number of counts per degree

void setupMotor2();
void calibrateMotor2();
void handleMotor2Interrupt();
void setMotor2On();
void setMotor2Off();
void setMotor2DutyCycle(uint16_t dutyCycle);
void setMotor2Forward();
void setMotor2Backward();
void returnMotor2ToRefPosition();

/**************************************************
 * Function Definition: resetMotor2Count()
 *
 * Description: Resets the counter for motor2
 *
 * Parameters: None
 *
 * Returns: None
***************************************************/
void resetMotor2Count();

/**************************************************
 * Function Definition: getMotor2Count()
 *
 * Description: Gets the current counter for motor2
 *
 * Parameters: None
 *
 * Returns: The current encoder count.
 *          @01c - update to be 32bit instead of 16
***************************************************/
int32_t getMotor2Count();



void setupMotor1();
void calibrateMotor1();
void handleMotor1Interrupt();
void setMotor1On();
void setMotor1Off();
void setMotor1DutyCycle(uint16_t dutyCycle);
void setMotor1Forward();
void setMotor1Backward();
void returnMotor1ToRefPosition();

/**************************************************
 * Function Definition: resetMotor2Count()
 *
 * Description: Resets the counter for motor2
 *
 * Parameters: None
 *
 * Returns: None
***************************************************/
void resetMotor1Count();

/**************************************************
 * Function Definition: getMotor2Count()
 *
 * Description: Gets the current counter for motor2
 *
 * Parameters: None
 *
 * Returns: The current encoder count.
 *          @01c - update to be 32bit instead of 16
***************************************************/
int32_t getMotor1Count();

#endif
