/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <dq/driver.h>
#include <kernel/io.h>
#include <FreeRTOS.h>

/*******************************************************************************
**                            DQ
*******************************************************************************/
void dq_dq_lpde_set_cfg_reg_if(dq_reg_t *dq_reg,
                               wddr_msr_t msr,
                               wddr_rank_t rank,
                               uint8_t bit,
                               bool enable,
                               uint32_t cfg)
{
    uint32_t reg_val = cfg;
    configASSERT(bit < WDDR_PHY_DQ_SLICE_NUM);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_LPDE_M0_R0_CFG_0_EN, enable);
    dq_reg->DDR_DQ_DQ_TX_LPDE_CFG[msr][rank][bit] = cfg;
}

/*******************************************************************************
**                            DQS
*******************************************************************************/
void dq_dqs_lpde_set_cfg_reg_if(dq_reg_t *dq_reg,
                                wddr_msr_t msr,
                                wddr_rank_t rank,
                                uint8_t bit,
                                bool enable,
                                uint32_t cfg)
{
    uint32_t reg_val = cfg;

    configASSERT(bit < WDDR_PHY_DQS_TXRX_SLICE_NUM);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_LPDE_M0_R0_CFG_0_EN, enable);
    dq_reg->DDR_DQ_DQS_TX_LPDE_CFG[msr][rank][bit] = cfg;
}

void dq_dqs_lpde_rx_sdr_set_cfg_reg_if(dq_reg_t *dq_reg,
                                       wddr_msr_t msr,
                                       wddr_rank_t rank,
                                       bool enable,
                                       uint32_t cfg)
{
    uint32_t reg_val = cfg;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_SDR_LPDE_M0_R0_CFG_EN, enable);
    dq_reg->DDR_DQ_DQS_RX_SDR_LPDE_CFG[msr][rank] = cfg;
}
