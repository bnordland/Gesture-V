/*****************************************************************************
* FILENAME: main.c                                                           *
*                                                                            *
* DESCRIPTION: main entry point for the Glove BLE Device                     *
*                                                                            *
* AUTHOR:  Brian Nordland                                                    *
*                                                                            *
* LICENSE: The MIT License (MIT)                                             *
*          Copyright (c) 2017 Brian Nordland                                 *
*                                                                            *
* CREDITS: The following tutorials were used as starting points:             *
*     Nordic SDK BLE Advertising:                                            *
*         Link: https://devzone.nordicsemi.com/tutorials/5/                  *
*         Notes: For initial BLE Advertising Base                            *
*     Nordic SDK BLE Peer Manager:                                           *
*         Link: https://devzone.nordicsemi.com/tutorials/24/                 *
*         Notes: To migrate BLE Advertising Base for SDK 12.0 as the tutorial*
*                was not compatible with the v12.0 of the SDK and did not    *
*                build as is.                                                *
*     Nordic SDK Bluetooth Services:                                         *
*         Link: https://devzone.nordicsemi.com/tutorials/8/                  *
*         Notes: For information on bluetooth services and                   *
*     Nordic SDK Characteristics:                                            *
*         Link: https://devzone.nordicsemi.com/tutorials/17/                 *
*         Notes: For information on bluetooth service characteristics        *
*     Arduino Map Implementaiton:                                            *
*         Link: https://www.arduino.cc/en/Reference/Map                      *
*                                                                            *
*                                                                            *
* ------------------------------------------------------------------------   *
* | Change  | Date     |            |                                     |  *
* | Flag    | (DDMYY)  | Author     | Description                         |  *
* |---------|----------|------------|-------------------------------------   *
* | None    | 31Mar17  | BNordland  | Initial creation                    |  *
*  ------------------------------------------------------------------------  *
******************************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "math.h"

// Need PI defined for some sensor calculations
#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

// Include the NordicSDK
#include "NordicSDK.h"

// Glove service Includes
#include "Service_Glove.h"

// Hardware pin locations
#include "Config_Hardware.h"

// Include Sensors
#include "Sensors_AccelGyro.h"

// Global Constants
#define DEVICE_NAME                      "Glove"                                    // Name of the bluetooth device
#define APP_TIMER_PRESCALER              0                                          // Timer prescaler (RTC1 PRESCALER register)
#define APP_TIMER_OP_QUEUE_SIZE          4                                          // Timer operation queue size
#define GLOVE_TIMER_INTERVAL             APP_TIMER_TICKS(100, APP_TIMER_PRESCALER)  // Set the timer interval

// Global Variables
static uint16_t  mConnectionHandle = BLE_CONN_HANDLE_INVALID;   // Bluetooth stack connection handle
APP_TIMER_DEF(mTimerId); // The timer
static Sensors_Accel_Data_t accel_data; // accelerometer data

// Channel for Throttle
static nrf_drv_adc_channel_t mThrottleADCChannelConfig = NRF_DRV_ADC_DEFAULT_CHANNEL(HDW_CONFIG_THROTTLE_FLEX_ADC_PIN);
static nrf_adc_value_t mThrottleAdcValue;
static uint8_t mThrottleValue; // The interpreted throttle value: 0 to 100

// Channel for Direction
static nrf_drv_adc_channel_t mDirectionADCChannelConfig = NRF_DRV_ADC_DEFAULT_CHANNEL(HDW_CONFIG_DIR_FLEX_ADC_PIN);
static nrf_adc_value_t mDirectionAdcValue;
static uint8_t mDirectionValue; // The interpreted direction value: 1 for forward, 0 for backward

// Function Definitions
    // Functions Required for Setup
    static void pSetupTimers(); // Called to set up timers
    static void pStartTimers(); // Called to start timers
    static void pSetupBLEStack(); // Called to set up the ble stack
    static void pSetupPeerManager(); // Called to set up the peer manager
    static void pSetupGAPParameters(); // Called to set up generic access profile
    static void pSetupBluetoothServices(); // Called to set up bluetooth services
    static void pSetupBluetoothAdvertising(); // Called to set up bluetooth device advertising
    static void pSetupConnectionParameters(); // Called to set up the connection parameters
    static void pSetupFlexSensors(); // Called to set up the flex sensors

    // Required for Starting

    // Functions required for Runtime
    static void pMainTimerHandler(void * p_context); // Main application timer

    // Functions for Event Handling
    static void pBLEEventHandler(ble_evt_t * event); // Dispatches bluetooth events to all modules
    static void pBLEEventHandlerImpl(ble_evt_t * event); // Handles bluetooth events for our main application
    static void pSystemEventHandler(uint32_t event); // Dispatches system events to all modules
    static void pPeerManagerEventHandler(pm_evt_t const * event); // Handles peer manager events
    static void pAdvertisingEventHandler(ble_adv_evt_t event); // Handles advertising events
    static void pConnectionParametersEventHandler(ble_conn_params_evt_t* event); // Handles connection parameters events

    // Functions for Error Handling
    static void pConnectionParametersErrorHandler(uint32_t nrf_error);

    // Helper functions
    static uint16_t pInterpretFlexSensorValue(uint16_t input, uint16_t inputMin, uint16_t inputMax, uint16_t outputMin, uint16_t outputMax);

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
    uint32_t err_code;

    // Initialize Hardware
    pSetupTimers();
    pSetupBLEStack();
    pSetupPeerManager();
    pSetupGAPParameters();
    pSetupBluetoothServices();
    pSetupBluetoothAdvertising();
    pSetupConnectionParameters();

    // turn on the LED
    nrf_gpio_cfg_output(HDW_CONFIG_ONBOARD_LED_PIN);
    nrf_gpio_pin_clear(HDW_CONFIG_ONBOARD_LED_PIN);

    // Initialize Accelerometer via SPI
    Sensors_AccelGyro_Init();

    // For protection, Only set up flex sensors after we
    // have determined that we are connected to sensors
    // This is because this applies power to pins
    pSetupFlexSensors();

    // Start execution of bluetooth and timers
    pStartTimers();
    err_code = ble_advertising_start(BLE_ADV_MODE_FAST);
    APP_ERROR_CHECK(err_code);

    // Enter main loop.
    // All interaction is currently done via
    // interrupts and timers
    while(1)
    {
        // ADC
        nrf_drv_adc_sample_convert(&mThrottleADCChannelConfig,&mThrottleAdcValue);
        nrf_drv_adc_sample_convert(&mDirectionADCChannelConfig,&mDirectionAdcValue);

        // interpret the values
        mThrottleValue = pInterpretFlexSensorValue(mThrottleAdcValue, 750, 1023, 0, 100);
        mDirectionValue = (mDirectionAdcValue < 800) ? 1 : 0; // if the sensor is bent, then go forward(1), else go backward (0)

        // TODO: we should also be using a complimentary filter and gyroscope data.
        Sensors_AccelGyro_GetAccelerometerData(&accel_data); // TODO: probably want to move this to a timer
        // Perform device power management
        uint32_t err_code = sd_app_evt_wait();
        APP_ERROR_CHECK(err_code);
    }
}

/*****************************************************************************
 ********************Start of Runtime Functions*******************************
 *****************************************************************************/

