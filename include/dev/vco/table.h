/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#ifndef _VCO_TABLE_H_
#define _VCO_TABLE_H_

#include <stdint.h>

/**
 * @brief   VCO Frequency Calibration Structure
 *
 * @details VCO Frequency calibration structure used to store calibrated
 *          settings for a single frequency.
 *
 * band     band value of VCO.
 * fine     fine band value of VCO.
 */
typedef struct vco_cal_t
{
    uint8_t    band;
    uint8_t    fine;
} vco_cal_t;

/**
 * @brief   VCO Frequency Configuration Structure
 *
 * @details VCO Frequency configuration structure used to store configuration
 *          settings for a single frequency.
 *
 * prop_gain                proportional gain value.
 * int_comp                 integer divider value.
 * post_div                 post divider setting.
 * fll_refclk_count         refclk counter value. Used for calibration.
 * fll_range                VCO range to consider locked. Used for calibration.
 * lock_count_threshold     VCO threshold to pass to be considered locked. Used
 *                          for calibration.
 * fll_vco_count_target     VCO count target. Used for calibration.
 */
typedef struct vco_cfg_t
{
    uint8_t     prop_gain;
    uint8_t     int_comp;
    uint8_t     post_div;
    uint8_t     fll_refclk_count;
    uint8_t     fll_range;
    uint8_t     lock_count_threshold;
    uint16_t    fll_vco_count_target;
} vco_cfg_t;

#endif /* _VCO_TABLE_H_ */
