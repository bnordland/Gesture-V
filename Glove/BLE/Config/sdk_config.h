/************************************************************************
* FILENAME: sdk_config.h                                                *
*                                                                       *
* DESCRIPTION: Nordic SDK Configuration File. Based off of Nordic SDK   *
*              12.0 examples SDK configuration file.                    *
*                                                                       *
* LICENSE:                                                              *
* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.         *
*                                                                       *
* The information contained herein is property of Nordic Semiconductor  *
* ASA. Terms and conditions of usage are described in detail in NORDIC  *
* SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.                    *
*                                                                       *
* Licensees are granted free, non-transferable use of the information.  *
* NO WARRANTY of ANY KIND is provided. This heading must NOT be removed *
* from the file.                                                        *
*                                                                       *
*                                                                       *
* --------------------------------------------------------------------  *
* | Change  | Date     |            |                                 | *
* | Flag    | (DDMYY)  | Author     | Description                     | *
* |---------|----------|------------|---------------------------------  *
* | None    | 10Mar17  | BNordland  | Initial creation                | *
* | @01     | 14Apr17  | BNordland  | Enable ADC                      | *
* | @02     | 18Apr17  | BNordland  | Making connection interval      | *
* |         |          |            | updates.                        | *
*  -------------------------------------------------------------------  *
*************************************************************************/

#ifndef SDK_CONFIG_H
#define SDK_CONFIG_H

#define IS_SRVC_CHANGED_CHARACT_PRESENT  1                                          /**< Include or not the service_changed characteristic. if not enabled, the server's database cannot be changed for the lifetime of the device*/

#define APP_ADV_INTERVAL                 50                                         /**< The advertising interval (in units of 0.625 ms. This value corresponds to 25 ms). */
#define APP_ADV_TIMEOUT_IN_SECONDS       180                                        /**< The advertising timeout in units of seconds. */

#define MIN_CONN_INTERVAL                MSEC_TO_UNITS(20, UNIT_1_25_MS)            /** @02c Minimum acceptable connection interval. */
#define MAX_CONN_INTERVAL                MSEC_TO_UNITS(75, UNIT_1_25_MS)            /** @02c Maximum acceptable connection interval. */
#define SLAVE_LATENCY                    0                                          /**< Slave latency. */
#define CONN_SUP_TIMEOUT                 MSEC_TO_UNITS(4000, UNIT_10_MS)            /**< Connection supervisory timeout (4 seconds). */

#define FIRST_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER) /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY    APP_TIMER_TICKS(30000, APP_TIMER_PRESCALER)/**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT     3                                          /**< Number of attempts before giving up the connection parameter negotiation. */

