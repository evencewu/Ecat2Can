#include <string.h>
#include "stm32f4xx.h"
#include "spi.h"

#include "esc.h"
#include "ecat_slv.h"
#include "utypes.h"

#include "pdo_override.h"


static int et1100 = 1;

static volatile DMA_PDI_transmission_state_t pdi_dma_transmission = NOT_STARTED;

/* SPI DMA RX buffer */
uint8_t pdo_rxbuf[PDO_TR_SIZE + RX_PADDING_SIZE] __attribute__((aligned (8)));
uint8_t * pdo_spi_rxbuf = pdo_rxbuf + RX_PADDING_SIZE;
uint8_t * pdo_coe_rxbuf = pdo_rxbuf + RX_PADDING_SIZE + ADDR_SIZE + WAIT_SIZE;

/* SPI DMA TX buffer  */
uint8_t pdo_txbuf[PDO_TR_SIZE + TX_PADDING_SIZE] __attribute__((aligned (8))) = { 0 };
uint8_t * pdo_spi_txbuf = pdo_txbuf + TX_PADDING_SIZE;
uint8_t * pdo_coe_txbuf = pdo_txbuf + TX_PADDING_SIZE + ADDR_SIZE;


void spi_dma_prepare_transmission(uint16_t address, uint8_t * tx_buffer, uint8_t cmd)
{
    // set address and command
    tx_buffer[0] = address >> 5;
    tx_buffer[1] = ((address & 0x1F) << 3) | cmd;

    if (cmd == ESC_CMD_READ_WAIT_STATE) {
        // set wait state byte
        tx_buffer[ADDR_SIZE] = READ_END_BYTE; 
        // set read termination
        tx_buffer[PDO_TR_SIZE - 1] = READ_END_BYTE;
    } else if (cmd == ESC_CMD_READ) {
        // set read termination
        tx_buffer[PDO_TR_SIZE - 1] = READ_END_BYTE;
    }
}

void spi_dma_setup(void)
{

}


void spi_dma_start(void)
{

}


void spi_dma_stop(void)
{

}


void spi_dma_rxpdo_start(void)
{
    memset(pdo_coe_txbuf, 0, PDO_SIZE);  /* make sure TX buffer is filled with 0s, no read end byte */
    spi_dma_prepare_transmission(ESC_SM2_sma, pdo_spi_txbuf, ESC_CMD_READ_WAIT_STATE);

    spi_dma_start();
}


void spi_dma_txpdo_start()
{
    spi_dma_prepare_transmission(ESC_SM3_sma, pdo_spi_txbuf, ESC_CMD_WRITE);

    spi_dma_start();
}


void init_override(void)
{
    spi_dma_setup();
    spi_setup();
    
    /* SMx_sml are calculated at runtime on PREOP_TO_SAFEOP, buffer size is needed on compilation
        TODO test if they match, when ESC is OP */
    // CC_ASSERT (ESCvar.ESC_SM2_sml == PDO_SIZE);
    // CC_ASSERT (ESCvar.ESC_SM3_sml == PDO_SIZE);
}


void rxpdo_override(void)
{
    spi_dma_rxpdo_start();
    spi_bidirectionally_transfer(et1100,pdo_spi_rxbuf,pdo_spi_txbuf,sizeof(pdo_spi_txbuf));
    on_rxpdo_transfer_finished();
}


void on_rxpdo_transfer_finished(void)
{
    spi_dma_stop();
    ESCvar.ALevent = etohs ((uint16_t)*pdo_spi_rxbuf);
    
    if (MAX_MAPPINGS_SM2 > 0)
    {
        COE_pdoUnpack (pdo_coe_rxbuf, ESCvar.sm2mappings, SMmap2);
    }
}


void txpdo_override(void)
{
    if (MAX_MAPPINGS_SM3 > 0)
    {
        COE_pdoPack (pdo_coe_txbuf, ESCvar.sm3mappings, SMmap3);
    }

    spi_dma_txpdo_start();
    spi_write(et1100,pdo_spi_txbuf,sizeof(pdo_spi_txbuf));
    on_txpdo_transfer_finished();
}


void on_txpdo_transfer_finished(void)
{
    spi_dma_stop();
    ESCvar.ALevent = etohs ((uint16_t)*pdo_spi_rxbuf);
}