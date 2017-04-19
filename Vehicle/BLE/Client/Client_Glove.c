/*****************************************************************************
* FILENAME: Client_Glove.h                                                   *
*                                                                            *
* DESCRIPTION: Glove Bluetooth Client implementation for handling data       *
*              received from the glove ble service.                          *
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
* | @01     | 19Apr17  | BNordland  | Notification Enable Improvements    |  *
*  ------------------------------------------------------------------------  *
******************************************************************************/

#include "Client_Glove.h"

#include "Config_Hardware.h"

// Private Structures

// Glove Client Characteristics Data Structure
typedef struct
{
    uint16_t    anglePitch_handle;      // handle of the angle pitch characteristic as provided by a discovery.
    uint16_t    anglePitch_cccd_handle; // handle of the CCCD of the angle pitch characteristic as provided by a discovery
    uint16_t    throttle_handle;        // handle of the throttle characteristic as provided by a discovery.
    uint16_t    throttle_cccd_handle;   // handle of the CCCD of the throttle characteristic as provided by a discovery
    uint16_t    direction_handle;       // handle of the direction characteristic as provided by a discovery.
    uint16_t    direction_cccd_handle;  // handle of the CCCD of the direction characteristic as provided by a discovery
} Client_Glove_Handles_t;

// The client data
typedef struct Client_Glove_Data
{
    uint8_t                      uuid_type;          // UUID Type
    uint16_t                     conn_handle;        // Handle of the current connection.
    Client_Glove_Handles_t       handles;            // Handles on the connected peer device needed to interact with it.
    Client_Glove_Event_Handler_t evt_handler;        // Application event handler to be called when there is an event
    bool                         pitch_notify_enable; // Indicates if the throttle notifications have been enabled
    bool                         throttle_notify_enable; // Indicates if the direction notifications have been enabled
    bool                         notifyEnableComplete;   // @01a Indicates if all notifications are enabled.
} Client_Glove_Data_t;

// Private variables
static Client_Glove_Data_t   mClientData; // Note: since we only accept from one glove, we have simplified the client glove
                                          //       implementation to be a singleton rather than requiring the application to
                                          //       track client data.

// Private functions
static void pNotifyApplication(const ble_evt_t * event); // used to dispatch the application event handler
static uint32_t pEnableNotifications(); // Used to enable all notifications
static uint32_t pConfigureNotification(uint16_t cccdHandle, bool enable); // Enable or Disable Notifications


