/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdbool.h>
#include <string.h>
#include <dfi/packet.h>

#define DFI_PACK_CKE_VAL        ((WDDR_PHY_RANK << 1) - 1)
#define CMD_PHASE_PER_CYC_SHFT  (0)
#define DATA_PHASE_PER_CYC_SHFT (1)

/** @brief  Internal Function for creating a new packet_item_t instance */
static void create_packet(dfi_tx_packet_buffer_t *buffer, packet_item_t **packet);

/** @brief  Internal Function for determining phase mask to fill packets */
static uint32_t determine_group_mask(packet_group_info_t *group_info,
                                     uint8_t cycles_per_packet,
                                     uint8_t phases_per_cycle_shft);

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

packet_item_t * create_ck_packet_sequence(dfi_tx_packet_buffer_t *buffer,
                                          uint16_t time_offset)
{
    packet_item_t *packet;
    create_packet(buffer, &packet);
    if (packet == NULL)
    {
        return NULL;
    }

    time_offset += buffer->ts_last_packet;
    packet->packet.packet.dce_p0 = 1;
    packet->packet.packet.dce_p1 = 1;
#if WDDR_PHY_MAX_FREQ_RATIO >= WDDR_FREQ_RATIO_1TO2
    packet->packet.packet.dce_p2 = 1;
    packet->packet.packet.dce_p3 = 1;
#endif
#if WDDR_PHY_MAX_FREQ_RATIO == WDDR_FREQ_RATIO_1TO4
    packet->packet.packet.dce_p4 = 1;
    packet->packet.packet.dce_p5 = 1;
    packet->packet.packet.dce_p6 = 1;
    packet->packet.packet.dce_p7 = 1;
#endif

    packet->packet.packet.time = time_offset;
    buffer->ts_last_packet = time_offset;

    // Add to list
    vListInitialiseItem(&packet->list_item);
    packet->packet.packet.time = time_offset;
    listSET_LIST_ITEM_VALUE(&packet->list_item, time_offset);
    listSET_LIST_ITEM_OWNER(&packet->list_item, packet);
    vListInsertEnd(&buffer->list, &packet->list_item);

    return packet;
}

packet_item_t * create_cke_packet_sequence(dfi_tx_packet_buffer_t *buffer,
                                         uint16_t time_offset)
{
    packet_item_t *packet;
    packet = create_ck_packet_sequence(buffer, time_offset);
    if (packet == NULL)
    {
        return NULL;
    }

    packet->packet.packet.cke_p0 = DFI_PACK_CKE_VAL;
    packet->packet.packet.cke_p1 = DFI_PACK_CKE_VAL;
#if WDDR_PHY_MAX_FREQ_RATIO >= WDDR_FREQ_RATIO_1TO2
    packet->packet.packet.cke_p2 = DFI_PACK_CKE_VAL;
    packet->packet.packet.cke_p3 = DFI_PACK_CKE_VAL;
#endif
#if WDDR_PHY_MAX_FREQ_RATIO == WDDR_FREQ_RATIO_1TO4
    packet->packet.packet.cke_p4 = DFI_PACK_CKE_VAL;
    packet->packet.packet.cke_p5 = DFI_PACK_CKE_VAL;
    packet->packet.packet.cke_p6 = DFI_PACK_CKE_VAL;
    packet->packet.packet.cke_p7 = DFI_PACK_CKE_VAL;
#endif
    return packet;
}

