/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _VCO_TABLE_H_
#define _VCO_TABLE_H_

#include <stdint.h>

/**
 * @brief   VCO Frequency Configuration Structure
 *
 * @details VCO Frequency configuration structure used to store configuration
 *          settings for a single frequency.
 *
 * band                     band value of VCO.
 * fine                     fine band value of VCO.
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
    uint8_t     band;
    uint8_t     fine;
    const uint8_t     prop_gain;
    const uint8_t     int_comp;
    const uint8_t     post_div;
    const uint8_t     fll_refclk_count;
    const uint8_t     fll_range;
    const uint8_t     lock_count_threshold;
    const uint16_t    fll_vco_count_target;
} vco_cfg_t;

#endif /* _VCO_TABLE_H_ */
