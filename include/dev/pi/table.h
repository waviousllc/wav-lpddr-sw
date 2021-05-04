/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _PI_TABLE_H_
#define _PI_TABLE_H_

#include <stdint.h>

/**
 * @brief   Phase Interpolator Calibration Structure
 *
 * code     Calibrated code value.
 */
typedef struct pi_cal_t
{
    uint8_t code;
} pi_cal_t;

/**
 * @brief   Phase Interpolator Configuration Structure
 *
 * gear     Configured gear value.
 * xcpl     Configured xcpl value.
 */
typedef struct pi_cfg_t
{
    uint8_t gear;
    uint8_t xcpl;
} pi_cfg_t;

#endif /* _PI_TABLE_H_ */
