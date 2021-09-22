/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
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
 * @brief   VREF Configuration Structure
 *
 * pwr_mode     Configured power mode.
 * code         Calibrated code value.
 */
typedef struct vref_cfg_t
{
    const vref_pwr_mode_t power_mode;
    uint16_t code;
} vref_cfg_t;

#endif /* _VREF_TABLE_H_ */
