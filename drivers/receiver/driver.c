/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#include <compiler.h>
#include <receiver/driver.h>
#include <wddr/memory_map.h>
#include <kernel/io.h>


#define RECEIVER_DISABLE    (0x0)
#define RECEIVER_ENABLE     (0x1)

#define RX_DQS_BASE__ADDR       (DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG__ADR)

// Rank / Mode Offsets
#define RX_DQS_RANK__OFFSET     (DDR_DQ_DQS_RX_IO_CMN_M0_R1_CFG__ADR - DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG__ADR)
#define RX_DQS_MODE__OFFSET     (DDR_DQ_DQS_RX_IO_CMN_M1_R0_CFG__ADR - DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG__ADR)

void receiver_set_msr_reg_if(receiver_dev_t *receiver,
                             wddr_msr_t msr,
                             wddr_rank_t rank)
{
    uint32_t base = receiver->base & ~WDDR_MEMORY_MAP_REL_MASK;
    receiver->base = base + RX_DQS_BASE__ADDR + rank * RX_DQS_RANK__OFFSET +
                     msr * RX_DQS_MODE__OFFSET;
}

void receiver_init_reg_if(receiver_dev_t *receiver,
                          uint32_t base,
                          wddr_rank_t rank)
{
    receiver->base = base;
    receiver_set_msr_reg_if(receiver, WDDR_MSR_0, rank);
}

void receiver_set_state_reg_if(receiver_dev_t *receiver,
                               receiver_state_t state)
{
    uint32_t reg_val;
    reg_val = reg_read(receiver->base);

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

    reg_write(receiver->base, reg_val);
}

void receiver_set_mode_reg_if(receiver_dev_t *receiver,
                              receiver_mode_t mode,
                              receiver_path_state_t path_state)
{
    uint32_t reg_val;
    reg_val = reg_read(receiver->base);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_SE_MODE, mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_DCPATH_EN, path_state);
    reg_write(receiver->base, reg_val);
}

void receiver_set_code_reg_if(receiver_dev_t *receiver,
                              uint8_t code[REC_PN_SIDE_NUM][REC_TC_SIDE_NUM])
{
    uint32_t reg_val;
    reg_val = reg_read(receiver->base);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_CAL_P_C, code[REC_P_SIDE][REC_C_SIDE]);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_CAL_P_T, code[REC_P_SIDE][REC_T_SIDE]);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_CAL_N_C, code[REC_N_SIDE][REC_C_SIDE]);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_CAL_N_T, code[REC_N_SIDE][REC_T_SIDE]);
    reg_write(receiver->base, reg_val);
}

void receiver_set_feedback_resistor_reg_if(receiver_dev_t *receiver,
                                           uint8_t feedback_resistor)
{
    uint32_t reg_val;
    reg_val = reg_read(receiver->base);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_FB_EN, feedback_resistor);
    reg_write(receiver->base, reg_val);
}
