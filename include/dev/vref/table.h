/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#ifndef _VREF_TABLE_H_
#define _VREF_TABLE_H_

#include <stdint.h>

/**
 * @brief   VREF Power Mode Enumeration
 *
 * VREF_POWER_MODE_0    Power mode 0
 * VREF_POWER_MODE_1    Power mode 1
 * VREF_POWER_MODE_2    Power mode 2
 * VREF_POWER_MODE_3    Power mode 3
 */
typedef enum vref_power_mode_t
{
    VREF_POWER_MODE_0,
    VREF_POWER_MODE_1,
    VREF_POWER_MODE_2,
    VREF_POWER_MODE_3
} vref_pwr_mode_t;

/**
 * @brief   VREF Calibration Structure
 *
 * code   Calibrated code value.
 */
typedef struct vref_cal_t
{
    uint16_t code;
} vref_cal_t;

/**
 * @brief   VREF Configuration Structure
 *
 * pwr_mode    Configured power mode.
 */
typedef struct vref_cfg_t
{
    vref_pwr_mode_t power_mode;
} vref_cfg_t;

#endif /* _VREF_TABLE_H_ */
