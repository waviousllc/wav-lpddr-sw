/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#include <pipeline/qdr_driver.h>
#include <kernel/io.h>

// BASE
#define QDR_DQ_BIT_BASE__ADDR           (DDR_DQ_DQ_TX_QDR_M0_R0_CFG_0__ADR)
#define QDR_DQS_BIT_BASE__ADDR          (DDR_DQ_DQS_TX_QDR_M0_R0_CFG_0__ADR)
#define QDR_CA_BIT_BASE__ADDR           (DDR_CA_DQ_TX_QDR_M0_R0_CFG_0__ADR)
#define QDR_CK_BIT_BASE__ADDR           (DDR_CA_DQS_TX_QDR_M0_R0_CFG_0__ADR)

/** @brief  QDR Pipeline Set Pipe Enable MSR0 Register Interface */
static void qdr_pipeline_set_pipe_en_msr0_reg_if(qdr_pipeline_dev_t *qdr_pipeline,
                                                 wddr_slice_type_t slice_type,
                                                 uint32_t pipe_en);

/** @brief  QDR Pipeline Set Pipe Enable MSR1 Register Interface */
static void qdr_pipeline_set_pipe_en_msr1_reg_if(qdr_pipeline_dev_t *qdr_pipeline,
                                                 wddr_slice_type_t slice_type,
                                                 uint32_t pipe_en);

/** @brief  QDR Pipeline Set Phase Select MSR0 Register Interface */
static void qdr_pipeline_set_x_sel_msr0_reg_if(qdr_pipeline_dev_t *qdr_pipeline,
                                               wddr_slice_type_t slice_type,
                                               uint32_t x_sel);

/** @brief  QDR Pipeline Set Phase Select MSR1 Register Interface */
static void qdr_pipeline_set_x_sel_msr1_reg_if(qdr_pipeline_dev_t *qdr_pipeline,
                                               wddr_slice_type_t slice_type,
                                               uint32_t x_sel);

static void qdr_pipeline_set_base_reg_if(qdr_pipeline_dev_t *qdr_pipeline,
                                         wddr_slice_type_t slice_type,
                                         wddr_rank_t rank,
                                         uint8_t bit_index)
{
    uint32_t base, bit_offset, rank_offset;

    switch (slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            base = QDR_DQ_BIT_BASE__ADDR;
            rank_offset = QDR_DQ_RANK__OFFSET;
            bit_offset = QDR_DQ_BIT__OFFSET;
            break;
        case WDDR_SLICE_TYPE_DQS:
            base = QDR_DQS_BIT_BASE__ADDR;
            rank_offset = QDR_DQS_RANK__OFFSET;
            bit_offset = QDR_DQS_BIT__OFFSET;
            break;
        case WDDR_SLICE_TYPE_CA:
            base = QDR_CA_BIT_BASE__ADDR;
            rank_offset = QDR_CA_RANK__OFFSET;
            bit_offset = QDR_CA_BIT__OFFSET;
            break;
        case WDDR_SLICE_TYPE_CK:
            base = QDR_CK_BIT_BASE__ADDR;
            rank_offset = QDR_CK_RANK__OFFSET;
            bit_offset = QDR_CK_BIT__OFFSET;
            break;
        default:
            return;
    }

    qdr_pipeline->base = (qdr_pipeline->base & ~WDDR_MEMORY_MAP_REL_MASK) +
                  base + bit_index * bit_offset + rank * rank_offset;
}

void qdr_pipeline_set_msr_reg_if(qdr_pipeline_dev_t *qdr_pipeline,
                                 wddr_msr_t msr)
{
    qdr_pipeline->msr = msr;
}

void qdr_pipeline_init_reg_if(qdr_pipeline_dev_t *qdr_pipeline,
                              uint32_t base,
                              wddr_slice_type_t slice_type,
                              wddr_rank_t rank,
                              uint8_t bit)
{
    qdr_pipeline->base = base;
    qdr_pipeline_set_msr_reg_if(qdr_pipeline, WDDR_MSR_0);
    qdr_pipeline_set_base_reg_if(qdr_pipeline, slice_type, rank, bit);
}

