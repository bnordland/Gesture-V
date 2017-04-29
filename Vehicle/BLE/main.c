/*****************************************************************************
* FILENAME: main.c                                                           *
*                                                                            *
* DESCRIPTION: main entry point for the Vehicle BLE Device                   *
*                                                                            *
* AUTHOR:  Brian Nordland                                                    *
*                                                                            *
* LICENSE: The MIT License (MIT)                                             *
*          Copyright (c) 2017 Brian Nordland                                 *
*                                                                            *
* CREDITS: The following tutorials were used as starting points:             *
*     Nordic BLE Central Tutorial:                                           *
*         Link: https://devzone.nordicsemi.com/tutorials/21/                 *
*         Notes: For base on how to do central mode bluetooth devices        *
*                                                                            *
*                                                                            *
* ------------------------------------------------------------------------   *
* | Change  | Date     |            |                                     |  *
* | Flag    | (DDMYY)  | Author     | Description                         |  *
* |---------|----------|------------|-------------------------------------   *
* | None    | 16Apr17  | BNordland  | Initial creation                    |  *
* | @01     | 19Apr17  | BNordland  | Clear out data on disconnect.       |  *
*  ------------------------------------------------------------------------  *
******************************************************************************/

#include <stdint.h>
#include <stdbool.h>

#include "NordicSDK.h"

// Hardware config
#include "Config_Hardware.h"

// The glove client
#include "Client_Glove.h"

#define APP_TIMER_PRESCALER     0                               // RTC1 PRESCALER register.
#define APP_TIMER_OP_QUEUE_SIZE 2                               // Size of timer operation queues.

// Constants used for bluetooth stack
    // Connection parameters requested for connection.
    static const ble_gap_conn_params_t mConnectionParam =
    {
        // Note, these are defined in sdk_config.h
        (uint16_t)MIN_CONNECTION_INTERVAL,  // Minimum connection
        (uint16_t)MAX_CONNECTION_INTERVAL,  // Maximum connection
        (uint16_t)SLAVE_LATENCY,            // Slave latency
        (uint16_t)SUPERVISION_TIMEOUT       // Supervision time-out
    };

    // Parameters used when scanning.
    static const ble_gap_scan_params_t mScanParams =
    {
        .active   = 1,
        .interval = SCAN_INTERVAL,
        .window   = SCAN_WINDOW,
        .timeout  = SCAN_TIMEOUT,
        #if (NRF_SD_BLE_API_VERSION == 2)
            .selective   = 0,
            .p_whitelist = NULL,
        #endif
        #if (NRF_SD_BLE_API_VERSION == 3)
            .use_whitelist = 0,
        #endif
    };

// Structure definitions
// Application data is stored in a structure for ease of sending
// via SPI when requested by the SPI master (A* Controller)
typedef struct AppData
{
    uint16_t    anglePitch; // The pitch angle of the glove
    uint8_t     direction; // The direction - forward(1) or backward(0)
    uint8_t     throttle;  // The throttle - 0-100;

} AppData_t;

// Global Variables
static ble_db_discovery_t       mDbDiscovery;    // Database discovery module instance
static AppData_t                mAppData; // The application data structure.

#define SPIS_INSTANCE 1 /**< SPIS instance index. */
static const nrf_drv_spis_t mSPIsDriver = NRF_DRV_SPIS_INSTANCE(SPIS_INSTANCE); // SPI Slave Driver
static uint8_t       mRxBuffer[sizeof(AppData_t) + 1]; // RxBuffer that is the size AppData + 1 buffer byte
static AppData_t                mTxBuffer; // The transmit buffer
static volatile bool mSPITxComplete; //Indicates if the SPI transfer is done

