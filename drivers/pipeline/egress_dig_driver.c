/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <pipeline/egress_dig_driver.h>
#include <wddr/memory_map.h>
#include <kernel/io.h>

// Base
#define EGRESS_DQ_BIT_BASE__ADDR    (DDR_DQ_DQ_TX_EGRESS_DIG_M0_CFG_0__ADR)
#define EGRESS_DQS_BIT_BASE__ADDR   (DDR_DQ_DQS_TX_EGRESS_DIG_M0_CFG_0__ADR)
#define EGRESS_CA_BIT_BASE__ADDR    (DDR_CA_DQ_TX_EGRESS_DIG_M0_CFG_0__ADR)
#define EGRESS_CK_BIT_BASE__ADDR    (DDR_CA_DQS_TX_EGRESS_DIG_M0_CFG_0__ADR)

/** @brief  Egress Ananlog Set Mode MSR0 Register Interface */
static void egress_dig_set_mode_msr0_reg_if(bit_egress_dev_t *egress_dig,
                                            wddr_slice_type_t slice_type,
                                            egress_dig_mode_t mode);

/** @brief  Egress Ananlog Set Mode MSR1 Register Interface */
static void egress_dig_set_mode_msr1_reg_if(bit_egress_dev_t *egress_dig,
                                            wddr_slice_type_t slice_type,
                                            egress_dig_mode_t mode);

static void egress_dig_set_base_reg_if(bit_egress_dev_t *egress_dig,
                                       wddr_slice_type_t slice_type,
                                       wddr_rank_t rank,
                                       uint8_t bit_index)
{
    uint32_t base, bit_offset, rank_offset;

    switch (slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            base = EGRESS_DQ_BIT_BASE__ADDR;
            rank_offset = EGRESS_DIG_DQ_RANK__OFFSET;
            bit_offset = EGRESS_DIG_DQ_BIT__OFFSET;
            break;
        case WDDR_SLICE_TYPE_DQS:
            base = EGRESS_DQS_BIT_BASE__ADDR;
            rank_offset = EGRESS_DIG_DQS_RANK__OFFSET;
            bit_offset = EGRESS_DIG_DQS_BIT__OFFSET;
            break;
        case WDDR_SLICE_TYPE_CA:
            base = EGRESS_CA_BIT_BASE__ADDR;
            rank_offset = EGRESS_DIG_CA_RANK__OFFSET;
            bit_offset = EGRESS_DIG_CA_BIT__OFFSET;
            break;
        case WDDR_SLICE_TYPE_CK:
            base = EGRESS_CK_BIT_BASE__ADDR;
            rank_offset = EGRESS_DIG_CK_RANK__OFFSET;
            bit_offset = EGRESS_DIG_CK_BIT__OFFSET;
            break;
        default:
           return;
    }

    egress_dig->base = (egress_dig->base & ~WDDR_MEMORY_MAP_REL_MASK) +
                  base + bit_index * bit_offset + rank * rank_offset;
}

void egress_dig_set_msr_reg_if(bit_egress_dev_t *egress_dig,
                               wddr_msr_t msr)
{
    egress_dig->msr = msr;
}

void egress_dig_init_reg_if(bit_egress_dev_t *egress_dig,
                            uint32_t base,
                            wddr_slice_type_t slice_type,
                            wddr_rank_t rank,
                            uint8_t bit)
{
    egress_dig->base = base;
    egress_dig_set_msr_reg_if(egress_dig, WDDR_MSR_0);
    egress_dig_set_base_reg_if(egress_dig, slice_type, rank, bit);
}

static void egress_dig_set_mode_msr0_reg_if(bit_egress_dev_t *egress_dig,
                                            wddr_slice_type_t slice_type,
                                            egress_dig_mode_t mode)
{
    uint32_t reg_val;
    switch (slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQ_TX_EGRESS_DIG_M0_CFG_0_EGRESS_MODE, mode);
            reg_write(egress_dig->base + EGRESS_DIG_DQ_CFG__OFFSET, reg_val);
            break;
        case WDDR_SLICE_TYPE_DQS:
            reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQS_TX_EGRESS_DIG_M0_CFG_0_EGRESS_MODE, mode);
            reg_write(egress_dig->base + EGRESS_DIG_DQS_CFG__OFFSET, reg_val);
            break;
        case WDDR_SLICE_TYPE_CA:
            reg_val = UPDATE_REG_FIELD(0x0, DDR_CA_DQ_TX_EGRESS_DIG_M0_CFG_0_EGRESS_MODE, mode);
            reg_write(egress_dig->base + EGRESS_DIG_CA_CFG__OFFSET, reg_val);
            break;
        case WDDR_SLICE_TYPE_CK:
            reg_val = UPDATE_REG_FIELD(0x0, DDR_CA_DQS_TX_EGRESS_DIG_M0_CFG_0_EGRESS_MODE, mode);
            reg_write(egress_dig->base + EGRESS_DIG_CK_CFG__OFFSET, reg_val);
            break;
    }
}

static void egress_dig_set_mode_msr1_reg_if(bit_egress_dev_t *egress_dig,
                                            wddr_slice_type_t slice_type,
                                            egress_dig_mode_t mode)
{
    uint32_t reg_val;
    switch (slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQ_TX_EGRESS_DIG_M0_CFG_0_EGRESS_MODE, mode);
            reg_write(egress_dig->base + EGRESS_DIG_DQ_CFG__OFFSET + EGRESS_DIG_DQ_MODE__OFFSET, reg_val);
            break;
        case WDDR_SLICE_TYPE_DQS:
            reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQS_TX_EGRESS_DIG_M0_CFG_0_EGRESS_MODE, mode);
            reg_write(egress_dig->base + EGRESS_DIG_DQS_CFG__OFFSET + EGRESS_DIG_DQS_MODE__OFFSET, reg_val);
            break;
        case WDDR_SLICE_TYPE_CA:
            reg_val = UPDATE_REG_FIELD(0x0, DDR_CA_DQ_TX_EGRESS_DIG_M0_CFG_0_EGRESS_MODE, mode);
            reg_write(egress_dig->base + EGRESS_DIG_CA_CFG__OFFSET + EGRESS_DIG_CA_MODE__OFFSET, reg_val);
            break;
        case WDDR_SLICE_TYPE_CK:
            reg_val = UPDATE_REG_FIELD(0x0, DDR_CA_DQS_TX_EGRESS_DIG_M0_CFG_0_EGRESS_MODE, mode);
            reg_write(egress_dig->base + EGRESS_DIG_CK_CFG__OFFSET + EGRESS_DIG_CK_MODE__OFFSET, reg_val);
            break;
    }
}

void egress_dig_set_mode_reg_if(bit_egress_dev_t *egress_dig,
                                wddr_slice_type_t slice_type,
                                egress_dig_mode_t mode)
{
    if (egress_dig->msr == WDDR_MSR_1)
    {
        egress_dig_set_mode_msr1_reg_if(egress_dig, slice_type, mode);
        return;
    }

    egress_dig_set_mode_msr0_reg_if(egress_dig, slice_type, mode);
}
