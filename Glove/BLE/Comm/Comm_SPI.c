/*****************************************************************************
* FILENAME: Comm_SPI.c                                                       *
*                                                                            *
* DESCRIPTION: Simple SPI communication wrapper                              *
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
* | @01     | 09Apr17  | BNordland  | Allow write only mode               |  *
*  ------------------------------------------------------------------------  *
******************************************************************************/

#include "Comm_SPI.h"

// Include the NordicSDK
#include "NordicSDK.h"

// Hardware pin locations
#include "Config_Hardware.h"

// Function Definitions
void pSPIEventHandler(nrf_drv_spi_evt_t const * p_event); // Handles events from SPI driver


// SPI Variables
static const nrf_drv_spi_t mSpi = NRF_DRV_SPI_INSTANCE(0);  // Initialize SPI instance 0
static volatile bool mSPITransferComplete;  // Indicates of a SPI transfer is complete

// Transmit and receive buffers
static uint8_t  mTxBuffer[16];
static uint8_t  mRxBuffer[16];

/*****************************************************************************
 * Description: Initializes the SPI interface to be ready for data transfer  *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters: None                                                          *
 *                                                                           *
 *****************************************************************************/
void Comm_SPI_Init()
{
    nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
    spi_config.ss_pin   = HDW_CONFIG_SPI_SS_PIN;
    spi_config.miso_pin = HDW_CONFIG_SPI_MISO_PIN;
    spi_config.mosi_pin = HDW_CONFIG_SPI_MOSI_PIN;
    spi_config.sck_pin  = HDW_CONFIG_SPI_SCK_PIN;
    APP_ERROR_CHECK(nrf_drv_spi_init(&mSpi, &spi_config, pSPIEventHandler));
}

/*****************************************************************************
 * Description: A serial transmit of data followed by receive from SPI       *
 *              interface. This works by doing the following:                *
 *                 1. Transmit txBytes of txBuffer via SPI                   *
 *                 2. Then Receive rxBytes into rxBuffer via SPI             *
 *                                                                           *
 *              Note: This works different from SPI which has the ability    *
 *                    to transmit the data and receive at the same time      *
 *                                                                           *
 * Returns: None                                                             *
 *                                                                           *
 * Parameters:                                                               *
 *     txBuffer* - A byte buffer of bytes to transmit                        *
 *     txBytes   - Number of bytes to transmit (max 8)                       *
 *     rxBuffer* - A byte buffer of bytes to receive                         *
 *     rxBytes   - Number of bytes to receive (max 8)                        *
 *                                                                           *
 *****************************************************************************/
void Comm_SPI_Transfer(uint8_t * txBuffer, uint8_t txBytes, uint8_t * rxBuffer, uint8_t rxBytes)
{
    if(txBytes <= 8 && rxBytes <= 8)
    {
        mSPITransferComplete = false;

        // Use static variables that are double our max size of both to handle SPI
        // transmit structure.
        memset(mTxBuffer, 0x00, sizeof(mTxBuffer));
        memset(mRxBuffer, 0x00, sizeof(mRxBuffer));

        // copy the tx buffer
        memcpy(mTxBuffer, txBuffer, txBytes);

        APP_ERROR_CHECK(nrf_drv_spi_transfer(&mSpi, mTxBuffer, (txBytes+rxBytes), mRxBuffer, (txBytes+rxBytes)));

        // wait for the transfer to be complete
        while(!mSPITransferComplete)
        {
            __WFE();
        }

        if(rxBytes > 0) // @01a - check before copying that we have at least 1 byte of received data
        {
            // copy from our static buffer into the receive buffer
            memcpy(rxBuffer, &mRxBuffer[txBytes],rxBytes);
        }
    }
    // TODO: we should throw an error here in the else clause
}


/*****************************************************************************
 * Description: Handles events from SPI driver. Used to indicate transmit is *
 *              complete.                                                    *
 *****************************************************************************/
void pSPIEventHandler(nrf_drv_spi_evt_t const * p_event)
{
    mSPITransferComplete = true;
}
