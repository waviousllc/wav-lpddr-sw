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
 * @brief   Sense Amp (Sensamp) DQBit Calibration Structure
 *
 * code     calibration code for a Sensamp bit.
 */
typedef struct sensamp_dqbit_cal_t {
    uint8_t     code[WDDR_PHY_CFG];
} sensamp_dqbit_cal_t;

/**
 * @brief   Sense Amp (Sensamp) Calibration Structure
 *
 * @details Calibration for entire DQ Byte that is common across all
 *          frequencies.
 *
 * dq       Array of DQBit cal codes for each Rank and DQ Slice.
 */
typedef struct sensamp_dqbyte_common_cal_t {
    sensamp_dqbit_cal_t dq[WDDR_PHY_RANK][WDDR_PHY_DQ_SLICE_NUM];
} sensamp_dqbyte_common_cal_t;

#endif /* _SENSAMP_TABLE_H_ */
