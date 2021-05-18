/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#include <pipeline/rt_driver.h>
#include <kernel/io.h>

// BASE
#define RT_DQ_BIT_BASE__ADDR           (DDR_DQ_DQ_TX_RT_M0_R0_CFG__ADR)
#define RT_DQS_BIT_BASE__ADDR          (DDR_DQ_DQS_TX_RT_M0_R0_CFG__ADR)
#define RT_CA_BIT_BASE__ADDR           (DDR_CA_DQ_TX_RT_M0_R0_CFG__ADR)
#define RT_CK_BIT_BASE__ADDR           (DDR_CA_DQS_TX_RT_M0_R0_CFG__ADR)

/** @brief  RT Pipeline Set Pipe Enable MSR0 Register Interface */
static void rt_pipeline_set_pipe_en_msr0_reg_if(rt_pipeline_dev_t *rt_pipeline,
                                                wddr_slice_type_t slice_type,
                                                uint32_t pipe_en);

/** @brief  RT Pipeline Set Pipe Enable MSR1 Register Interface */
static void rt_pipeline_set_pipe_en_msr1_reg_if(rt_pipeline_dev_t *rt_pipeline,
                                                wddr_slice_type_t slice_type,
                                                uint32_t pipe_en);

static void rt_pipeline_set_base_reg_if(rt_pipeline_dev_t *rt_pipeline,
                                        wddr_slice_type_t slice_type,
                                        wddr_rank_t rank)
{
    uint32_t base, rank_offset;

    switch (slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            base = RT_DQ_BIT_BASE__ADDR;
            rank_offset = RT_DQ_RANK__OFFSET;
            break;
        case WDDR_SLICE_TYPE_DQS:
            base = RT_DQS_BIT_BASE__ADDR;
            rank_offset = RT_DQS_RANK__OFFSET;
            break;
        case WDDR_SLICE_TYPE_CA:
            base = RT_CA_BIT_BASE__ADDR;
            rank_offset = RT_CA_RANK__OFFSET;
            break;
        case WDDR_SLICE_TYPE_CK:
            base = RT_CK_BIT_BASE__ADDR;
            rank_offset = RT_CK_RANK__OFFSET;
            break;
        default:
            return;
    }

    rt_pipeline->base = (rt_pipeline->base & ~WDDR_MEMORY_MAP_REL_MASK) +
                  base +  rank * rank_offset;

}

void rt_pipeline_set_msr_reg_if(rt_pipeline_dev_t *rt_pipeline,
                                wddr_msr_t msr)
{
    rt_pipeline->msr = msr;
}

void rt_pipeline_init_reg_if(rt_pipeline_dev_t *rt_pipeline,
                             uint32_t base,
                             wddr_slice_type_t slice_type,
                             wddr_rank_t rank)
{
    rt_pipeline->base = base;
    rt_pipeline_set_msr_reg_if(rt_pipeline, WDDR_MSR_0);
    rt_pipeline_set_base_reg_if(rt_pipeline, slice_type, rank);
}

static void rt_pipeline_set_pipe_en_msr0_reg_if(rt_pipeline_dev_t *rt_pipeline,
                                                 wddr_slice_type_t slice_type,
                                                 uint32_t pipe_en)
{
    switch (slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
        case WDDR_SLICE_TYPE_DQS:
            reg_write(rt_pipeline->base + RT_PIPE_EN__OFFSET, pipe_en);
            break;
        case WDDR_SLICE_TYPE_CA:
        case WDDR_SLICE_TYPE_CK:
            reg_write(rt_pipeline->base + RT_CA_PIPE_EN__OFFSET, pipe_en);
            break;
    }
}

static void rt_pipeline_set_pipe_en_msr1_reg_if(rt_pipeline_dev_t *rt_pipeline,
                                                 wddr_slice_type_t slice_type,
                                                 uint32_t pipe_en)
{
    switch (slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            reg_write(rt_pipeline->base + RT_PIPE_EN__OFFSET + RT_DQ_MODE__OFFSET, pipe_en);
            break;
        case WDDR_SLICE_TYPE_DQS:
            reg_write(rt_pipeline->base + RT_PIPE_EN__OFFSET + RT_DQS_MODE__OFFSET, pipe_en);
            break;
        case WDDR_SLICE_TYPE_CA:
            reg_write(rt_pipeline->base + RT_CA_PIPE_EN__OFFSET + RT_CA_MODE__OFFSET, pipe_en);
            break;
        case WDDR_SLICE_TYPE_CK:
            reg_write(rt_pipeline->base + RT_CA_PIPE_EN__OFFSET + RT_CK_MODE__OFFSET, pipe_en);
            break;
    }
}

void rt_pipeline_set_pipe_en_reg_if(rt_pipeline_dev_t *rt_pipeline,
                                     wddr_slice_type_t slice_type,
                                     uint32_t pipe_en)
{
    if (rt_pipeline->msr == WDDR_MSR_1)
    {
        rt_pipeline_set_pipe_en_msr1_reg_if(rt_pipeline, slice_type, pipe_en);
        return;
    }

    rt_pipeline_set_pipe_en_msr0_reg_if(rt_pipeline, slice_type, pipe_en);
}
