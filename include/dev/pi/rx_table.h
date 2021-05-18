/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _PI_RX_TABLE_H_
#define _PI_RX_TABLE_H_

#include <pi/table.h>

/**
 * @brief   RX Phase Interpolator Calibration Frequency Structure
 *
 * @details Per frequency calibration data for RX PIs.
 *
 * rcs      Calibration for RCS Phase Interpolator.
 * ren      Calibration for REN Phase Interpolator.
 * rdqs     Calibration for RDQS Phase Interpolator.
 */
typedef struct rx_pi_cal_t
{
    pi_cal_t rcs;
    pi_cal_t ren;
    pi_cal_t rdqs;
} rx_pi_cal_t;

/**
 * @brief   RX Phase Interpolator Configuration Frequency Structure
 *
 * @details Per frequency configuration data for RX PIs.
 *
 * rcs      Configuration for RCS Phase Interpolator.
 * ren      Configuration for REN Phase Interpolator.
 * rdqs     Configuration for RDQS Phase Interpolator.
 */
typedef struct rx_pi_cfg_t
{
    pi_cfg_t rcs;
    pi_cfg_t ren;
    pi_cfg_t rdqs;
} rx_pi_cfg_t;

#endif /* _PI_RX_TABLE_H_ */
