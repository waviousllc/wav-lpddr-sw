/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _DFI_BUFFER_DRIVER_H_
#define _DFI_BUFFER_DRIVER_H_

#include <stdint.h>
#include <stdbool.h>
#include <error.h>
#include <dfi/buffer_device.h>

/**
 * @brief   DFI Buffer Initialization Register Interface
 *
 * @details Initializes DFI Buffer device at register interface level.
 *
 * @param[in]   dfi_buffer  pointer to DFI Buffer device.
 * @param[in]   base        base address of the DFI Buffer device.
 *
 * @return      void
 */
void dfi_buffer_init_reg_if(dfi_buffer_dev_t *buffer, uint32_t base);

/**
 * @brief   DFI Buffer Enable Clock Register Interface
 *
 * @details Enables DFI Buffer device clock via CSR.
 *          This allows user to write and read from the DFI FIFOs and
 *          does not interfere with the DFI signals. dfi_buffer_set_mode_reg_if
 *          should be used to put the DFI buffer device into Buffer Mode. This
 *          mode will send DFI packets which will control DFI signals.
 *
 * @note    This resets the HW Buffer state.
 *
 * @param[in]   dfi_buffer  pointer to DFI Buffer device.
 *
 * @return  void.
 */
void dfi_buffer_enable_clock_reg_if(dfi_buffer_dev_t *dfi_buffer);

/**
 * @brief   DFI Buffer Set Mode Register Interface
 *
 * @details Sets the DFI Buffer device Buffer Mode via CSR. This will enable
 *          or disables both the buffer mode and the timestamp comparison
 *          logic.
 *
 * @param[in]   dfi_buffer  pointer to DFI Buffer device.
 * @param[in]   enable      flag to indicate if Buffer Mode should be enabled.
 *
 * @return  void.
 */
void dfi_buffer_set_mode_reg_if(dfi_buffer_dev_t *dfi_buffer, bool enable);

/**
 * @brief   DFI Buffer Set Write Data Hold Register Interface
 *
 * @details Sets whether DFI Buffer holds DFI signals in state based on last
 *          packet that was sent via CSR. This feature is typically used when
 *          the SW is driving DFI signals. This feature can be used to ensure
 *          that signals remain active while SW prepares the next set
 *          of packets to control DFI signals.
 *
 * @param[in]   dfi_buffer  pointer to DFI Buffer device.
 * @param[in]   enable      flag to indicate if WHOLD feature should be enabled.
 *
 * @return  void.
 */
void dfi_buffer_set_wdata_hold_reg_if(dfi_buffer_dev_t *dfi_buffer, bool enable);

/**
 * @brief   DFI Buffer Send Packets Register Interface
 *
 * @details Enables logic to send packets from IG FIFO. This function blocks
 *          until the IG FIFO is empty. If IG FIFO is arleady empty, then
 *          function returns immediately. This function turns on and leaves
 *          Buffer Mode enabled after returning.
 *
 * @param[in]   dfi_buffer  pointer to DFI Buffer device.
 *
 * @return      void
 */
void dfi_buffer_send_packets_reg_if(dfi_buffer_dev_t *dfi_buffer);

/**
 * @brief   DFI Buffer Write IG FIFO Register Interface
 *
 * @details Writes the provided data buffer into the IG FIFO via CSR. This data
 *          should represent a single DFI TX Packet.
 *
 * @param[in]   dfi_buffer  pointer to DFI Buffer device.
 * @param[in]   data        pointer to data to be writen into IG FIFO.
 *
 * @return      returns whether data was written to IG FIFO.
 * @retval      WDDR_SUCCESS if data was written.
 * @retval      WDDR_ERROR_DFI_PACKET_FIFO_FULL if no data written.
 * @retval      WDDR_ERROR otherwise.
 */
wddr_return_t dfi_buffer_write_ig_fifo_reg_if(dfi_buffer_dev_t *dfi_buffer,
                                              uint32_t data[DFI_IG_FIFO_LOAD_NUM]);

/**
 * @brief   DFI Buffer Read EG FIFO Register Interface
 *
 * @details Reads a single DFI RX packet from the EG FIFO via CSR. Data is stored
 *          in the provided data buffer.
 *
 * @param[in]   dfi_buffer  pointer to DFI Buffer device.
 * @param[out]  data        pointer to store data read from EG FIFO.
 *
 * @return      returns whether data was read from EG FIFO.
 * @retval      WDDR_SUCCESS if data was read.
 * @retval      WDDR_ERROR_DFI_PACKET_FIFO_EMPTY if no data to read.
 * @retval      WDDR_ERROR otherwise.
 */
wddr_return_t dfi_buffer_read_eg_fifo_reg_if(dfi_buffer_dev_t *dfi_buffer,
                                             uint32_t data[DFI_EG_FIFO_LOAD_NUM]);

#endif /* _DFI_BUFFER_DRIVER_H_ */
