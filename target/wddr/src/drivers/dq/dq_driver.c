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
void dq_dq_driver_set_impedance_reg_if(dq_reg_t *dq_reg,
                                       wddr_msr_t msr,
                                       uint8_t bit,
                                       driver_impedance_t tx,
                                       driver_impedance_t rx)
{
    configASSERT(bit < WDDR_PHY_DQ_SLICE_NUM);
    uint32_t reg_val = dq_reg->DDR_DQ_DQ_TX_IO_CFG[msr][bit];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_IO_M0_CFG_0_TX_IMPD, tx);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_IO_M0_CFG_0_RX_IMPD, rx);
    dq_reg->DDR_DQ_DQ_TX_IO_CFG[msr][bit] = reg_val;
}

void dq_dq_driver_set_oe_reg_if(dq_reg_t *dq_reg,
                                wddr_msr_t msr,
                                uint8_t bit,
                                bool enable)
{
    configASSERT(bit < WDDR_PHY_DQ_SLICE_NUM);
    uint32_t reg_val = dq_reg->DDR_DQ_DQ_TX_IO_CFG[msr][bit];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_IO_M0_CFG_0_SW_OVR, enable);
    dq_reg->DDR_DQ_DQ_TX_IO_CFG[msr][bit] = reg_val;
}

void dq_dq_driver_set_override_reg_if(dq_reg_t *dq_reg,
                                      wddr_msr_t msr,
                                      uint8_t bit,
                                      uint8_t sel,
                                      uint8_t val_t)
{
    configASSERT(bit < WDDR_PHY_DQ_SLICE_NUM);
    uint32_t reg_val = dq_reg->DDR_DQ_DQ_TX_IO_CFG[msr][bit];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_IO_M0_CFG_0_OVRD_SEL, sel);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_IO_M0_CFG_0_OVRD_VAL, val_t);
    dq_reg->DDR_DQ_DQ_TX_IO_CFG[msr][bit] = reg_val;
}

/*******************************************************************************
**                            DQS
*******************************************************************************/
void dq_dqs_driver_set_mode_reg_if(dq_reg_t *dq_reg,
                                   wddr_msr_t msr,
                                   wddr_rank_t rank,
                                   driver_mode_t mode)
{
    uint32_t reg_val = dq_reg->DDR_DQ_DQS_TX_IO_CMN_CFG[msr][rank];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_IO_CMN_M0_R0_CFG_SE_MODE, mode);
    dq_reg->DDR_DQ_DQS_TX_IO_CMN_CFG[msr][rank] = reg_val;
}

void dq_dqs_driver_set_code_reg_if(dq_reg_t *dq_reg,
                                   wddr_msr_t msr,
                                   wddr_rank_t rank,
                                   uint8_t code[DRVR_PN_CAL_NUM])
{
    uint32_t reg_val = dq_reg->DDR_DQ_DQS_TX_IO_CMN_CFG[msr][rank];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_IO_CMN_M0_R0_CFG_NCAL, code[DRVR_N_CAL]);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_IO_CMN_M0_R0_CFG_PCAL, code[DRVR_P_CAL]);
    dq_reg->DDR_DQ_DQS_TX_IO_CMN_CFG[msr][rank] = reg_val;
}

void dq_dqs_driver_set_loopback_mode_reg_if(dq_reg_t *dq_reg,
                                            wddr_msr_t msr,
                                            wddr_rank_t rank,
                                            driver_loopback_mode_t mode)
{
    uint32_t reg_val = dq_reg->DDR_DQ_DQS_TX_IO_CMN_CFG[msr][rank];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_IO_CMN_M0_R0_CFG_LPBK_EN, mode);
    dq_reg->DDR_DQ_DQS_TX_IO_CMN_CFG[msr][rank] = reg_val;
}

void dq_dqs_driver_set_impedance_reg_if(dq_reg_t *dq_reg,
                                        wddr_msr_t msr,
                                        uint8_t bit,
                                        driver_impedance_t tx,
                                        driver_impedance_t rx)
{
    configASSERT(bit < WDDR_PHY_DQS_TXRX_SLICE_NUM);
    uint32_t reg_val = dq_reg->DDR_DQ_DQS_TX_IO_CFG[msr][bit];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_IO_M0_CFG_0_TX_IMPD, tx);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_IO_M0_CFG_0_RX_IMPD, rx);
    dq_reg->DDR_DQ_DQS_TX_IO_CFG[msr][bit] = reg_val;
}

void dq_dqs_driver_set_oe_reg_if(dq_reg_t *dq_reg,
                                 wddr_msr_t msr,
                                 uint8_t bit,
                                 bool enable)
{
    configASSERT(bit < WDDR_PHY_DQS_TXRX_SLICE_NUM);
    uint32_t reg_val = dq_reg->DDR_DQ_DQS_TX_IO_CFG[msr][bit];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_IO_M0_CFG_0_SW_OVR, enable);
    dq_reg->DDR_DQ_DQS_TX_IO_CFG[msr][bit] = reg_val;
}

void dq_dqs_driver_set_override_reg_if(dq_reg_t *dq_reg,
                                       wddr_msr_t msr,
                                       uint8_t bit,
                                       uint8_t sel,
                                       uint8_t val_t,
                                       uint8_t val_c)
{
    configASSERT(bit < WDDR_PHY_DQS_TXRX_SLICE_NUM);
    uint32_t reg_val = dq_reg->DDR_DQ_DQS_TX_IO_CFG[msr][bit];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_IO_M0_CFG_0_OVRD_SEL, sel);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_IO_M0_CFG_0_OVRD_VAL_T, val_t);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_IO_M0_CFG_0_OVRD_VAL_C, val_c);
    dq_reg->DDR_DQ_DQS_TX_IO_CFG[msr][bit] = reg_val;
}
