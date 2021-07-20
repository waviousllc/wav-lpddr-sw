/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <dfi/buffer_device.h>
#include <dfi/packet.h>
#include <dfi/buffer_driver.h>

/**
 * @brief   DFI Buffer Write Packets
 *
 * @details Writes all pakets in given TX Packet Buffer to the IG FIFO.
 *
 * @param[in]   dfi_buffer      pointer to DFI Buffer device.
 * @param[in]   packet_list     pointer to list of packets to write.
 *
 * @return      returns whether all packets were written to IG FIFO.
 * @retval      WDDR_SUCCESS if all packets successfully written.
 * @retval      WDDR_ERROR_DFI_PACKET_FIFO_FULL if IG FIFO is full before all
 *              packets have been written.
 */
static wddr_return_t dfi_buffer_write_packets(dfi_buffer_dev_t *dfi_buffer,
                                              List_t *packet_list);

void dfi_buffer_init(dfi_buffer_dev_t *dfi_buffer, uint32_t base)
{
    dfi_buffer->base = base;
    dfi_buffer_init_reg_if(dfi_buffer, base);
    // By default, enable hold feature
    dfi_buffer_set_wdata_hold_reg_if(dfi_buffer, true);
}

void dfi_buffer_enable(dfi_buffer_dev_t *dfi_buffer)
{
    dfi_buffer_enable_clock_reg_if(dfi_buffer);
}

void dfi_buffer_disable(dfi_buffer_dev_t *dfi_buffer)
{
    dfi_buffer_set_mode_reg_if(dfi_buffer, false);
}

wddr_return_t dfi_buffer_fill_packets(dfi_buffer_dev_t *dfi_buffer,
                                      List_t *packet_list)
{
    dfi_buffer_enable(dfi_buffer);
    PROPAGATE_ERROR(dfi_buffer_write_packets(dfi_buffer, packet_list));
    return WDDR_SUCCESS;
}

void dfi_buffer_send_packets(dfi_buffer_dev_t *dfi_buffer, bool should_block)
{
    // Call blocking version of API
    if (should_block)
    {
        return dfi_buffer_send_packets_reg_if(dfi_buffer);
    }

    return dfi_buffer_send_packets_non_blocking_reg_if(dfi_buffer);
}

wddr_return_t dfi_buffer_fill_and_send_packets(dfi_buffer_dev_t *dfi_buffer,
                                               List_t *packet_list)
{
    PROPAGATE_ERROR(dfi_buffer_fill_packets(dfi_buffer, packet_list));
    dfi_buffer_send_packets(dfi_buffer, true);
    return WDDR_SUCCESS;
}


static wddr_return_t dfi_buffer_write_packets(dfi_buffer_dev_t *dfi_buffer,
                                              List_t *packet_list)
{
    wddr_return_t ret = WDDR_SUCCESS;
    packet_item_t *packet_item;
    ListItem_t *next = listGET_HEAD_ENTRY(packet_list);

    /**
     * Send all packets except the last packet.
     * The last packet is empty except for timestamp.
     */
    do
    {
        packet_item = (packet_item_t *) listGET_LIST_ITEM_OWNER(next);
        next = listGET_NEXT(next);
        ret = dfi_buffer_write_ig_fifo_reg_if(dfi_buffer, packet_item->packet.raw_data);
    } while (next != listGET_END_MARKER(packet_list) && ret == WDDR_SUCCESS);
    return ret;
}


wddr_return_t dfi_buffer_read_packets(dfi_buffer_dev_t *dfi_buffer,
                                      dfi_rx_packet_buffer_t *rx_buffer,
                                      uint8_t num_packets)
{
    wddr_return_t ret;
    uint8_t nn = 0;

    if (num_packets > DFI_FIFO_DEPTH)
    {
        return WDDR_ERROR;
    }

    do
    {

        ret = dfi_buffer_read_eg_fifo_reg_if(dfi_buffer, rx_buffer->buffer[nn].raw_data);
    } while(++nn < num_packets && ret == WDDR_SUCCESS);
    return ret;
}
