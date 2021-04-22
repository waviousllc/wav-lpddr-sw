/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#include <pipeline/sdr_driver.h>
#include <wddr/memory_map.h>
#include <kernel/io.h>

// BASE
#define SDR_DQ_BIT_BASE__ADDR           (DDR_DQ_DQ_TX_SDR_M0_R0_CFG_0__ADR)
#define SDR_DQS_BIT_BASE__ADDR          (DDR_DQ_DQS_TX_SDR_M0_R0_CFG_0__ADR)
#define SDR_CA_BIT_BASE__ADDR           (DDR_CA_DQ_TX_SDR_M0_R0_CFG_0__ADR)
#define SDR_CK_BIT_BASE__ADDR           (DDR_CA_DQS_TX_SDR_M0_R0_CFG_0__ADR)

/** @brief  SDR Pipeline Set Pipe Enable MSR0 Register Interface */
static void sdr_pipeline_set_pipe_en_msr0_reg_if(sdr_pipeline_dev_t *sdr_pipeline,
                                                 wddr_slice_type_t slice_type,
                                                 uint32_t pipe_en);

/** @brief  SDR Pipeline Set Pipe Enable MSR1 Register Interface */
static void sdr_pipeline_set_pipe_en_msr1_reg_if(sdr_pipeline_dev_t *sdr_pipeline,
                                                 wddr_slice_type_t slice_type,
                                                 uint32_t pipe_en);

/** @brief  SDR Pipeline Set Phase Select MSR0 Register Interface */
static void sdr_pipeline_set_x_sel_msr0_reg_if(sdr_pipeline_dev_t *sdr_pipeline,
                                               wddr_slice_type_t slice_type,
                                               uint32_t x_sel);

/** @brief  SDR Pipeline Set Phase Select MSR1 Register Interface */
static void sdr_pipeline_set_x_sel_msr1_reg_if(sdr_pipeline_dev_t *sdr_pipeline,
                                               wddr_slice_type_t slice_type,
                                               uint32_t x_sel);

/** @brief  SDR Pipeline Set FC Delay MSR0 Register Interface */
static void sdr_pipeline_set_fc_delay_msr0_reg_if(sdr_pipeline_dev_t *sdr_pipeline,
                                                  wddr_slice_type_t slice_type,
                                                  uint32_t fc_delay);

/** @brief  SDR Pipeline Set FC Delay SR1 Register Interface */
static void sdr_pipeline_set_fc_delay_msr1_reg_if(sdr_pipeline_dev_t *sdr_pipeline,
                                                  wddr_slice_type_t slice_type,
                                                  uint32_t fc_delay);


static void sdr_pipeline_set_base_reg_if(sdr_pipeline_dev_t *sdr_pipeline,
                                         wddr_slice_type_t slice_type,
                                         wddr_rank_t rank,
                                         uint8_t bit_index)
{
    uint32_t base, bit_offset, rank_offset;

    switch (slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            base = SDR_DQ_BIT_BASE__ADDR;
            rank_offset = SDR_DQ_RANK__OFFSET;
            bit_offset = SDR_DQ_BIT__OFFSET;
            break;
        case WDDR_SLICE_TYPE_DQS:
            base = SDR_DQS_BIT_BASE__ADDR;
            rank_offset = SDR_DQS_RANK__OFFSET;
            bit_offset = SDR_DQS_BIT__OFFSET;
            break;
        case WDDR_SLICE_TYPE_CA:
            base = SDR_CA_BIT_BASE__ADDR;
            rank_offset = SDR_CA_RANK__OFFSET;
            bit_offset = SDR_CA_BIT__OFFSET;
            break;
        case WDDR_SLICE_TYPE_CK:
            base = SDR_CK_BIT_BASE__ADDR;
            rank_offset = SDR_CK_RANK__OFFSET;
            bit_offset = SDR_CK_BIT__OFFSET;
            break;
        default:
            return;
    }

    sdr_pipeline->base = (sdr_pipeline->base & ~WDDR_MEMORY_MAP_REL_MASK) +
                  base + bit_index * bit_offset + rank * rank_offset;
}

