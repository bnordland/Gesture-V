/*****************************************************************************
* FILENAME: Comm_SPI.h                                                       *
*                                                                            *
* DESCRIPTION: Simple SPI communication wrapper                              *
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
* | None    | 08Apr17  | BNordland  | Initial creation                    |  *
*  ------------------------------------------------------------------------  *
******************************************************************************/

#ifndef COMM_SPI_H__
#define COMM_SPI_H__

#include <stdint.h>

/*****************************************************************************
 * Description: Initializes the SPI interface to be ready for data transfer  *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: None                                                          *
 *                                                                           *
 *****************************************************************************/
void Comm_SPI_Init();


/*****************************************************************************
 * Description: A serial transmit of data followed by receive from SPI       *
 *              interface. This works by doing the following:                *
 *                 1. Transmit txBytes of txBuffer via SPI                   *
 *                 2. Then Receive rxBytes into rxBuffer via SPI             *
 *                                                                           *
 *              Note: This works different from SPI which has the ability    *
 *                    to transmit the data and receive at the same time      *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters:                                                               *
 *     txBuffer* - A byte buffer of bytes to transmit                        *
 *     txBytes   - Number of bytes to transmit (max 8)                       *
 *     rxBuffer* - A byte buffer of bytes to receive                         *
 *     rxBytes   - Number of bytes to receive (max 8)                        *
 *                                                                           *
 *****************************************************************************/
void Comm_SPI_Transfer(uint8_t * txBuffer, uint8_t txBytes, uint8_t * rxBuffer, uint8_t rxBytes);


#endif /* _ COMM_SPI_H__ */