void fill_address_packet(packet_item_t *packet,
                         packet_group_info_t *group_info,
                         uint8_t cycles_per_packet,
                         command_t *command,
                         uint16_t phase_length)
{
    uint32_t mask = determine_group_mask(group_info,
                                         cycles_per_packet,
                                         CMD_PHASE_PER_CYC_SHFT);

    uint16_t phase_offset = phase_length - group_info->phase_remaining;

    if (mask & 0x1)
    {
        packet->packet.packet.cs_p0 = command->address[phase_offset].cs;
        packet->packet.packet.cs_p1 = command->address[phase_offset].cs;
        packet->packet.packet.address_p0 = command->address[phase_offset].ca_pins;
        group_info->phase_remaining -= 1;
        phase_offset++;
    }
#if WDDR_PHY_MAX_FREQ_RATIO >= WDDR_FREQ_RATIO_1TO2
    if (mask & 0x2)
    {
        packet->packet.packet.cs_p2 = command->address[phase_offset].cs;
        packet->packet.packet.cs_p3 = command->address[phase_offset].cs;
        packet->packet.packet.address_p2 = command->address[phase_offset].ca_pins;
        group_info->phase_remaining -= 1;
        phase_offset++;
    }
#endif
#if WDDR_PHY_MAX_FREQ_RATIO == WDDR_FREQ_RATIO_1TO4
    if (mask & 0x4)
    {
        packet->packet.packet.cs_p4 = command->address[phase_offset].cs;
        packet->packet.packet.cs_p5 = command->address[phase_offset].cs;
        packet->packet.packet.address_p4 = command->address[phase_offset].ca_pins;
        group_info->phase_remaining -= 1;
        phase_offset++;
    }
    if (mask & 0x8)
    {
        packet->packet.packet.cs_p6 = command->address[phase_offset].cs;
        packet->packet.packet.cs_p7 = command->address[phase_offset].cs;
        packet->packet.packet.address_p6 = command->address[phase_offset].ca_pins;
        group_info->phase_remaining -= 1;
        phase_offset++;
    }
#endif
}

void fill_wrdata_en_packet(packet_item_t *packet,
                           packet_group_info_t *group_info,
                           uint8_t cycles_per_packet)
{
    uint32_t mask = determine_group_mask(group_info,
                                         cycles_per_packet,
                                         DATA_PHASE_PER_CYC_SHFT);

    if (mask & 0x1)
    {
        packet->packet.packet.wrdata_en_p0 = 1;
        packet->packet.packet.wrdata_en_p1 = 1;
        group_info->phase_remaining -= 2;
    }
#if WDDR_PHY_MAX_FREQ_RATIO >= WDDR_FREQ_RATIO_1TO2
    if (mask & 0x2)
    {
        packet->packet.packet.wrdata_en_p2 = 1;
        packet->packet.packet.wrdata_en_p3 = 1;
        group_info->phase_remaining -= 2;
    }
#endif
#if WDDR_PHY_MAX_FREQ_RATIO == WDDR_FREQ_RATIO_1TO4
    if (mask & 0x4)
    {
        packet->packet.packet.wrdata_en_p4 = 1;
        packet->packet.packet.wrdata_en_p5 = 1;
        group_info->phase_remaining -= 2;
    }
    if (mask & 0x8)
    {
        packet->packet.packet.wrdata_en_p6 = 1;
        packet->packet.packet.wrdata_en_p7 = 1;
        group_info->phase_remaining -= 2;
    }
#endif
}

