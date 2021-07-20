/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _LPDE_RX_DRIVER_H_
#define _LPDE_RX_DRIVER_H_

#include <lpde/rx_device.h>

/**
 * @brief   Receive (RX) LPDE Initalization Register Interface
 *
 * @details Initializes RX LPDE device at register interface level.
 *
 * @param[in]   rx_lpde     pointer to RX LPDE device.
 * @param[in]   base        base address of RX LPDE device.
 * @param[in]   slice_type  slice type of RX LPDE device.
 * @param[in]   rank        rank of RX LPDE device.
 *
 * @return      void
 */
void rx_lpde_init_reg_if(lpde_dev_t *rx_lpde,
                         uint32_t base,
                         wddr_slice_type_t slice_type,
                         wddr_rank_t rank);

/**
 * @brief   Receive (RX) LPDE Set Mode Set Register (MSR) Register Interface
 *
 * @details Configures RX LPDE device for given MSR value.
 *
 * @param[in]   rx_lpde     pointer to RX LPDE device.
 * @param[in]   msr         mode set register to configure for.
 * @param[in]   slice_type  slice type of RX LPDE device.
 * @param[in]   rank        rank of RX LPDE device.
 *
 * @return      void
 */
void rx_lpde_set_msr_reg_if(lpde_dev_t *rx_lpde,
                            wddr_msr_t msr,
                            wddr_slice_type_t slice_type,
                            wddr_rank_t rank);

#endif /* _LPDE_RX_DRIVER_H_ */