#define SEC_PARAM_BOND                   1                                          /**< Perform bonding. */
#define SEC_PARAM_MITM                   0                                          /**< Man In The Middle protection not required. */
#define SEC_PARAM_LESC                   0                                          /**< LE Secure Connections not enabled. */
#define SEC_PARAM_KEYPRESS               0                                          /**< Keypress notifications not enabled. */
#define SEC_PARAM_IO_CAPABILITIES        BLE_GAP_IO_CAPS_NONE                       /**< No I/O capabilities. */
#define SEC_PARAM_OOB                    0                                          /**< Out Of Band data not available. */
#define SEC_PARAM_MIN_KEY_SIZE           7                                          /**< Minimum encryption key size. */
#define SEC_PARAM_MAX_KEY_SIZE           16                                         /**< Maximum encryption key size. */

    //==========================================================
    // <o> NRF_BLE_CENTRAL_LINK_COUNT - Number of central links
    #ifndef NRF_BLE_CENTRAL_LINK_COUNT
        #define NRF_BLE_CENTRAL_LINK_COUNT 0
    #endif

    // <o> NRF_BLE_PERIPHERAL_LINK_COUNT - Number of peripheral links
    #ifndef NRF_BLE_PERIPHERAL_LINK_COUNT
        #define NRF_BLE_PERIPHERAL_LINK_COUNT 1
    #endif

    // <q> BLE_ADVERTISING_ENABLED  - ble_advertising - Advertising module
    #ifndef BLE_ADVERTISING_ENABLED
        #define BLE_ADVERTISING_ENABLED 1
    #endif

    // <q> BLE_DTM_ENABLED  - ble_dtm - Module for testing RF/PHY using DTM commands
    #ifndef BLE_DTM_ENABLED
        #define BLE_DTM_ENABLED 0
    #endif

    // <q> BLE_RACP_ENABLED  - ble_racp - Record Access Control Point library
    #ifndef BLE_RACP_ENABLED
        #define BLE_RACP_ENABLED 0
    #endif

    // NRF_BLE_GATT_ENABLED - nrf_ble_gatt - GATT module
    //==========================================================
    #ifndef NRF_BLE_GATT_ENABLED
        #define NRF_BLE_GATT_ENABLED 1
    #endif
    #if  NRF_BLE_GATT_ENABLED
        // <o> NRF_BLE_GATT_MAX_MTU_SIZE - Static maximum MTU size that is passed to the @ref sd_ble_enable function.
        #ifndef NRF_BLE_GATT_MAX_MTU_SIZE
            #define NRF_BLE_GATT_MAX_MTU_SIZE 158
        #endif
    #endif //NRF_BLE_GATT_ENABLED

    // <q> NRF_BLE_QWR_ENABLED  - nrf_ble_qwr - Queued writes support module (prepare/execute write)
    #ifndef NRF_BLE_QWR_ENABLED
        #define NRF_BLE_QWR_ENABLED 0
    #endif

    // <q> PEER_MANAGER_ENABLED  - peer_manager - Peer Manager
    #ifndef PEER_MANAGER_ENABLED
        #define PEER_MANAGER_ENABLED 1
    #endif

    //==========================================================
    // ADC_ENABLED - nrf_drv_adc - Driver for ADC peripheral (nRF51)
    //==========================================================
    #ifndef ADC_ENABLED
        #define ADC_ENABLED 1   // @01a enable ADC
    #endif
    #if  ADC_ENABLED
        // <o> ADC_CONFIG_IRQ_PRIORITY  - Interrupt priority
        #ifndef ADC_CONFIG_IRQ_PRIORITY
            #define ADC_CONFIG_IRQ_PRIORITY 3
        #endif
    #endif //ADC_ENABLED

// <e> CLOCK_ENABLED - nrf_drv_clock - CLOCK peripheral driver
//==========================================================
#ifndef CLOCK_ENABLED
#define CLOCK_ENABLED 1
#endif
#if  CLOCK_ENABLED
// <o> CLOCK_CONFIG_XTAL_FREQ  - HF XTAL Frequency

// <0=> Default (64 MHz)
// <255=> Default (16 MHz)
// <0=> 32 MHz

#ifndef CLOCK_CONFIG_XTAL_FREQ
#define CLOCK_CONFIG_XTAL_FREQ 255
#endif

// <o> CLOCK_CONFIG_LF_SRC  - LF Clock Source

// <0=> RC
// <1=> XTAL
// <2=> Synth

#ifndef CLOCK_CONFIG_LF_SRC
#define CLOCK_CONFIG_LF_SRC 1
#endif

// <o> CLOCK_CONFIG_IRQ_PRIORITY  - Interrupt priority
#ifndef CLOCK_CONFIG_IRQ_PRIORITY
#define CLOCK_CONFIG_IRQ_PRIORITY 3
#endif

#endif //CLOCK_ENABLED
// </e>

// <e> GPIOTE_ENABLED - nrf_drv_gpiote - GPIOTE peripheral driver
//==========================================================
#ifndef GPIOTE_ENABLED
#define GPIOTE_ENABLED 1
#endif
#if  GPIOTE_ENABLED
// <o> GPIOTE_CONFIG_NUM_OF_LOW_POWER_EVENTS - Number of lower power input pins
#ifndef GPIOTE_CONFIG_NUM_OF_LOW_POWER_EVENTS
#define GPIOTE_CONFIG_NUM_OF_LOW_POWER_EVENTS 4
#endif

// <o> GPIOTE_CONFIG_IRQ_PRIORITY  - Interrupt priority