/*****************************************************************************
 * Description: Main application timer. Responsible for LED control and      *
 *              for updating characteristics                                 *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: p_context -> provided with timer events                       *
 *                                                                           *
 *****************************************************************************/
static void pMainTimerHandler(void * p_context)
{
    if(Service_Glove_IsConnected())
    {
        // If we have a connection, the LED is solid
        nrf_gpio_pin_clear(HDW_CONFIG_ONBOARD_LED_PIN);

        int16_t pitch = atan2(-accel_data.xData, sqrt(accel_data.yData*accel_data.yData + accel_data.zData*accel_data.zData)) * 180/M_PI;
        Service_Glove_SetAnglePitch(&pitch);
        Service_Glove_SetThrottle(&mThrottleValue);
        Service_Glove_SetDirection(&mDirectionValue);
    }
    else
    {
        // If we do not have a connection, toggle the LED.
        nrf_gpio_pin_toggle(HDW_CONFIG_ONBOARD_LED_PIN);
    }
}

/*****************************************************************************
 ******************Start of Helper Handler Functions**************************
 *****************************************************************************/

uint16_t pInterpretFlexSensorValue(uint16_t input, uint16_t inputMin, uint16_t inputMax, uint16_t outputMin, uint16_t outputMax)
{
    // The map formula does not work well without bounding
    if(input > inputMax) input = inputMax;
    if(input < inputMin) input = inputMin;

    // the following formula was taken from https://www.arduino.cc/en/Reference/Map
  uint16_t value = (input - inputMin) * (outputMax - outputMin) / (inputMax - inputMin) + outputMin;

  // The value will be inverted from where we want since high is not bent, so invert the value
  return 100 - value;
}

