/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _LPDE_TX_DRIVER_H_
#define _LPDE_TX_DRIVER_H_

#include <lpde/tx_device.h>

/**
 * @brief   Transmit (TX) LPDE Initalization Register Interface
 *
 * @details Initializes TX LPDE device at register interface level.
 *
 * @param[in]   tx_lpde     pointer to TX LPDE device.
 * @param[in]   base        base address of TX LPDE device.
 * @param[in]   slice_type  slice type of TX LPDE device.
 * @param[in]   rank        rank of TX LPDE device.
 * @param[in]   bit         bit index fo TX LPDE device.
 *
 * @return      void
 */
void tx_lpde_init_reg_if(lpde_dev_t *tx_lpde,
                         uint32_t base,
                         wddr_slice_type_t slice_type,
                         wddr_rank_t rank,
                         uint8_t bit);

/**
 * @brief   Transmit (TX) LPDE Set Mode Set Register (MSR) Register Interface
 *
 * @details Configures TX LPDE device for given MSR value.
 *
 * @param[in]   tx_lpde     pointer to TX LPDE device.
 * @param[in]   msr         mode set register to configure for.
 * @param[in]   slice_type  slice type of TX LPDE device.
 * @param[in]   rank        rank of TX LPDE device.
 * @param[in]   bit         bit index fo TX LPDE device.
 *
 * @return      void
 */
void tx_lpde_set_msr_reg_if(lpde_dev_t *tx_lpde,
                            wddr_msr_t msr,
                            wddr_slice_type_t slice_type,
                            wddr_rank_t rank,
                            uint8_t bit);

#endif /* _LPDE_TX_DRIVER_H_ */
