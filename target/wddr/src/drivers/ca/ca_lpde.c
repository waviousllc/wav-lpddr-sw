/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <ca/driver.h>
#include <kernel/io.h>
#include <FreeRTOS.h>

/*******************************************************************************
**                            CA
*******************************************************************************/
void ca_dq_lpde_set_cfg_reg_if(ca_reg_t *ca_reg,
                               wddr_msr_t msr,
                               wddr_rank_t rank,
                               uint8_t bit,
                               bool enable,
                               uint32_t cfg)
{
    uint32_t reg_val = cfg;
    configASSERT(bit < WDDR_PHY_CA_SLICE_NUM);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_TX_LPDE_M0_R0_CFG_0_EN, enable);
    ca_reg->DDR_CA_DQ_TX_LPDE_CFG[msr][rank][bit] = cfg;
}

/*******************************************************************************
**                            CK
*******************************************************************************/
void ca_dqs_lpde_set_cfg_reg_if(ca_reg_t *ca_reg,
                                wddr_msr_t msr,
                                wddr_rank_t rank,
                                uint8_t bit,
                                bool enable,
                                uint32_t cfg)
{
    uint32_t reg_val = cfg;
    configASSERT(bit < WDDR_PHY_CK_TXRX_SLICE_NUM);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_LPDE_M0_R0_CFG_0_EN, enable);
    ca_reg->DDR_CA_DQS_TX_LPDE_CFG[msr][rank][bit] = cfg;
}

void ca_dqs_lpde_rx_sdr_set_cfg_reg_if(ca_reg_t *ca_reg,
                                       wddr_msr_t msr,
                                       wddr_rank_t rank,
                                       bool enable,
                                       uint32_t cfg)
{
    uint32_t reg_val = cfg;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_RX_SDR_LPDE_M0_R0_CFG_EN, enable);
    ca_reg->DDR_CA_DQS_RX_SDR_LPDE_CFG[msr][rank] = cfg;
}
