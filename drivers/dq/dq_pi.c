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
void dq_dq_pi_odr_set_cfg_reg_if(dq_reg_t *dq_reg,
                                 wddr_msr_t msr,
                                 wddr_rank_t rank,
                                 bool enable,
                                 uint32_t cfg)
{
    uint32_t reg_val = cfg;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_ODR_PI_M0_R0_CFG_EN, enable);
    dq_reg->DDR_DQ_DQ_TX_ODR_PI_CFG[msr][rank] = reg_val;
}

void dq_dq_pi_qdr_set_cfg_reg_if(dq_reg_t *dq_reg,
                                 wddr_msr_t msr,
                                 wddr_rank_t rank,
                                 bool enable,
                                 uint32_t cfg)
{
    uint32_t reg_val = cfg;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_QDR_PI_0_M0_R0_CFG_EN, enable);
    dq_reg->DDR_DQ_DQ_TX_QDR_PI_0_CFG[msr][rank] = reg_val;
}

void dq_dq_pi_ddr_set_cfg_reg_if(dq_reg_t *dq_reg,
                                 wddr_msr_t msr,
                                 wddr_rank_t rank,
                                 bool enable,
                                 uint32_t cfg)
{
    uint32_t reg_val = cfg;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_DDR_PI_0_M0_R0_CFG_EN, enable);
    dq_reg->DDR_DQ_DQ_TX_DDR_PI_0_CFG[msr][rank] = reg_val;
}

void dq_dq_pi_rt_set_cfg_reg_if(dq_reg_t *dq_reg,
                                wddr_msr_t msr,
                                wddr_rank_t rank,
                                bool enable,
                                uint32_t cfg)
{
    uint32_t reg_val = cfg;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_PI_RT_M0_R0_CFG_EN, enable);
    dq_reg->DDR_DQ_DQ_TX_PI_RT_CFG[msr][rank] = reg_val;
}

/*******************************************************************************
**                            DQS
*******************************************************************************/
void dq_dqs_pi_odr_set_cfg_reg_if(dq_reg_t *dq_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  bool enable,
                                  uint32_t cfg)
{
    uint32_t reg_val = cfg;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_ODR_PI_M0_R0_CFG_EN, enable);
    dq_reg->DDR_DQ_DQS_TX_ODR_PI_CFG[msr][rank] = reg_val;
}

void dq_dqs_pi_qdr_set_cfg_reg_if(dq_reg_t *dq_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  bool enable,
                                  uint32_t cfg)
{
    uint32_t reg_val = cfg;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_QDR_PI_0_M0_R0_CFG_EN, enable);
    dq_reg->DDR_DQ_DQS_TX_QDR_PI_0_CFG[msr][rank] = reg_val;
}

void dq_dqs_pi_ddr_set_cfg_reg_if(dq_reg_t *dq_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  bool enable,
                                  uint32_t cfg)
{
    uint32_t reg_val = cfg;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_DDR_PI_0_M0_R0_CFG_EN, enable);
    dq_reg->DDR_DQ_DQS_TX_DDR_PI_0_CFG[msr][rank] = reg_val;
}

void dq_dqs_pi_rt_set_cfg_reg_if(dq_reg_t *dq_reg,
                                 wddr_msr_t msr,
                                 wddr_rank_t rank,
                                 bool enable,
                                 uint32_t cfg)
{
    uint32_t reg_val = cfg;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_PI_RT_M0_R0_CFG_EN, enable);
    dq_reg->DDR_DQ_DQS_TX_PI_RT_CFG[msr][rank] = reg_val;
}

void dq_dqs_pi_sdr_set_cfg_reg_if(dq_reg_t *dq_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  bool enable,
                                  uint32_t cfg)
{
    uint32_t reg_val = cfg;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_SDR_PI_M0_R0_CFG_EN, enable);
    dq_reg->DDR_DQ_DQS_TX_SDR_PI_CFG[msr][rank] = reg_val;
}

void dq_dqs_pi_dfi_set_cfg_reg_if(dq_reg_t *dq_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  bool enable,
                                  uint32_t cfg)
{
    uint32_t reg_val = cfg;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_DFI_PI_M0_R0_CFG_EN, enable);
    dq_reg->DDR_DQ_DQS_TX_DFI_PI_CFG[msr][rank] = reg_val;
}

void dq_dqs_pi_ren_set_cfg_reg_if(dq_reg_t *dq_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  bool enable,
                                  uint32_t cfg)
{
    uint32_t reg_val = cfg;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_REN_PI_M0_R0_CFG_EN, enable);
    dq_reg->DDR_DQ_DQS_RX_REN_PI_CFG[msr][rank] = reg_val;
}

void dq_dqs_pi_rcs_set_cfg_reg_if(dq_reg_t *dq_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  bool enable,
                                  uint32_t cfg)
{
    uint32_t reg_val = cfg;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_RCS_PI_M0_R0_CFG_EN, enable);
    dq_reg->DDR_DQ_DQS_RX_RCS_PI_CFG[msr][rank] = reg_val;
}

void dq_dqs_pi_rdqs_set_cfg_reg_if(dq_reg_t *dq_reg,
                                   wddr_msr_t msr,
                                   wddr_rank_t rank,
                                   bool enable,
                                   uint32_t cfg)
{
    uint32_t reg_val = cfg;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_RDQS_PI_0_M0_R0_CFG_EN, enable);
    dq_reg->DDR_DQ_DQS_RX_RDQS_PI_0_CFG[msr][rank] = reg_val;
}

void dq_dqs_pi_get_status(dq_reg_t *dq_reg,
                          uint8_t *ren_status,
                          uint8_t *rcs_status)
{
    uint32_t reg_val = dq_reg->DDR_DQ_DQS_RX_PI_STA;
    if (ren_status)
    {
        *ren_status = GET_REG_FIELD(reg_val, DDR_DQ_DQS_RX_PI_STA_REN_PI_PHASE);
    }

    if (rcs_status)
    {
        *rcs_status = GET_REG_FIELD(reg_val, DDR_DQ_DQS_RX_PI_STA_RCS_PI_PHASE);
    }
}