/*****************************************************************************
 ******************Start of Event Handler Functions***************************
 *****************************************************************************/

/*****************************************************************************
 * Description: Forwards bluetooth events to required modules                *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: event -> event details                                        *
 *                                                                           *
 *****************************************************************************/
static void pBLEEventHandler(ble_evt_t* event)
{
    // Forward BLE events to the Connection State module.
    // This must be called before any event handler that uses this module.
    ble_conn_state_on_ble_evt(event);

    // Forward BLE events to the Peer Manager
    pm_on_ble_evt(event);

    // Forward BLE events to other event handlers
    ble_conn_params_on_ble_evt(event);
    bsp_btn_ble_on_ble_evt(event);
    pBLEEventHandlerImpl(event);
    ble_advertising_on_ble_evt(event);

    // Call glove service event handling (our event handler)
    Service_Glove_BluetoothEventHandler(event);
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
    uint32_t err_code;

    switch (event->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            err_code = bsp_indication_set(BSP_INDICATE_CONNECTED);
            APP_ERROR_CHECK(err_code);
            mConnectionHandle = event->evt.gap_evt.conn_handle;
            break;
        case BLE_GAP_EVT_DISCONNECTED:
            mConnectionHandle = BLE_CONN_HANDLE_INVALID;
            break;
        default:
            break;
    }
}

/*****************************************************************************
 * Description: Forwards system events to required modules                   *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: event -> event details                                        *
 *                                                                           *
 *****************************************************************************/
static void pSystemEventHandler(uint32_t event)
{
    // Forward Softdevice events to the fstorage module
    // (used by peer manager)
    fs_sys_event_handler(event);

    ble_advertising_on_sys_evt(event);
}

/*****************************************************************************
 * Description: Handles Peer Manager Events                                  *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: event -> event details                                        *
 *                                                                           *
 *****************************************************************************/
static void pPeerManagerEventHandler(pm_evt_t const * event)
{
    // At the moment this is empty
}

/*****************************************************************************
 * Description: Handles Advertising Events                                   *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: event -> event details                                        *
 *                                                                           *
 *****************************************************************************/
static void pAdvertisingEventHandler(ble_adv_evt_t event)
{
    uint32_t err_code;

    switch (event)
    {
        case BLE_ADV_EVT_FAST:
            err_code = bsp_indication_set(BSP_INDICATE_ADVERTISING);
            APP_ERROR_CHECK(err_code);
            break;
        case BLE_ADV_EVT_IDLE:
            // put the device to sleep
            err_code = bsp_indication_set(BSP_INDICATE_IDLE);
            APP_ERROR_CHECK(err_code);

            // Prepare wakeup buttons.
            err_code = bsp_btn_ble_sleep_mode_prepare();
            APP_ERROR_CHECK(err_code);

            // Go to system-off mode (this function will not return; wakeup will cause a reset).
            err_code = sd_power_system_off();
            APP_ERROR_CHECK(err_code);
            break;
        default:
            break;
    }
}

