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
void dq_dq_egress_ana_set_mode_reg_if(dq_reg_t *dq_reg,
                                      wddr_msr_t msr,
                                      uint8_t bit,
                                      egress_ana_mode_t mode)
{
    configASSERT(bit < WDDR_PHY_DQ_SLICE_NUM);
    dq_reg->DDR_DQ_DQ_TX_EGRESS_ANA_CFG[msr][bit] = mode;
}

void dq_dq_egress_dig_set_mode_reg_if(dq_reg_t *dq_reg,
                                      wddr_msr_t msr,
                                      uint8_t bit,
                                      egress_dig_mode_t mode)
{
    configASSERT(bit < WDDR_PHY_DQ_SLICE_NUM);
    dq_reg->DDR_DQ_DQ_TX_EGRESS_DIG_CFG[msr][bit] = mode;
}

/*******************************************************************************
**                            DQS
*******************************************************************************/
void dq_dqs_egress_ana_set_mode_reg_if(dq_reg_t *dq_reg,
                                       wddr_msr_t msr,
                                       uint8_t bit,
                                       egress_ana_mode_t mode)
{
    configASSERT(bit < WDDR_PHY_DQS_SLICE_NUM);
    dq_reg->DDR_DQ_DQS_TX_EGRESS_ANA_CFG[msr][bit] = mode;
}

void dq_dqs_egress_dig_set_mode_reg_if(dq_reg_t *dq_reg,
                                       wddr_msr_t msr,
                                       uint8_t bit,
                                       egress_dig_mode_t mode)
{
    configASSERT(bit < WDDR_PHY_DQS_SLICE_NUM);
    dq_reg->DDR_DQ_DQS_TX_EGRESS_DIG_CFG[msr][bit] = mode;

}
