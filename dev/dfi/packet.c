/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#include <stdbool.h>
#include <string.h>
#include <dfi/packet.h>

#define DFI_PACK_CKE_VAL        ((WDDR_PHY_RANK << 1) - 1)

/** @brief  Internal Function for creating a new packet_item_t instance */
static void create_packet(dfi_tx_packet_buffer_t *buffer, packet_item_t **packet);

/** @brief  Internal Function for creating sequence of packets to send command to DRAM */
static wddr_return_t create_address_packet_sequence(dfi_tx_packet_buffer_t *buffer,
                                                    wddr_freq_ratio_t ratio,
                                                    command_t *command,
                                                    uint16_t time_offset);

/** @brief  Internal Function for extracting data from a packet and storing in a data buffer */
static void extract_packet_data(dfi_rx_packet_desc_t *packet,
                                uint8_t data_packet[PACKET_MAX_NUM_PHASES],
                                wddr_dq_byte_t dq_byte,
                                uint8_t phases);

/** @brief  Internal Function to compare received data versus expected data */
static bool compare_received_data(uint8_t received[PACKET_MAX_NUM_PHASES],
                                  uint8_t *expected,
                                  uint8_t phases,
                                  packet_data_mask_t data_mask);

void dfi_tx_packet_buffer_init(dfi_tx_packet_buffer_t *buffer)
{
    vListInitialise(&buffer->list);
    buffer->ts_last_packet = 1;
    buffer->storage = NULL;
}

void dfi_tx_packet_buffer_free(dfi_tx_packet_buffer_t *buffer)
{
    ListItem_t *pxCurr, *pxNext;
    packet_item_t *packet;
    listFOR_EACH_LIST_ITEM_SAFE(pxCurr, pxNext, &buffer->list)
    {
        uxListRemove(pxCurr);
        if (buffer->storage == NULL)
        {
            packet = (packet_item_t *) listGET_LIST_ITEM_OWNER(pxCurr);
            vPortFree(packet);
        }
        else
        {
            buffer->storage->index--;
        }
    }
    buffer->ts_last_packet = 1;
}

void dfi_rx_packet_buffer_init(dfi_rx_packet_buffer_t *buffer)
{
    memset(buffer, 0, sizeof(dfi_rx_packet_buffer_t));
}



wddr_return_t create_ck_packet_sequence(dfi_tx_packet_buffer_t *buffer,
                                        uint16_t time_offset)
{
    packet_item_t *packet;
    create_packet(buffer, &packet);
    if (packet == NULL)
    {
        return WDDR_ERROR;
    }

    time_offset += buffer->ts_last_packet;
    packet->packet.packet.dce_p0 = 1;
    packet->packet.packet.dce_p1 = 1;
    packet->packet.packet.dce_p2 = 1;
    packet->packet.packet.dce_p3 = 1;
    packet->packet.packet.time = time_offset;
    buffer->ts_last_packet = time_offset - 1;

    // Add to list
    vListInitialiseItem(&packet->list_item);
    packet->packet.packet.time = time_offset;
    listSET_LIST_ITEM_VALUE(&packet->list_item, time_offset);
    listSET_LIST_ITEM_OWNER(&packet->list_item, packet);
    vListInsertEnd(&buffer->list, &packet->list_item);

    return WDDR_SUCCESS;
}

wddr_return_t create_cke_packet_sequence(dfi_tx_packet_buffer_t *buffer,
                                         uint16_t time_offset)
{
    packet_item_t *packet;
    create_packet(buffer, &packet);
    if (packet == NULL)
    {
        return WDDR_ERROR;
    }

    time_offset += buffer->ts_last_packet;
    packet->packet.packet.dce_p0 = 1;
    packet->packet.packet.dce_p1 = 1;
    packet->packet.packet.dce_p2 = 1;
    packet->packet.packet.dce_p3 = 1;
    packet->packet.packet.cke_p0 = DFI_PACK_CKE_VAL;
    packet->packet.packet.cke_p1 = DFI_PACK_CKE_VAL;
    packet->packet.packet.cke_p2 = DFI_PACK_CKE_VAL;
    packet->packet.packet.cke_p3 = DFI_PACK_CKE_VAL;
    packet->packet.packet.time = time_offset;
    buffer->ts_last_packet = time_offset - 1;

    // Add to list
    vListInitialiseItem(&packet->list_item);
    packet->packet.packet.time = time_offset;
    listSET_LIST_ITEM_VALUE(&packet->list_item, time_offset);
    listSET_LIST_ITEM_OWNER(&packet->list_item, packet);
    vListInsertEnd(&buffer->list, &packet->list_item);

    return WDDR_SUCCESS;
}

wddr_return_t create_mrw_packet_sequence(dfi_tx_packet_buffer_t *buffer,
                                        wddr_freq_ratio_t ratio,
                                        chipselect_t cs,
                                        uint8_t mode_register,
                                        uint8_t op,
                                        uint16_t time_offset)
{
    command_t command = {0};
    create_write_mode_register_command(&command, cs, mode_register, op);
    return create_address_packet_sequence(buffer, ratio, &command, time_offset);
}

