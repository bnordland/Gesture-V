/************************************************************************
* FILENAME: hardware.h                                                  *
*                                                                       *
* DESCRIPTION: Central location for pin definitions                     *
*                                                                       *
* LICENSE: The MIT License (MIT)                                        *
*          Copyright (c) 2017 Brian Nordland                            *
*                                                                       *
* AUTHOR:  Brian Nordland                                               *
*                                                                       *
* --------------------------------------------------------------------  *
* | Change  | Date     |            |                                 | *
* | Flag    | (DDMYY)  | Author     | Description                     | *
* |---------|----------|------------|---------------------------------  *
* | None    | 26Apr17  | BNordland  | Initial creation                | *
* | @01     | 30Apr17  | BNordland  | Adding BLE Nano Power Wiring    | *
*  -------------------------------------------------------------------  *
*************************************************************************/

#ifndef _hardware_H_
#define _hardware_H_

    // motor location definitions
    #define motor1EncoderChannelADDR    DDRB
    #define motor1EncoderChannelAPIN    DDB4
    #define motor1EncoderChannelAINT    PCINT4
    #define motor1EncoderChannelBDDR    DDRB
    #define motor1EncoderChannelBPIN    DDB7
    #define motor1EncoderChannelBINT    PCINT7
    #define motor1EncoderPowerDDR       DDRC
    #define motor1EncoderPowerPin       DDC6
    #define motor1EncoderPowerPort      PORTC


    // Note: Channel A and Channel B pins are flipped from wires
    //       in this config. This is so that we can go forward
    //       with motor2 with increasing counts just like motor1
    #define motor2EncoderChannelBDDR    DDRD
    #define motor2EncoderChannelBPIN    DDD3
    #define motor2EncoderChannelBINT    INT3
    #define motor2EncoderChannelADDR    DDRD
    #define motor2EncoderChannelAPIN    DDD1
    #define motor2EncoderChannelAINT    INT1
    #define motor2EncoderPowerDDR       DDRD
    #define motor2EncoderPowerPin       DDD4
    #define motor2EncoderPowerPort      PORTD

    // BLE Nano Power  @01a
    #define EXT_BLE_DDR      DDRF
    #define EXT_BLE_DDRBIT   DDF0
    #define EXT_BLE_PORT PORTF
    #define EXT_BLE_PORTBIT  PORTF0

#endif // _hardware_H_
