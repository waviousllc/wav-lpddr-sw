/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <dfi/packet.h>
#include <string.h>

#define DFI_PACK_CKE_VAL    ((WDDR_PHY_RANK << 1) - 1)

static dfi_command_cfg_t dfi_command_mission_cfg =
{
    .group_offsets    = {
        {0,0,0,0,0,0,0,0},      // Write Level
        {0,0,0,0,0,0,0,0},      // Read
        {0,0,0,0,0,0,0,0},      // Write
        {0,0,0,4,0,0,19,19},    // MRR  (TODO: Need to verify)
        {0,0,0,0,0,0,0,0},      // MRW
        {0,0,0,0,0,0,0,0},      // CK
        {0,0,0,0,0,0,0,0},      // CKE
        {0,20,20,20,0,0,0,0},   // CBT
        {0,0,0,0,0,0,0,0}       // SR
    },
    .group_lengths    = {
        {8,8,0,0,8,0,0,0},      // Write Level
        {0,0,4,4,0,0,0,0},      // Read
        {0,0,4,4,0,0,0,0},      // Write
        {23,23,4,4,0,0,4,4},    // MRR  (TODO: Use same timing as READ)
        {6,6,4,4,0,0,0,0},      // MRW
        {1,0,0,0,0,0,0,0},      // CK
        {1,1,0,0,0,0,0,0},      // CKE
        {24,0,4,4,2,20,0,0},    // CBT
        {4,4,2,2,0,0,0,0}       // SR
    },
    // Packet Group Phases
    .group_num_phases = {1,1,1,1,2,2,2,2},
};

/**
 * @brief   DFI TX Packet Buffer Pop Packet
 *
 * @details Helper function to get next free packet in the buffer.
 *
 * @param[in]   buffer      pointer to tx packet buffer.
 *
 * @return      pointer to packet item.
 * @retval      NULL if no free packets.
 * @retval      valid pointer otherwise
 */
static packet_item_t * dfi_tx_packet_buffer_pop_packet(dfi_tx_packet_buffer_t *buffer);

/**
 * @brief   DFI Command Fill Packet Group
 *
 * @details Helper function to fill in a particular group of a DFI packet.
 *
 * @param[in]       command         pointer to command that packet belongs to.
 * @param[out]      packet          pointer to packet to fill in.
 * @param[inout]    phase_offset    pointer to how many phases have been filled
 *                                  in for given packet group for given command.
 * @param[in]       group           which packet group to fill.
 * @param[in]       phase_mask      mask to indicate which phases to fill in.
 *
 * @return  void
 */
static void dfi_command_fill_packet_group(command_t *command,
                                          dfi_tx_packet_t *packet,
                                          uint8_t *phase_offset,
                                          uint8_t group,
                                          uint8_t phase_mask);
/**
 * @brief   DFI TX Packet Buffer Insert Packet
 *
 * @details Helper function that inserts a packet for given timestamp
 *          if it's not already in the list.
 *
 * @param[in]   buffer      pointer to packet buffer that packet belongs to.
 * @param[in]   timestamp   timestamp of pcaket to insert.
 * @param[in]   head        pointer to list to search / add packet to.
 *
 * @return      returns whether timestamp was accounted for.
 * @retval      WDDR_SUCCESS if added or already present.
 * @retval      WDDR_ERROR if packet couldn't be allocated.
 */
static wddr_return_t dfi_tx_packet_buffer_insert_packet(dfi_tx_packet_buffer_t *buffer,
                                                        uint16_t timestamp,
                                                        list_head_t *head);


void dfi_tx_packet_buffer_init(dfi_tx_packet_buffer_t *buffer,
                               packet_item_t *packets,
                               uint8_t num_packets)
{
    memset(buffer, 0, sizeof(dfi_tx_packet_buffer_t));
    memset(packets, 0, sizeof(packet_item_t) * num_packets);
    init_list_head(&buffer->list);
    buffer->packets = packets;
    buffer->num_packets = num_packets;
}

void dfi_rx_packet_buffer_init(dfi_rx_packet_buffer_t *buffer)
{
    memset(buffer, 0, sizeof(dfi_rx_packet_buffer_t));
}

void dfi_tx_packet_get_mission_cfg(dfi_command_cfg_t **cfg)
{
    *cfg = &dfi_command_mission_cfg;
}

static packet_item_t * dfi_tx_packet_buffer_pop_packet(dfi_tx_packet_buffer_t *buffer)
{
    if (buffer->packet_index < buffer->num_packets)
    {
        return &buffer->packets[buffer->packet_index++];
    }
    return NULL;
}

