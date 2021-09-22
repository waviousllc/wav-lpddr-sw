/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _WDDR_TABLE_H_
#define _WDDR_TABLE_H_

#include <dfi/table.h>
#include <dram/table.h>
#include <cmn/table.h>
#include <channel/table.h>
#include <pll/table.h>
#include "table_defs.h"

/**
 * @brief   Wavious DDR Frequency Configuration Structure
 *
 * @details Stores all configuration data for all devices for a specific frequency.
 *
 * channel  per-frequency configuration data for all channels.
 * dfi      per-frequency configuration data for DFI.
 * dram     per-frequency configuration data for DRAM.
 * pll      per-frequency configuration data for PLL.
 * common   per-frequency configuration data for common path.
 */
typedef struct wddr_freq_cfg_t
{
    channel_freq_cfg_t      channel[WDDR_PHY_CHANNEL_NUM];
    dfi_freq_cfg_t          dfi;
    dram_freq_cfg_t         dram;
    pll_freq_cfg_t          pll;
    common_path_freq_cfg_t  common;
} wddr_freq_cfg_t;

/**
 * @brief   Wavious DDR Common Configuration Structure
 *
 * @details Stores all configuration data for all devices that is common for all
 *          frequencies.
 *
 * channel  frequency independent configuration data for all channels.
 * common   frequency independent configuration data for common path.
 */
typedef struct wddr_common_cfg_t
{
    channel_common_cfg_t        channel[WDDR_PHY_CHANNEL_NUM];
    common_path_common_cfg_t    common;
} wddr_common_cfg_t;

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
 * cfg      configuration table.
 * valid    table of which frequencies are valid.
 */
typedef struct wddr_table_t
{
    wddr_cfg_t cfg;
    uint8_t    valid[WDDR_PHY_FREQ_NUM];
} wddr_table_t;

#endif /* _WDDR_TABLE_H_ */
