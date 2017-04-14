/*****************************************************************************
* FILENAME: Service_Glove.c                                                  *
*                                                                            *
* DESCRIPTION: Glove Bluetooth Service implementation for sending data from  *
*              the glove to the vehicle.                                     *
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

#include <stdint.h>
#include <string.h>
#include "Service_Glove.h"
#include "ble_srv_common.h"
#include "app_error.h"

// 16-bit characteristic UUIDs
#define BLE_UUID_GLOVE_ANGLEPITCH_CHARACTERISTC_UUID          0x1001 // Glove Service AnglePitch Characterstic
#define BLE_UUID_GLOVE_THROTTLE_CHARACTERISTC_UUID          0x10A0 // Glove Service Throttle Characterstic
#define BLE_UUID_GLOVE_DIRECTION_CHARACTERISTC_UUID          0x10A1 // Glove Service Direction Characterstic

// Type Definitions (Private service variables)
typedef struct
{
    uint16_t    conn_handle;         // Current Connection Handle -> provided by bluetooth stack.
    uint16_t    service_handle;      // Handle for the Glove Service -> provided by the bluetooth stack.
    ble_gatts_char_handles_t anglePitch_char_handles; // Handle for the pitch characteristic
    ble_gatts_char_handles_t direction_char_handles; // Handle for the direction characteristic
    ble_gatts_char_handles_t throttle_char_handles; // Handle for the throttle characteristic
} Service_Glove_t;


// Internal Function Definitions
uint32_t pAddCharacteristics();
uint32_t pAddCharacteristicImpl(uint16_t characteristicUUID, char user_desc[],
                                    uint8_t attributeMaxLen, uint8_t attributeInitLen, uint8_t * attributeValue, ble_gatts_char_handles_t* char_handles);

// Internal Global Variables
static Service_Glove_t mGloveService;

/*****************************************************************************
 * Description: Handles all events from the Nordic bluetooth static related  *
 *              to the glove service                                         *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters:                                                               *
 *                                                                           *
 *****************************************************************************/
void Service_Glove_BluetoothEventHandler(ble_evt_t * event)
{
    // Implement switch case handling BLE events related to our service.
    switch (event->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            mGloveService.conn_handle = event->evt.gap_evt.conn_handle;
            break;
        case BLE_GAP_EVT_DISCONNECTED:
            mGloveService.conn_handle = BLE_CONN_HANDLE_INVALID;
            break;
        default:
            // No implementation needed.
            break;
    }
}

/*****************************************************************************
 * Description: Initializes the Glove Service                                *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: None                                                          *
 *                                                                           *
 *****************************************************************************/
void Service_Glove_Init()
{
    // Declare 16 bit service and 128 bit base UUIDs and add them to BLE stack table
    uint32_t   err_code;
    ble_uuid_t        service_uuid;
    ble_uuid128_t     base_uuid = BLE_UUID_GLOVE_BASE_UUID;
    service_uuid.uuid = BLE_UUID_GLOVE_SERVICE;
    err_code = sd_ble_uuid_vs_add(&base_uuid, &service_uuid.type);
    APP_ERROR_CHECK(err_code);

    // Set our service connection handle to default value. I.e. an invalid handle since we are not yet in a connection.
    mGloveService.conn_handle = BLE_CONN_HANDLE_INVALID;

    // Add our service
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &service_uuid,
                                        &mGloveService.service_handle);
    APP_ERROR_CHECK(err_code);

    // Add the characteristics to the service
    pAddCharacteristics();
}

/*****************************************************************************
 * Description: Indicates if the service has a valid connection              *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: None                                                          *
 *                                                                           *
 *****************************************************************************/
bool Service_Glove_IsConnected()
{
    return (mGloveService.conn_handle != BLE_CONN_HANDLE_INVALID);
}

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
void Service_Glove_SetAnglePitch(int16_t *anglePitch)
{
    // Update characteristic value
    if (mGloveService.conn_handle != BLE_CONN_HANDLE_INVALID)
    {
        uint16_t               len = 2;
        ble_gatts_hvx_params_t hvx_params;
        memset(&hvx_params, 0, sizeof(hvx_params));

        hvx_params.handle = mGloveService.anglePitch_char_handles.value_handle;
        hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = 0;
        hvx_params.p_len  = &len;
        hvx_params.p_data = (uint8_t*)anglePitch;

        sd_ble_gatts_hvx(mGloveService.conn_handle, &hvx_params);
    }
}

/*****************************************************************************
 * Description: Updates the Throttle Characteristic and sends it to          *
 *              connected bluetooth device                                   *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters:                                                               *
 *      uint8_t *throttle - The throttle value of the glove                  *
 *                                                                           *
 *****************************************************************************/
void Service_Glove_SetThrottle(uint8_t *throttle)
{
    // Update characteristic value
    if (mGloveService.conn_handle != BLE_CONN_HANDLE_INVALID)
    {
        uint16_t               len = 1;
        ble_gatts_hvx_params_t hvx_params;
        memset(&hvx_params, 0, sizeof(hvx_params));

        hvx_params.handle = mGloveService.throttle_char_handles.value_handle;
        hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = 0;
        hvx_params.p_len  = &len;
        hvx_params.p_data = (uint8_t*)throttle;

        sd_ble_gatts_hvx(mGloveService.conn_handle, &hvx_params);
    }
}

