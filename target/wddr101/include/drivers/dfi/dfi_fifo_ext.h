/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DFI_FIFO_EXT_H_
#define _DFI_FIFO_EXT_H_

#include <stdint.h>
#include <stdbool.h>
#include <dfich_reg.h>

/**
 * @brief   DFI FIFO Set Pointer Register Interface
 *
 * @details Sets the START and STOP pointer values for the DFI FIFO. This
 *          feature is meant to be used with looping mode feature of the
 *          DFI IG FIFO. Packets are stored in the DFI FIFO with a timestamp.
 *          These packets are transmitted when a running counter matches that
 *          timestamp. The pointers should store the timestamp values that
 *          mark the beginning and end timestamps of the loop. Thus, packets
 *          between start pointer and stop pointer are sent.
 *
 * @note    Must call dfi_fifo_load_ptr_reg_if for pointers to take effect.
 *
 * @param[in]   dfich_reg   pointer to DFICH register space.
 * @param[in]   start_ptr   timestamp value that marks the start of the loop.
 * @param[in]   stop_ptr    timestamp value that marks the end of the loop.
 *
 * @return      void.
 */
void dfi_fifo_set_ptr_reg_if(dfich_reg_t *dfich_reg,
                             uint8_t start_ptr,
                             uint8_t stop_ptr);

/**
 * @brief   DFI FIFO Load Pointer Register Interface
 *
 * @details Loads the values stored in start_ptr and stop_ptr fields into the
 *          DFI FIFO logic. This function must be called whenever start_ptr
 *          and stop_ptr fields need to be updated.
 *
 * @param[in]   dfich_reg   pointer to DFICH register space.
 *
 * @return      void.
 */
void dfi_fifo_load_ptr_reg_if(dfich_reg_t *dfich_reg);

/**
 * @brief   DFI FIFO Set Loop Mode Register Interface
 *
 * @details Configures the DFI FIFO for loop mode. The loop mode allows the
 *          DFI FIFO to send the same sequence of packets multiple times without
 *          interaction from the MCU. This can be useful when the same sequence
 *          of packets need to be sent repeatedly.
 *
 * @param[in]   dfich_reg   pointer to DFICH register space.
 * @param[in]   enable      flag to indicate if loop mode should be enabled.
 * @param[in]   num_loops   the number of times the FIFO should loop over
 *                          packets.
 *
 * @return      void.
 */
void dfi_fifo_set_loop_mode_reg_if(dfich_reg_t *dfich_reg,
                                   bool enable,
                                   uint8_t num_loops);
#endif /* _DFI_FIFO_EXT_H_ */
