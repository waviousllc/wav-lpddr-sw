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
void ca_dq_egress_ana_set_mode_reg_if(ca_reg_t *ca_reg,
                                      wddr_msr_t msr,
                                      uint8_t bit,
                                      egress_ana_mode_t mode)
{
    configASSERT(bit < WDDR_PHY_CA_SLICE_NUM);
    ca_reg->DDR_CA_DQ_TX_EGRESS_ANA_CFG[msr][bit] = mode;
}

void ca_dq_egress_dig_set_mode_reg_if(ca_reg_t *ca_reg,
                                      wddr_msr_t msr,
                                      uint8_t bit,
                                      egress_dig_mode_t mode)
{
    configASSERT(bit < WDDR_PHY_CA_SLICE_NUM);
    ca_reg->DDR_CA_DQ_TX_EGRESS_DIG_CFG[msr][bit] = mode;
}

/*******************************************************************************
**                            CK
*******************************************************************************/
void ca_dqs_egress_ana_set_mode_reg_if(ca_reg_t *ca_reg,
                                       wddr_msr_t msr,
                                       uint8_t bit,
                                       egress_ana_mode_t mode)
{
    configASSERT(bit < WDDR_PHY_CK_SLICE_NUM);
    ca_reg->DDR_CA_DQS_TX_EGRESS_ANA_CFG[msr][bit] = mode;
}

void ca_dqs_egress_dig_set_mode_reg_if(ca_reg_t *ca_reg,
                                       wddr_msr_t msr,
                                       uint8_t bit,
                                       egress_dig_mode_t mode)
{
    configASSERT(bit < WDDR_PHY_CK_SLICE_NUM);
    ca_reg->DDR_CA_DQS_TX_EGRESS_DIG_CFG[msr][bit] = mode;

}