void dfi_rx_packet_buffer_data_compare(dfi_rx_packet_buffer_t *buffer,
                                       command_data_t *expected,
                                       wddr_dq_byte_t dq_byte,
                                       packet_data_mask_t data_mask,
                                       uint8_t num,
                                       uint8_t phases,
                                       uint8_t *is_same)
{
    dfi_rx_packet_desc_t *packet;
    bool same = false;
    uint8_t data_packet[PACKET_MAX_NUM_PHASES];

    for (uint8_t ii = 0; ii < num; ii++)
    {
        packet = &buffer->buffer[ii].packet;
        extract_packet_data(packet, data_packet, dq_byte, phases);
        same = compare_received_data(data_packet, &expected->dq[dq_byte][ii * phases], phases, data_mask);
        if (!same)
        {
            break;
        }
    }
    *is_same = same;
}

static void create_packet(dfi_tx_packet_buffer_t *buffer, packet_item_t **packet)
{
    packet_item_t *new_packet = NULL;

    if (buffer->storage != NULL && buffer->storage->packets != NULL)
    {
        if (buffer->storage->index < buffer->storage->len)
        {

            new_packet = &buffer->storage->packets[buffer->storage->index++];
        }
    }
    else
    {
        new_packet = pvPortMalloc(sizeof(packet_item_t));
        // Intialize packet
        for (uint8_t i = 0; i < TX_PACKET_SIZE_WORDS; i--)
        {
            new_packet->packet.raw_data[i] = 0;
        }
    }

    *packet = new_packet;
}

static wddr_return_t create_address_packet_sequence(dfi_tx_packet_buffer_t *buffer,
                                                    wddr_freq_ratio_t ratio,
                                                    command_t *command,
                                                    uint16_t time_offset)
{
    uint8_t phase_offset = 0;
    uint8_t num_packets = (MAX_COMMAND_FRAMES >> ratio);
    // Five packets required for this command
    packet_item_t *packet;

    time_offset += buffer->ts_last_packet;

    for (uint8_t i = 0; i < num_packets; i++)
    {
        create_packet(buffer, &packet);
        if (packet == NULL)
        {
            return WDDR_ERROR;
        }

        packet->packet.packet.dce_p0 = 1;
        packet->packet.packet.dce_p1 = 1;
        packet->packet.packet.dce_p2 = 1;
        packet->packet.packet.dce_p3 = 1;
        packet->packet.packet.cke_p0 = DFI_PACK_CKE_VAL;
        packet->packet.packet.cke_p1 = DFI_PACK_CKE_VAL;
        packet->packet.packet.cke_p2 = DFI_PACK_CKE_VAL;
        packet->packet.packet.cke_p3 = DFI_PACK_CKE_VAL;

        // Command
        packet->packet.packet.cs_p0 = command->address[phase_offset].cs;
        packet->packet.packet.cs_p1 = command->address[phase_offset].cs;
        packet->packet.packet.address_p0 = command->address[phase_offset].ca_pins;
        phase_offset++;

        if (ratio == WDDR_FREQ_RATIO_1TO2)
        {
            packet->packet.packet.cs_p2 = command->address[phase_offset].cs;
            packet->packet.packet.cs_p3 = command->address[phase_offset].cs;
            packet->packet.packet.address_p2 = command->address[phase_offset].ca_pins;
            phase_offset++;
        }

        // Add to list
        vListInitialiseItem(&packet->list_item);
        packet->packet.packet.time = time_offset;
        listSET_LIST_ITEM_VALUE(&packet->list_item, time_offset);
        listSET_LIST_ITEM_OWNER(&packet->list_item, packet);
        vListInsertEnd(&buffer->list, &packet->list_item);
        time_offset++;
    }

    buffer->ts_last_packet = time_offset - 1;
    return WDDR_SUCCESS;
}

// TODO: Need to support 8 phases
static void extract_packet_data(dfi_rx_packet_desc_t *packet,
                                uint8_t data_packet[PACKET_MAX_NUM_PHASES],
                                wddr_dq_byte_t dq_byte,
                                uint8_t phases)
{
    data_packet[0] = (dq_byte == WDDR_DQ_BYTE_0) ? packet->dq0_dfi_rddata_w0 : packet->dq1_dfi_rddata_w0;
    data_packet[1] = (dq_byte == WDDR_DQ_BYTE_0) ? packet->dq0_dfi_rddata_w1 : packet->dq1_dfi_rddata_w1;

    if (phases == 4)
    {
        data_packet[2] = (dq_byte == WDDR_DQ_BYTE_0) ? packet->dq0_dfi_rddata_w2 : packet->dq1_dfi_rddata_w2;
        data_packet[3] = (dq_byte == WDDR_DQ_BYTE_0) ? packet->dq0_dfi_rddata_w3 : packet->dq1_dfi_rddata_w3;
    }
}

static bool compare_received_data(uint8_t received[PACKET_MAX_NUM_PHASES],
                                  uint8_t *expected,
                                  uint8_t phases,
                                  packet_data_mask_t data_mask)
{
    uint8_t step_size = data_mask == PACKET_DATA_MASK_BOTH ? 1 : 2;
    uint8_t i = data_mask & PACKET_DATA_MASK_EVEN ? 0 : 1;

    // compare all phases
    for (; i < phases; i += step_size)
    {
        if (expected[i] != received[i])
        {
            return false;
        }
    }

    return true;
}
