/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdbool.h>
#include <string.h>
#include <dfi/packet.h>

#define DFI_PACK_CKE_VAL        ((WDDR_PHY_RANK << 1) - 1)
#define CS_LOW                  (0)
#define CS_HIGH                 (1)

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
    buffer->ts_last_packet = time_offset;

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
    buffer->ts_last_packet = time_offset;

    // Add to list
    vListInitialiseItem(&packet->list_item);
    packet->packet.packet.time = time_offset;
    listSET_LIST_ITEM_VALUE(&packet->list_item, time_offset);
    listSET_LIST_ITEM_OWNER(&packet->list_item, packet);
    vListInsertEnd(&buffer->list, &packet->list_item);

    return WDDR_SUCCESS;
}

wddr_return_t create_wrfifo_packet_sequence(dfi_tx_packet_buffer_t *buffer,
                                            command_data_t *data,
                                            burst_length_t burst_length,
                                            dram_freq_cfg_t *dram_cfg,
                                            uint8_t wr_offset,
                                            chipselect_t cs,
                                            uint16_t time_offset)
{
    uint8_t phase_offset = 0;
    uint8_t data_offset = 0;
    uint8_t offset_remainder = 0;
    packet_item_t *packet;
    command_t command = {0};

    // 2 data phases per clock, need to shift by 1 to convert to phases.
    uint8_t phy_wr_en = dram_cfg->phy_wr_en << 1;

    // Need to calculate DRAM clock delay last CA was from last buffer timestamp.
    // If 1TO1 ratio, this is always 0, if 1TO2 ratio, there is 1 DRAM clk delay.
    uint8_t ca_last = (dram_cfg->ratio == WDDR_FREQ_RATIO_1TO2 ? 1 : 0);

    // Calculate wr latency required for wr packet from last CA clk.
    uint8_t phy_wr_lat = dram_cfg->phy_wr_lat + wr_offset + ca_last;

    // Create WRFIFO command sequence.
    create_wrfifo_command(&command, data, cs);
    create_address_packet_sequence(buffer, dram_cfg->ratio, &command, time_offset);

    // Create the write packets
    // Get timestamp of last address packet and add wr latency delay.
    time_offset = buffer->ts_last_packet + (phy_wr_lat >> dram_cfg->ratio);
    offset_remainder = (phy_wr_lat & dram_cfg->ratio);

    // If enough delay is required, insert CKE packet to keep clocks cycling.
    if ((phy_wr_lat >> dram_cfg->ratio) > 1)
    {
        create_cke_packet_sequence(buffer, 1);
    }

    // Need to calculate number of packets
    // Adding 1 extra ensures 2 remainders are not missed, if extra, will only be CKE
    uint8_t num_packets = (((burst_length + (phy_wr_en > 4 ? phy_wr_en : 4)) >> dram_cfg->ratio) >> 1) + offset_remainder + 1;

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

        if (offset_remainder == 0)
        {
            // wrdata_en offset from phy_wr_lat = 0
            // length = burst_length + 4
            if (phase_offset < burst_length + 4)
            {
                packet->packet.packet.wrdata_en_p0 = 1;
                packet->packet.packet.wrdata_en_p1 = 1;
            }
            // wrdata offset from phy_wr_lat = phy_wr_en
            // length = burst_length
            if ((phase_offset >= phy_wr_en) &&
                (phase_offset < burst_length + phy_wr_en))
            {
                data_offset = phase_offset - phy_wr_en;
                packet->packet.packet.wrdata_cs_p0  = 1 << cs;
                packet->packet.packet.dq0_wrdata_p0 = data->dq[0][data_offset];
                packet->packet.packet.dq1_wrdata_p0 = data->dq[1][data_offset];
                packet->packet.packet.dq0_wrdata_mask_p0 = data->dm[0][data_offset];
                packet->packet.packet.dq1_wrdata_mask_p0 = data->dm[1][data_offset];
                packet->packet.packet.wrdata_cs_p1  = 1 << cs;
                packet->packet.packet.dq0_wrdata_p1 = data->dq[0][data_offset + 1];
                packet->packet.packet.dq1_wrdata_p1 = data->dq[1][data_offset + 1];
                packet->packet.packet.dq0_wrdata_mask_p1 = data->dm[0][data_offset + 1];
                packet->packet.packet.dq1_wrdata_mask_p1 = data->dm[1][data_offset + 1];
            }
            phase_offset += 2;
        }
        else
        {
            offset_remainder--;
        }

        if (dram_cfg->ratio == WDDR_FREQ_RATIO_1TO2)
        {
            // wrdata_en offset from phy_wr_lat = 0
            // length = burst_length + 4
            if (phase_offset < burst_length + 4)
            {
                packet->packet.packet.wrdata_en_p2 = 1;
                packet->packet.packet.wrdata_en_p3 = 1;
            }
            // wrdata offset from phy_wr_lat = phy_wr_en
            // length = burst_length
            if ((phase_offset >= phy_wr_en) &&
                (phase_offset < burst_length + phy_wr_en))
            {
                data_offset = phase_offset - phy_wr_en;
                packet->packet.packet.wrdata_cs_p2  = 1 << cs;
                packet->packet.packet.dq0_wrdata_p2 = data->dq[0][data_offset];
                packet->packet.packet.dq1_wrdata_p2 = data->dq[1][data_offset];
                packet->packet.packet.dq0_wrdata_mask_p2 = data->dm[0][data_offset];
                packet->packet.packet.dq1_wrdata_mask_p2 = data->dm[1][data_offset];
                packet->packet.packet.wrdata_cs_p3  = 1 << cs;
                packet->packet.packet.dq0_wrdata_p3 = data->dq[0][data_offset + 1];
                packet->packet.packet.dq1_wrdata_p3 = data->dq[1][data_offset + 1];
                packet->packet.packet.dq0_wrdata_mask_p3 = data->dm[0][data_offset + 1];
                packet->packet.packet.dq1_wrdata_mask_p3 = data->dm[1][data_offset + 1];
            }
            phase_offset += 2;
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

static wddr_return_t create_read_packet_sequence(dfi_tx_packet_buffer_t *buffer,
                                          burst_length_t burst_length,
                                          uint8_t ratio,
                                          uint8_t phy_rd_en,
                                          command_t *command,
                                          chipselect_t cs,
                                          uint16_t time_offset)
{
    uint8_t phase_offset = 0;
    uint8_t offset_remainder = 0;
    // Five packets required for this command
    packet_item_t *packet;

    create_address_packet_sequence(buffer, ratio, command, time_offset);

    // Need to add phy_rd_en to get next packet offset.
    time_offset = buffer->ts_last_packet + (phy_rd_en >> ratio);
    offset_remainder = (phy_rd_en & ratio);

    if ((phy_rd_en >> ratio) > 1)
    {
        create_cke_packet_sequence(buffer, 1);
    }

    // There is 2 data values per packet, shift by 1 to divide by 2
    uint8_t num_packets = ((burst_length >> ratio) >> 1) + offset_remainder;

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

        if (offset_remainder == 0)
        {
            packet->packet.packet.rddata_en_p0 = 1;
            packet->packet.packet.rddata_en_p1 = 1;
            packet->packet.packet.rddata_cs_p0 = 1 << cs;
            packet->packet.packet.rddata_cs_p1 = 1 << cs;
            phase_offset += 2;
        }
        else
        {
            offset_remainder--;
        }

        if (ratio == WDDR_FREQ_RATIO_1TO2 && phase_offset < burst_length)
        {
            packet->packet.packet.rddata_en_p2 = 1;
            packet->packet.packet.rddata_en_p3 = 1;
            packet->packet.packet.rddata_cs_p2 = 1 << cs;
            packet->packet.packet.rddata_cs_p3 = 1 << cs;
            phase_offset += 2;
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

wddr_return_t create_rdfifo_packet_sequence(dfi_tx_packet_buffer_t *buffer,
                                            burst_length_t burst_length,
                                            uint8_t ratio,
                                            uint8_t phy_rd_en,
                                            chipselect_t cs,
                                            uint16_t time_offset)
{
    command_t command = {0};
    create_rdfifo_command(&command, cs);
    return create_read_packet_sequence(buffer, burst_length, ratio, phy_rd_en, &command, cs, time_offset);
}

wddr_return_t create_rddq_packet_sequence(dfi_tx_packet_buffer_t *buffer,
                                          burst_length_t burst_length,
                                          uint8_t ratio,
                                          uint8_t phy_rd_en,
                                          chipselect_t cs,
                                          uint16_t time_offset)
{
    command_t command = {0};
    create_rddq_command(&command, cs);
    return create_read_packet_sequence(buffer, burst_length, ratio, phy_rd_en, &command, cs, time_offset);
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

wddr_return_t create_cbt_packet_sequence(dfi_tx_packet_buffer_t *buffer,
                                         dram_freq_cfg_t *dram_cfg,
                                         chipselect_t cs,
                                         uint8_t vref_setting,
                                         uint8_t command_address,
                                         uint16_t time_offset)
{
    uint8_t offset_remainder = 0;
    uint8_t ratio = dram_cfg->ratio;
    uint8_t num_packets;
    packet_item_t *packet;

    // Initialize time_offset with last packet timestamp
    time_offset += buffer->ts_last_packet;

    // Generate DQ data packets for setting new CA VREF value
    // First Packet is setting DQ pins to CA VREF value
    create_packet(buffer, &packet);
    if (packet == NULL)
    {
        return WDDR_ERROR;
    }

    packet->packet.packet.dce_p0 = 1;
    packet->packet.packet.dce_p1 = 1;
    packet->packet.packet.dce_p2 = 1;
    packet->packet.packet.dce_p3 = 1;

    packet->packet.packet.wrdata_cs_p0  = 1 << cs;
    packet->packet.packet.dq0_wrdata_p0 = vref_setting;
    packet->packet.packet.dq1_wrdata_p0 = vref_setting;
    packet->packet.packet.wrdata_cs_p1  = 1 << cs;
    packet->packet.packet.dq0_wrdata_p1 = vref_setting;
    packet->packet.packet.dq1_wrdata_p1 = vref_setting;

    if (ratio == WDDR_FREQ_RATIO_1TO2)
    {
        packet->packet.packet.wrdata_cs_p2  = 1 << cs;
        packet->packet.packet.dq0_wrdata_p2 = vref_setting;
        packet->packet.packet.dq1_wrdata_p2 = vref_setting;
        packet->packet.packet.wrdata_cs_p3  = 1 << cs;
        packet->packet.packet.dq0_wrdata_p3 = vref_setting;
        packet->packet.packet.dq1_wrdata_p3 = vref_setting;
    }

    // Add to list
    vListInitialiseItem(&packet->list_item);
    packet->packet.packet.time = time_offset;
    listSET_LIST_ITEM_VALUE(&packet->list_item, time_offset);
    listSET_LIST_ITEM_OWNER(&packet->list_item, packet);
    vListInsertEnd(&buffer->list, &packet->list_item);

    // Second packet is toggling DQS tDStrain after setting DQ pins
    // Minimum of 2 DQS clocks required, this requires 4 phases of data
    // Need to extend offset by 2 to account for preamble
    time_offset += ((dram_cfg->t_sh_train) >> ratio);
    offset_remainder = (dram_cfg->t_sh_train & ratio);
    num_packets = (2 >> ratio);
    if (offset_remainder != 0)
    {
        time_offset++;
    }

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

        packet->packet.packet.wrdata_cs_p0  = 1 << cs;
        packet->packet.packet.dq0_wrdata_p0 = vref_setting;
        packet->packet.packet.dq1_wrdata_p0 = vref_setting;
        packet->packet.packet.wrdata_cs_p1  = 1 << cs;
        packet->packet.packet.dq0_wrdata_p1 = vref_setting;
        packet->packet.packet.dq1_wrdata_p1 = vref_setting;

        packet->packet.packet.wrdata_cs_p2  = 1 << cs;
        packet->packet.packet.dq0_wrdata_p2 = vref_setting;
        packet->packet.packet.dq1_wrdata_p2 = vref_setting;
        packet->packet.packet.wrdata_cs_p3  = 1 << cs;
        packet->packet.packet.dq0_wrdata_p3 = vref_setting;
        packet->packet.packet.dq1_wrdata_p3 = vref_setting;

        packet->packet.packet.wrdata_en_p0 = 1;
        packet->packet.packet.wrdata_en_p1 = 1;

        if (ratio == WDDR_FREQ_RATIO_1TO2)
        {
            packet->packet.packet.wrdata_en_p2 = 1;
            packet->packet.packet.wrdata_en_p3 = 1;
        }

        // Add to list
        vListInitialiseItem(&packet->list_item);
        packet->packet.packet.time = time_offset;
        listSET_LIST_ITEM_VALUE(&packet->list_item, time_offset);
        listSET_LIST_ITEM_OWNER(&packet->list_item, packet);
        vListInsertEnd(&buffer->list, &packet->list_item);
        time_offset++;
    }

    // Third Packet is setting DQ pins to CA VREF value immediately after DQS toggle
    create_packet(buffer, &packet);
    if (packet == NULL)
    {
        return WDDR_ERROR;
    }

    packet->packet.packet.dce_p0 = 1;
    packet->packet.packet.dce_p1 = 1;
    packet->packet.packet.dce_p2 = 1;
    packet->packet.packet.dce_p3 = 1;

    packet->packet.packet.wrdata_cs_p0  = 1 << cs;
    packet->packet.packet.dq0_wrdata_p0 = vref_setting;
    packet->packet.packet.dq1_wrdata_p0 = vref_setting;
    packet->packet.packet.wrdata_cs_p1  = 1 << cs;
    packet->packet.packet.dq0_wrdata_p1 = vref_setting;
    packet->packet.packet.dq1_wrdata_p1 = vref_setting;

    if (ratio == WDDR_FREQ_RATIO_1TO2)
    {
        packet->packet.packet.wrdata_cs_p2  = 1 << cs;
        packet->packet.packet.dq0_wrdata_p2 = vref_setting;
        packet->packet.packet.dq1_wrdata_p2 = vref_setting;
        packet->packet.packet.wrdata_cs_p3  = 1 << cs;
        packet->packet.packet.dq0_wrdata_p3 = vref_setting;
        packet->packet.packet.dq1_wrdata_p3 = vref_setting;
    }

    // Add to list
    vListInitialiseItem(&packet->list_item);
    packet->packet.packet.time = time_offset;
    listSET_LIST_ITEM_VALUE(&packet->list_item, time_offset);
    listSET_LIST_ITEM_OWNER(&packet->list_item, packet);
    vListInsertEnd(&buffer->list, &packet->list_item);

    // Set buffer last packet timestamp before calling create CK
    buffer->ts_last_packet = time_offset;

    // Send CK packet tDHtrain + phy_wr_en after last packet
    time_offset = ((dram_cfg->t_sh_train + dram_cfg->phy_wr_en) >> ratio);
    offset_remainder = ((dram_cfg->t_sh_train + dram_cfg->phy_wr_en) & ratio);

    // Add one extra DFI cycle if tDHtrain is odd value
    if (offset_remainder > 0)
    {
        time_offset++;
    }
    create_ck_packet_sequence(buffer, time_offset);

    // Generate CA data packets to send training sequence
    time_offset = buffer->ts_last_packet + ((dram_cfg->t_vref_ca_long - dram_cfg->t_sh_train) >> ratio);
    offset_remainder = (dram_cfg->t_vref_ca_long & ratio);
    // Add one extra DFI cycle if tDHtrain is odd value
    if (offset_remainder > 0)
    {
        time_offset++;
    }

    create_packet(buffer, &packet);
    if (packet == NULL)
    {
        return WDDR_ERROR;
    }

    packet->packet.packet.dce_p0 = 1;
    packet->packet.packet.dce_p1 = 1;
    packet->packet.packet.dce_p2 = 1;
    packet->packet.packet.dce_p3 = 1;

    // Command
    packet->packet.packet.cs_p0 = CS_HIGH << cs;
    packet->packet.packet.cs_p1 = CS_HIGH << cs;
    packet->packet.packet.address_p0 = command_address;

    // Add to list
    vListInitialiseItem(&packet->list_item);
    packet->packet.packet.time = time_offset;
    listSET_LIST_ITEM_VALUE(&packet->list_item, time_offset);
    listSET_LIST_ITEM_OWNER(&packet->list_item, packet);
    vListInsertEnd(&buffer->list, &packet->list_item);

    // Set buffer last packet timestamp before calling create CK
    buffer->ts_last_packet = time_offset;
    time_offset = 1;

    create_ck_packet_sequence(buffer, time_offset);

    create_ck_packet_sequence(buffer, 2);

    return WDDR_SUCCESS;
}

wddr_return_t create_wrlvl_packet_sequence(dfi_tx_packet_buffer_t *buffer,
                                        wddr_freq_ratio_t ratio,
                                        chipselect_t cs,
                                        uint16_t time_offset)
{
    packet_item_t *packet;
    // Need at least 2 cycles of DQS to send
    uint8_t num_packets = (2 >> ratio);

    // Initialize time_offset with last packet timestamp
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

        packet->packet.packet.wrdata_en_p0 = 1;
        packet->packet.packet.wrdata_en_p1 = 1;

        if (ratio == WDDR_FREQ_RATIO_1TO2)
        {
            packet->packet.packet.wrdata_en_p2 = 1;
            packet->packet.packet.wrdata_en_p3 = 1;
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