static void dfi_command_fill_packet_group(command_t *command,
                                          dfi_tx_packet_t *packet,
                                          uint8_t *phase_offset,
                                          uint8_t group,
                                          uint8_t phase_mask)
{
    switch (group)
    {
        case PACKET_GROUP_COMMAND_DCE:
            packet->packet.dce_p0 = 1;
            packet->packet.dce_p1 = 1;
            packet->packet.dce_p2 = 1;
            packet->packet.dce_p3 = 1;
            break;
        case PACKET_GROUP_COMMAND_CKE:
            packet->packet.cke_p0 = DFI_PACK_CKE_VAL;
            packet->packet.cke_p1 = DFI_PACK_CKE_VAL;
            packet->packet.cke_p2 = DFI_PACK_CKE_VAL;
            packet->packet.cke_p3 = DFI_PACK_CKE_VAL;
            break;
        case PACKET_GROUP_COMMAND_CS:
            if (phase_mask & 0x1)
            {
                packet->packet.cs_p0 = command->address[*phase_offset].cs;
                packet->packet.cs_p1 = command->address[*phase_offset].cs;
                *phase_offset += 1;
            }
            if (phase_mask & 0x2)
            {
                packet->packet.cs_p2 = command->address[*phase_offset].cs;
                packet->packet.cs_p3 = command->address[*phase_offset].cs;
                *phase_offset += 1;
            }
            break;
        case PACKET_GROUP_COMMAND:
            // TODO: Need Phase 1 & 3 for LPDDR5??
            if (phase_mask & 0x1)
            {
                packet->packet.address_p0 = command->address[*phase_offset].ca_pins;
                *phase_offset += 1;
            }
            if (phase_mask & 0x2)
            {
                packet->packet.address_p2 = command->address[*phase_offset].ca_pins;
                *phase_offset += 1;
            }
            break;
        case PACKET_GROUP_WRDATA_EN:
            if (phase_mask & 0x1)
            {
                packet->packet.wrdata_en_p0 = 1;
                packet->packet.wrdata_en_p1 = 1;
                *phase_offset += 2;
            }

            if (phase_mask & 0x2)
            {
                packet->packet.wrdata_en_p2 = 1;
                packet->packet.wrdata_en_p3 = 1;
                *phase_offset += 2;
            }
            break;
        case PACKET_GROUP_WRDATA:
            if (phase_mask & 0x1)
            {
                packet->packet.wrdata_cs_p0  = 1 << command->cs;
                packet->packet.dq0_wrdata_p0 = command->data->dq[0][*phase_offset];
                packet->packet.dq1_wrdata_p0 = command->data->dq[1][*phase_offset];
                packet->packet.dq0_wrdata_mask_p0 = command->data->dm[0][*phase_offset];
                packet->packet.dq1_wrdata_mask_p0 = command->data->dm[1][*phase_offset];
                packet->packet.wrdata_cs_p1  = 1 << command->cs;
                packet->packet.dq0_wrdata_p1 = command->data->dq[0][*phase_offset + 1];
                packet->packet.dq1_wrdata_p1 = command->data->dq[1][*phase_offset + 1];
                packet->packet.dq0_wrdata_mask_p1 = command->data->dm[0][*phase_offset + 1];
                packet->packet.dq1_wrdata_mask_p1 = command->data->dm[1][*phase_offset + 1];
                *phase_offset += 2;
            }

            if (phase_mask & 0x2)
            {
                packet->packet.wrdata_cs_p2  = 1 << command->cs;
                packet->packet.dq0_wrdata_p2 = command->data->dq[0][*phase_offset];
                packet->packet.dq1_wrdata_p2 = command->data->dq[1][*phase_offset];
                packet->packet.dq0_wrdata_mask_p2 = command->data->dm[0][*phase_offset];
                packet->packet.dq1_wrdata_mask_p2 = command->data->dm[1][*phase_offset];
                packet->packet.wrdata_cs_p3  = 1 << command->cs;
                packet->packet.dq0_wrdata_p3 = command->data->dq[0][*phase_offset + 1];
                packet->packet.dq1_wrdata_p3 = command->data->dq[1][*phase_offset + 1];
                packet->packet.dq0_wrdata_mask_p3 = command->data->dm[0][*phase_offset + 1];
                packet->packet.dq1_wrdata_mask_p3 = command->data->dm[1][*phase_offset + 1];
                *phase_offset += 2;
            }
            break;
        case PACKET_GROUP_RDDATA_EN:
            if (phase_mask & 0x1)
            {
                packet->packet.rddata_en_p0 = 1;
                packet->packet.rddata_en_p1 = 1;
                *phase_offset += 2;
            }

            if (phase_mask & 0x2)
            {
                packet->packet.rddata_en_p2 = 1;
                packet->packet.rddata_en_p3 = 1;
                *phase_offset += 2;
            }
            break;
        case PACKET_GROUP_RDDATA_CS:
            if (phase_mask & 0x1)
            {
                packet->packet.rddata_cs_p0 = 1 << command->cs;
                packet->packet.rddata_cs_p1 = 1 << command->cs;
                *phase_offset += 2;
            }
            if (phase_mask & 0x2)
            {
                packet->packet.rddata_cs_p2 = 1 << command->cs;
                packet->packet.rddata_cs_p3 = 1 << command->cs;
                *phase_offset += 2;
            }
            break;
    }
}

