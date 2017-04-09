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
* | None    | 08Apr17  | BNordland  | Initial creation                    |  *
*  ------------------------------------------------------------------------  *
******************************************************************************/


// nordic SDK header files
#include "nordic_common.h"
#include "nrf.h"
#include "app_error.h"
#include "ble.h"
#include "ble_hci.h"
#include "ble_srv_common.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "boards.h"
#include "softdevice_handler.h"
#include "app_timer.h"
#include "bsp.h"
#include "bsp_btn_ble.h"
#include "sensorsim.h"
#include "nrf_gpio.h"
#include "ble_hci.h"
#include "ble_advdata.h"
#include "ble_advertising.h"

// Nodric SDK Peer Manager Includes
#include "fstorage.h"
#include "fds.h"
#include "peer_manager.h"
#include "ble_conn_state.h"

// Nordic SDK GPIO
#include "nrf_gpio.h"

// Nordic SPI Includes
#include "nrf_drv_spi.h"
