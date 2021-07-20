/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <pipeline/device.h>
#include <pipeline/driver.h>

void pipeline_bit_init(pipeline_bit_dev_t *pipeline,
                       uint32_t base,
                       wddr_slice_type_t slice_type,
                       wddr_rank_t rank,
                       uint8_t bit)
{
    sdr_pipeline_init_reg_if(&pipeline->sdr, base, slice_type, rank, bit);
    ddr_pipeline_init_reg_if(&pipeline->ddr, base, slice_type, rank, bit);
    qdr_pipeline_init_reg_if(&pipeline->qdr, base, slice_type, rank, bit);
}
