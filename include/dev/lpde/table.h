/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _LPDE_TABLE_H_
#define _LPDE_TABLE_H_

#include <stdint.h>

/**
 * @brief   LPDE Calibration Structure
 *
 * delay   Calibrated delay value.
 */
typedef struct lpde_cal_t
{
    uint8_t delay;
} lpde_cal_t;

/**
 * @brief   LPDE Configuration Structure
 *
 * gear   Configured gear value.
 */
typedef struct lpde_cfg_t
{
    uint8_t gear;
} lpde_cfg_t;

#endif /* _LPDE_TABLE_H_ */
