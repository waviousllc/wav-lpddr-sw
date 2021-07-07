/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#ifndef _PLL_TABLE_H_
#define _PLL_TABLE_H_

#include <stdint.h>
#include <vco/table.h>

/**
 * @brief   VCO Indices
 *
 * @details Available VCO indices.
 *
 * MCU          Low Power VCO that is reserved for MCU clock.
 * PHY_START    Index of where PHY clock VCOs start.
 * PHY_1        First VCO available for PHY clock.
 * PHY_2        Second VCO available for PHY clock.
 * PHY_END      Index of where PHY clock VCOs end.
 * VCO_NUM      Total number of available VCOs in the design.
 */
typedef enum vco_index_t
{
    VCO_INDEX_MCU,
    VCO_INDEX_PHY_START,
    VCO_INDEX_PHY_1 = VCO_INDEX_PHY_START,
    VCO_INDEX_PHY_2,
    VCO_INDEX_PHY_END,
    VCO_INDEX_NUM = VCO_INDEX_PHY_END,
} vco_index_t;

/**
 * @brief   PLL Frequency Calibration Structure
 *
 * @details PLL Frequency calibration structure used to store calibrated
 *          settings for a single frequency.
 *
 * vco_cal  Table of VCO calibrated values for each VCO used to power PHY clock.
 */
typedef struct pll_freq_cal_t
{
    vco_cal_t   vco_cal[VCO_INDEX_PHY_END - VCO_INDEX_PHY_START];
} pll_freq_cal_t;

/**
 * @brief   PLL Frequency Configuration Structure
 *
 * @details PLL Frequency configuration structure used to store configuration
 *          settings for a single frequency.
 *
 * vco_cal  Table of VCO configuration values for each VCO used to power PHY
 *          clock.
 */
typedef struct pll_freq_cfg_t
{
    vco_cfg_t   vco_cfg[VCO_INDEX_PHY_END - VCO_INDEX_PHY_START];
} pll_freq_cfg_t;

#endif /* _PLL_TABLE_H_ */