/*****************************************************************************
 ****************Start of Public Function Implementations ********************
 *****************************************************************************/


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
uint32_t Client_Glove_Init(Client_Glove_Init_t * initData)
{
    uint32_t      err_code;
    ble_uuid_t    glove_uuid;
    ble_uuid128_t base_uuid = BLE_UUID_GLOVE_BASE_UUID;

    if(initData == NULL)
    {
        return NRF_ERROR_NULL;
    }

    // Register with the bluetooth service discovery module, so that
    // it will look for our uuid.
    err_code = sd_ble_uuid_vs_add(&base_uuid, &mClientData.uuid_type);
    if(err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Set the UUID
    glove_uuid.type = mClientData.uuid_type;
    glove_uuid.uuid = BLE_UUID_GLOVE_SERVICE;

    // Set the event handler
    mClientData.evt_handler = initData->evt_handler;

    // Invalidate all handles
    mClientData.conn_handle                     = BLE_CONN_HANDLE_INVALID;
    mClientData.handles.anglePitch_handle       = BLE_GATT_HANDLE_INVALID;
    mClientData.handles.anglePitch_cccd_handle  = BLE_GATT_HANDLE_INVALID;
    mClientData.handles.throttle_handle         = BLE_GATT_HANDLE_INVALID;
    mClientData.handles.throttle_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    mClientData.handles.direction_handle        = BLE_GATT_HANDLE_INVALID;
    mClientData.handles.direction_cccd_handle   = BLE_GATT_HANDLE_INVALID;
    mClientData.pitch_notify_enable             = false; // @01a
    mClientData.throttle_notify_enable          = false; // @01a
    mClientData.notifyEnableComplete            = false; // @01a

    return ble_db_discovery_evt_register(&glove_uuid);
}


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
void Client_Glove_DiscoveryEventHandler(ble_db_discovery_evt_t * event)
{
    ble_gatt_db_char_t * characteristics = event->params.discovered_db.charateristics;

    // Check if the Glove Service was discovered.
    if (event->evt_type == BLE_DB_DISCOVERY_COMPLETE &&
        event->params.discovered_db.srv_uuid.uuid == BLE_UUID_GLOVE_SERVICE &&
        event->params.discovered_db.srv_uuid.type == mClientData.uuid_type)
    {

        uint32_t i;

        // set the characteristics handles
        for (i = 0; i < event->params.discovered_db.char_count; i++)
        {
            switch (characteristics[i].characteristic.uuid.uuid)
            {
                case BLE_UUID_GLOVE_ANGLEPITCH_CHARACTERISTC_UUID:
                {
                    mClientData.handles.anglePitch_handle = characteristics[i].characteristic.handle_value;
                    mClientData.handles.anglePitch_cccd_handle = characteristics[i].cccd_handle;
                    break;
                }
                case BLE_UUID_GLOVE_THROTTLE_CHARACTERISTC_UUID:
                {
                    mClientData.handles.throttle_handle = characteristics[i].characteristic.handle_value;
                    mClientData.handles.throttle_cccd_handle = characteristics[i].cccd_handle;
                    break;
                }
                case BLE_UUID_GLOVE_DIRECTION_CHARACTERISTC_UUID:
                {
                    mClientData.handles.direction_handle = characteristics[i].characteristic.handle_value;
                    mClientData.handles.direction_cccd_handle = characteristics[i].cccd_handle;
                    break;
                }
                default:
                {
                    //break;
                }
            }
        }

        // Set the connection handle
        mClientData.conn_handle = event->conn_handle;

        // enable notifications
        pEnableNotifications();
    }
}

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
void Client_Glove_BLEEventHandler(const ble_evt_t * event)
{
    // Verify that we were passed an event, otherwise
    // there is nothing to do.
    if (event == NULL)
    {
        return;
    }

    if ((mClientData.conn_handle != BLE_CONN_HANDLE_INVALID)
       && (mClientData.conn_handle != event->evt.gap_evt.conn_handle)
       )
    {
        // If we are not connected, there is nothing to do,
        // simply return.
        // Likewise, if the connection handle is not for us,
        // don't do anything.
        return;
    }

    switch (event->header.evt_id)
    {
        case BLE_GATTC_EVT_HVX:
            // notification received, process it and call the event handler
            pNotifyApplication(event);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            if (event->evt.gap_evt.conn_handle == mClientData.conn_handle
                    && mClientData.evt_handler != NULL)
            {
                Client_Glove_Event_t gloveEvent;

                gloveEvent.evt_type = Client_Glove_Event_DISCONNECTED;

                mClientData.conn_handle = BLE_CONN_HANDLE_INVALID;
                mClientData.throttle_notify_enable          = false; // @01a reset notification enables on disconnect
                mClientData.pitch_notify_enable             = false; // @01a reset notification enables on disconnect
                mClientData.notifyEnableComplete            = false; // @01a reset notification enables on disconnect
                mClientData.evt_handler(&gloveEvent);
            }
            break;
    }
}

/*****************************************************************************
 ****************Start of Private Function Implementations *******************
 *****************************************************************************/

/*****************************************************************************
 * Description: Given a notification event, calls the applications event     *
 *              handler with the correct type of event and with the          *
 *              appropriate data.                                            *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: None                                                          *
 *                                                                           *
 *****************************************************************************/
static void pNotifyApplication(const ble_evt_t * event)
{
    // figure out what type of notification event was sent.
    if ( (mClientData.handles.anglePitch_handle != BLE_GATT_HANDLE_INVALID)
            && (event->evt.gattc_evt.params.hvx.handle == mClientData.handles.anglePitch_handle)
            && (mClientData.evt_handler != NULL)
        )
    {

        // If it is an angle pitch update notification, call the appropriate
        // event handler in the application.
        Client_Glove_Event_t notifyEventData;

        notifyEventData.evt_type = Client_Glove_Event_ANGLEPITCH_UPDATED;
        notifyEventData.p_data   = (uint8_t *)event->evt.gattc_evt.params.hvx.data;
        notifyEventData.data_len = event->evt.gattc_evt.params.hvx.len;

        // @01a - Only send notifications if all notifications are enabled
        if(mClientData.notifyEnableComplete)
        {
            mClientData.evt_handler(&notifyEventData);
        }
        else if(!mClientData.pitch_notify_enable)
        {
            pConfigureNotification(mClientData.handles.throttle_cccd_handle, true);
            mClientData.pitch_notify_enable = true;
        }
    }
    else if ( (mClientData.handles.throttle_handle != BLE_GATT_HANDLE_INVALID)
            && (event->evt.gattc_evt.params.hvx.handle == mClientData.handles.throttle_handle)
            && (mClientData.evt_handler != NULL)
        )
    {
        // If it is a throttle update notification, call the appropriate
        // event handler in the application.
        Client_Glove_Event_t notifyEventData;

        notifyEventData.evt_type = Client_Glove_Event_THROTTLE_UPDATED;
        notifyEventData.p_data   = (uint8_t *)event->evt.gattc_evt.params.hvx.data;
        notifyEventData.data_len = event->evt.gattc_evt.params.hvx.len;

        // @01a - Only send notifications if all notifications are enabled
        if(mClientData.notifyEnableComplete)
        {
            mClientData.evt_handler(&notifyEventData);
        }
        else if(!mClientData.throttle_notify_enable)
        {
            pConfigureNotification(mClientData.handles.direction_cccd_handle, true);
            mClientData.throttle_notify_enable = true;
        }
    }
    else if ( (mClientData.handles.direction_handle != BLE_GATT_HANDLE_INVALID)
            && (event->evt.gattc_evt.params.hvx.handle == mClientData.handles.direction_handle)
            && (mClientData.evt_handler != NULL)
        )
    {

        // If it is a throttle update notification, call the appropriate
        // event handler in the application.
        Client_Glove_Event_t notifyEventData;

        notifyEventData.evt_type = Client_Glove_Event_DIRECTION_UPDATED;
        notifyEventData.p_data   = (uint8_t *)event->evt.gattc_evt.params.hvx.data;
        notifyEventData.data_len = event->evt.gattc_evt.params.hvx.len;

        // @01a - Only send notifications if all notifications are enabled
        if(mClientData.notifyEnableComplete)
        {
            mClientData.evt_handler(&notifyEventData);
        }
        else
        {
            mClientData.notifyEnableComplete = true;
        }
    }
}

/*****************************************************************************
 * Description: Enables all notifications for our three characteristics      *
 *                                                                           *
 * Returns: NRF_SUCCESS if notifications were successfully enabled or        *
 *          disabled or a bluetooth stack error otherwise.                   *
 *                                                                           *
 * Parameters: None                                                          *
 *                                                                           *
 *****************************************************************************/
static uint32_t pEnableNotifications()
{
    // Error validation
    if(mClientData.conn_handle == BLE_CONN_HANDLE_INVALID)
    {
        return NRF_ERROR_INVALID_STATE;
    }

    // Note this will trigger a cascading effect.
    // Once notifications are enabled for the pitch angle, then,
    // notifications will be enabled for the throttle and direction.
    // The client will not be notified until notifications have been enabled
    // for all three.
    pConfigureNotification(mClientData.handles.anglePitch_cccd_handle, true);

    return 0;//returnCode;
}

/*****************************************************************************
 * Description: Configures a particular notification, by writing to the      *
 *              cccd the enable or disable byte for notifications.           *
 *                                                                           *
 * Returns: NRF_SUCCESS if notifications were successfully enabled or        *
 *          disabled or a bluetooth stack error otherwise.                   *
 *                                                                           *
 * Parameters: cccdHandle -> the bluetooth cccd handle for the characteristic*
 *                           that notifications are being enabled or         *
 *                           disabled for.                                   *
 *             enable     -> true to enable notifications, false to disable  *
 *                                                                           *
 *****************************************************************************/
static uint32_t pConfigureNotification(uint16_t cccdHandle, bool enable)
{
    // Configure or deconfigure notifications
    uint8_t gattValue[BLE_CCCD_VALUE_LEN];
    gattValue[0] = enable ? BLE_GATT_HVX_NOTIFICATION : 0;
    gattValue[1] = 0;

    // Set the cccd parameters
    const ble_gattc_write_params_t write_params = {
        .write_op = BLE_GATT_OP_WRITE_REQ,
        .flags    = BLE_GATT_EXEC_WRITE_FLAG_PREPARED_WRITE,
        .handle   = cccdHandle,
        .offset   = 0,
        .len      = sizeof(gattValue),
        .p_value  = gattValue
    };

    return sd_ble_gattc_write(mClientData.conn_handle, &write_params);
}
