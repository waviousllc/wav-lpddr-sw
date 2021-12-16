/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* Standard includes. */
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* Kernel includes. */
#include <kernel/io.h>

/* LPDDR includes. */
#include <wddr/device.h>
#include <wddr/memory_map.h>
#include <dfi/buffer.h>
#include <dfi/driver.h>

#define MISR_STEP_SIZE  (2)

wddr_return_t wddr_load_packets(wddr_handle_t wddr, const List_t *packets)
{
    dfi_return_t ret;
    ListItem_t const *end;
    const packet_item_t *last;
    dfi_tx_packet_t packet;

    // Reset start pointers
    dfi_fifo_set_ptr_reg_if(wddr->dfi.dfich_reg, 0x0, 0x0);
    dfi_fifo_load_ptr_reg_if(wddr->dfi.dfich_reg);

    // Load packets
    ret = dfi_buffer_fill_packets(&wddr->dfi, packets);
    switch(ret)
    {
        case DFI_ERROR_FIFO_FULL:
            return WDDR_ERROR_FIFO_FULL;
        case DFI_ERROR:
            return WDDR_ERROR;
        default:
            break;
    }

    /**
     * @note    Hardware requires that an additional packet sits in the FIFO,
     *          such that its timestamp is given by stop_ptr + 1. This packet
     *          isn't transmitted but must be loaded for FIFO to loop properly.
     *          This function will insert it.
     */
    end = listGET_END_MARKER(packets);
    last = (packet_item_t *) listGET_LIST_ITEM_OWNER(end->pxPrevious);
    packet.packet.time = last->packet.packet.time + 1;

    dfi_fifo_write_ig_reg_if(wddr->dfi.dfich_reg, packet.raw_data);

    // Update the stop pointer based on number of packets loaded
    dfi_fifo_set_ptr_reg_if(wddr->dfi.dfich_reg, 0x0, listCURRENT_LIST_LENGTH(packets) - 1);

    return WDDR_SUCCESS;
}

wddr_return_t wddr_send_packets(wddr_handle_t wddr, const List_t *packets)
{
    // Setup MISR
    misr_clear_lfsr_reg_if(wddr->dfi.dfich_reg);
    misr_set_enable_reg_if(wddr->dfi.dfich_reg, MISR_EN_ALL, true);

    // Setup loop mode
    dfi_fifo_set_loop_mode_reg_if(wddr->dfi.dfich_reg, true, 0x1);
    dfi_fifo_load_ptr_reg_if(wddr->dfi.dfich_reg);

    // Send packets
    dfi_buffer_send_packets(&wddr->dfi);

    // Turn off loop mode
    dfi_fifo_set_loop_mode_reg_if(wddr->dfi.dfich_reg, false, 0x0);

    return WDDR_SUCCESS;
}

bool wddr_validate_recv_data(wddr_handle_t wddr,
                             __UNUSED__ const command_data_t *data,
                             const command_data_sig_t *sig,
                             burst_length_t bl,
                             uint8_t dq_byte_mask)
{
    uint32_t misr_sig[SIG_WORDS];
    const uint32_t *signature;
    uint8_t misr_index = MISR_DQ0_W3_TO_W0;
    wddr_freq_ratio_t ratio = wddr->dram.cfg->ratio;

    // Compare signature for each byte
    for (uint8_t byte = WDDR_DQ_BYTE_0; byte < WDDR_DQ_BYTE_TOTAL; byte++, misr_index += MISR_STEP_SIZE)
    {
        uint8_t byte_mask = 1 << byte;

        signature = sig->signature[ratio][byte];

        // Don't process if byte_mask isn't set
        if (!(dq_byte_mask & byte_mask))
        {
            continue;
        }

        // Read Signature
        misr_read_reg_if(wddr->dfi.dfich_reg, misr_index, &misr_sig[0], &misr_sig[1]);

        // High and Low must match
        if (misr_sig[0] != signature[0] ||
            misr_sig[1] != signature[1])
        {
            return false;
        }
    }

    // Turn off MISR
    misr_set_enable_reg_if(wddr->dfi.dfich_reg, MISR_EN_ALL, false);

    return true;
}
