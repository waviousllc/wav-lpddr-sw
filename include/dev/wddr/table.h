/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _WDDR_TABLE_H_
#define _WDDR_TABLE_H_

#include <dfi/table.h>
#include <dram/table.h>
#include <path/common_table.h>
#include <path/dq_table.h>
#include <path/ca_table.h>
#include <pll/table.h>
#include "table_defs.h"

/**
 * @brief   Channel Frequency Calibration Structure
 *
 * @details Stores all calibration data for entire channel for a specific
 *          frequency.
 *
 * dq       per-frequency calibration data for both DQ bytes.
 * ca       per-frequency calibration data for CA.
 */
typedef struct channel_freq_cal_t
{
    dq_path_freq_cal_t dq[WDDR_PHY_DQ_BYTE_NUM];
    ca_path_freq_cal_t ca;
} channel_freq_cal_t;

/**
 * @brief   Channel Frequency Configuration Structure
 *
 * @details Stores all configuration data for entire channel for a specific
 *          frequency.
 *
 * dq       per-frequency configuration data for both DQ bytes.
 * ca       per-frequency configuration data for CA.
 */
typedef struct channel_freq_cfg_t
{
    dq_path_freq_cfg_t dq[WDDR_PHY_DQ_BYTE_NUM];
    ca_path_freq_cfg_t ca;
} channel_freq_cfg_t;

/**
 * @brief   Channel Common Frequency Calibration Structure
 *
 * @details Stores all calibration data for entire channel that is common across
 *          all frequencies.
 *
 * dq       frequency independent calibration data for both DQ bytes.
 */
typedef struct channel_common_cal_t
{
    dq_path_common_cal_t dq[WDDR_PHY_DQ_BYTE_NUM];
} channel_common_cal_t;

/**
 * @brief   Wavious DDR Frequency Calibration Structure
 *
 * @details Stores all calibration data for all devices for a specific frequency.
 *
 * pll      per-frequency calibration data for PLL.
 * channel  per-frequency calibration data for all Channels.
 * common   per-frequency calibration data for Common path.
 * dram     per-frequency calibration data for DRAM.
 */
typedef struct wddr_freq_cal_t
{
    channel_freq_cal_t          channel[WDDR_PHY_CHANNEL_NUM];
    common_path_freq_cal_t      common;
    dram_freq_cal_t             dram;
    pll_freq_cal_t              pll;
} wddr_freq_cal_t;

/**
 * @brief   Wavious DDR Frequency Configuration Structure
 *
 * @details Stores all configuration data for all devices for a specific frequency.
 *
 * pll      per-frequency configuration data for PLL.
 * channel  per-frequency configuration data for all channels.
 * common   per-frequency configuration data for common path.
 * dfi      per-frequency configuration data for DFI.
 * dram     per-frequency configuration data for DRAM.
 */
typedef struct wddr_freq_cfg_t
{
    channel_freq_cfg_t      channel[WDDR_PHY_CHANNEL_NUM];
    common_path_freq_cfg_t  common;
    dfi_freq_cfg_t          dfi;
    dram_freq_cfg_t         dram;
    pll_freq_cfg_t          pll;
} wddr_freq_cfg_t;

/**
 * @brief   Wavious DDR Common Calibration Structure
 *
 * @details Stores all calibration data for all devices that is common for all
 *          frequencies.
 *
 * channel  frequency independent calibration data for all channels.
 * common   frequency independent calibration data for common path.
 */
typedef struct wddr_common_cal_t
{
    channel_common_cal_t        channel[WDDR_PHY_CHANNEL_NUM];
    common_path_common_cal_t    common;
} wddr_common_cal_t;

/**
 * @brief   Wavious DDR Common Configuration Structure
 *
 * @details Stores all configuration data for all devices that is common for all
 *          frequencies.
 *
 * common   frequency independent configuration data for common path.
 */
typedef struct wddr_common_cfg_t
{
    common_path_common_cfg_t    common;
} wddr_common_cfg_t;

/**
 * @brief   WDDR Calibration Structure
 *
 * @details Stores all frequency dependent and frequency independent
 *          calibration data for all devices.
 *
 * freq     Per-Frequency calibration table.
 * common   Frequency independent calibration table.
 */
typedef struct wddr_cal_t
{
    wddr_freq_cal_t      freq[WDDR_PHY_FREQ_NUM];
    wddr_common_cal_t    common;
} wddr_cal_t;

/**
 * @brief   WDDR Configuration Structure
 *
 * @details Stores all frequency dependent and frequency independent
 *          configuration data for all devices.
 *
 * freq     Per-Frequency configuration table.
 * common   Frequency independent configuration table.
 */
typedef struct wddr_cfg_t
{
    wddr_freq_cfg_t      freq[WDDR_PHY_FREQ_NUM];
    wddr_common_cfg_t    common;
} wddr_cfg_t;

/**
 * @brief   WDDR Table Structure
 *
 * @details Table that stores all calibration and configuration data for
 *          the entire WDDR device for all frequencies.
 *
 * cal      calibration table.
 * cfg      configuration table.
 * valid    table of which frequencies are valid.
 */
typedef struct wddr_table_t
{
    wddr_cal_t cal;
    wddr_cfg_t cfg;
    uint8_t    valid[WDDR_PHY_FREQ_NUM];
} wddr_table_t;

#endif /* _WDDR_TABLE_H_ */
