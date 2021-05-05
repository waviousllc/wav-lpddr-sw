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
 * @brief   Common Path Frequency Calibration Structure
 *
 * @details Per-Frequency Calibration data for Common Path.
 *
 * vref     Voltage Reference calibration data.
 */
typedef struct common_path_freq_cal_t
{
    vref_cal_t vref;
} common_path_freq_cal_t;

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
 * @brief   Common Path Common Frequency Calibration Structure
 *
 * @details Frequency independent Calibration data for Common Path.
 *
 * zqcal    ZQCAL calibration data.
 */
typedef struct common_path_common_cal_t
{
    zqcal_cal_t zqcal;
} common_path_common_cal_t;

/**
 * @brief   Common Path Common Frequency Configuration Structure
 *
 * @details Frequency independent Configuration data for Common Path.
 *
 * zqcal    Process Monitor calibration data.
 */
typedef struct common_path_common_cfg_t
{
    pmon_cfg_t pmon;
} common_path_common_cfg_t;

#endif /* _PATH_COMMON_TABLE_H_ */