static wddr_return_t dfi_tx_packet_buffer_insert_packet(dfi_tx_packet_buffer_t *buffer,
                                                        uint16_t timestamp,
                                                        list_head_t *head)
{
    packet_item_t *current_packet, *next_packet;
    // Search through packet list for command and find where to insert
    // Have to find at least one because of the end packet that's added
    list_for_each_entry(next_packet, head, list)
    {
        if (next_packet->packet.packet.time == timestamp)
        {
            return WDDR_SUCCESS;
        }

        if (next_packet->packet.packet.time > timestamp)
        {
            current_packet = dfi_tx_packet_buffer_pop_packet(buffer);
            if (current_packet == NULL)
            {
                return WDDR_ERROR;
            }

            current_packet->packet.packet.time = timestamp;

            // Insert in the middle
            __list_add(&current_packet->list, next_packet->list.prev, &next_packet->list);
            return WDDR_SUCCESS;
        }
    }

    // If hit here add to the end
    current_packet = dfi_tx_packet_buffer_pop_packet(buffer);
    if (current_packet == NULL)
    {
        return WDDR_ERROR;
    }
    current_packet->packet.packet.time = timestamp;

    // Insert at the end
    list_add_tail(&current_packet->list, head);
    return WDDR_SUCCESS;
}

/**
 *
 * @todo    This function is complicated and difficult to understand. It needs
 *          to be better documented. A potential redsign or more straight
 *          forward approach should be considered.
 */
