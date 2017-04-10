/*****************************************************************************
* FILENAME: Sensors_AccelGyro.c                                              *
*                                                                            *
* DESCRIPTION: Library for communicating with an Accelerometer + Gyroscope   *
*              sensor: LSM6DS33 3D Accelerometer and Gyro Carrier with       *
*              Voltage Regulator                                             *
*              Pololu Product: https://www.pololu.com/product/2736/          *
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
* | None    | 09Apr17  | BNordland  | Initial creation                    |  *
*  ------------------------------------------------------------------------  *
******************************************************************************/

#include "Sensors_AccelGyro.h"

#include "Comm_SPI.h" // For SPI communication

// Register Definitions
#define WHO_AM_I        0x0F    // The device SPI WHO_AM_I register

#define CTRL1_XL        0x10    // Control register 1
#define CTRL9_XL        0x18    // Control register 9

#define STATUS_REG      0x1E    // The status register
    #define XLDA            0x1 // Accelerometer Data Available bit

// Accelerometer Output registers
#define OUTX_L_XL       0x28
#define OUTX_H_XL       0x29
#define OUTY_L_XL       0x2A
#define OUTY_H_XL       0x2B
#define OUTZ_L_XL       0x2C
#define OUTZ_H_XL       0x2D

// Constants
#define WHO_AM_I_ID     0x69    // The device SPI WHO_AM_I ID that should be in the WHO_AM_I register

// Private functions:
uint8_t pReadRegister(uint8_t registerAddress); // reads a register
void pWriteRegister(uint8_t registerAddress, uint8_t value);

/*****************************************************************************
 * Description: Initializes the device, and waits for the device to come     *
 *              online successfully.                                         *
 *              Note: If the device is not connected, this will not return   *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: None                                                          *
 *                                                                           *
 *****************************************************************************/
void Sensors_AccelGyro_Init()
{
    // Initialize Accelerometer via SPI
    Comm_SPI_Init(); // init SPI interface

    // Wait for accelerometer SPI device to become active.
    uint8_t data = 0x00;
    while(data != WHO_AM_I_ID)
    {
        data = pReadRegister(WHO_AM_I);
    }

    // Now that we are sure that the accelerometer is present, let's enable it

    // As per the application note to enable accelerometer
    // 1. Write CTRL9_XL = 38h // Enable accelerometer axis X, Y and Z
    pWriteRegister(CTRL9_XL,0x38);

    // 2. Write CTRL1_XL = 60h // Put the accelerometer in 416Hz (High-Performance mode)
    pWriteRegister(CTRL1_XL,0x60);
}

/*****************************************************************************
 * Description: Gets the accelerometer data. If data is not available,       *
 *              this will block until data is available.                     *
 *                                                                           *
 * Returns: None (data is returned via 'data' parameter.                     *
 *                                                                           *
 * Parameters:                                                               *
 *  Sensors_Accel_Data_t* data - The accelerometer data                      *
 *                                                                           *
 *****************************************************************************/
void Sensors_AccelGyro_GetAccelerometerData(Sensors_Accel_Data_t * data)
{
    // While there is no data available, keep waiting for XLDA to be 1
    while((pReadRegister(STATUS_REG) & XLDA) == 0)
    {
    }

    // Note: Looking at the data sheet it appears as though we may be able to read more
    //       efficiently if we set a control register appropriately, to be able to read the
    //       entire set of registers in one read.
    data->xData = ((uint16_t)pReadRegister(OUTX_H_XL) << 8) | (uint16_t)pReadRegister(OUTX_L_XL);
    data->yData = ((uint16_t)pReadRegister(OUTY_H_XL) << 8) | (uint16_t)pReadRegister(OUTY_L_XL);
    data->zData = ((uint16_t)pReadRegister(OUTZ_H_XL) << 8) | (uint16_t)pReadRegister(OUTZ_L_XL);
}

/*****************************************************************************
 * Description: Reads a register from the device                             *
 *                                                                           *
 * Returns: The register value                                               *
 *                                                                           *
 * Parameters:                                                               *
 *      registerAddress - The register to read                               *
 *****************************************************************************/
uint8_t pReadRegister(uint8_t registerAddress)
{
    // 0x80 = 1000 0000 which represents the following
    //   bit0=1 for read
    //   bit1-7 for address
    uint8_t tx = 0x80 | registerAddress;
    uint8_t rx = 0x00;
    Comm_SPI_Transfer(&tx, 1, &rx, 1);
    return rx;
}

/*****************************************************************************
 * Description: Writes a register value to the device                        *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters:                                                               *
 *      registerAddress - The register to read                               *
 *      value           - The value to write to the register                 *
 *****************************************************************************/
void pWriteRegister(uint8_t registerAddress, uint8_t value)
{
    uint8_t txData[2];

    // Makes sure that
    // bit0 is set to 0 for write
    // bit1-7 are as set in address
    txData[0] = 0x7F & registerAddress;

    // second byte is value
    txData[1] = value;
    Comm_SPI_Transfer(&txData[0], 2, 0, 0);
}
