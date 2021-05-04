/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _PIPELINE_DEV_H_
#define _PIPELINE_DEV_H_

#include <stdint.h>
#include <pipeline/table.h>

/**
 * @brief   Pipeline Device Structure
 *
 * @details Pipeline device structure used for all pipeline types.
 *
 * base     base address of Pipeline device.
 * msr      current MSR value for the pipeline.
 */
typedef struct pipeline_dev_t
{
    uint32_t    base;
    wddr_msr_t  msr;
} pipeline_dev_t;

/* Aliases */
typedef pipeline_dev_t sdr_pipeline_dev_t;
typedef pipeline_dev_t ddr_pipeline_dev_t;
typedef pipeline_dev_t qdr_pipeline_dev_t;
typedef pipeline_dev_t rt_pipeline_dev_t;

/**
 * @brief   Bit Egress Structure
 *
 * @details Bit Egress device structure used for all egress types.
 *
 * base     base address of Pipeline device.
 * msr      current MSR value for the pipeline.
 */
typedef struct bit_egress_dev_t
{
    uint32_t    base;
    wddr_msr_t  msr;
} bit_egress_dev_t;

/**
 * @brief   Pipeline Bit Device Structure
 *
 * @details Pipeline Bit device structured used to represent full TX pipeilne
 *          for a given bit.
 *
 * qdr          QDR pipeline device.
 * ddr          DDR pipeline device.
 * sdr          SDR pipeline device.
 * egress_dig   Digital egress device.
 * egress_ana   Analog egress device.
 */
typedef struct pipeline_bit_dev_t
{
    qdr_pipeline_dev_t              qdr;
    ddr_pipeline_dev_t              ddr;
    sdr_pipeline_dev_t              sdr;
    bit_egress_dev_t                egress_dig;
    bit_egress_dev_t                egress_ana;
} pipeline_bit_dev_t;

/**
 * @brief   Pipeline Bit Intialization
 *
 * @details Initializes Pipeline Bit device at the device level.
 *
 * @param[in]   pipeline    pointer to the Pipeline Bit device.
 * @param[in]   base        base address of the Pipeline Bit device.
 * @param[in]   slice_type  type of slice of the Pipeline Bit device.
 * @param[in]   rank        rank of Pipeline Bit device.
 * @param[in]   bit         bit index of the Pipeline Bit device.
 *
 * @return      void
 */
void pipeline_bit_init(pipeline_bit_dev_t *pipeline,
                       uint32_t base,
                       wddr_slice_type_t slice_type,
                       wddr_rank_t rank,
                       uint8_t bit);

#endif /* _PIPELINE_DEV_H_ */
