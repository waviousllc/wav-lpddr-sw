/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _PI_RX_DRIVER_H_
#define _PI_RX_DRIVER_H_

#include <pi/rx_device.h>

/**
 * @brief   RX Phase Interpolator Initialization
 *
 * @details Initializes RX Phase Interpolator device at register interface level.
 *
 * @param[in]   rx_pi       pointer to RX Phase Interpolator device.
 * @param[in]   base        base address of RX Phase Interpolator device.
 * @param[in]   slice_type  slice type of RX Phase Interpolator device.
 * @param[in]   rank        rank of RX Phase Interpolator device.
 *
 * @return      void
 */
void rx_pi_init_reg_if(rx_pi_dev_t *rx_pi,
                       uint32_t base,
                       wddr_slice_type_t slice_type,
                       wddr_rank_t rank);

/**
 * @brief   RX Phase Interpolator Set MSR Register Interface
 *
 * @details Configures RX Phase Interpolator device for given mode set register
 *          value.
 *
 * @param[in]   rx_pi       pointer to RX Phase Interpolator device.
 * @param[in]   msr         MSR value to configure.
 * @param[in]   slice_type  slice type of RX Phase Interpolator device.
 * @param[in]   rank        rank of RX Phase Interpolator device.
 *
 * @return      void
 */
void rx_pi_set_msr_reg_if(rx_pi_dev_t *rx_pi,
                          wddr_msr_t msr,
                          wddr_slice_type_t slice_type,
                          wddr_rank_t rank);

#endif /* _PI_RX_DRIVER_H_ */
