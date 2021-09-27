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
void ca_dq_driver_set_impedance_reg_if(ca_reg_t *ca_reg,
                                       wddr_msr_t msr,
                                       uint8_t bit,
                                       driver_impedance_t tx,
                                       driver_impedance_t rx)
{
    configASSERT(bit < WDDR_PHY_CA_SLICE_NUM);
    uint32_t reg_val = ca_reg->DDR_CA_DQ_TX_IO_CFG[msr][bit];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_TX_IO_M0_CFG_0_TX_IMPD, tx);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_TX_IO_M0_CFG_0_RX_IMPD, rx);
    ca_reg->DDR_CA_DQ_TX_IO_CFG[msr][bit] = reg_val;
}

void ca_dq_driver_set_oe_reg_if(ca_reg_t *ca_reg,
                                wddr_msr_t msr,
                                uint8_t bit,
                                bool enable)
{
    configASSERT(bit < WDDR_PHY_CA_SLICE_NUM);
    uint32_t reg_val = ca_reg->DDR_CA_DQ_TX_IO_CFG[msr][bit];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_TX_IO_M0_CFG_0_SW_OVR, enable);
    ca_reg->DDR_CA_DQ_TX_IO_CFG[msr][bit] = reg_val;
}

void ca_dq_driver_set_override_reg_if(ca_reg_t *ca_reg,
                                      wddr_msr_t msr,
                                      uint8_t bit,
                                      uint8_t sel,
                                      uint8_t val_t)
{
    configASSERT(bit < WDDR_PHY_CA_SLICE_NUM);
    uint32_t reg_val = ca_reg->DDR_CA_DQ_TX_IO_CFG[msr][bit];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_TX_IO_M0_CFG_0_OVRD_SEL, sel);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_TX_IO_M0_CFG_0_OVRD_VAL, val_t);
    ca_reg->DDR_CA_DQ_TX_IO_CFG[msr][bit] = reg_val;
}

/*******************************************************************************
**                            CK
*******************************************************************************/
void ca_dqs_driver_set_mode_reg_if(ca_reg_t *ca_reg,
                                   wddr_msr_t msr,
                                   wddr_rank_t rank,
                                   driver_mode_t mode)
{
    uint32_t reg_val = ca_reg->DDR_CA_DQS_TX_IO_CMN_CFG[msr][rank];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_IO_CMN_M0_R0_CFG_SE_MODE, mode);
    ca_reg->DDR_CA_DQS_TX_IO_CMN_CFG[msr][rank] = reg_val;
}

void ca_dqs_driver_set_code_reg_if(ca_reg_t *ca_reg,
                                   wddr_msr_t msr,
                                   wddr_rank_t rank,
                                   uint8_t code[DRVR_PN_CAL_NUM])
{
    uint32_t reg_val = ca_reg->DDR_CA_DQS_TX_IO_CMN_CFG[msr][rank];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_IO_CMN_M0_R0_CFG_NCAL, code[DRVR_N_CAL]);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_IO_CMN_M0_R0_CFG_PCAL, code[DRVR_P_CAL]);
    ca_reg->DDR_CA_DQS_TX_IO_CMN_CFG[msr][rank] = reg_val;
}

void ca_dqs_driver_set_loopback_mode_reg_if(ca_reg_t *ca_reg,
                                            wddr_msr_t msr,
                                            wddr_rank_t rank,
                                            driver_loopback_mode_t mode)
{
    uint32_t reg_val = ca_reg->DDR_CA_DQS_TX_IO_CMN_CFG[msr][rank];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_IO_CMN_M0_R0_CFG_LPBK_EN, mode);
    ca_reg->DDR_CA_DQS_TX_IO_CMN_CFG[msr][rank] = reg_val;
}

void ca_dqs_driver_set_impedance_reg_if(ca_reg_t *ca_reg,
                                        wddr_msr_t msr,
                                        uint8_t bit,
                                        driver_impedance_t tx,
                                        driver_impedance_t rx)
{
    configASSERT(bit < WDDR_PHY_CK_TXRX_SLICE_NUM);
    uint32_t reg_val = ca_reg->DDR_CA_DQS_TX_IO_CFG[msr][bit];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_IO_M0_CFG_0_TX_IMPD, tx);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_IO_M0_CFG_0_RX_IMPD, rx);
    ca_reg->DDR_CA_DQS_TX_IO_CFG[msr][bit] = reg_val;
}

void ca_dqs_driver_set_oe_reg_if(ca_reg_t *ca_reg,
                                 wddr_msr_t msr,
                                 uint8_t bit,
                                 bool enable)
{
    configASSERT(bit < WDDR_PHY_CK_TXRX_SLICE_NUM);
    uint32_t reg_val = ca_reg->DDR_CA_DQS_TX_IO_CFG[msr][bit];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_IO_M0_CFG_0_SW_OVR, enable);
    ca_reg->DDR_CA_DQS_TX_IO_CFG[msr][bit] = reg_val;
}

void ca_dqs_driver_set_override_reg_if(ca_reg_t *ca_reg,
                                       wddr_msr_t msr,
                                       uint8_t bit,
                                       uint8_t sel,
                                       uint8_t val_t,
                                       uint8_t val_c)
{
    configASSERT(bit < WDDR_PHY_CK_TXRX_SLICE_NUM);
    uint32_t reg_val = ca_reg->DDR_CA_DQS_TX_IO_CFG[msr][bit];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_IO_M0_CFG_0_OVRD_SEL, sel);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_IO_M0_CFG_0_OVRD_VAL_T, val_t);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_IO_M0_CFG_0_OVRD_VAL_C, val_c);
    ca_reg->DDR_CA_DQS_TX_IO_CFG[msr][bit] = reg_val;
}
