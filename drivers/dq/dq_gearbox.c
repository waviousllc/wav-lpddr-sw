/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <dq/driver.h>
#include <kernel/io.h>

/*******************************************************************************
**                            DQ
*******************************************************************************/
void dq_dq_rx_gb_set_mode_reg_if(dq_reg_t *dq_reg,
                                 wddr_msr_t msr,
                                 rx_gb_cfg_t *cfg)
{
    uint32_t reg_val = dq_reg->DDR_DQ_DQ_RX_CFG[msr];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_RX_M0_CFG_RGB_MODE, cfg->data_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_RX_M0_CFG_FGB_MODE, cfg->fifo_mode);
    dq_reg->DDR_DQ_DQ_RX_CFG[msr] = reg_val;
}

/*******************************************************************************
**                            DQS
*******************************************************************************/
void dq_dqs_rx_gb_set_mode_reg_if(dq_reg_t *dq_reg,
                                  wddr_msr_t msr,
                                  rx_gb_cfg_t *cfg)
{
    uint32_t reg_val = dq_reg->DDR_DQ_DQS_RX_CFG[msr];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_M0_CFG_RGB_MODE, cfg->data_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_M0_CFG_FGB_MODE, cfg->fifo_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_M0_CFG_WCK_MODE, cfg->wck_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_M0_CFG_PRE_FILTER_SEL, cfg->pre_filter_sel);
    dq_reg->DDR_DQ_DQS_RX_CFG[msr] = reg_val;
}

void dq_dqs_tx_gb_set_mode_reg_if(dq_reg_t *dq_reg,
                                  wddr_msr_t msr,
                                  tx_gb_cfg_t *cfg)
{
    uint32_t reg_val = dq_reg->DDR_DQ_DQS_TX_CFG[msr];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_M0_CFG_TGB_MODE, cfg->data_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_M0_CFG_WGB_MODE, cfg->write_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_M0_CFG_CK2WCK_RATIO, cfg->ck2wck_ratio);
    dq_reg->DDR_DQ_DQS_TX_CFG[msr] = reg_val;
}
