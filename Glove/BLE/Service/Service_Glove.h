/*****************************************************************************
* FILENAME: Service_Glove.h                                                  *
*                                                                            *
* DESCRIPTION: Glove Bluetooth Service for sending data from glove to the    *
*              vehicle.                                                      *
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

#ifndef SERVICE_GLOVE_H__
#define SERVICE_GLOVE_H__

#include <stdint.h>
#include "ble.h"
#include "ble_srv_common.h"

// UUID Definitions
#define BLE_UUID_GLOVE_BASE_UUID              {{0x76, 0xdb, 0xcd, 0xad, 0x65, 0x39, \
                                                0x3e, 0x5b, 0xe2, 0x43, 0x2d, 0x2d, \
                                                0x00, 0x00, 0x00, 0x00}} // 128-bit base UUID

#define BLE_UUID_GLOVE_SERVICE                0x1000 // 16-bit Glove service UUID


/*****************************************************************************
 * Description: Handles all events from the Nordic bluetooth static related  *
 *              to the glove service                                         *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters:                                                               *
 *                                                                           *
 *****************************************************************************/
void Service_Glove_BluetoothEventHandler(ble_evt_t * event);


/*****************************************************************************
 * Description: Initializes the Glove Service                                *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: None                                                          *
 *                                                                           *
 *****************************************************************************/
void Service_Glove_Init();


/*****************************************************************************
 * Description: Indicates if the service has a valid connection              *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: None                                                          *
 *                                                                           *
 *****************************************************************************/
bool Service_Glove_IsConnected();

/*****************************************************************************
 * Description: Updates the AnglePitch Characteristic and sends it to         *
 *              connected bluetooth device                                   *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters:                                                               *
 *      int16_t *anglePitch - The Pitch angle of the glove                     *
 *                                                                           *
 *****************************************************************************/
void Service_Glove_SetAnglePitch(int16_t *anglePitch);

#endif  /* _ SERVICE_GLOVE_H__ */
