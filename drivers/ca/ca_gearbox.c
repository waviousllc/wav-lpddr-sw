/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <ca/driver.h>
#include <kernel/io.h>

/*******************************************************************************
**                            CA
*******************************************************************************/
void ca_dq_rx_gb_set_mode_reg_if(ca_reg_t *ca_reg,
                                 wddr_msr_t msr,
                                 rx_gb_cfg_t *cfg)
{
    uint32_t reg_val = ca_reg->DDR_CA_DQ_RX_CFG[msr];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_RX_M0_CFG_RGB_MODE, cfg->data_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_RX_M0_CFG_FGB_MODE, cfg->fifo_mode);
    ca_reg->DDR_CA_DQ_RX_CFG[msr] = reg_val;
}

/*******************************************************************************
**                            CK
*******************************************************************************/
void ca_dqs_rx_gb_set_mode_reg_if(ca_reg_t *ca_reg,
                                  wddr_msr_t msr,
                                  rx_gb_cfg_t *cfg)
{
    uint32_t reg_val = ca_reg->DDR_CA_DQS_RX_CFG[msr];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_RX_M0_CFG_RGB_MODE, cfg->data_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_RX_M0_CFG_FGB_MODE, cfg->fifo_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_RX_M0_CFG_WCK_MODE, cfg->wck_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_RX_M0_CFG_PRE_FILTER_SEL, cfg->pre_filter_sel);
    ca_reg->DDR_CA_DQS_RX_CFG[msr] = reg_val;
}

void ca_dqs_tx_gb_set_mode_reg_if(ca_reg_t *ca_reg,
                                  wddr_msr_t msr,
                                  tx_gb_cfg_t *cfg)
{
    uint32_t reg_val = ca_reg->DDR_CA_DQS_TX_CFG[msr];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_M0_CFG_TGB_MODE, cfg->data_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_M0_CFG_WGB_MODE, cfg->write_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_M0_CFG_CK2WCK_RATIO, cfg->ck2wck_ratio);
    ca_reg->DDR_CA_DQS_TX_CFG[msr] = reg_val;
}