/*****************************************************************************
 * Description: Handles Connection Parameters Events                         *
 *              Note from Nordic examples:                                   *
 *              All this function does is to disconnect. This could have     *
 *              been done by simply setting the disconnect_on_fail config    *
 *              parameter, but instead we use the event handler mechanism to *
 *              demonstrate its use.                                         *
 *              TODO: switch to use disconnect_on_fail config parameter      *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: event -> event details                                        *
 *                                                                           *
 *****************************************************************************/
static void pConnectionParametersEventHandler(ble_conn_params_evt_t* event)
{
    uint32_t err_code;

    if (event->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        err_code = sd_ble_gap_disconnect(mConnectionHandle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
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
    // Initialize timer module.
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, false);

    // Initiate the main application timer
    app_timer_create(&mTimerId, APP_TIMER_MODE_REPEATED, pMainTimerHandler);
}

/*****************************************************************************
 * Description: Start application timers                                     *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: None                                                          *
 *                                                                           *
 *****************************************************************************/
static void pStartTimers()
{
    // Start our timer
    app_timer_start(mTimerId, GLOVE_TIMER_INTERVAL, NULL);
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
    err_code = softdevice_enable_get_default_config(NRF_BLE_CENTRAL_LINK_COUNT,
                                                    NRF_BLE_PERIPHERAL_LINK_COUNT,
                                                    &ble_enable_params);
    APP_ERROR_CHECK(err_code);

    ble_enable_params.gatts_enable_params.service_changed = IS_SRVC_CHANGED_CHARACT_PRESENT;

    //Check the ram settings against the used number of links
    CHECK_RAM_START_ADDR(NRF_BLE_CENTRAL_LINK_COUNT,NRF_BLE_PERIPHERAL_LINK_COUNT);

    // Enable BLE stack.
    err_code = softdevice_enable(&ble_enable_params);
    APP_ERROR_CHECK(err_code);

    // Register with the SoftDevice handler module for BLE events.
    err_code = softdevice_ble_evt_handler_set(pBLEEventHandler);
    APP_ERROR_CHECK(err_code);

    // Register with the SoftDevice handler module for system events.
    err_code = softdevice_sys_evt_handler_set(pSystemEventHandler);
    APP_ERROR_CHECK(err_code);
}

/*****************************************************************************
 * Description: Set up and configure the bluetooth low energy peer manager   *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: None                                                          *
 *                                                                           *
 *****************************************************************************/
static void pSetupPeerManager()
{
    ble_gap_sec_params_t sec_param;
    ret_code_t err_code;

    err_code = pm_init();
    APP_ERROR_CHECK(err_code);

    memset(&sec_param, 0, sizeof(ble_gap_sec_params_t));

    // Security parameters to be used for all security procedures.
    sec_param.bond              = SEC_PARAM_BOND;
    sec_param.mitm              = SEC_PARAM_MITM;
    sec_param.lesc              = SEC_PARAM_LESC;
    sec_param.keypress          = SEC_PARAM_KEYPRESS;
    sec_param.io_caps           = SEC_PARAM_IO_CAPABILITIES;
    sec_param.oob               = SEC_PARAM_OOB;
    sec_param.min_key_size      = SEC_PARAM_MIN_KEY_SIZE;
    sec_param.max_key_size      = SEC_PARAM_MAX_KEY_SIZE;
    sec_param.kdist_own.enc     = 1;
    sec_param.kdist_own.id      = 1;
    sec_param.kdist_peer.enc    = 1;
    sec_param.kdist_peer.id     = 1;

    err_code = pm_sec_params_set(&sec_param);
    APP_ERROR_CHECK(err_code);

    err_code = pm_register(pPeerManagerEventHandler);
    APP_ERROR_CHECK(err_code);
}

/*****************************************************************************
 * Description: Set up and configure the Generic Access Profile (GAP)        *
 *              parameters. This is including but not limited to the device  *
 *              name and appearance.                                         *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: None                                                          *
 *                                                                           *
 *****************************************************************************/
static void pSetupGAPParameters()
{
    uint32_t                err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                          (const uint8_t *)DEVICE_NAME,
                                          strlen(DEVICE_NAME));
    APP_ERROR_CHECK(err_code);

    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    // Note: parameters are defined in sdk_onfig.h
    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);

    err_code = sd_ble_gap_appearance_set(BLE_APPEARANCE_GENERIC_REMOTE_CONTROL);
    APP_ERROR_CHECK(err_code);
}