// <i> Priorities 0,2 (nRF51) and 0,1,4,5 (nRF52) are reserved for SoftDevice
#ifndef GPIOTE_CONFIG_IRQ_PRIORITY
#define GPIOTE_CONFIG_IRQ_PRIORITY 3
#endif

#endif //GPIOTE_ENABLED
// </e>

// <e> SPI_ENABLED - nrf_drv_spi - SPI/SPIM peripheral driver
//==========================================================
    #ifndef SPI_ENABLED
        #define SPI_ENABLED 1
    #endif
    #if  SPI_ENABLED
        // SPI_DEFAULT_CONFIG_IRQ_PRIORITY  - Interrupt priority
        #ifndef SPI_DEFAULT_CONFIG_IRQ_PRIORITY
            #define SPI_DEFAULT_CONFIG_IRQ_PRIORITY 3
        #endif

        // <e> SPI0_ENABLED - Enable SPI0 instance
        #ifndef SPI0_ENABLED
            #define SPI0_ENABLED 1
        #endif
        #if  SPI0_ENABLED
            // SPI0_USE_EASY_DMA  - Use EasyDMA
            #ifndef SPI0_USE_EASY_DMA
                #define SPI0_USE_EASY_DMA 0
            #endif

            // <o> SPI0_DEFAULT_FREQUENCY  - SPI frequency
            // <33554432=> 125 kHz
            // <67108864=> 250 kHz
            // <134217728=> 500 kHz
            // <268435456=> 1 MHz
            // <536870912=> 2 MHz
            // <1073741824=> 4 MHz
            // <2147483648=> 8 MHz
            #ifndef SPI0_DEFAULT_FREQUENCY
                #define SPI0_DEFAULT_FREQUENCY 1073741824
            #endif

        #endif //SPI0_ENABLED

        // <e> SPI1_ENABLED - Enable SPI1 instance
        //==========================================================
        #ifndef SPI1_ENABLED
        #define SPI1_ENABLED 0
        #endif
        #if  SPI1_ENABLED
        // <q> SPI1_USE_EASY_DMA  - Use EasyDMA


        #ifndef SPI1_USE_EASY_DMA
        #define SPI1_USE_EASY_DMA 0
        #endif

        // <o> SPI1_DEFAULT_FREQUENCY  - SPI frequency
        // <33554432=> 125 kHz
        // <67108864=> 250 kHz
        // <134217728=> 500 kHz
        // <268435456=> 1 MHz
        // <536870912=> 2 MHz
        // <1073741824=> 4 MHz
        // <2147483648=> 8 MHz
        #ifndef SPI1_DEFAULT_FREQUENCY
        #define SPI1_DEFAULT_FREQUENCY 1073741824
        #endif

        #endif //SPI1_ENABLED
        // </e>

        // <e> SPI2_ENABLED - Enable SPI2 instance
        //==========================================================
        #ifndef SPI2_ENABLED
        #define SPI2_ENABLED 0
        #endif
        #if  SPI2_ENABLED
        // <q> SPI2_USE_EASY_DMA  - Use EasyDMA
        #ifndef SPI2_USE_EASY_DMA
        #define SPI2_USE_EASY_DMA 0
        #endif

        // <q> SPI2_DEFAULT_FREQUENCY  - Use EasyDMA
        #ifndef SPI2_DEFAULT_FREQUENCY
        #define SPI2_DEFAULT_FREQUENCY 0
        #endif
        #endif //SPI2_ENABLED

    #endif //SPI_ENABLED

//==========================================================

// <h> nRF_Libraries

//==========================================================
// <e> APP_TIMER_ENABLED - app_timer - Application timer functionality
//==========================================================
#ifndef APP_TIMER_ENABLED
#define APP_TIMER_ENABLED 1
#endif
#if  APP_TIMER_ENABLED
// <q> APP_TIMER_WITH_PROFILER  - Enable app_timer profiling


#ifndef APP_TIMER_WITH_PROFILER
#define APP_TIMER_WITH_PROFILER 0
#endif

// <q> APP_TIMER_KEEPS_RTC_ACTIVE  - Enable RTC always on


// <i> If option is enabled RTC is kept running even if there is no active timers.
// <i> This option can be used when app_timer is used for timestamping.

