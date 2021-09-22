/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <dq/driver.h>
#include <kernel/io.h>
#include <FreeRTOS.h>

#define RECEIVER_DISABLE    (0)
#define RECEIVER_ENABLE     (1)

/*******************************************************************************
**                            DQS
*******************************************************************************/
void dq_dqs_receiver_set_state_reg_if(dq_reg_t *dq_reg,
                                       wddr_msr_t msr,
                                       wddr_rank_t rank,
                                       receiver_state_t state)
{
    uint32_t reg_val = dq_reg->DDR_DQ_DQS_RX_IO_CMN_CFG[msr][rank];
    switch(state)
    {
        case REC_STATE_DISABLE:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_EN, RECEIVER_DISABLE);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_RXCAL_EN, RECEIVER_DISABLE);
            break;
        case REC_STATE_ENABLE:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_EN, RECEIVER_ENABLE);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_RXCAL_EN, RECEIVER_DISABLE);
            break;
        case REC_STATE_CAL:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_EN, RECEIVER_ENABLE);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_RXCAL_EN, RECEIVER_ENABLE);
            break;
        default:
            return;
    }
    dq_reg->DDR_DQ_DQS_RX_IO_CMN_CFG[msr][rank] = reg_val;
}

void dq_dqs_receiver_set_mode_reg_if(dq_reg_t *dq_reg,
                                     wddr_msr_t msr,
                                     wddr_rank_t rank,
                                     receiver_mode_t mode,
                                     receiver_path_state_t path_state)
{
    uint32_t reg_val = dq_reg->DDR_DQ_DQS_RX_IO_CMN_CFG[msr][rank];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_SE_MODE, mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_DCPATH_EN, path_state);
    dq_reg->DDR_DQ_DQS_RX_IO_CMN_CFG[msr][rank] = reg_val;
}

void dq_dqs_receiver_set_code_reg_if(dq_reg_t *dq_reg,
                                     wddr_msr_t msr,
                                     wddr_rank_t rank,
                                     uint8_t code[REC_PN_SIDE_NUM][REC_TC_SIDE_NUM])
{
    uint32_t reg_val = dq_reg->DDR_DQ_DQS_RX_IO_CMN_CFG[msr][rank];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_CAL_P_C, code[REC_P_SIDE][REC_C_SIDE]);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_CAL_P_T, code[REC_P_SIDE][REC_T_SIDE]);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_CAL_N_C, code[REC_N_SIDE][REC_C_SIDE]);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_CAL_N_T, code[REC_N_SIDE][REC_T_SIDE]);
    dq_reg->DDR_DQ_DQS_RX_IO_CMN_CFG[msr][rank] = reg_val;
}

void dq_dqs_receiver_set_feedback_resistor_reg_if(dq_reg_t *dq_reg,
                                                  wddr_msr_t msr,
                                                  wddr_rank_t rank,
                                                  uint8_t feedback_resistor)
{
    uint32_t reg_val = dq_reg->DDR_DQ_DQS_RX_IO_CMN_CFG[msr][rank];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_FB_EN, feedback_resistor);
    dq_reg->DDR_DQ_DQS_RX_IO_CMN_CFG[msr][rank] = reg_val;
}

void dq_dqs_receiver_set_delay_reg_if(dq_reg_t *dq_reg,
                                      wddr_msr_t msr,
                                      wddr_rank_t rank,
                                      uint8_t bit,
                                      uint16_t delay,
                                      receiver_tc_side_mask_t side)
{
    configASSERT(bit < WDDR_PHY_DQS_TXRX_SLICE_NUM);
    uint32_t reg_val = dq_reg->DDR_DQ_DQS_RX_IO_CFG[msr][rank][bit];
    if (side & REC_T_SIDE_MASK)
    {
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_M0_R0_CFG_0_DLY_CTRL_T, delay);
    }

    if (side & REC_C_SIDE_MASK)
    {
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_M0_R0_CFG_0_DLY_CTRL_C, delay);
    }
    dq_reg->DDR_DQ_DQS_RX_IO_CFG[msr][rank][bit] = reg_val;
}