void fill_wrdata_packet(packet_item_t *packet,
                        packet_group_info_t *group_info,
                        chipselect_t cs,
                        const command_data_t *data,
                        uint8_t cycles_per_packet,
                        uint16_t phase_length)
{
    uint32_t mask = determine_group_mask(group_info,
                                         cycles_per_packet,
                                         DATA_PHASE_PER_CYC_SHFT);

    uint16_t data_offset = phase_length - group_info->phase_remaining;

    if (mask & 0x1)
    {
        packet->packet.packet.wrdata_cs_p0  = 1 << cs;
        packet->packet.packet.dq0_wrdata_p0 = data->dq[0][data_offset];
        packet->packet.packet.dq1_wrdata_p0 = data->dq[1][data_offset];
        packet->packet.packet.wrdata_cs_p1  = 1 << cs;
        packet->packet.packet.dq0_wrdata_p1 = data->dq[0][data_offset + 1];
        packet->packet.packet.dq1_wrdata_p1 = data->dq[1][data_offset + 1];
        group_info->phase_remaining -= 2;
        data_offset += 2;
    }
#if WDDR_PHY_MAX_FREQ_RATIO >= WDDR_FREQ_RATIO_1TO2
    if (mask & 0x2)
    {
        packet->packet.packet.wrdata_cs_p2  = 1 << cs;
        packet->packet.packet.dq0_wrdata_p2 = data->dq[0][data_offset];
        packet->packet.packet.dq1_wrdata_p2 = data->dq[1][data_offset];
        packet->packet.packet.wrdata_cs_p3  = 1 << cs;
        packet->packet.packet.dq0_wrdata_p3 = data->dq[0][data_offset + 1];
        packet->packet.packet.dq1_wrdata_p3 = data->dq[1][data_offset + 1];
        group_info->phase_remaining -= 2;
        data_offset += 2;
    }
#endif
#if WDDR_PHY_MAX_FREQ_RATIO == WDDR_FREQ_RATIO_1TO4
    if (mask & 0x4)
    {
        packet->packet.packet.wrdata_cs_p4  = 1 << cs;
        packet->packet.packet.dq0_wrdata_p4 = data->dq[0][data_offset];
        packet->packet.packet.dq1_wrdata_p4 = data->dq[1][data_offset];
        packet->packet.packet.wrdata_cs_p5  = 1 << cs;
        packet->packet.packet.dq0_wrdata_p5 = data->dq[0][data_offset + 1];
        packet->packet.packet.dq1_wrdata_p5 = data->dq[1][data_offset + 1];
        group_info->phase_remaining -= 2;
        data_offset += 2;
    }
    if (mask & 0x8)
    {
        packet->packet.packet.wrdata_cs_p6  = 1 << cs;
        packet->packet.packet.dq0_wrdata_p6 = data->dq[0][data_offset];
        packet->packet.packet.dq1_wrdata_p6 = data->dq[1][data_offset];
        packet->packet.packet.wrdata_cs_p7  = 1 << cs;
        packet->packet.packet.dq0_wrdata_p7 = data->dq[0][data_offset + 1];
        packet->packet.packet.dq1_wrdata_p7 = data->dq[1][data_offset + 1];
        group_info->phase_remaining -= 2;
        data_offset += 2;
    }
#endif
}

void fill_rddata_packet(packet_item_t *packet,
                        packet_group_info_t *group_info,
                        chipselect_t cs,
                        uint8_t cycles_per_packet)
{
    uint32_t mask = determine_group_mask(group_info,
                                         cycles_per_packet,
                                         DATA_PHASE_PER_CYC_SHFT);

    if (mask & 0x1)
    {

        packet->packet.packet.rddata_en_p0 = 1;
        packet->packet.packet.rddata_en_p1 = 1;
        packet->packet.packet.rddata_cs_p0 = 1 << cs;
        packet->packet.packet.rddata_cs_p1 = 1 << cs;
        group_info->phase_remaining -= 2;
    }
#if WDDR_PHY_MAX_FREQ_RATIO >= WDDR_FREQ_RATIO_1TO2
    if (mask & 0x2)
    {
        packet->packet.packet.rddata_en_p2 = 1;
        packet->packet.packet.rddata_en_p3 = 1;
        packet->packet.packet.rddata_cs_p2 = 1 << cs;
        packet->packet.packet.rddata_cs_p3 = 1 << cs;
        group_info->phase_remaining -= 2;
    }
#endif
#if WDDR_PHY_MAX_FREQ_RATIO == WDDR_FREQ_RATIO_1TO4
    if (mask & 0x4)
    {
        packet->packet.packet.rddata_en_p4 = 1;
        packet->packet.packet.rddata_en_p5 = 1;
        packet->packet.packet.rddata_cs_p4 = 1 << cs;
        packet->packet.packet.rddata_cs_p5 = 1 << cs;
        group_info->phase_remaining -= 2;
    }
    if (mask & 0x8)
    {
        packet->packet.packet.rddata_en_p6 = 1;
        packet->packet.packet.rddata_en_p7 = 1;
        packet->packet.packet.rddata_cs_p6 = 1 << cs;
        packet->packet.packet.rddata_cs_p7 = 1 << cs;
        group_info->phase_remaining -= 2;
    }
#endif
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
        new_packet = (packet_item_t *) pvPortMalloc(sizeof(packet_item_t));
        if (new_packet != NULL)
        {
            // Intialize packet
            volatile uint32_t *data = new_packet->packet.raw_data;
            uint8_t i = TX_PACKET_SIZE_WORDS;
            while (i--)
            {
                *data++ = 0;
            }
        }
    }

    *packet = new_packet;
}