void sdr_pipeline_set_msr_reg_if(sdr_pipeline_dev_t *sdr_pipeline,
                                 wddr_msr_t msr)
{
    sdr_pipeline->msr = msr;
}

void sdr_pipeline_init_reg_if(sdr_pipeline_dev_t *sdr_pipeline,
                              uint32_t base,
                              wddr_slice_type_t slice_type,
                              wddr_rank_t rank,
                              uint8_t bit)
{
    sdr_pipeline->base = base;
    sdr_pipeline_set_msr_reg_if(sdr_pipeline, WDDR_MSR_0);
    sdr_pipeline_set_base_reg_if(sdr_pipeline, slice_type, rank, bit);
}

static void sdr_pipeline_set_pipe_en_msr0_reg_if(sdr_pipeline_dev_t *sdr_pipeline,
                                                 wddr_slice_type_t slice_type,
                                                 uint32_t pipe_en)
{
    switch (slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            reg_write(sdr_pipeline->base + SDR_DQ_PIPE_EN__OFFSET, pipe_en);
            break;
        case WDDR_SLICE_TYPE_DQS:
            reg_write(sdr_pipeline->base + SDR_DQS_PIPE_EN__OFFSET, pipe_en);
            break;
        case WDDR_SLICE_TYPE_CA:
            reg_write(sdr_pipeline->base + SDR_CA_PIPE_EN__OFFSET, pipe_en);
            break;
        case WDDR_SLICE_TYPE_CK:
            reg_write(sdr_pipeline->base + SDR_CK_PIPE_EN__OFFSET, pipe_en);
            break;
    }
}

static void sdr_pipeline_set_pipe_en_msr1_reg_if(sdr_pipeline_dev_t *sdr_pipeline,
                                                 wddr_slice_type_t slice_type,
                                                 uint32_t pipe_en)
{
    switch (slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            reg_write(sdr_pipeline->base + SDR_DQ_PIPE_EN__OFFSET + SDR_DQ_MODE__OFFSET, pipe_en);
            break;
        case WDDR_SLICE_TYPE_DQS:
            reg_write(sdr_pipeline->base + SDR_DQS_PIPE_EN__OFFSET + SDR_DQS_MODE__OFFSET, pipe_en);
            break;
        case WDDR_SLICE_TYPE_CA:
            reg_write(sdr_pipeline->base + SDR_CA_PIPE_EN__OFFSET + SDR_CA_MODE__OFFSET, pipe_en);
            break;
        case WDDR_SLICE_TYPE_CK:
            reg_write(sdr_pipeline->base + SDR_CK_PIPE_EN__OFFSET + SDR_CK_MODE__OFFSET, pipe_en);
            break;
    }
}

void sdr_pipeline_set_pipe_en_reg_if(sdr_pipeline_dev_t *sdr_pipeline,
                                     wddr_slice_type_t slice_type,
                                     uint32_t pipe_en)
{
    if (sdr_pipeline->msr == WDDR_MSR_1)
    {
        sdr_pipeline_set_pipe_en_msr1_reg_if(sdr_pipeline, slice_type, pipe_en);
        return;
    }

    sdr_pipeline_set_pipe_en_msr0_reg_if(sdr_pipeline, slice_type, pipe_en);
}

static void sdr_pipeline_set_x_sel_msr0_reg_if(sdr_pipeline_dev_t *sdr_pipeline,
                                               wddr_slice_type_t slice_type,
                                               uint32_t x_sel)
{
    switch (slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            reg_write(sdr_pipeline->base + SDR_DQ_X_SEL__OFFSET, x_sel);
            break;
        case WDDR_SLICE_TYPE_DQS:
            reg_write(sdr_pipeline->base + SDR_DQS_X_SEL__OFFSET, x_sel);
            break;
        case WDDR_SLICE_TYPE_CA:
            reg_write(sdr_pipeline->base + SDR_CA_X_SEL__OFFSET, x_sel);
            break;
        case WDDR_SLICE_TYPE_CK:
            reg_write(sdr_pipeline->base + SDR_CK_X_SEL__OFFSET, x_sel);
            break;
    }
}

