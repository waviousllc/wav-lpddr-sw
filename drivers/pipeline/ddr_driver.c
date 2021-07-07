/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#include <pipeline/ddr_driver.h>
#include <kernel/io.h>

// BASE
#define DDR_DQ_BIT_BASE__ADDR           (DDR_DQ_DQ_TX_DDR_M0_R0_CFG_0__ADR)
#define DDR_DQS_BIT_BASE__ADDR          (DDR_DQ_DQS_TX_DDR_M0_R0_CFG_0__ADR)
#define DDR_CA_BIT_BASE__ADDR           (DDR_CA_DQ_TX_DDR_M0_R0_CFG_0__ADR)
#define DDR_CK_BIT_BASE__ADDR           (DDR_CA_DQS_TX_DDR_M0_R0_CFG_0__ADR)

/** @brief  DDR Pipeline Set Pipe Enable MSR0 Register Interface */
static void ddr_pipeline_set_pipe_en_msr0_reg_if(ddr_pipeline_dev_t *ddr_pipeline,
                                                 wddr_slice_type_t slice_type,
                                                 uint32_t pipe_en);

/** @brief  DDR Pipeline Set Pipe Enable MSR1 Register Interface */
static void ddr_pipeline_set_pipe_en_msr1_reg_if(ddr_pipeline_dev_t *ddr_pipeline,
                                                 wddr_slice_type_t slice_type,
                                                 uint32_t pipe_en);

/** @brief  DDR Pipeline Set Phase Select MSR0 Register Interface */
static void ddr_pipeline_set_x_sel_msr0_reg_if(ddr_pipeline_dev_t *ddr_pipeline,
                                               wddr_slice_type_t slice_type,
                                               uint32_t x_sel);

/** @brief  DDR Pipeline Set Phase Select MSR1 Register Interface */
static void ddr_pipeline_set_x_sel_msr1_reg_if(ddr_pipeline_dev_t *ddr_pipeline,
                                               wddr_slice_type_t slice_type,
                                               uint32_t x_sel);

static void ddr_pipeline_set_base_reg_if(ddr_pipeline_dev_t *ddr_pipeline,
                                         wddr_slice_type_t slice_type,
                                         wddr_rank_t rank,
                                         uint8_t bit_index)
{
    uint32_t base, bit_offset, rank_offset;

    switch (slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            base = DDR_DQ_BIT_BASE__ADDR;
            rank_offset = DDR_DQ_RANK__OFFSET;
            bit_offset = DDR_DQ_BIT__OFFSET;
            break;
        case WDDR_SLICE_TYPE_DQS:
            base = DDR_DQS_BIT_BASE__ADDR;
            rank_offset = DDR_DQS_RANK__OFFSET;
            bit_offset = DDR_DQS_BIT__OFFSET;
            break;
        case WDDR_SLICE_TYPE_CA:
            base = DDR_CA_BIT_BASE__ADDR;
            rank_offset = DDR_CA_RANK__OFFSET;
            bit_offset = DDR_CA_BIT__OFFSET;
            break;
        case WDDR_SLICE_TYPE_CK:
            base = DDR_CK_BIT_BASE__ADDR;
            rank_offset = DDR_CK_RANK__OFFSET;
            bit_offset = DDR_CK_BIT__OFFSET;
            break;
        default:
            return;
    }

    ddr_pipeline->base = (ddr_pipeline->base & ~WDDR_MEMORY_MAP_REL_MASK) +
                  base + bit_index * bit_offset + rank * rank_offset;
}

void ddr_pipeline_set_msr_reg_if(ddr_pipeline_dev_t *ddr_pipeline,
                                 wddr_msr_t msr)
{
    ddr_pipeline->msr = msr;
}

void ddr_pipeline_init_reg_if(ddr_pipeline_dev_t *ddr_pipeline,
                              uint32_t base,
                              wddr_slice_type_t slice_type,
                              wddr_rank_t rank,
                              uint8_t bit)
{
    ddr_pipeline->base = base;
    ddr_pipeline_set_msr_reg_if(ddr_pipeline, WDDR_MSR_0);
    ddr_pipeline_set_base_reg_if(ddr_pipeline, slice_type, rank, bit);
}

