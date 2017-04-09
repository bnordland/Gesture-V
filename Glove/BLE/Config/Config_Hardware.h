/*****************************************************************************
* FILENAME: Config_Hardware.h                                                *
*                                                                            *
* DESCRIPTION: Contains Hardware PIN Configurations                          *
*                                                                            *
* AUTHOR:  Brian Nordland                                                    *
*                                                                            *
* LICENSE: The MIT License (MIT)                                             *
*          Copyright (c) 2017 Brian Nordland                                 *
*                                                                            *
*                                                                            *
* ------------------------------------------------------------------------   *
* | Change  | Date     |            |                                     |  *
* | Flag    | (DDMYY)  | Author     | Description                         |  *
* |---------|----------|------------|-------------------------------------   *
* | None    | 07Apr17  | BNordland  | Initial creation                    |  *
*  ------------------------------------------------------------------------  *
******************************************************************************/

// The location of the onboard LED
#ifndef HDW_CONFIG_ONBOARD_LED_PIN
    #define HDW_CONFIG_ONBOARD_LED_PIN 19
#endif

// The location of the SPI PINs
// according the the BLE NANO pinout guide: http://redbearlab.com/blenano/
// P10: CS   (Slave Select)
// P9:  MOSI (Master Out/Slave In)
// P11: MISO (Master In/Slave Out)
// P8:  SCK (System Clock)
#ifndef HDW_CONFIG_SPI_SS_PIN
    #define HDW_CONFIG_SPI_SS_PIN 10
#endif
#ifndef HDW_CONFIG_SPI_MOSI_PIN
    #define HDW_CONFIG_SPI_MOSI_PIN 9
#endif
#ifndef HDW_CONFIG_SPI_MISO_PIN
    #define HDW_CONFIG_SPI_MISO_PIN 11
#endif
#ifndef HDW_CONFIG_SPI_SCK_PIN
    #define HDW_CONFIG_SPI_SCK_PIN 8
#endif
