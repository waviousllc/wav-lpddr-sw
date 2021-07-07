/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#ifndef _LPDE_TX_DEV_H_
#define _LPDE_TX_DEV_H_

#include <stdint.h>
#include <lpde/device.h>
#include <wddr/phy_defs.h>

/**
 * @brief   Transmit (TX) LPDE Initalization
 *
 * @details Initializes TX LPDE device at device level.
 *
 * @param[in]   tx_lpde     pointer to TX LPDE device.
 * @param[in]   base        base address of TX LPDE device.
 * @param[in]   slice_type  slice type of TX LPDE device.
 * @param[in]   rank        rank of TX LPDE device.
 * @param[in]   bit         bit index fo TX LPDE device.
 *
 * @return      void
 */
void tx_lpde_init(lpde_dev_t *tx_lpde,
                  uint32_t base,
                  wddr_slice_type_t slice_type,
                  wddr_rank_t rank,
                  uint8_t bit);

#endif /* _LPDE_TX_DEV_H_ */