wddr_return_t dfi_tx_packet_buffer_fill(command_t *command,
                                       dfi_tx_packet_buffer_t *buffer,
                                       dfi_command_cfg_t *cfg,
                                       uint16_t ts_offset)
{
    uint16_t group_offset, group_packet_length, num_packets;
    uint16_t start_time = 1;
    uint16_t *group_lengths;
    uint8_t *group_num_phases;
    packet_item_t *current_packet;
    LIST_HEAD(command_list);
    uint8_t ratio = cfg->ratio;

    // Based on DRAM Cycles
    group_lengths = cfg->group_lengths[command->command_type];
    group_num_phases = cfg->group_num_phases;

    if (!list_empty(&buffer->list))
    {
        // Insert last packet into this group as start
        current_packet = list_last_entry(&buffer->list, packet_item_t, list);
        start_time = current_packet->packet.packet.time + ts_offset - 1;
        current_packet->packet.packet.time = start_time;
        // Move to command list
        list_del(&current_packet->list);
        list_add(&current_packet->list, &command_list);
    }

    /*******************************************************************
     **                         PART 1
     *******************************************************************
     ** Process each packet group. Create the minimum number of packets
     ** required to create the correct DFI signal sequence. Add
     ** timestamps to internal list, creating a "timeline" of packets.
     *******************************************************************
     ******************************************************************/
    for (uint8_t group_index = 0; group_index < PACKET_GROUP_TOTAL_NUM; group_index++)
    {
        uint8_t offset_remainder, length_remainder, phases_per_packet;

        // Skip group_length of zero
        if (group_lengths[group_index] == 0)
        {
            continue;
        }

        phases_per_packet = group_num_phases[group_index] * ratio;

        // Group Offset determines timestamp where this group starts as offset from start_time
        group_offset = cfg->group_offsets[command->command_type][group_index] / ratio;
        offset_remainder = cfg->group_offsets[command->command_type][group_index] % ratio;

        // Determine number of packets required
        group_packet_length = group_lengths[group_index] / phases_per_packet;
        length_remainder = group_lengths[group_index] % phases_per_packet;

        if (offset_remainder || length_remainder)
        {
            // Need extra packet for either case
            group_packet_length += 1;
        }

        // CKE / CK only require one packet to produce correct pattern.
        if (group_index <= PACKET_GROUP_COMMAND_CKE)
        {
            num_packets = 1;
        }
        // COMMAND / WRITE / READ
        else
        {
            num_packets = group_packet_length;
        }

        /**
         * NOTE: 2 Types of allocations required for each group
         * 1. Unique packets per group : start + any others
         * 2. Where group ends + 1 (Think as packet to turn off group)
         */

        // This accounts for 1
        for (uint16_t time_offset = start_time + group_offset; time_offset < start_time + group_offset + num_packets; time_offset++)
        {
            PROPAGATE_ERROR(dfi_tx_packet_buffer_insert_packet(buffer, time_offset, &command_list));
        }

        // This accounts for 2
        PROPAGATE_ERROR(dfi_tx_packet_buffer_insert_packet(buffer, start_time + group_offset + group_packet_length, &command_list));
    }

    /*******************************************************************
     **                         PART 2
     *******************************************************************
     ** Fill in the packets created in PART 1 based on which groups
     ** are valid within the timestamp of each packet.
     *******************************************************************
     ******************************************************************/
    for (uint8_t group_index = 0; group_index < PACKET_GROUP_TOTAL_NUM; group_index++)
    {
        // Skip group_length of zero
        if (group_lengths[group_index] == 0)
        {
            continue;
        }

        group_packet_length = group_lengths[group_index];
        group_offset = cfg->group_offsets[command->command_type][group_index] / ratio;

        uint8_t phases_per_packet = group_num_phases[group_index] * ratio;
        uint8_t offset_remainder = cfg->group_offsets[command->command_type][group_index] % ratio;
        uint8_t length_remainder = group_lengths[group_index] % phases_per_packet;
        uint16_t group_start = start_time + group_offset;
        uint16_t group_end_time = group_start + (group_packet_length / phases_per_packet) + (offset_remainder || length_remainder ? 1 : 0);
        uint8_t phase_offset = 0;

        uint8_t phase_mask = ((1 << ratio) - 1);
        if (group_packet_length < phases_per_packet)
        {
            phase_mask = (1 << (group_packet_length / group_num_phases[group_index])) - 1;
        }
        else if (offset_remainder)
        {
            phase_mask = 1 << offset_remainder;
        }

        // Fill in packets based on if they're valid at packet timestamp for this group
        list_for_each_entry(current_packet, &command_list, list)
        {
            // Within time window that group should write
            if (group_start <= current_packet->packet.packet.time && current_packet->packet.packet.time < group_end_time)
            {
                // Fill in packet for this group
                dfi_command_fill_packet_group(command,
                                              &current_packet->packet,
                                              &phase_offset,
                                              group_index,
                                              phase_mask);
                phase_mask = ((1 << ((group_packet_length - phase_offset) / group_num_phases[group_index])) - 1) & ((1 << ratio) - 1);
            }
        }
    }

    // Add to existing packet list
    list_splice_tail(&command_list, &buffer->list);
    return WDDR_SUCCESS;
}

// TODO: Need to support 8 phases
static void dfi_buffer_convert_packet_to_array(wddr_dq_byte_t dq_byte,
                                               dfi_rx_packet_desc_t *packet,
                                               uint8_t data_packet[PACKET_MAX_NUM_PHASES],
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
static void dfi_buffer_rx_cycle_data_compare(uint8_t packet_data[PACKET_MAX_NUM_PHASES],
                                           uint8_t *data,
                                           uint8_t phases,
                                           packet_data_mask_t data_mask,
                                           uint8_t *is_same)
{
    uint8_t same = 1;
    uint8_t step_size = data_mask == PACKET_DATA_MASK_BOTH ? 1 : 2;
    uint8_t i = data_mask & PACKET_DATA_MASK_EVEN ? 0 : 1;

    // compare all phases
    for (; i < phases; i += step_size)
    {
        if (data[i] != packet_data[i])
        {
            same = 0;
            break;
        }
    }
    *is_same = same;
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
    uint8_t data_packet[PACKET_MAX_NUM_PHASES];

    for (uint8_t ii = 0; ii < num; ii++)
    {
        packet = &buffer->buffer[ii].packet;
        dfi_buffer_convert_packet_to_array(dq_byte, packet, data_packet, phases);
        dfi_buffer_rx_cycle_data_compare(data_packet, &expected->dq[dq_byte][ii * phases], phases, data_mask, is_same);
        if (*is_same == 0)
        {
            break;
        }
    }
}