// Function Definitions
    // Functions Required for Setup
    static void pSetupTimers(); // Called to set up timers
    static void pSetupDbDiscovery(); // Called to setup bluetooth discovery
    static void pSetupBLEStack(); // sets up the Nordic bluetooth stack
    static void pSetupGloveClient(); // sets up the glove client

    // Required for Starting
    static void pStartScanning();
    static void pBLEEventHandler(ble_evt_t * event); // Dispatches bluetooth events to all modules
    static void pBLEEventHandlerImpl(ble_evt_t * event); // Handles bluetooth events for our main application

    // Functions required for event handling
    static void pDbDiscoveryEventHandler(ble_db_discovery_evt_t * event); // Handles events
    static void pGloveClientEventHandler(const Client_Glove_Event_t * event); // Handles events from the glove client
    static void pSPIEventHandler(nrf_drv_spis_event_t event);

    // Helper functions
    static bool isUuidInReport(const ble_uuid_t *targetUuid, const ble_gap_evt_adv_report_t *report);


/*****************************************************************************
 * Description: Main application entry point                                 *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: None                                                          *
 *                                                                           *
 *****************************************************************************/
int main(void)
{
    // @01c - to start with, until we are connected, we don't want to move
    memset(&mAppData, 0x00, sizeof(mAppData));
    memset(&mTxBuffer, 0x00, sizeof(mTxBuffer));

    // Initialize and setup hardware
    pSetupTimers();
    pSetupDbDiscovery();
    pSetupBLEStack();

    // Prepare the glove client
    pSetupGloveClient();

    // start scanning for devices, which
    // will trigger events on successful finding of
    // devices, and then trigger the connection to the
    // devices. Main application control is then event
    // driven for this application.
    pStartScanning();

    nrf_drv_spis_config_t spis_config = NRF_DRV_SPIS_DEFAULT_CONFIG;
    spis_config.csn_pin               = HDW_CONFIG_SPI_SS_PIN;
    spis_config.miso_pin              = HDW_CONFIG_SPI_MISO_PIN;
    spis_config.mosi_pin              = HDW_CONFIG_SPI_MOSI_PIN;
    spis_config.sck_pin               = HDW_CONFIG_SPI_SCK_PIN;
    APP_ERROR_CHECK(nrf_drv_spis_init(&mSPIsDriver, &spis_config, pSPIEventHandler));
    // TODO: we should create a Comm_SPISlave


    while(1)
    {
        mSPITxComplete = false;

        APP_ERROR_CHECK(nrf_drv_spis_buffers_set(&mSPIsDriver, (uint8_t*)&mTxBuffer, sizeof(AppData_t), mRxBuffer, sizeof(AppData_t)));

        while (!mSPITxComplete)
        {
            __WFE();
        }
        // Perform device power management
        uint32_t err_code = sd_app_evt_wait();
        APP_ERROR_CHECK(err_code);
    }
}



/*****************************************************************************
 ******************Start of Event Handler Functions***************************
 *****************************************************************************/

void pSPIEventHandler(nrf_drv_spis_event_t event)
    {
        if (event.evt_type == NRF_DRV_SPIS_XFER_DONE)
        {
            memcpy(&mTxBuffer, &mAppData, sizeof(AppData_t)); // copy the new tx buffer
            mSPITxComplete = true;
        }
    }

/*****************************************************************************
 * Description: Forwards database discovery events to those interested       *
 *              When the database discovery module sends an event, it is     *
 *              forwarded on to a client (in our case the glove client)      *
 *              so that it can check if the UUIDs are of interest to it, and *
 *              act accordingly.                                             *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: event -> event details                                        *
 *                                                                           *
 *****************************************************************************/
void pDbDiscoveryEventHandler(ble_db_discovery_evt_t * event)
{
    // Will check to see if UUID applies to it (if it is a glove service uuid)
    // if so, then characteristics will be updated with characteristic handles
    // and the connection handle will be set in the glove client.
    // This will also enable notifications on the glove client.
    Client_Glove_DiscoveryEventHandler(event);
}

/*****************************************************************************
 * Description: When the Glove Client receives a relevant event, it will     *
 *              translate the event into a glove event, that comes here      *
 *              This can then properly handle the event. In our case, the    *
 *              biggest thing is updating our internal variables with the    *
 *              data that is received, so that the SPI module is ready to    *
 *              reply with the most up to date information.                  *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: event -> Glove event details.                                  *
 *                                                                           *
 *****************************************************************************/
