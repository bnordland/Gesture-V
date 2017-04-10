/*****************************************************************************
* FILENAME: Sensors_AccelGyro.h                                              *
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

#ifndef _SENSORS_ACCELGYRO_H
#define _SENSORS_ACCELGYRO_H

#include <stdint.h>

// A structure that holds accelerometer data
typedef struct
{
    int16_t    xData;
    int16_t    yData;
    int16_t    zData;
} Sensors_Accel_Data_t;

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
void Sensors_AccelGyro_Init();

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
void Sensors_AccelGyro_GetAccelerometerData(Sensors_Accel_Data_t * data);

#endif // _SENSORS_ACCELGYRO_H
