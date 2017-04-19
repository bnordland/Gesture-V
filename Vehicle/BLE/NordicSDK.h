/*****************************************************************************
* FILENAME: NordicSDK.h                                                      *
*                                                                            *
* DESCRIPTION: includes all of the Nordic SDK files required for our project *
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
* | @01     | 18Apr17  | BNordland  | Add SPI Slave Driver                |  *
*  ------------------------------------------------------------------------  *
******************************************************************************/

#include "nordic_common.h"
#include "app_error.h"
#include "ble_db_discovery.h"
#include "app_timer.h"
#include "app_util.h"
#include "boards.h"
#include "ble.h"
#include "ble_gap.h"
#include "ble_hci.h"
#include "softdevice_handler.h"
#include "ble_advdata.h"
#include "ble_nus_c.h"
#include "ble_srv_common.h"
#include "ble_gattc.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_drv_spis.h" // @01a - SPI Slave Driver