#ifndef APP_TIMER_KEEPS_RTC_ACTIVE
#define APP_TIMER_KEEPS_RTC_ACTIVE 0
#endif

#endif //APP_TIMER_ENABLED
// </e>

// <q> CRC16_ENABLED  - crc16 - CRC16 calculation routines
#ifndef CRC16_ENABLED
#define CRC16_ENABLED 1
#endif

// <e> FDS_ENABLED - fds - Flash data storage module
//==========================================================
#ifndef FDS_ENABLED
#define FDS_ENABLED 1
#endif
#if  FDS_ENABLED
// <o> FDS_OP_QUEUE_SIZE - Size of the internal queue.
#ifndef FDS_OP_QUEUE_SIZE
#define FDS_OP_QUEUE_SIZE 4
#endif

// <o> FDS_CHUNK_QUEUE_SIZE - Determines how many @ref fds_record_chunk_t structures can be buffered at any time.
#ifndef FDS_CHUNK_QUEUE_SIZE
#define FDS_CHUNK_QUEUE_SIZE 8
#endif

// <o> FDS_MAX_USERS - Maximum number of callbacks that can be registered.
#ifndef FDS_MAX_USERS
#define FDS_MAX_USERS 8
#endif

// <o> FDS_VIRTUAL_PAGES - Number of virtual flash pages to use.
// <i> One of the virtual pages is reserved by the system for garbage collection.
// <i> Therefore, the minimum is two virtual pages: one page to store data and
// <i> one page to be used by the system for garbage collection. The total amount
// <i> of flash memory that is used by FDS amounts to @ref FDS_VIRTUAL_PAGES
// <i> @ref FDS_VIRTUAL_PAGE_SIZE * 4 bytes.

#ifndef FDS_VIRTUAL_PAGES
#define FDS_VIRTUAL_PAGES 3
#endif

// <o> FDS_VIRTUAL_PAGE_SIZE  - The size of a virtual page of flash memory, expressed in number of 4-byte words.


// <i> By default, a virtual page is the same size as a physical page.
// <i> The size of a virtual page must be a multiple of the size of a physical page.
// <256=> 256
// <512=> 512
// <1024=> 1024

#ifndef FDS_VIRTUAL_PAGE_SIZE
#define FDS_VIRTUAL_PAGE_SIZE 256
#endif

#endif //FDS_ENABLED
// </e>

// <e> FSTORAGE_ENABLED - fstorage - Flash storage module
//==========================================================
#ifndef FSTORAGE_ENABLED
#define FSTORAGE_ENABLED 1
#endif
#if  FSTORAGE_ENABLED
// <o> FS_QUEUE_SIZE - Configures the size of the internal queue.
// <i> Increase this if there are many users, or if it is likely that many
// <i> operation will be queued at once without waiting for the previous operations
// <i> to complete. In general, increase the queue size if you frequently receive
// <i> @ref FS_ERR_QUEUE_FULL errors when calling @ref fs_store or @ref fs_erase.
#ifndef FS_QUEUE_SIZE
#define FS_QUEUE_SIZE 4
#endif

// <o> FS_OP_MAX_RETRIES - Number attempts to execute an operation if the SoftDevice fails.
// <i> Increase this value if events return the @ref FS_ERR_OPERATION_TIMEOUT
// <i> error often. The SoftDevice may fail to schedule flash access due to high BLE activity.
#ifndef FS_OP_MAX_RETRIES
#define FS_OP_MAX_RETRIES 3
#endif

// <o> FS_MAX_WRITE_SIZE_WORDS - Maximum number of words to be written to flash in a single operation.
// <i> Tweaking this value can increase the chances of the SoftDevice being
// <i> able to fit flash operations in between radio activity. This value is bound by the
// <i> maximum number of words which the SoftDevice can write to flash in a single call to
// <i> @ref sd_flash_write, which is 256 words for nRF51 ICs and 1024 words for nRF52 ICs.
#ifndef FS_MAX_WRITE_SIZE_WORDS
#define FS_MAX_WRITE_SIZE_WORDS 256
#endif

#endif //FSTORAGE_ENABLED
// </e>


#endif //SDK_CONFIG_H
