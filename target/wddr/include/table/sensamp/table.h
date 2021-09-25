/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _SENSAMP_TABLE_H_
#define _SENSAMP_TABLE_H_

#include <stdint.h>
#include <wddr/phy_config.h>

/**
 * @brief   Sense Amp (Sensamp) Index Enumerations
 *
 * @details Enumeration to indicate valid Sensamp indices.
 *
 * @note    0 and 180 are indices used for DDR mode.
 * @note    90 and 270 are additional indices used for QDR mode.
 *
 */
typedef enum sensamp_index_t
{
    SA_0_INDEX = 0,
    SA_180_INDEX = 1,
    SA_90_INDEX = 2,
    SA_270_INDEX = 3
} sensamp_index_t;

/**
 * @brief   Sense Amp (Sensamp) DQBit Configuration Structure
 *
 * code     calibration code for a Sensamp bit.
 */
typedef struct sensamp_dqbit_cfg_t {
    uint8_t     code[WDDR_PHY_CFG];
} sensamp_dqbit_cfg_t;

/**
 * @brief   Sense Amp (Sensamp) Common Configuration Structure
 *
 * @details Calibration for entire DQ Byte that is common across all
 *          frequencies.
 *
 * dq       Array of DQBit cal codes for each DQ Slice.
 */
typedef struct sensamp_dqbyte_common_cfg_t {
    sensamp_dqbit_cfg_t dq[WDDR_PHY_DQ_SLICE_NUM];
} sensamp_dqbyte_common_cfg_t;

#endif /* _SENSAMP_TABLE_H_ */