/*****************************************************************************
 * Description: Set up and configure services (currently only the glove      *
 *              service                                                      *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: None                                                          *
 *                                                                           *
 *****************************************************************************/
static void pSetupBluetoothServices()
{
    Service_Glove_Init();
}

/*****************************************************************************
 * Description: Set up and configure bluetooth advertising                   *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: None                                                          *
 *                                                                           *
 *****************************************************************************/
static void pSetupBluetoothAdvertising()
{
    uint32_t      err_code;
    ble_advdata_t advdata;

    // Build advertising data struct to pass into @ref ble_advertising_init.
    memset(&advdata, 0, sizeof(advdata));

    advdata.name_type               = BLE_ADVDATA_FULL_NAME;
    advdata.flags                   = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    advdata.include_appearance      = true;

    ble_adv_modes_config_t options = {0};
    options.ble_adv_fast_enabled  = true;
    options.ble_adv_fast_interval = APP_ADV_INTERVAL;
    options.ble_adv_fast_timeout  = APP_ADV_TIMEOUT_IN_SECONDS;

    // Declare variable holding glove service UUID
    ble_uuid_t m_adv_uuids[] = {{BLE_UUID_GLOVE_SERVICE, BLE_UUID_TYPE_VENDOR_BEGIN}};

    // Declare and instantiate the scan response
    ble_advdata_t srdata;
    memset(&srdata, 0, sizeof(srdata));

    // Include scan response packet in advertising
    srdata.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
    srdata.uuids_complete.p_uuids = m_adv_uuids;

    err_code = ble_advertising_init(&advdata, &srdata, &options, pAdvertisingEventHandler, NULL);
    APP_ERROR_CHECK(err_code);
}

/*****************************************************************************
 * Description: Set up and configure bluetooth connection parameters         *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: None                                                          *
 *                                                                           *
 *****************************************************************************/
static void pSetupConnectionParameters()
{
    uint32_t               err_code;
    ble_conn_params_init_t cp_init;

    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = false;
    cp_init.evt_handler                    = pConnectionParametersEventHandler;
    cp_init.error_handler                  = pConnectionParametersErrorHandler;

    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}

/*****************************************************************************
 * Description: Set up flex sensors                                          *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: None                                                          *
 *                                                                           *
 *****************************************************************************/
static void pSetupFlexSensors()
{
    // Apply Power to the flex sensors
    nrf_gpio_cfg_output(HDW_CONFIG_DIR_FLEX_VIN_PIN);
    nrf_gpio_pin_set(HDW_CONFIG_DIR_FLEX_VIN_PIN);
    nrf_gpio_cfg_output(HDW_CONFIG_THROTTLE_FLEX_VIN_PIN);
    nrf_gpio_pin_set(HDW_CONFIG_THROTTLE_FLEX_VIN_PIN);

    // init ADC driver
    ret_code_t ret_code;
    nrf_drv_adc_config_t config = NRF_DRV_ADC_DEFAULT_CONFIG;

    ret_code = nrf_drv_adc_init(&config, NULL);
    APP_ERROR_CHECK(ret_code);
}

/*****************************************************************************
 *****************Start of Error Handling Functions***************************
 *****************************************************************************/

/*****************************************************************************
 * Description: Function for handling a Connection Parameters error.         *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: errorCode - information about specific error                  *
 *                                                                           *
 *****************************************************************************/
static void pConnectionParametersErrorHandler(uint32_t errorCode)
{
    APP_ERROR_HANDLER(errorCode);
}
