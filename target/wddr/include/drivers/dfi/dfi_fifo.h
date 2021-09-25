/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DFI_FIFO_H_
#define _DFI_FIFO_H_

#include <stdbool.h>
#include <dfich_reg.h>
#include "dfi_ret.h"
#include <dfi/packet.h>

#define DFI_FIFO_DEPTH          (64)
#define DFI_IG_FIFO_LOAD_NUM    (TX_PACKET_SIZE_WORDS)
#define DFI_EG_FIFO_LOAD_NUM    (RX_PACKET_SIZE_WORDS)

/**
 * @brief DFI FIFO State
 *
 * @details States of DFI Hadrware FIFO. These values represent bits in the CSR.
 *
 * EMPTY    buffer is empty.
 * FULL     buffer is full.
 */
enum
{
    DFI_FIFO_STATE_EMPTY = 1,
    DFI_FIFO_STATE_FULL = 2,
};

/**
 * @brief   DFI FIFO Enable CA Read Data Loopback Register Interface
 *
 * @details Enable CA Read Data Loopback feature of DFI FIFO. This allows
 *          software to read CA data that is usually not received on the return
 *          path.
 *
 * @param[in]   dfich_reg   pointer to DFICH register space.
 * @param[in]   enable      flag to indicate if CA RDATA loopback should be
 *                          enabled.
 */
void dfi_fifo_enable_ca_rdata_loopback_reg_if(dfich_reg_t *dfi_ch_reg,
                                              bool enable);


/**
 * @brief   DFI FIFO Read Data Out Enable Override Register Interface
 *
 * @details Sets the read data out enable override for the DFI interface.
 *
 * @param[in]   dfich_reg   pointer to DFICH register space.
 * @param[in]   ovr_sel     flag to indicate if SW should override rdout.
 * @param[in]   ovr         flag to indicate if override output is HIGH.
 *
 * @return      void
 */
void dfi_fifo_rdout_en_ovr_reg_if(dfich_reg_t *dfich_reg,
                                  bool ovr_sel,
                                  bool ovr);

/**
 * @brief   DFI FIFO Enable Clock Register Interface
 *
 * @details Enables DFI FIFO device clock via CSR.
 *          This allows user to write and read from the DFI FIFOs and
 *          does not interfere with the DFI signals. dfi_buffer_set_mode_reg_if
 *          should be used to put the DFI buffer device into FIFO Mode. This
 *          mode will send DFI packets which will control DFI signals.
 *
 * @note    This resets the HW FIFO state.
 *
 * @param[in]   dfich_reg   pointer to DFICH register space.
 *
 * @return  void.
 */
void dfi_fifo_enable_clock_reg_if(dfich_reg_t *dfich_reg);

/**
 * @brief   DFI FIFO Set Mode Register Interface
 *
 * @details Sets the DFI FIFO Mode via CSR. This will enable
 *          or disable both the buffer mode and the timestamp comparison
 *          logic.
 *
 * @param[in]   dfich_reg   pointer to DFICH register space.
 * @param[in]   enable      flag to indicate if Buffer Mode should be enabled.
 *
 * @return  void.
 */
void dfi_fifo_set_mode_reg_if(dfich_reg_t *dfich_reg, bool enable);

/**
 * @brief   DFI FIFO Set Write Data Hold Register Interface
 *
 * @details Sets whether DFI FIFO holds DFI signals in state based on last
 *          packet that was sent via CSR. This feature is typically used when
 *          the SW is driving DFI signals. This feature can be used to ensure
 *          that signals remain active while SW prepares the next set
 *          of packets to control DFI signals.
 *
 * @param[in]   dfich_reg   pointer to DFICH register space.
 * @param[in]   enable      flag to indicate if WHOLD feature should be enabled.
 *
 * @return  void.
 */
void dfi_fifo_set_wdata_hold_reg_if(dfich_reg_t *dfich_reg, bool enable);

/**
 * @brief   DFI FIFO Send Packets Register Interface
 *
 * @details Enables logic to send packets from IG FIFO. This function blocks
 *          (via polling) until the IG FIFO is empty. If IG FIFO is arleady
 *          empty, then function returns immediately. This function turns on
 *          and leaves Buffer Mode enabled.
 *
 * @param[in]   dfich_reg   pointer to DFICH register space.
 *
 * @return      void
 */
void dfi_fifo_send_packets_reg_if(dfich_reg_t *dfich_reg);

/**
 * @brief   DFI FIFO Write IG FIFO Register Interface
 *
 * @details Writes the provided data buffer into the IG FIFO via CSR. This data
 *          should represent a single DFI TX Packet.
 *
 * @param[in]   dfich_reg   pointer to DFICH register space.
 * @param[in]   data        pointer to data to be writen into IG FIFO.
 *
 * @return      returns whether data was written to IG FIFO.
 * @retval      DFI_SUCCESS if data was written.
 * @retval      DFI_ERROR_FIFO_FULL if no data written.
 * @retval      DFI_ERROR otherwise.
 */
dfi_return_t dfi_fifo_write_ig_reg_if(dfich_reg_t *dfich_reg,
                                      const uint32_t data[DFI_IG_FIFO_LOAD_NUM]);

/**
 * @brief   DFI FIFO Read EG FIFO Register Interface
 *
 * @details Reads a single DFI RX packet from the EG FIFO via CSR. Data is stored
 *          in the provided data buffer.
 *
 * @param[in]   dfich_reg   pointer to DFICH register space.
 * @param[out]  data        pointer to store data read from EG FIFO.
 *
 * @return      returns whether data was read from EG FIFO.
 * @retval      DFI_SUCCESS if data was read.
 * @retval      DFI_ERROR_FIFO_EMPTY if no data to read.
 * @retval      DFI_ERROR otherwise.
 */
dfi_return_t dfi_fifo_read_eg_reg_if(dfich_reg_t *dfich_reg,
                                     uint32_t data[DFI_EG_FIFO_LOAD_NUM]);

#endif /* _DFI_FIFO_H_ */
