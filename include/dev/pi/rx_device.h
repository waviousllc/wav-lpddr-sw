/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _PI_RX_DEV_H_
#define _PI_RX_DEV_H_

#include <pi/device.h>
#include <wddr/phy_defs.h>

/**
 * @brief   RX Phase Interpolator Structure
 *
 * @details Device structure representing all RX Phase Interpolators.
 *
 * ren      REN phase interpolator.
 * rcs      RCS phase interpolator.
 * rdqs     RDQS phase interpolator.
 */
typedef struct rx_pi_dev_t
{
    pi_dev_t ren;
    pi_dev_t rcs;
    pi_dev_t rdqs;
} rx_pi_dev_t;

/**
 * @brief   RX Phase Interpolator Initialization
 *
 * @details Initializes RX Phase Interpolator device at device level.
 *
 * @param[in]   rx_pi       pointer to RX Phase Interpolator device.
 * @param[in]   base        base address of RX Phase Interpolator device.
 * @param[in]   slice_type  slice type of RX Phase Interpolator device.
 * @param[in]   rank        rank of RX Phase Interpolator device.
 *
 * @return      void
 */
void rx_pi_init(rx_pi_dev_t *rx_pi,
                uint32_t base,
                wddr_slice_type_t slice_type,
                wddr_rank_t rank);

#endif /* _PI_RX_DEV_H_ */
