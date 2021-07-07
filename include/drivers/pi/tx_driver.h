/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#ifndef _PI_TX_DRIVER_H_
#define _PI_TX_DRIVER_H_

#include <pi/tx_device.h>

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
void tx_pi_init_reg_if(tx_pi_dev_t *tx_pi,
                       uint32_t base,
                       wddr_slice_type_t slice_type,
                       wddr_rank_t rank);

/**
 * @brief   TX Phase Interpolator Set MSR Register Interface
 *
 * @details Configures TX Phase Interpolator device for given mode set register
 *          value.
 *
 * @param[in]   tx_pi       pointer to TX Phase Interpolator device.
 * @param[in]   msr         MSR value to configure.
 * @param[in]   slice_type  slice type of TX Phase Interpolator device.
 * @param[in]   rank        rank of TX Phase Interpolator device.
 *
 * @return      void
 */
void tx_pi_set_msr_reg_if(tx_pi_dev_t *tx_pi,
                          wddr_msr_t msr,
                          wddr_slice_type_t slice_type,
                          wddr_rank_t rank);


#endif /* _PI_TX_DRIVER_H_ */