void pGloveClientEventHandler(const Client_Glove_Event_t * event)
{
    switch (event->evt_type)
    {
        case Client_Glove_Event_ANGLEPITCH_UPDATED:
        {
            mAppData.anglePitch = ((uint16_t)event->p_data[0] << 8) | (uint16_t)event->p_data[1];
            break;
        }
        case Client_Glove_Event_THROTTLE_UPDATED:
        {
            mAppData.throttle = event->p_data[0];
            break;
        }
        case Client_Glove_Event_DIRECTION_UPDATED:
        {
            mAppData.direction = event->p_data[0];
            // turn on the LED
            nrf_gpio_cfg_output(HDW_CONFIG_ONBOARD_LED_PIN);
            nrf_gpio_pin_clear(HDW_CONFIG_ONBOARD_LED_PIN);
            break;
        }
        case Client_Glove_Event_DISCONNECTED:
        {
            // @01a - disconnected, we want to set all to zero in order to stop activity
            memset(&mAppData, 0x00, sizeof(mAppData));
            nrf_gpio_pin_set(HDW_CONFIG_ONBOARD_LED_PIN);
            // disconnected, start scanning again.
            pStartScanning();
            break;
        }
    }
}

/*****************************************************************************
 * Description: Forwards bluetooth events to required modules                *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: event -> event details                                        *
 *                                                                           *
 *****************************************************************************/
static void pBLEEventHandler(ble_evt_t * event)
{
    // Handle the event for the application, including kicking off discovery
    // and restarting scanning in the event of timeouts.
    pBLEEventHandlerImpl(event);

    // To the Nordic discovery module
    ble_db_discovery_on_ble_evt(&mDbDiscovery, event);

    // Handles events relevant to the glove client, for example when discovery is complete
    Client_Glove_BLEEventHandler(event);
}

/*****************************************************************************
 * Description: Handles our application's bluetooth events                   *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: event -> event details                                        *
 *                                                                           *
 *****************************************************************************/
