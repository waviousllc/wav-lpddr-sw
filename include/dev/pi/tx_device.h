/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#ifndef _PI_TX_DEV_H_
#define _PI_TX_DEV_H_

#include <pi/device.h>
#include <wddr/phy_defs.h>

/**
 * @brief   TX Phase Interpolator Structure
 *
 * @details Device structure representing all TX Phase Interpolators.
 *
 * odr      ODR Phase Interpolator.
 * qdr      QDR Phase Interpolator.
 * ddr      DDR Phase Interpolator.
 * rt       RT Phase Interpolator.
 * sdr      SDR delay match Phase Interpolator.
 * dfi      DFI delay match Phase Interpolator.
 */
typedef struct tx_pi_dev_t
{
    pi_dev_t    odr;
    pi_dev_t    qdr;
    pi_dev_t    ddr;
    pi_dev_t    rt;
    pi_dev_t    sdr;
    pi_dev_t    dfi;
} tx_pi_dev_t;

/**
 * @brief   TX Phase Interpolator Initialization
 *
 * @details Initializes TX Phase Interpolator device at device level.
 *
 * @param[in]   tx_pi       pointer to TX Phase Interpolator device.
 * @param[in]   base        base address of TX Phase Interpolator device.
 * @param[in]   slice_type  slice type of TX Phase Interpolator device.
 * @param[in]   rank        rank of TX Phase Interpolator device.
 *
 * @return      void
 */
void tx_pi_init(tx_pi_dev_t *tx_pi,
                uint32_t base,
                wddr_slice_type_t slice_type,
                wddr_rank_t rank);

#endif /* _PI_RX_DEV_H_ */
