/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _RECEIVER_DELAY_DRIVER_H_
#define _RECEIVER_DELAY_DRIVER_H_

#include <stdint.h>
#include <receiver/device.h>
#include <wddr/phy_defs.h>

/**
 * @brief   Receiver Delay Initialize Register Interface
 *
 * @details Initializes Receiver Delay device at the register interface level.
 *
 * @param[in]   rx_delay    pointer to Receiver Delay device.
 * @param[in]   base        base address of Receiver Delay device.
 * @param[in]   rank        rank of Receiver Delay device.
 *
 * @return      void
 */
void receiver_delay_init_reg_if(receiver_delay_dev_t *rx_delay,
                                uint32_t base,
                                wddr_rank_t rank);

/**
 * @brief   Receiver Delay Set Mode Set Register (MSR) Register Interface
 *
 * @details Configures Receiver Delay device for given mode set register value.
 *
 * @param[in]   rx_delay    pointer to the Receiver Delay device.
 * @param[in]   msr         MSR value to configure.
 * @param[in]   rank        rank of the Receiver device.
 *
 * @return      void
 */
void receiver_delay_set_msr_reg_if(receiver_delay_dev_t *rx_delay,
                                   wddr_msr_t msr,
                                   wddr_rank_t rank);
/**
 * @brief   Receiver Set Delay
 *
 * @details Sets the Receiver Delay device delay via CSR.
 *
 * @param[in]   rx_delay    pointer to the Receiver Delay device.
 * @param[in]   delay       delay (code) value to set.
 * @param[in]   side        mask indicating which side should be configured.
 *                          (T, C or both)
 *
 * @return      void
 */
void receiver_delay_set_delay_code_reg_if(receiver_delay_dev_t *rx_delay,
                                          uint16_t delay,
                                          receiver_tc_side_mask_t side);

#endif /* _RECEIVER_DELAY_DRIVER_H_ */
