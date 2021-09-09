/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _PI_TX_TABLE_H_
#define _PI_TX_TABLE_H_

#include <pi/table.h>

/**
 * @brief   TX Phase Interpolator Configuration Frequency Structure
 *
 * @details Per frequency configuration data for TX PIs.
 *
 * @odr     Configuration for ODR Phase Interpolator.
 * @qdr     Configuration for QDR Phase Interpolator.
 * @ddr     Configuration for DDR Phase Interpolator.
 * @rt      Configuration for RT Phase Interpolator.
 * @sdr     Configuration for SDR delay match Phase Interpolator.
 * @dfi     Configuration for DFI delay match Phase Interpolator.
 */
typedef struct tx_pi_cfg_t
{
    pi_cfg_t    odr;
    pi_cfg_t    qdr;
    pi_cfg_t    ddr;
    pi_cfg_t    rt;
    pi_cfg_t    sdr;
    pi_cfg_t    dfi;
} tx_pi_cfg_t;

#endif /* _PI_TX_TABLE_H_ */