static void ddr_pipeline_set_pipe_en_msr0_reg_if(ddr_pipeline_dev_t *ddr_pipeline,
                                                 wddr_slice_type_t slice_type,
                                                 uint32_t pipe_en)
{
    switch (slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            reg_write(ddr_pipeline->base + DDR_DQ_PIPE_EN__OFFSET, pipe_en);
            break;
        case WDDR_SLICE_TYPE_DQS:
            reg_write(ddr_pipeline->base + DDR_DQS_PIPE_EN__OFFSET, pipe_en);
            break;
        case WDDR_SLICE_TYPE_CA:
            reg_write(ddr_pipeline->base + DDR_CA_PIPE_EN__OFFSET, pipe_en);
            break;
        case WDDR_SLICE_TYPE_CK:
            reg_write(ddr_pipeline->base + DDR_CK_PIPE_EN__OFFSET, pipe_en);
            break;
    }
}

static void ddr_pipeline_set_pipe_en_msr1_reg_if(ddr_pipeline_dev_t *ddr_pipeline,
                                                 wddr_slice_type_t slice_type,
                                                 uint32_t pipe_en)
{
    switch (slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            reg_write(ddr_pipeline->base + DDR_DQ_PIPE_EN__OFFSET + DDR_DQ_MODE__OFFSET, pipe_en);
            break;
        case WDDR_SLICE_TYPE_DQS:
            reg_write(ddr_pipeline->base + DDR_DQS_PIPE_EN__OFFSET + DDR_DQS_MODE__OFFSET, pipe_en);
            break;
        case WDDR_SLICE_TYPE_CA:
            reg_write(ddr_pipeline->base + DDR_CA_PIPE_EN__OFFSET + DDR_CA_MODE__OFFSET, pipe_en);
            break;
        case WDDR_SLICE_TYPE_CK:
            reg_write(ddr_pipeline->base + DDR_CK_PIPE_EN__OFFSET + DDR_CK_MODE__OFFSET, pipe_en);
            break;
    }
}

void ddr_pipeline_set_pipe_en_reg_if(ddr_pipeline_dev_t *ddr_pipeline,
                                     wddr_slice_type_t slice_type,
                                     uint32_t pipe_en)
{
    if (ddr_pipeline->msr == WDDR_MSR_1)
    {
        ddr_pipeline_set_pipe_en_msr1_reg_if(ddr_pipeline, slice_type, pipe_en);
        return;
    }

    ddr_pipeline_set_pipe_en_msr0_reg_if(ddr_pipeline, slice_type, pipe_en);
}

static void ddr_pipeline_set_x_sel_msr0_reg_if(ddr_pipeline_dev_t *ddr_pipeline,
                                               wddr_slice_type_t slice_type,
                                               uint32_t x_sel)
{
    switch (slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            reg_write(ddr_pipeline->base + DDR_DQ_X_SEL__OFFSET, x_sel);
            break;
        case WDDR_SLICE_TYPE_DQS:
            reg_write(ddr_pipeline->base + DDR_DQS_X_SEL__OFFSET, x_sel);
            break;
        case WDDR_SLICE_TYPE_CA:
            reg_write(ddr_pipeline->base + DDR_CA_X_SEL__OFFSET, x_sel);
            break;
        case WDDR_SLICE_TYPE_CK:
            reg_write(ddr_pipeline->base + DDR_CK_X_SEL__OFFSET, x_sel);
            break;
    }
}

static void ddr_pipeline_set_x_sel_msr1_reg_if(ddr_pipeline_dev_t *ddr_pipeline,
                                               wddr_slice_type_t slice_type,
                                               uint32_t x_sel)
{
    switch (slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            reg_write(ddr_pipeline->base + DDR_DQ_X_SEL__OFFSET + DDR_DQ_MODE__OFFSET, x_sel);
            break;
        case WDDR_SLICE_TYPE_DQS:
            reg_write(ddr_pipeline->base + DDR_DQS_X_SEL__OFFSET + DDR_DQS_MODE__OFFSET, x_sel);
            break;
        case WDDR_SLICE_TYPE_CA:
            reg_write(ddr_pipeline->base + DDR_CA_X_SEL__OFFSET + DDR_CA_MODE__OFFSET, x_sel);
            break;
        case WDDR_SLICE_TYPE_CK:
            reg_write(ddr_pipeline->base + DDR_CK_X_SEL__OFFSET + DDR_CK_MODE__OFFSET, x_sel);
            break;
    }
}

void ddr_pipeline_set_x_sel_reg_if(ddr_pipeline_dev_t *ddr_pipeline,
                                   wddr_slice_type_t slice_type,
                                   uint32_t x_sel)
{
    if (ddr_pipeline->msr == WDDR_MSR_1)
    {
        ddr_pipeline_set_x_sel_msr1_reg_if(ddr_pipeline, slice_type, x_sel);
        return;
    }

    ddr_pipeline_set_x_sel_msr0_reg_if(ddr_pipeline, slice_type, x_sel);
}