static void pBLEEventHandlerImpl(ble_evt_t * event)
{
    uint32_t              err_code = 0;
    const ble_gap_evt_t * gapEvent = &event->evt.gap_evt;

    switch (event->header.evt_id)
    {
        case BLE_GAP_EVT_ADV_REPORT:
        {
            // Get the advertisement report from the bluetooth stack
            const ble_gap_evt_adv_report_t * adReport = &gapEvent->params.adv_report;

            // Declare variable holding glove service UUID
            ble_uuid_t glove_uuids[] = {{BLE_UUID_GLOVE_SERVICE, BLE_UUID_TYPE_VENDOR_BEGIN}};

            // Check to see if our glove client uuid is in the report
            if (isUuidInReport(glove_uuids, adReport))
            {
                // if so, connect to it.
                err_code = sd_ble_gap_connect(&adReport->peer_addr,
                                              &mScanParams,
                                              &mConnectionParam);
                APP_ERROR_CHECK(err_code);
                // scan is automatically stopped by the connect
            }
            break;
        }
        case BLE_GAP_EVT_CONNECTED:
        {
            // after we are connected, start discovery of services
            // This will trigger a discovery event that the glove client reacts to.
            err_code = ble_db_discovery_start(&mDbDiscovery, event->evt.gap_evt.conn_handle);
            APP_ERROR_CHECK(err_code);
            break;
        }
        case BLE_GAP_EVT_TIMEOUT:
        {
            if (gapEvent->params.timeout.src == BLE_GAP_TIMEOUT_SRC_SCAN)
            {
                pStartScanning();
            }
            else if (gapEvent->params.timeout.src == BLE_GAP_TIMEOUT_SRC_CONN)
            {
                // Connection Request timed out
            }
            break; // BLE_GAP_EVT_TIMEOUT
        }
        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
        {
            // Pairing not supported
            err_code = sd_ble_gap_sec_params_reply(event->evt.gap_evt.conn_handle, BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP, NULL, NULL);
            APP_ERROR_CHECK(err_code);
            break;
        }
        case BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST:
        {
            // Accepting parameters requested by peer.
            err_code = sd_ble_gap_conn_param_update(gapEvent->conn_handle,
                                                    &gapEvent->params.conn_param_update_request.conn_params);
            APP_ERROR_CHECK(err_code);
            break;
        }
        case BLE_GATTC_EVT_TIMEOUT:
        {
            // Disconnect on GATT Client timeout event.
            err_code = sd_ble_gap_disconnect(event->evt.gattc_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break; // BLE_GATTC_EVT_TIMEOUT
        }
        case BLE_GATTS_EVT_TIMEOUT:
        {
            // Disconnect on GATT Server timeout event.
            err_code = sd_ble_gap_disconnect(event->evt.gatts_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break; // BLE_GATTS_EVT_TIMEOUT
        }
#if (NRF_SD_BLE_API_VERSION == 3)
        case BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST:
        {
            err_code = sd_ble_gatts_exchange_mtu_reply(event->evt.gatts_evt.conn_handle,
                                                       NRF_BLE_MAX_MTU_SIZE);
            APP_ERROR_CHECK(err_code);
            break; // BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST
        }
#endif
        default:
        {
            break;
        }
    }
}

/*****************************************************************************
 ************Start of Setup and Configuration Functions***********************
 *****************************************************************************/

/*****************************************************************************
 * Description: Set up and configure all timers used                         *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: None                                                          *
 *                                                                           *
 *****************************************************************************/
static void pSetupTimers()
{
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, NULL);
}

/*****************************************************************************
 * Description: Set up and configure the database discovery service module   *
 *              of the bluetooth stack.                                      *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: None                                                          *
 *                                                                           *
 *****************************************************************************/
static void pSetupDbDiscovery()
{
    uint32_t err_code = ble_db_discovery_init(pDbDiscoveryEventHandler);
    APP_ERROR_CHECK(err_code);
}

/*****************************************************************************
 * Description: Set up and configure the bluetooth low energy stack          *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: None                                                          *
 *                                                                           *
 *****************************************************************************/
static void pSetupBLEStack()
{
    uint32_t err_code;

    nrf_clock_lf_cfg_t clock_lf_cfg = NRF_CLOCK_LFCLKSRC;

    // Initialize the SoftDevice handler module.
    SOFTDEVICE_HANDLER_INIT(&clock_lf_cfg, NULL);

    ble_enable_params_t ble_enable_params;
    err_code = softdevice_enable_get_default_config(CENTRAL_LINK_COUNT,
                                                    PERIPHERAL_LINK_COUNT,
                                                    &ble_enable_params);
    APP_ERROR_CHECK(err_code);

    //Check the ram settings against the used number of links
    CHECK_RAM_START_ADDR(CENTRAL_LINK_COUNT,PERIPHERAL_LINK_COUNT);

    // Enable BLE stack.
#if (NRF_SD_BLE_API_VERSION == 3)
    ble_enable_params.gatt_enable_params.att_mtu = NRF_BLE_MAX_MTU_SIZE;
#endif
    err_code = softdevice_enable(&ble_enable_params);
    APP_ERROR_CHECK(err_code);

    // Register with the SoftDevice handler module for BLE events.
    err_code = softdevice_ble_evt_handler_set(pBLEEventHandler);
    APP_ERROR_CHECK(err_code);
}

/*****************************************************************************
 * Description: Initializes everything that is needed for the glove client   *
 *              As well as sets our event handler to receive events from     *
 *              notifications that the glove pushes out                      *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: None                                                          *
 *                                                                           *
 *****************************************************************************/
static void pSetupGloveClient()
{
    // Set up the init data structure.
    // The only initialization data required currently
    // for our glove client is the event handler
    Client_Glove_Init_t glove_init;
    glove_init.evt_handler = pGloveClientEventHandler;

    // Actually perform the init.
    uint32_t err_code = Client_Glove_Init(&glove_init);
    APP_ERROR_CHECK(err_code);
}

/*****************************************************************************
 * Description: Starts scanning for bluetooth devices that we can try to     *
 *              connect to.                                                  *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: None                                                          *
 *                                                                           *
 *****************************************************************************/
static void pStartScanning()
{
    ret_code_t ret = sd_ble_gap_scan_start(&mScanParams);
    APP_ERROR_CHECK(ret);
}

/*****************************************************************************
 *************************Start of Helper Functions***************************
 *****************************************************************************/

/*****************************************************************************
 * Description: Looks at an advertisement report and checks to see if a UUID *
 *              is present in the list of services so that we can            *
 *              appropriately connect to the right device.                   *
 *                                                                           *
 * Credits: This function was taken from Nordic SDK examples.                *
 *     License:                                                              *
 *     Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.         *
 *                                                                           *
 *     The information contained herein is property of Nordic Semiconductor  *
 *     ASA. Terms and conditions of usage are described in detail in NORDIC  *
 *     SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.                    *
 *                                                                           *
 *     Licensees are granted free, non-transferable use of the information.  *
 *     NO WARRANTY of ANY KIND is provided. This heading must NOT be removed *
 *     from the file.                                                        *
 *                                                                           *
 * Returns: True if the UUID was found in the report, or false otherwise     *
 *                                                                           *
 * Parameters: targetUuid -> the uuid to search for in the advertisement     *
 *                           report.                                         *
 *             report     -> the advertisement report.                       *
 *                                                                           *
 *****************************************************************************/
static bool isUuidInReport(const ble_uuid_t *targetUuid,
                            const ble_gap_evt_adv_report_t *report)
{
    uint32_t err_code;
    uint32_t index = 0;
    uint8_t *p_data = (uint8_t *)report->data;
    ble_uuid_t extracted_uuid;

    while (index < report->dlen)
    {
        uint8_t field_length = p_data[index];
        uint8_t field_type   = p_data[index + 1];

        if ( (field_type == BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_MORE_AVAILABLE)
           || (field_type == BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_COMPLETE)
           )
        {
            for (uint32_t u_index = 0; u_index < (field_length / UUID16_SIZE); u_index++)
            {
                err_code = sd_ble_uuid_decode(  UUID16_SIZE,
                                                &p_data[u_index * UUID16_SIZE + index + 2],
                                                &extracted_uuid);
                if (err_code == NRF_SUCCESS)
                {
                    if ((extracted_uuid.uuid == targetUuid->uuid)
                        && (extracted_uuid.type == targetUuid->type))
                    {
                        return true;
                    }
                }
            }
        }

        else if ( (field_type == BLE_GAP_AD_TYPE_32BIT_SERVICE_UUID_MORE_AVAILABLE)
                || (field_type == BLE_GAP_AD_TYPE_32BIT_SERVICE_UUID_COMPLETE)
                )
        {
            for (uint32_t u_index = 0; u_index < (field_length / UUID32_SIZE); u_index++)
            {
                err_code = sd_ble_uuid_decode(UUID16_SIZE,
                &p_data[u_index * UUID32_SIZE + index + 2],
                &extracted_uuid);
                if (err_code == NRF_SUCCESS)
                {
                    if ((extracted_uuid.uuid == targetUuid->uuid)
                        && (extracted_uuid.type == targetUuid->type))
                    {
                        return true;
                    }
                }
            }
        }

        else if ( (field_type == BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_MORE_AVAILABLE)
                || (field_type == BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_COMPLETE)
                )
        {
            err_code = sd_ble_uuid_decode(UUID128_SIZE,
                                          &p_data[index + 2],
                                          &extracted_uuid);
            if (err_code == NRF_SUCCESS)
            {
                if ((extracted_uuid.uuid == targetUuid->uuid)
                    && (extracted_uuid.type == targetUuid->type))
                {
                    return true;
                }
            }
        }
        index += field_length + 1;
    }
    return false;
}
