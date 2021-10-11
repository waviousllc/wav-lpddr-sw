/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _WDDR_DRIVER_H_
#define _WDDR_DRIVER_H_

#include <stdbool.h>
#include <wddr/device.h>

/**
 * @brief   Wavious DDR (WDDR) Clear FIFO Register Interface
 *
 * @details Clears FIFOs in all channels.
 *
 * @param[in]   wddr        pointer to WDDR device.
 * @param[in]   channel     channel to set.
 *
 * @return      void
 */
void wddr_clear_fifo_reg_if(wddr_dev_t *wddr, wddr_channel_t channel);

/**
 * @brief   Wavious DDR (WDDR) Set RESETN Pin Register Interface
 *
 * @details Sets the state of the RESETN pin to the DRAM. Driving RESETN high
 *          releases the RESET of the DRAM chip.
 *
 * @param[in]   wddr        pointer to WDDR device.
 * @param[in]   override    flag to indicate if SW should override RESETN pin
 *                          state.
 * @param[in]   high        flag to indicate if RESETN pin should be driven
 *                          high.
 *
 * @return      void
 */
void wddr_set_dram_resetn_pin_reg_if(wddr_dev_t *wddr,
                                     bool override,
                                     bool high);

/**
 * @brief   Wavious DDR (WDDR) Set Chip Select Register Interface
 *
 * @details Sets the state of Write and Read Chip Select for all channels.
 *
 * @param[in]   wddr        pointer to WDDR device.
 * @param[in]   channel     channel to set.
 * @param[in]   override    flag to indicate if SW should override CS state.
 * @param[in]   rank        rank / chipselect value to set.
 *
 * @return      void
 */
void wddr_set_chip_select_reg_if(wddr_dev_t *wddr,
                                 wddr_channel_t channel,
                                 wddr_rank_t rank,
                                 bool override);

/**
 * @brief   Wavious DDR (WDDR) Read Boundary Scan Result Register Interface
 *
 * @details Reads the boundary scan value for specified dq_byte and channel.
 *
 * @param[in]   wddr        pointer to WDDR device.
 * @param[in]   dq_byte     dq_byte to read bscan for.
 * @param[in]   channel     channel to read bscan for.
 * @param[out]  result      pointer to store bscan result in.
 *
 * @return      void
 */
void wddr_read_bscan_result_reg_if(wddr_dev_t *wddr,
                                   wddr_dq_byte_t dq_byte,
                                   wddr_channel_t channel,
                                   uint8_t *result);

#endif /* _WDDR_DRIVER_H_ */
