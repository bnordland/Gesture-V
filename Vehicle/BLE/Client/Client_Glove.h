/*****************************************************************************
* FILENAME: Client_Glove.h                                                   *
*                                                                            *
* DESCRIPTION: Glove Bluetooth Client for handling data received from the    *
*              glove ble service.                                            *
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
* | None    | 16Apr17  | BNordland  | Initial creation                    |  *
*  ------------------------------------------------------------------------  *
******************************************************************************/

#ifndef _CLIENT_GLOVE_H_
#define _CLIENT_GLOVE_H_

#include <stdint.h>
#include "NordicSDK.h"

// UUID definitions
#define BLE_UUID_GLOVE_BASE_UUID              {{0x76, 0xdb, 0xcd, 0xad, 0x65, 0x39, \
                                                0x3e, 0x5b, 0xe2, 0x43, 0x2d, 0x2d, \
                                                0x00, 0x00, 0x00, 0x00}} // 128-bit base UUID

#define BLE_UUID_GLOVE_SERVICE                              0x1000 // 16-bit Glove service UUID

// 16-bit characteristic UUIDs
#define BLE_UUID_GLOVE_ANGLEPITCH_CHARACTERISTC_UUID        0x1001 // Glove Service AnglePitch Characterstic
#define BLE_UUID_GLOVE_THROTTLE_CHARACTERISTC_UUID          0x10A0 // Glove Service Throttle Characterstic
#define BLE_UUID_GLOVE_DIRECTION_CHARACTERISTC_UUID         0x10A1 // Glove Service Direction Characterstic


// The types of the events from the glove client
// For the most part these are when either characteristics
// are updated via a notification from the glove peripheral
// or when there is a disconnect event.
typedef enum
{
    Client_Glove_Event_ANGLEPITCH_UPDATED = 1, // Event indicating that the central device has received an updated pitch angle value
    Client_Glove_Event_THROTTLE_UPDATED,       // Event indicating that the central device has received an updated throttle value
    Client_Glove_Event_DIRECTION_UPDATED,      // Event indicating that the central device has received an updated direction value
    Client_Glove_Event_DISCONNECTED            // Event indicating that the Glove server (peripheral) has disconnected.
} Client_Glove_Event_Type_t;


// Event Data Structure
// This is given to the subscriber to
// Glove events.
typedef struct
{
    Client_Glove_Event_Type_t   evt_type; // The type of event.
    uint16_t                    conn_handle; // The connection handle for the glove client.
    uint8_t                     * p_data; // The data corresponding to the event type
    uint8_t                     data_len; // The length of the data received
} Client_Glove_Event_t;

/*****************************************************************************
 * Description: An event handler that the application must subscribe to      *
 *              in order to receive glove client events. This is to be       *
 *              implemented by the application consuming the glove client    *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: event -> event details (see structure above                   *
 *                                                                           *
 *****************************************************************************/
typedef void (* Client_Glove_Event_Handler_t)(const Client_Glove_Event_t * event);


// Client initialization structure.
// This is used to pass all data required to initialize the glove
// client. In our case the only interesting data is the event handler.
// This could be simplified to not require a structure, but is left
// in its complex form in case additional variables are added down the road.
typedef struct
{
    Client_Glove_Event_Handler_t evt_handler;
} Client_Glove_Init_t;


/*****************************************************************************
 * Description: For initializing the Glove Client module. This function      *
 *              will make sure that the database discovery module is looking *
 *              for the correct UUID type of the glove.                      *
 *              and will also register the application event handler passed  *
 *              to it. In addition, all variables for the Glove client are   *
 *              initialized to their default values.                         *
 *                                                                           *
 * Returns: NRF_SUCCESS if everything was initialized successfully.          *
 *          Otherwise and error code is returned. For example if the         *
 *          discovery module failed.                                         *
 *                                                                           *
 * Parameters: initData -> Data structure required to initialize the client. *
 *                         This includes the event handler of the application*
 *                                                                           *
 *****************************************************************************/
uint32_t Client_Glove_Init(Client_Glove_Init_t * initData);


/*****************************************************************************
 * Description: An event handler that is called when a discovery event       *
 *              occurs. This will check to see if the discovery event        *
 *              applies to the glove client, and then will set all handles   *
 *              for the connection and for the characteristics as well       *
 *              as enable notifications for the appropriate characteristics  *
 *              so that the client event handler can be called when          *
 *              there are updates to the data values.                        *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: event -> event from the discovery event                       *
 *                                                                           *
 *****************************************************************************/
void Client_Glove_DiscoveryEventHandler(ble_db_discovery_evt_t * event);


/*****************************************************************************
 * Description: Handles bluetooth events from the bluetooth stack.           *
 *              This will verify that the events are for our client.         *
 *              by validating connection handles.                            *
 *                                                                           *
 *              Types of events include:                                     *
 *                  Receiving a notification -> will call proper application *
 *                      event handler.                                       *
 *                  Disconnect -> will invalidate handles, and call the      *
 *                      application disconnect event handler.                *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: event -> event from the bluetooth stack                       *
 *                                                                           *
 *****************************************************************************/
void Client_Glove_BLEEventHandler(const ble_evt_t * p_ble_evt);


#endif
