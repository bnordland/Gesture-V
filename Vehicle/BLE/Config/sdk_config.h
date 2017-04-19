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
* | None    | 16Apr17  | BNordland  | Initial creation                | *
*  -------------------------------------------------------------------  *
*************************************************************************/


#ifndef SDK_CONFIG_H
#define SDK_CONFIG_H

#define CENTRAL_LINK_COUNT      1                               /**< Number of central links used by the application. When changing this number remember to adjust the RAM settings*/
#define PERIPHERAL_LINK_COUNT   0                               /**< Number of peripheral links used by the application. When changing this number remember to adjust the RAM settings*/

#if (NRF_SD_BLE_API_VERSION == 3)
#define NRF_BLE_MAX_MTU_SIZE    GATT_MTU_SIZE_DEFAULT           /**< MTU size used in the softdevice enabling and to reply to a BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST event. */
#endif

#define SCAN_INTERVAL           0x00A0                          /**< Determines scan interval in units of 0.625 millisecond. */
#define SCAN_WINDOW             0x0050                          /**< Determines scan window in units of 0.625 millisecond. */
#define SCAN_ACTIVE             1                               /**< If 1, performe active scanning (scan requests). */
#define SCAN_SELECTIVE          0                               /**< If 1, ignore unknown devices (non whitelisted). */
#define SCAN_TIMEOUT            0x0000                          /**< Timout when scanning. 0x0000 disables timeout. */

#define MIN_CONNECTION_INTERVAL MSEC_TO_UNITS(20, UNIT_1_25_MS) /**< Determines minimum connection interval in millisecond. */
#define MAX_CONNECTION_INTERVAL MSEC_TO_UNITS(75, UNIT_1_25_MS) /**< Determines maximum connection interval in millisecond. */
#define SLAVE_LATENCY           0                               /**< Determines slave latency in counts of connection events. */
#define SUPERVISION_TIMEOUT     MSEC_TO_UNITS(4000, UNIT_10_MS) /**< Determines supervision time-out in units of 10 millisecond. */

#define UUID16_SIZE             2                               /**< Size of 16 bit UUID */
#define UUID32_SIZE             4                               /**< Size of 32 bit UUID */
#define UUID128_SIZE            16                              /**< Size of 128 bit UUID */


// <q> BLE_DB_DISCOVERY_ENABLED  - ble_db_discovery - Database discovery module
#ifndef BLE_DB_DISCOVERY_ENABLED
#define BLE_DB_DISCOVERY_ENABLED 1
#endif

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
// <i> Priorities 0,2 (nRF51) and 0,1,4,5 (nRF52) are reserved for SoftDevice
// <0=> 0 (highest)
// <1=> 1
// <2=> 2
// <3=> 3
#ifndef CLOCK_CONFIG_IRQ_PRIORITY
#define CLOCK_CONFIG_IRQ_PRIORITY 3
#endif

#endif //CLOCK_ENABLED

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
#ifndef GPIOTE_CONFIG_IRQ_PRIORITY
#define GPIOTE_CONFIG_IRQ_PRIORITY 3
#endif

#endif //GPIOTE_ENABLED

// <e> SPIS_ENABLED - nrf_drv_spis - SPI Slave driver
//==========================================================
#ifndef SPIS_ENABLED
#define SPIS_ENABLED 1
#endif
#if  SPIS_ENABLED

// <o> SPIS_DEFAULT_CONFIG_IRQ_PRIORITY  - Interrupt priority
#ifndef SPIS_DEFAULT_CONFIG_IRQ_PRIORITY
#define SPIS_DEFAULT_CONFIG_IRQ_PRIORITY 3
#endif

// <o> SPIS_DEFAULT_MODE  - Mode
// <0=> MODE_0
// <1=> MODE_1
// <2=> MODE_2
// <3=> MODE_3
#ifndef SPIS_DEFAULT_MODE
#define SPIS_DEFAULT_MODE 0
#endif

// <o> SPIS_DEFAULT_BIT_ORDER  - SPIS default bit order
// <0=> MSB first
// <1=> LSB first
#ifndef SPIS_DEFAULT_BIT_ORDER
#define SPIS_DEFAULT_BIT_ORDER 0
#endif

// <o> SPIS_DEFAULT_DEF - SPIS default DEF character  <0-255>
#ifndef SPIS_DEFAULT_DEF
#define SPIS_DEFAULT_DEF 255
#endif

// <o> SPIS_DEFAULT_ORC - SPIS default ORC character  <0-255>
#ifndef SPIS_DEFAULT_ORC
#define SPIS_DEFAULT_ORC 255
#endif

// <q> SPIS0_ENABLED  - Enable SPIS0 instance
#ifndef SPIS0_ENABLED
#define SPIS0_ENABLED 0
#endif

// <q> SPIS1_ENABLED  - Enable SPIS1 instance
#ifndef SPIS1_ENABLED
#define SPIS1_ENABLED 1
#endif

// <q> SPIS2_ENABLED  - Enable SPIS2 instance
#ifndef SPIS2_ENABLED
#define SPIS2_ENABLED 0
#endif

#endif //SPIS_ENABLED

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

// <<< end of configuration section >>>
#endif //SDK_CONFIG_H
