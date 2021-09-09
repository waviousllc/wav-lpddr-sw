/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _PIPELINE_TABLE_H_
#define _PIPELINE_TABLE_H_

#include <stdint.h>
#include <wddr/phy_defs.h>

/**
 * @brief   SDR Pipeline Configuration Structure
 *
 * @details Per frequency SDR Pipeline configuration data.
 *
 * x_sel    Phase Select configuration.
 * fc_delay Full Cycle Delay value.
 * pipe_en  Pipe Enable (half cycle delay) value.
 */
typedef struct sdr_pipeline_cfg_t
{
    uint32_t x_sel;
    uint16_t fc_delay;
    uint16_t pipe_en;
} sdr_pipeline_cfg_t;

/**
 * @brief   DDR Pipeline Configuration Structure
 *
 * @details Per frequency DDR Pipeline configuration data.
 *
 * x_sel    Phase Select configuration.
 * pipe_en  Pipe Enable (half cycle delay) value.
 */
typedef struct ddr_pipeline_cfg_t
{
    uint32_t x_sel;
    uint16_t pipe_en;
} ddr_pipeline_cfg_t;

/**
 * @brief   QDR Pipeline Configuration Structure
 *
 * @details Per frequency QDR Pipeline configuration data.
 *
 * x_sel    Phase Select configuration.
 * pipe_en  Pipe Enable (half cycle delay) value.
 */
typedef struct qdr_pipeline_cfg_t
{
    uint32_t x_sel;
    uint16_t pipe_en;
} qdr_pipeline_cfg_t;

/**
 * @brief   Retimer (RT) Pipeline Configuration Structure
 *
 * @details Per frequency RT Pipeline configuration data.
 *
 * pipe_en  Pipe Enable (half cycle delay) value.
 */
typedef struct rt_pipeline_cfg_t
{
    uint16_t pipe_en;
} rt_pipeline_cfg_t;

/**
 * @brief   Egress Digital Mode Enumerations
 *
 * @details Supported WDDR Egress Digital Modes.
 *
 * @note    DDR prefix must be here for not very interesting reasons.
 */
typedef enum egress_dig_mode_t
{
    DDR_DIG_EGRESS_SDR = 1,
    DDR_DIG_EGRESS_DDR_2TO1 = 2,
    DDR_DIG_EGRESS_QDR_2TO1 = 4,
    DDR_DIG_EGRESS_ODR_2TO1 = 8,
    DDR_DIG_EGRESS_QDR_4TO1 = 16,
    DDR_DIG_EGRESS_ODR_4TO1 = 32,
    DDR_DIG_EGRESS_BSCAN = 64,
} egress_dig_mode_t;

/**
 * @brief   Egress Analog Mode Enumerations
 *
 * @details Supported WDDR Egress Analog Modes.
 *
 * @note    DDR prefix must be here for not very interesting reasons.
 */
typedef enum egress_ana_mode_t
{
    DDR_ANA_EGRESS_BYPASS = 1,
    DDR_ANA_EGRESS_DDR_2TO1 = 2,
    DDR_ANA_EGRESS_QDR_2TO1 = 4,
    DDR_ANA_EGRESS_ODR_2TO1 = 8,
    DDR_ANA_EGRESS_QDR_4TO1 = 16,
    DDR_ANA_EGRESS_ODR_4TO1 = 32,
} egress_ana_mode_t;

/**
 * @brief Pipeline Bit Configuration Structure
 *
 * @details Per frequency configuration data for each bit in the pipeline.
 *
 * sdr      sdr dp parameters.
 * ddr      ddr dp parameters.
 * qdr      qdr dp parameters.

 */
typedef struct pipeline_bit_cfg_t
{
    sdr_pipeline_cfg_t sdr;
    ddr_pipeline_cfg_t ddr;
    qdr_pipeline_cfg_t qdr;
} pipeline_bit_cfg_t;

/**
 *  @brief  Egress Bit Configuration Structure
 *
 * @details Per-bit settings for analog and digital egress modes.
 *
 * dig_mode digital egress mode.
 * ana_mode analog egress mode.
 */
typedef struct egress_bit_cfg_t
{
    egress_dig_mode_t dig_mode;
    egress_ana_mode_t ana_mode;
} egress_bit_cfg_t;

#endif /* _PIPELINE_TABLE_H_ */
