/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#include <pipeline/egress_ana_driver.h>
#include <wddr/memory_map.h>
#include <kernel/io.h>

// Base
#define EGRESS_DQ_BIT_BASE__ADDR    (DDR_DQ_DQ_TX_EGRESS_ANA_M0_CFG_0__ADR)
#define EGRESS_DQS_BIT_BASE__ADDR   (DDR_DQ_DQS_TX_EGRESS_ANA_M0_CFG_0__ADR)
#define EGRESS_CA_BIT_BASE__ADDR    (DDR_CA_DQ_TX_EGRESS_ANA_M0_CFG_0__ADR)
#define EGRESS_CK_BIT_BASE__ADDR    (DDR_CA_DQS_TX_EGRESS_ANA_M0_CFG_0__ADR)

/** @brief  Egress Ananlog Set Mode MSR0 Register Interface */
static void egress_ana_set_mode_msr0_reg_if(bit_egress_dev_t *egress_ana,
                                            wddr_slice_type_t slice_type,
                                            egress_ana_mode_t mode);

/** @brief  Egress Ananlog Set Mode MSR1 Register Interface */
static void egress_ana_set_mode_msr1_reg_if(bit_egress_dev_t *egress_ana,
                                            wddr_slice_type_t slice_type,
                                            egress_ana_mode_t mode);

static void egress_ana_set_base_reg_if(bit_egress_dev_t *egress_ana,
                                       wddr_slice_type_t slice_type,
                                       uint8_t bit_index)
{
    uint32_t base, bit_offset;

    switch (slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            base = EGRESS_DQ_BIT_BASE__ADDR;
            bit_offset = EGRESS_ANA_DQ_BIT__OFFSET;
            break;
        case WDDR_SLICE_TYPE_DQS:
            base = EGRESS_DQS_BIT_BASE__ADDR;
            bit_offset = EGRESS_ANA_DQS_BIT__OFFSET;
            break;
        case WDDR_SLICE_TYPE_CA:
            base = EGRESS_CA_BIT_BASE__ADDR;
            bit_offset = EGRESS_ANA_CA_BIT__OFFSET;
            break;
        case WDDR_SLICE_TYPE_CK:
            base = EGRESS_CK_BIT_BASE__ADDR;
            bit_offset = EGRESS_ANA_CK_BIT__OFFSET;
            break;
        default:
           return;
    }

    egress_ana->base = (egress_ana->base & ~WDDR_MEMORY_MAP_REL_MASK) +
                  base + bit_index * bit_offset;
}

void egress_ana_set_msr_reg_if(bit_egress_dev_t *egress_ana,
                               wddr_msr_t msr)
{
    egress_ana->msr = msr;
}

void egress_ana_init_reg_if(bit_egress_dev_t *egress_ana,
                            uint32_t base,
                            wddr_slice_type_t slice_type,
                            uint8_t bit)
{
    egress_ana->base = base;
    egress_ana_set_msr_reg_if(egress_ana, WDDR_MSR_0);
    egress_ana_set_base_reg_if(egress_ana, slice_type, bit);
}

static void egress_ana_set_mode_msr0_reg_if(bit_egress_dev_t *egress_ana,
                                            wddr_slice_type_t slice_type,
                                            egress_ana_mode_t mode)
{
    uint32_t reg_val;
    switch (slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQ_TX_EGRESS_ANA_M0_CFG_0_EGRESS_MODE, mode);
            reg_write(egress_ana->base + EGRESS_ANA_DQ_CFG__OFFSET, reg_val);
            break;
        case WDDR_SLICE_TYPE_DQS:
            reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQS_TX_EGRESS_ANA_M0_CFG_0_EGRESS_MODE, mode);
            reg_write(egress_ana->base + EGRESS_ANA_DQS_CFG__OFFSET, reg_val);
            break;
        case WDDR_SLICE_TYPE_CA:
            reg_val = UPDATE_REG_FIELD(0x0, DDR_CA_DQ_TX_EGRESS_ANA_M0_CFG_0_EGRESS_MODE, mode);
            reg_write(egress_ana->base + EGRESS_ANA_CA_CFG__OFFSET, reg_val);
            break;
        case WDDR_SLICE_TYPE_CK:
            reg_val = UPDATE_REG_FIELD(0x0, DDR_CA_DQS_TX_EGRESS_ANA_M0_CFG_0_EGRESS_MODE, mode);
            reg_write(egress_ana->base + EGRESS_ANA_CK_CFG__OFFSET, reg_val);
            break;
    }
}

static void egress_ana_set_mode_msr1_reg_if(bit_egress_dev_t *egress_ana,
                                            wddr_slice_type_t slice_type,
                                            egress_ana_mode_t mode)
{
    uint32_t reg_val;
    switch (slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQ_TX_EGRESS_ANA_M0_CFG_0_EGRESS_MODE, mode);
            reg_write(egress_ana->base + EGRESS_ANA_DQ_CFG__OFFSET + EGRESS_ANA_DQ_MODE__OFFSET, reg_val);
            break;
        case WDDR_SLICE_TYPE_DQS:
            reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQS_TX_EGRESS_ANA_M0_CFG_0_EGRESS_MODE, mode);
            reg_write(egress_ana->base + EGRESS_ANA_DQS_CFG__OFFSET + EGRESS_ANA_DQS_MODE__OFFSET, reg_val);
            break;
        case WDDR_SLICE_TYPE_CA:
            reg_val = UPDATE_REG_FIELD(0x0, DDR_CA_DQ_TX_EGRESS_ANA_M0_CFG_0_EGRESS_MODE, mode);
            reg_write(egress_ana->base + EGRESS_ANA_CA_CFG__OFFSET + EGRESS_ANA_CA_MODE__OFFSET, reg_val);
            break;
        case WDDR_SLICE_TYPE_CK:
            reg_val = UPDATE_REG_FIELD(0x0, DDR_CA_DQS_TX_EGRESS_ANA_M0_CFG_0_EGRESS_MODE, mode);
            reg_write(egress_ana->base + EGRESS_ANA_CK_CFG__OFFSET + EGRESS_ANA_CK_MODE__OFFSET, reg_val);
            break;
    }
}

void egress_ana_set_mode_reg_if(bit_egress_dev_t *egress_ana,
                                wddr_slice_type_t slice_type,
                                egress_ana_mode_t mode)
{
    if (egress_ana->msr == WDDR_MSR_1)
    {
        egress_ana_set_mode_msr1_reg_if(egress_ana, slice_type, mode);
        return;
    }

    egress_ana_set_mode_msr0_reg_if(egress_ana, slice_type, mode);
}
