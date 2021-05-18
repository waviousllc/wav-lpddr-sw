/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _LPDE_RX_DEV_H_
#define _LPDE_RX_DEV_H_

#include <stdint.h>
#include <lpde/device.h>
#include <wddr/phy_defs.h>

/**
 * @brief   Receive (RX) LPDE Initalization
 *
 * @details Initializes RX LPDE device at device level.
 *
 * @param[in]   rx_lpde     pointer to RX LPDE device.
 * @param[in]   base        base address of RX LPDE device.
 * @param[in]   slice_type  slice type of RX LPDE device.
 * @param[in]   rank        rank of RX LPDE device.
 *
 * @return      void
 */
void rx_lpde_init(lpde_dev_t *rx_lpde,
                  uint32_t base,
                  wddr_slice_type_t slice_type,
                  wddr_rank_t rank);

#endif /*_LPDE_RX_DEV_H_ */