// TODO: Need to support 8 phases
bool extract_packet_data(const dfi_rx_packet_desc_t *packet,
                         uint8_t data_packet[PACKET_MAX_NUM_PHASES],
                         wddr_dq_byte_t dq_byte,
                         uint8_t phases)
{
    // Assume that read data comes in all aligned (which it does)
    uint8_t valid = (dq_byte == WDDR_DQ_BYTE_0) ? packet->dq0_dfi_rddata_valid_w0 : packet->dq1_dfi_rddata_valid_w0;

    if (!valid)
    {
        return false;
    }

    data_packet[0] = (dq_byte == WDDR_DQ_BYTE_0) ? packet->dq0_dfi_rddata_w0 : packet->dq1_dfi_rddata_w0;
    data_packet[1] = (dq_byte == WDDR_DQ_BYTE_0) ? packet->dq0_dfi_rddata_w1 : packet->dq1_dfi_rddata_w1;

    if (phases == 4)
    {
        data_packet[2] = (dq_byte == WDDR_DQ_BYTE_0) ? packet->dq0_dfi_rddata_w2 : packet->dq1_dfi_rddata_w2;
        data_packet[3] = (dq_byte == WDDR_DQ_BYTE_0) ? packet->dq0_dfi_rddata_w3 : packet->dq1_dfi_rddata_w3;
    }

    return true;
}

bool compare_packet_data(const uint8_t *ptr1,
                         const uint8_t *ptr2,
                         uint8_t phases,
                         packet_data_mask_t mask)
{
    uint8_t step_size = mask == PACKET_DATA_MASK_BOTH ? 1 : 2;
    uint8_t i = mask & PACKET_DATA_MASK_EVEN ? 0 : 1;

    // compare all phases
    for (; i < phases; i += step_size)
    {
        if (ptr1[i] != ptr2[i])
        {
            return false;
        }
    }

    return true;
}

void create_packet_group_info(packet_group_info_t *group_info,
                              wddr_freq_ratio_t ratio,
                              uint16_t dram_clk_cycles,
                              uint16_t phase_length)
{
    group_info->dfi_ts_start = dram_clk_cycles >> ratio;
    group_info->cycle_offset = dram_clk_cycles & ((1 << ratio) - 1);
    group_info->phase_remaining = phase_length;
}

static uint32_t determine_group_mask(packet_group_info_t *group_info,
                                     uint8_t cycles_per_packet,
                                     uint8_t phases_per_cycle_shft)
{
    uint16_t cycles_remaining = group_info->phase_remaining >> phases_per_cycle_shft;
    uint8_t cycle_offset = group_info->cycle_offset;
    // Cycle offset can only be positive for one mask
    group_info->cycle_offset = 0;
    if(cycles_remaining > cycles_per_packet)
    {
        cycles_remaining = cycles_per_packet;
    }
    uint8_t base_mask = (1 << cycles_remaining) - 1;
    return (base_mask << cycle_offset) & base_mask;
}