static void qdr_pipeline_set_pipe_en_msr0_reg_if(qdr_pipeline_dev_t *qdr_pipeline,
                                                 wddr_slice_type_t slice_type,
                                                 uint32_t pipe_en)
{
    switch (slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            reg_write(qdr_pipeline->base + QDR_DQ_PIPE_EN__OFFSET, pipe_en);
            break;
        case WDDR_SLICE_TYPE_DQS:
            reg_write(qdr_pipeline->base + QDR_DQS_PIPE_EN__OFFSET, pipe_en);
            break;
        case WDDR_SLICE_TYPE_CA:
            reg_write(qdr_pipeline->base + QDR_CA_PIPE_EN__OFFSET, pipe_en);
            break;
        case WDDR_SLICE_TYPE_CK:
            reg_write(qdr_pipeline->base + QDR_CK_PIPE_EN__OFFSET, pipe_en);
            break;
    }
}

static void qdr_pipeline_set_pipe_en_msr1_reg_if(qdr_pipeline_dev_t *qdr_pipeline,
                                                 wddr_slice_type_t slice_type,
                                                 uint32_t pipe_en)
{
    switch (slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            reg_write(qdr_pipeline->base + QDR_DQ_PIPE_EN__OFFSET + QDR_DQ_MODE__OFFSET, pipe_en);
            break;
        case WDDR_SLICE_TYPE_DQS:
            reg_write(qdr_pipeline->base + QDR_DQS_PIPE_EN__OFFSET + QDR_DQS_MODE__OFFSET, pipe_en);
            break;
        case WDDR_SLICE_TYPE_CA:
            reg_write(qdr_pipeline->base + QDR_CA_PIPE_EN__OFFSET + QDR_CA_MODE__OFFSET, pipe_en);
            break;
        case WDDR_SLICE_TYPE_CK:
            reg_write(qdr_pipeline->base + QDR_CK_PIPE_EN__OFFSET + QDR_CK_MODE__OFFSET, pipe_en);
            break;
    }
}

void qdr_pipeline_set_pipe_en_reg_if(qdr_pipeline_dev_t *qdr_pipeline,
                                     wddr_slice_type_t slice_type,
                                     uint32_t pipe_en)
{
    if (qdr_pipeline->msr == WDDR_MSR_1)
    {
        qdr_pipeline_set_pipe_en_msr1_reg_if(qdr_pipeline, slice_type, pipe_en);
        return;
    }

    qdr_pipeline_set_pipe_en_msr0_reg_if(qdr_pipeline, slice_type, pipe_en);
}

static void qdr_pipeline_set_x_sel_msr0_reg_if(qdr_pipeline_dev_t *qdr_pipeline,
                                               wddr_slice_type_t slice_type,
                                               uint32_t x_sel)
{
    switch (slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            reg_write(qdr_pipeline->base + QDR_DQ_X_SEL__OFFSET, x_sel);
            break;
        case WDDR_SLICE_TYPE_DQS:
            reg_write(qdr_pipeline->base + QDR_DQS_X_SEL__OFFSET, x_sel);
            break;
        case WDDR_SLICE_TYPE_CA:
            reg_write(qdr_pipeline->base + QDR_CA_X_SEL__OFFSET, x_sel);
            break;
        case WDDR_SLICE_TYPE_CK:
            reg_write(qdr_pipeline->base + QDR_CK_X_SEL__OFFSET, x_sel);
            break;
    }
}

static void qdr_pipeline_set_x_sel_msr1_reg_if(qdr_pipeline_dev_t *qdr_pipeline,
                                               wddr_slice_type_t slice_type,
                                               uint32_t x_sel)
{
    switch (slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            reg_write(qdr_pipeline->base + QDR_DQ_X_SEL__OFFSET + QDR_DQ_MODE__OFFSET, x_sel);
            break;
        case WDDR_SLICE_TYPE_DQS:
            reg_write(qdr_pipeline->base + QDR_DQS_X_SEL__OFFSET + QDR_DQS_MODE__OFFSET, x_sel);
            break;
        case WDDR_SLICE_TYPE_CA:
            reg_write(qdr_pipeline->base + QDR_CA_X_SEL__OFFSET + QDR_CA_MODE__OFFSET, x_sel);
            break;
        case WDDR_SLICE_TYPE_CK:
            reg_write(qdr_pipeline->base + QDR_CK_X_SEL__OFFSET + QDR_CK_MODE__OFFSET, x_sel);
            break;
    }
}

void qdr_pipeline_set_x_sel_reg_if(qdr_pipeline_dev_t *qdr_pipeline,
                                   wddr_slice_type_t slice_type,
                                   uint32_t x_sel)
{
    if (qdr_pipeline->msr == WDDR_MSR_1)
    {
        qdr_pipeline_set_x_sel_msr1_reg_if(qdr_pipeline, slice_type, x_sel);
        return;
    }

    qdr_pipeline_set_x_sel_msr0_reg_if(qdr_pipeline, slice_type, x_sel);
}