/*****************************************************************************
 * Description: Updates the Direction Characteristic and sends it to         *
 *              connected bluetooth device                                   *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters:                                                               *
 *      uint8_t *throttle - The throttle value of the glove                  *
 *                                                                           *
 *****************************************************************************/
void Service_Glove_SetDirection(uint8_t *direction)
{
    // Update characteristic value
    if (mGloveService.conn_handle != BLE_CONN_HANDLE_INVALID)
    {
        uint16_t               len = 1;
        ble_gatts_hvx_params_t hvx_params;
        memset(&hvx_params, 0, sizeof(hvx_params));

        hvx_params.handle = mGloveService.direction_char_handles.value_handle;
        hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = 0;
        hvx_params.p_len  = &len;
        hvx_params.p_data = (uint8_t*)direction;

        sd_ble_gatts_hvx(mGloveService.conn_handle, &hvx_params);
    }
}

/*****************************************************************************
 * Description: Adds the characteristics to the service in the bluetooth     *
 *              stack.                                                       *
 *                                                                           *
 * Returns: Return code as per Nordic SDK                                    *
 *                                                                           *
 * Parameters: None                                                          *
 *                                                                           *
 *****************************************************************************/
uint32_t pAddCharacteristics()
{
    // Add the AnglePitch characteristic and set the initial value to 0
    uint8_t anglePitchValue[2]            = {0x00,0x00};
    pAddCharacteristicImpl(BLE_UUID_GLOVE_ANGLEPITCH_CHARACTERISTC_UUID, "AnglePitch",2, 2, anglePitchValue, &mGloveService.anglePitch_char_handles);

    // Add the Direction characteristic and set the initial value to 0
    uint8_t DirectionValue[1]            = {0x00};
    pAddCharacteristicImpl(BLE_UUID_GLOVE_DIRECTION_CHARACTERISTC_UUID, "Direction",1, 1, DirectionValue, &mGloveService.direction_char_handles);

    // Add the Throttle characteristic and set the initial value to 0
    uint8_t ThrottleValue[1]             = {0x00};
    pAddCharacteristicImpl(BLE_UUID_GLOVE_THROTTLE_CHARACTERISTC_UUID, "Throttle",1, 1, ThrottleValue, &mGloveService.throttle_char_handles);

    return NRF_SUCCESS;
}

/*****************************************************************************
 * Description: Implementation that actually calls the bluetooth stack to    *
 *              add a particular characteristic.                             *
 *                                                                           *
 * Returns: Return code as per Nordic SDK                                    *
 *                                                                           *
 * Parameters:                                                               *
 *     characteristicUUID - The UUID of the characteristic to add            *
 *     user_desc - The user visible description of the characteristic        *
 *     attributeMaxLen - The maximum length of the attribute                 *
 *     attributeInitLen - The initial length of the attribute                *
 *     attributeValue - The initial value of the attribute                   *
 *     char_hanldes - The BLE characteristic handle type to give to the      *
 *                    bluetooth stack.                                       *
 *                                                                           *
 *****************************************************************************/
uint32_t pAddCharacteristicImpl(uint16_t characteristicUUID, char user_desc[],
                                    uint8_t attributeMaxLen, uint8_t attributeInitLen, uint8_t * attributeValue, ble_gatts_char_handles_t* char_handles)
{
    // Add a custom characteristic UUID
    uint32_t            err_code;
    ble_uuid_t          char_uuid;
    ble_uuid128_t       base_uuid = BLE_UUID_GLOVE_BASE_UUID;
    char_uuid.uuid      = characteristicUUID;
    err_code = sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
    APP_ERROR_CHECK(err_code);

    // Add read properties to our characteristic
    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read = 1;

    // Add a user description
    char_md.p_char_user_desc  = (uint8_t *) user_desc;
    char_md.char_user_desc_size = strlen(user_desc);
    char_md.char_user_desc_max_size = strlen(user_desc);

    // Configuring Client Characteristic Configuration Descriptor metadata and add to char_md structure
    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc                = BLE_GATTS_VLOC_STACK;
    char_md.p_cccd_md           = &cccd_md;
    char_md.char_props.notify   = 1;

    // Configure the attribute metadata
    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md));
    attr_md.vloc        = BLE_GATTS_VLOC_STACK;


    // Set read security levels to our characteristic
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);

    // Configure the characteristic value attribute
    ble_gatts_attr_t    attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.p_uuid      = &char_uuid;
    attr_char_value.p_attr_md   = &attr_md;

    // Set characteristic length in number of bytes
    attr_char_value.max_len     = attributeMaxLen;
    attr_char_value.init_len    = attributeInitLen;
    attr_char_value.p_value     = attributeValue;

    // Add our new characteristic to the service
    err_code = sd_ble_gatts_characteristic_add(mGloveService.service_handle,
                                       &char_md,
                                       &attr_char_value,
                                       char_handles);
    APP_ERROR_CHECK(err_code);

    return NRF_SUCCESS;
}
