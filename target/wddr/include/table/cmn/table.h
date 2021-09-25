/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _PATH_COMMON_TABLE_H_
#define _PATH_COMMON_TABLE_H_

#include <pmon/table.h>
#include <vref/table.h>
#include <zqcal/table.h>

/**
 * @brief   Common Path Frequency Configuration Structure
 *
 * @details Per-Frequency Configuration data for Common Path.
 *
 * vref     Voltage Reference configuration data.
 */
typedef struct common_path_freq_cfg_t
{
    vref_cfg_t vref;
} common_path_freq_cfg_t;

/**
 * @brief   Common Path Common Frequency Configuration Structure
 *
 * @details Frequency independent Configuration data for Common Path.
 *
 * pmon     Process Monitor calibration data.
 * zqcal    ZQCAL calibration data.
 */
typedef struct common_path_common_cfg_t
{
    pmon_cfg_t pmon;
    zqcal_cfg_t zqcal;
} common_path_common_cfg_t;

#endif /* _PATH_COMMON_TABLE_H_ */