static void sdr_pipeline_set_x_sel_msr1_reg_if(sdr_pipeline_dev_t *sdr_pipeline,
                                               wddr_slice_type_t slice_type,
                                               uint32_t x_sel)
{
    switch (slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            reg_write(sdr_pipeline->base + SDR_DQ_X_SEL__OFFSET + SDR_DQ_MODE__OFFSET, x_sel);
            break;
        case WDDR_SLICE_TYPE_DQS:
            reg_write(sdr_pipeline->base + SDR_DQS_X_SEL__OFFSET + SDR_DQS_MODE__OFFSET, x_sel);
            break;
        case WDDR_SLICE_TYPE_CA:
            reg_write(sdr_pipeline->base + SDR_CA_X_SEL__OFFSET + SDR_CA_MODE__OFFSET, x_sel);
            break;
        case WDDR_SLICE_TYPE_CK:
            reg_write(sdr_pipeline->base + SDR_CK_X_SEL__OFFSET + SDR_CK_MODE__OFFSET, x_sel);
            break;
    }
}

void sdr_pipeline_set_x_sel_reg_if(sdr_pipeline_dev_t *sdr_pipeline,
                                   wddr_slice_type_t slice_type,
                                   uint32_t x_sel)
{
    if (sdr_pipeline->msr == WDDR_MSR_1)
    {
        sdr_pipeline_set_x_sel_msr1_reg_if(sdr_pipeline, slice_type, x_sel);
        return;
    }

    sdr_pipeline_set_x_sel_msr0_reg_if(sdr_pipeline, slice_type, x_sel);
}

static void sdr_pipeline_set_fc_delay_msr0_reg_if(sdr_pipeline_dev_t *sdr_pipeline,
                                               wddr_slice_type_t slice_type,
                                               uint32_t fc_delay)
{
    switch (slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            reg_write(sdr_pipeline->base + SDR_DQ_FC_DLY__OFFSET, fc_delay);
            break;
        case WDDR_SLICE_TYPE_DQS:
            reg_write(sdr_pipeline->base + SDR_DQS_FC_DLY__OFFSET, fc_delay);
            break;
        case WDDR_SLICE_TYPE_CA:
            reg_write(sdr_pipeline->base + SDR_CA_FC_DLY__OFFSET, fc_delay);
            break;
        case WDDR_SLICE_TYPE_CK:
            reg_write(sdr_pipeline->base + SDR_CK_FC_DLY__OFFSET, fc_delay);
            break;
    }
}

static void sdr_pipeline_set_fc_delay_msr1_reg_if(sdr_pipeline_dev_t *sdr_pipeline,
                                                  wddr_slice_type_t slice_type,
                                                  uint32_t fc_delay)
{
    switch (slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            reg_write(sdr_pipeline->base + SDR_DQ_FC_DLY__OFFSET + SDR_DQ_MODE__OFFSET, fc_delay);
            break;
        case WDDR_SLICE_TYPE_DQS:
            reg_write(sdr_pipeline->base + SDR_DQS_FC_DLY__OFFSET + SDR_DQS_MODE__OFFSET, fc_delay);
            break;
        case WDDR_SLICE_TYPE_CA:
            reg_write(sdr_pipeline->base + SDR_CA_FC_DLY__OFFSET + SDR_CA_MODE__OFFSET, fc_delay);
            break;
        case WDDR_SLICE_TYPE_CK:
            reg_write(sdr_pipeline->base + SDR_CK_FC_DLY__OFFSET + SDR_CK_MODE__OFFSET, fc_delay);
            break;
    }
}

void sdr_pipeline_set_fc_delay_reg_if(sdr_pipeline_dev_t *sdr_pipeline,
                                      wddr_slice_type_t slice_type,
                                      uint32_t fc_delay)
{
    if (sdr_pipeline->msr == WDDR_MSR_1)
    {
        sdr_pipeline_set_fc_delay_msr1_reg_if(sdr_pipeline, slice_type, fc_delay);
        return;
    }

    sdr_pipeline_set_fc_delay_msr0_reg_if(sdr_pipeline, slice_type, fc_delay);
}
