/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DFI_BUFFER_DEV_H_
#define _DFI_BUFFER_DEV_H_

#include <stdint.h>
#include <error.h>
#include <dfi/packet.h>

#define DFI_FIFO_DEPTH          (64)
#define DFI_IG_FIFO_LOAD_NUM    (sizeof(dfi_tx_packet_t) / sizeof(uint32_t))
#define DFI_EG_FIFO_LOAD_NUM    (sizeof(dfi_rx_packet_t) / sizeof(uint32_t))

/**
 * @brief DFI FIFO State
 *
 * @details States of DFI Hadrware FIFO. These values represent bits in the CSR.
 *
 * EMPTY    buffer is empty.
 * FULL     buffer is full.
 */
typedef enum dfi_fifo_state_t
{
    DFI_FIFO_STATE_EMPTY = 1,
    DFI_FIFO_STATE_FULL = 2,
} dfi_fifo_state_t;

/**
 * @brief   DFI Buffer Device Structure
 *
 * @details Device representing send and receive sides of DFI Buffer (FIFO).
 *
 * base             base address of the DFI Buffer Device.
 * tx_packet_buffer internal tx packet buffer that can be used to send Commands
 *                  to the DRAM.
 * tx_packets       storage of DFI TX Packets used with TX packet buffer member.
 * rx_packet_buffer rx packet buffer for storing received DFI packets.
 */
typedef struct dfi_buffer_dev_t
{
    uint32_t                base;
    dfi_tx_packet_buffer_t  tx_packet_buffer;
    packet_item_t           tx_packets[PACKET_BUFFER_DEPTH];
    dfi_rx_packet_buffer_t  rx_packet_buffer;
} dfi_buffer_dev_t;

/**
 * @brief   DFI Buffer Device Initalization
 *
 * @details Initalizes DFI Buffer device at device level.
 *
 * @param[in]   dfi_buffer  pointer to DFI Buffer device.
 * @param[in]   base        base address of the DFI Buffer device.
 *
 * @return      void
 */
void dfi_buffer_init(dfi_buffer_dev_t *dfi_buffer, uint32_t base);

/**
 * @brief   DFI Buffer Enable
 *
 * @details Enables DFI Buffer. Needs to be called before packets can be
 *          loaded into the IG FIFO or read from EG FIFO.
 *
 * @param[in]   dfi_buffer  pointer to DFI Buffer device.
 *
 * @return      void
 */
void dfi_buffer_enable(dfi_buffer_dev_t *dfi_buffer);

/**
 * @brief   DFI Buffer Disable
 *
 * @details Disables DFI Buffer. Needs to be called once done with DFI interface.
 *
 * @param[in]   dfi_buffer  pointer to DFI Buffer device.
 *
 * @return      void
 */
void dfi_buffer_disable(dfi_buffer_dev_t *dfi_buffer);

/**
 * @brief   DFI Buffer Fill Packets
 *
 * @details Fills in IG FIFO with given TX Packet Buffer.
 *
 * @param[in]   dfi_buffer  pointer to DFI Buffer device.
 * @param[in]   tx_buffer   pointer to TX packet buffer with packets to fill.
 *
 * @return      returns whether all packets were written to IG FIFO.
 * @retval      WDDR_SUCCESS if all packets successfully written.
 * @retval      WDDR_ERROR_DFI_PACKET_FIFO_FULL if IG FIFO is full before all
 *              packets have been written.
 */
wddr_return_t dfi_buffer_fill_packets(dfi_buffer_dev_t *dfi_buffer,
                                      dfi_tx_packet_buffer_t *tx_buffer);

/**
 * @brief   DFI Buffer Send Packets
 *
 * @details Sends the packets in the IG FIFO.
 *
 * @param[in]   dfi_buffer  pointer to DFI Buffer device.
 *
 * @return      void
 */
void dfi_buffer_send_packets(dfi_buffer_dev_t *dfi_buffer);

/**
 * @brief   DFI Buffer Fill and Send Packets
 *
 * @details Fills IG FIFO with packets and then immediately sends them.
 *
 * @param[in]   dfi_buffer  pointer to DFI Buffer device.
 * @param[in]   tx_buffer   pointer to TX packet buffer with packets to fill.
 *
 * @return      returns whether all packets were written to IG FIFO.
 * @retval      WDDR_SUCCESS if all packets successfully written.
 * @retval      WDDR_ERROR_DFI_PACKET_FIFO_FULL if IG FIFO is full before all
 *              packets have been written.
 */
wddr_return_t dfi_buffer_fill_and_send_packets(dfi_buffer_dev_t *dfi_buffer,
                                               dfi_tx_packet_buffer_t *tx_buffer);

/**
 * @brief   DFI Buffer Read Packets
 *
 * @details Read given number of packets from the EG FIFO.
 *
 * @param[in]       dfi_buffer      pointer to the DFI Buffer device.
 * @param[inout]    num_packets     number of packets to read / were read.
 *
 * @return          returns if requested number of packets were read.
 * @retval          WDDR_SUCCESS if number of packets requested were read.
 * @retval          WDDR_ERROR_DFI_PACKET_FIFO_EMPTY if EG FIFO is empty before
 *                  number of requested packets have been read.
 */
wddr_return_t dfi_buffer_read_packets(dfi_buffer_dev_t *dfi_buffer,
                                      uint8_t num_packets);

#endif /* _DFI_BUFFER_DEV_H_ */
