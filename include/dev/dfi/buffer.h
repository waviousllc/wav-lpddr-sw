/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DFI_BUFFER_DEV_H_
#define _DFI_BUFFER_DEV_H_

#include <stdint.h>
#include <stdbool.h>
#include <dfi/device.h>
#include <dfi/packet.h>

#define DFI_FIFO_DEPTH          (64)
#define DFI_IG_FIFO_LOAD_NUM    (TX_PACKET_SIZE_WORDS)
#define DFI_EG_FIFO_LOAD_NUM    (RX_PACKET_SIZE_WORDS)
/**
 * @brief   DFI Buffer Enable
 *
 * @details Enables DFI Buffer. Needs to be called before packets can be
 *          loaded into the IG FIFO or read from EG FIFO.
 *
 * @param[in]   dfi  pointer to DFI device.
 *
 * @return      void
 */
void dfi_buffer_enable(dfi_dev_t *dfi);

/**
 * @brief   DFI Buffer Disable
 *
 * @details Disables DFI Buffer. Needs to be called once done with DFI interface.
 *
 * @param[in]   dfi  pointer to DFI device.
 *
 * @return      void
 */
void dfi_buffer_disable(dfi_dev_t *dfi);

/**
 * @brief   DFI Buffer Fill Packets
 *
 * @details Fills in IG FIFO with given TX Packet Buffer.
 *
 * @param[in]   dfi         pointer to DFI device.
 * @param[in]   packet_list pointer to list of packets to write.
 *
 * @return      returns whether all packets were written to IG FIFO.
 * @retval      DFI_SUCCESS if all packets successfully written.
 * @retval      DFI_ERROR_FIFO_FULL if IG FIFO is full before all
 *              packets have been written.
 */
dfi_return_t dfi_buffer_fill_packets(dfi_dev_t *dfi,
                                     const List_t *packet_list);

/**
 * @brief   DFI Buffer Send Packets
 *
 * @details Sends the packets in the IG FIFO.
 *
 * @param[in]   dfi pointer to DFI Buffer device.
 *
 * @return      void
 */
void dfi_buffer_send_packets(dfi_dev_t *dfi);

/**
 * @brief   DFI Buffer Fill and Send Packets
 *
 * @details Fills IG FIFO with packets and then immediately sends them.
 *
 * @param[in]   dfi         pointer to DFI device.
 * @param[in]   packet_list pointer to list of packets to write.
 *
 * @return      returns whether all packets were written to IG FIFO.
 * @retval      DFI_SUCCESS if all packets successfully written.
 * @retval      DFI_ERROR_FIFO_FULL if IG FIFO is full before all
 *              packets have been written.
 */
dfi_return_t dfi_buffer_fill_and_send_packets(dfi_dev_t *dfi,
                                              const List_t *packet_list);

/**
 * @brief   DFI Buffer Read Packets
 *
 * @details Read given number of packets from the EG FIFO.
 *
 * @param[in]       dfi             pointer to DFI device.
 * @param[out]      rx_buffer       pointer to the RX Packet buffer to read into.
 * @param[inout]    num_packets     number of packets to read / were read.
 *
 * @return          returns if requested number of packets were read.
 * @retval          DFI_SUCCESS if number of packets requested were read.
 * @retval          DFI_ERROR_FIFO_EMPTY if EG FIFO is empty before
 *                  number of requested packets have been read.
 */
dfi_return_t dfi_buffer_read_packets(dfi_dev_t *dfi,
                                      dfi_rx_packet_buffer_t *rx_buffer,
                                      uint8_t num_packets);

#endif /* _DFI_BUFFER_DEV_H_ */
