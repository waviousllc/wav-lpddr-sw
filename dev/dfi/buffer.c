/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <dfi/device.h>
#include <dfi/packet.h>

/**
 * @brief   DFI Buffer Write Packets
 *
 * @details Writes all packets in given TX Packet Buffer to the IG FIFO.
 *
 * @param[in]   dfi             pointer to DFI device.
 * @param[in]   packet_list     pointer to list of packets to write.
 *
 * @return      returns whether all packets were written to IG FIFO.
 * @retval      DFI_SUCCESS if all packets successfully written.
 * @retval      DFI_ERROR_FIFO_FULL if IG FIFO is full before all
 *              packets have been written.
 */
static dfi_return_t dfi_buffer_write_packets(dfi_dev_t *dfi,
                                             const List_t *packet_list);

void dfi_buffer_enable(dfi_dev_t *dfi)
{
    dfi_fifo_enable_clock_reg_if(dfi->dfich_reg);
}

void dfi_buffer_disable(dfi_dev_t *dfi)
{
    dfi_fifo_set_mode_reg_if(dfi->dfich_reg, false);
}

dfi_return_t dfi_buffer_fill_packets(dfi_dev_t *dfi,
                                     const List_t *packet_list)
{
    dfi_buffer_enable(dfi);
    return dfi_buffer_write_packets(dfi, packet_list);
}

void dfi_buffer_send_packets(dfi_dev_t *dfi, bool should_block)
{
    dfi_fifo_send_packets_reg_if(dfi->dfich_reg);
}

dfi_return_t dfi_buffer_fill_and_send_packets(dfi_dev_t *dfi,
                                              const List_t *packet_list)
{
    dfi_return_t ret;

    ret = dfi_buffer_fill_packets(dfi, packet_list);

    if (ret != DFI_SUCCESS)
    {
        return ret;
    }

    dfi_buffer_send_packets(dfi, true);
    return DFI_SUCCESS;
}

static dfi_return_t dfi_buffer_write_packets(dfi_dev_t *dfi,
                                             const List_t *packet_list)
{
    dfi_return_t ret = DFI_SUCCESS;
    const packet_item_t *packet_item;
    const ListItem_t *next;

    // Should have at least one packet
    if (listLIST_IS_EMPTY(packet_list))
    {
        return DFI_ERROR;
    }

    next = listGET_HEAD_ENTRY(packet_list);

    /**
     * Send all packets except the last packet.
     * The last packet is empty except for timestamp.
     */
    do
    {
        packet_item = (packet_item_t *) listGET_LIST_ITEM_OWNER(next);
        next = listGET_NEXT(next);
        ret = dfi_fifo_write_ig_reg_if(dfi->dfich_reg, packet_item->packet.raw_data);
    } while (next != listGET_END_MARKER(packet_list) && ret == DFI_SUCCESS);
    return ret;
}

dfi_return_t dfi_buffer_read_packets(dfi_dev_t *dfi,
                                      dfi_rx_packet_buffer_t *rx_buffer,
                                      uint8_t num_packets)
{
    dfi_return_t ret;
    uint8_t nn = 0;

    if (num_packets > DFI_FIFO_DEPTH)
    {
        return DFI_ERROR;
    }

    do
    {

        ret = dfi_fifo_read_eg_reg_if(dfi->dfich_reg, rx_buffer->buffer[nn].raw_data);
    } while(++nn < num_packets && ret == DFI_SUCCESS);
    return ret;
}
