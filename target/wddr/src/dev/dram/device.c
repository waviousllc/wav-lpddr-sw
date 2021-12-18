/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <string.h>
#include <compiler.h>
#include <dram/device.h>

#define CS_LOW                  (0)
#define CS_HIGH                 (1)
#define CA_CLK_LEN              (3)

#define VRCG__SHFT      (3)
#define VRCG__MSK       (1 << VRCG__SHFT)
#define CBT__SHFT       (0)
#define CBT__MSK        (1 << CBT__SHFT)
#define FSP_WR__SHFT    (6)
#define FSP_WR__MSK     (1 << FSP_WR__SHFT)
#define FSP_OP__SHFT    (7)
#define FSP_OP__MSK     (1 << FSP_OP__SHFT)
#define WRLVL__SHFT     (7)
#define WRLVL__MSK      (1 << WRLVL__SHFT)

/** @brief  Internal Function for updating DRAM Mode Register */
static void dram_write_mode_register(dram_dev_t *dram,
                                     dfi_dev_t *dfi,
                                     uint8_t mr,
                                     uint8_t op);

/**
 * @brief   DRAM Create Address Packet Sequence
 *
 * @details Creates a sequence of packets for sending commands to DRAM
 *
 * @param[in]   buffer          pointer to packet buffer to save packet to.
 * @param[in]   ratio           ratio of DFI CK to DRAM CK.
 * @param[in]   command         pointer to command structure to send.
 * @param[in]   time_offset     time offset from previous packet that the packet
 *
 * @return      returns whether packet added to the packet buffer.
 * @retval      WDDR_SUCCESS if added.
 * @retval      WDDR_ERROR otherwise.
 */
static wddr_return_t dram_create_address_packet_sequence(dfi_tx_packet_buffer_t *buffer,
                                                         wddr_freq_ratio_t ratio,
                                                         command_t *command,
                                                         uint16_t time_offset);

/**
 * @brief   DRAM Create MRW Packet Sequence
 *
 * @details Creates a set of packets in order to send the specified MRW to
 *          the DRAM.
 *
 * @param[in]   buffer          pointer to packet buffer to save packet to.
 * @param[in]   ratio           ratio of DFI CK to DRAM CK.
 * @param[in]   cs              chipselect value.
 * @param[in]   mode_register   which mode reigster number.
 * @param[in]   op              mode register op data.
 * @param[in]   time_offset     time offset from previous packet that the packet
 *                              should be sent.
 *
 * @return      returns whether packet added to the packet buffer.
 * @retval      WDDR_SUCCESS if added.
 * @retval      WDDR_ERROR otherwise.
 */
static wddr_return_t dram_create_mrw_packet_sequence(dfi_tx_packet_buffer_t *buffer,
                                                     wddr_freq_ratio_t ratio,
                                                     chipselect_t cs,
                                                     uint8_t mode_register,
                                                     uint8_t op,
                                                     uint16_t time_offset);

void dram_init(dram_dev_t *dram,
               dram_freq_cfg_t *dram_cfg)
{
    dram->mr13 = 0;
    dram_frequency_set(dram, dram_cfg);
}

void dram_frequency_set(dram_dev_t *dram,
                        dram_freq_cfg_t *dram_cfg)
{
    dram->cfg = dram_cfg;
}

void dram_power_down(__UNUSED__ dram_dev_t *dram, dfi_dev_t *dfi)
{
    dfi_tx_packet_buffer_clear(&dfi->packet.buff);
    create_ck_packet_sequence(&dfi->packet.buff, 1);
    create_ck_packet_sequence(&dfi->packet.buff, 10);
    dfi_buffer_fill_and_send_packets(dfi, &dfi->packet.buff.list);
}

void dram_idle(__UNUSED__ dram_dev_t *dram, dfi_dev_t *dfi)
{
    dfi_tx_packet_buffer_clear(&dfi->packet.buff);
    create_ck_packet_sequence(&dfi->packet.buff, 1);
    create_cke_packet_sequence(&dfi->packet.buff, 30);
    dfi_buffer_fill_and_send_packets(dfi, &dfi->packet.buff.list);
}

void dram_frequency_init(dram_dev_t *dram,
                         dfi_dev_t *dfi,
                         dram_freq_cfg_t *dram_cfg)
{
    // Initialize TX Packet Buffer
    dfi_tx_packet_buffer_clear(&dfi->packet.buff);
    dram_prepare_mrw_update(dram, &dfi->packet.buff, dram_cfg);
    dfi_buffer_fill_and_send_packets(dfi, &dfi->packet.buff.list);

    // Update DRAM frequency after sending MRW to ensure previous tables
    // are used to send MRW.
    dram_frequency_set(dram, dram_cfg);
}

void dram_cbt_enter(dram_dev_t *dram,
                    dfi_dev_t *dfi)
{
    dram->mr13 |= CBT__MSK;

    dfi_tx_packet_buffer_clear(&dfi->packet.buff);
    dram_create_mrw_packet_sequence(&dfi->packet.buff, dram->cfg->ratio, CS_0, 0xD, dram->mr13, 15);
    create_cke_packet_sequence(&dfi->packet.buff, 1);
    create_ck_packet_sequence(&dfi->packet.buff, 15);
    dfi_buffer_fill_and_send_packets(dfi, &dfi->packet.buff.list);
}

void dram_cbt_exit(dram_dev_t *dram,
                   dfi_dev_t *dfi)
{
    dram->mr13 &= ~CBT__MSK;

    dfi_tx_packet_buffer_clear(&dfi->packet.buff);
    create_ck_packet_sequence(&dfi->packet.buff, 15);
    create_cke_packet_sequence(&dfi->packet.buff, 1);
    dram_create_mrw_packet_sequence(&dfi->packet.buff, dram->cfg->ratio, CS_0, 0xD, dram->mr13, dram->cfg->t_vref_ca_long);
    create_cke_packet_sequence(&dfi->packet.buff, 1);
    dfi_buffer_fill_and_send_packets(dfi, &dfi->packet.buff.list);
}

void dram_set_fsp_wr(dram_dev_t *dram,
                     dfi_dev_t *dfi,
                     uint8_t fsp)
{
    dram->mr13 &= ~FSP_WR__MSK;
    dram->mr13 |= (fsp << FSP_WR__SHFT);

    dram_write_mode_register_13(dram, dfi, dram->mr13);
}

void dram_set_fsp_op(dram_dev_t *dram,
                     dfi_dev_t *dfi,
                     uint8_t fsp)
{
    dram->mr13 &= ~FSP_OP__MSK & ~FSP_WR__MSK;
    dram->mr13 |= (fsp << FSP_OP__SHFT) | (fsp << FSP_WR__SHFT);

    dram_write_mode_register_13(dram, dfi, dram->mr13);
}

void dram_set_dq_vref(dram_dev_t *dram,
                      dfi_dev_t *dfi,
                      uint8_t vref_setting)
{
    dram_write_mode_register_14(dram, dfi, vref_setting);
}

void dram_wrlvl_enable(dram_dev_t *dram,
                       dfi_dev_t *dfi)
{
    dram->cfg->mr2 |= WRLVL__MSK;

    dram_write_mode_register_2(dram, dfi, dram->cfg->mr2);
}

void dram_wrlvl_disable(dram_dev_t *dram,
                        dfi_dev_t *dfi)
{
    dram->cfg->mr2 &= ~WRLVL__MSK;

    dram_write_mode_register_2(dram, dfi, dram->cfg->mr2);
}

void dram_vrcg_enable(dram_dev_t *dram,
                      dfi_dev_t *dfi)
{
    dram->mr13 |= VRCG__MSK;
    dram_write_mode_register_13(dram, dfi, dram->mr13);
}

void dram_vrcg_disable(dram_dev_t *dram,
                       dfi_dev_t *dfi)
{
    dram->mr13 &= ~VRCG__MSK;
    dram_write_mode_register_13(dram, dfi, dram->mr13);
}

void dram_write_mode_register_13(dram_dev_t *dram,
                                 dfi_dev_t *dfi,
                                 uint8_t mr13)
{
    dram->mr13 = mr13;
    return dram_write_mode_register(dram, dfi, 0xD, dram->mr13);
}

void dram_write_mode_register_14(dram_dev_t *dram,
                                 dfi_dev_t *dfi,
                                 uint8_t mr14)
{
    dram->cfg->mr14 = mr14;
    return dram_write_mode_register(dram, dfi, 0xE, dram->cfg->mr14);
}

void dram_write_mode_register_2(dram_dev_t *dram,
                                dfi_dev_t *dfi,
                                uint8_t mr2)
{
    dram->cfg->mr2 = mr2;
    return dram_write_mode_register(dram, dfi, 0x2, dram->cfg->mr2);
}


void dram_get_read_enable_offset_timing(dram_freq_cfg_t *dram_cfg,
                                        uint8_t *offset)
{
    *offset = (dram_cfg->phy_rd_en + CA_CLK_LEN);
}

void dram_get_write_enable_offset_timing(dram_freq_cfg_t *dram_cfg,
                                         uint8_t *offset)
{
    *offset = (dram_cfg->phy_wr_lat + CA_CLK_LEN);
}

void dram_get_write_data_offset_timing(dram_freq_cfg_t *dram_cfg,
                                       uint8_t *offset)
{
    *offset = (dram_cfg->phy_wr_lat + dram_cfg->phy_wr_en + CA_CLK_LEN);
}

wddr_return_t dram_prepare_cbt_sequence(dram_dev_t *dram,
                               dfi_tx_packet_buffer_t *buffer,
                               chipselect_t cs,
                               uint8_t vref_setting,
                               uint8_t command_address)
{
    uint8_t ratio = dram->cfg->ratio;
    packet_item_t *packet;
    uint8_t time_offset = 1;
    uint16_t initial_dram_offset = 0;
    uint16_t dram_clk = 0;
    uint16_t wrdata_phases;
    packet_group_info_t wrdata_en_info, wrdata_info;
    uint8_t cycles_per_packet = 1 << ratio;
    uint8_t phases_per_packet = cycles_per_packet << 1;
    uint8_t data_src[phases_per_packet];
    command_data_t data = {0};
    memset(&data_src[0], vref_setting, phases_per_packet);
    create_data_frame(&data, WDDR_DQ_BYTE_0, &data_src[0], phases_per_packet, 0);

    // Calculate timestamp of first CA packet (time 0).
    // Shifted by ratio to be measured in DRAM CLK cycles.
    initial_dram_offset = (buffer->ts_last_packet + time_offset) << ratio;

    create_ck_packet_sequence(buffer, time_offset);

    // First packet group is setting DQ pins to vref_setting
    dram_clk = initial_dram_offset + cycles_per_packet;
    // Need to ensure to cover tDStrain offset + 4 phases wrdata_en + 2 extra wrdata packet
    // Using 2 extra wrdata packets ensures any phase offset is accounted for
    // Always 2 phases per DRAM cycle, shift left by 1 to account for this
    wrdata_phases = ((dram->cfg->t_sh_train + (cycles_per_packet << 1)) << 1) + 4;
    create_packet_group_info(&wrdata_info, ratio, dram_clk, wrdata_phases);

    // Second packet group is toggling DQS tDStrain after setting DQ pins
    dram_clk += dram->cfg->t_sh_train;
    create_packet_group_info(&wrdata_en_info, ratio, dram_clk, 4);

    time_offset = wrdata_info.dfi_ts_start - buffer->ts_last_packet;
    do
    {
        packet = create_ck_packet_sequence(buffer, time_offset);
        if (packet == NULL)
        {
            return WDDR_ERROR;
        }

        if (packet->packet.packet.time >= wrdata_info.dfi_ts_start)
        {
            // Use phase_remaining will ensure data sent in only needs to by phases_per_packet long
            fill_wrdata_packet(packet, &wrdata_info, cs, &data, cycles_per_packet, wrdata_info.phase_remaining);
        }

        if (packet->packet.packet.time >= wrdata_en_info.dfi_ts_start)
        {
            fill_wrdata_en_packet(packet, &wrdata_en_info, cycles_per_packet);
        }

        time_offset = 1;
    } while (wrdata_info.phase_remaining >= phases_per_packet);

    // Send CK packet tDHtrain + phy_wr_en after last packet
    // Add 1 to offset to ensure beyond tDHtrain + phy_wr_en
    time_offset = ((dram->cfg->t_sh_train + dram->cfg->phy_wr_en) >> ratio) + 1;

    create_ck_packet_sequence(buffer, time_offset);

    // Generate CA data packets to send training sequence
    time_offset = ((dram->cfg->t_vref_ca_long - dram->cfg->t_sh_train) >> ratio) + 1;

    packet = create_ck_packet_sequence(buffer, time_offset);
    if (packet == NULL)
    {
        return WDDR_ERROR;
    }

    // Command
    packet->packet.packet.cs_p0 = CS_HIGH << cs;
    packet->packet.packet.cs_p1 = CS_HIGH << cs;
    packet->packet.packet.address_p0 = command_address;

    create_ck_packet_sequence(buffer, 1);

    create_ck_packet_sequence(buffer, 2);

    return WDDR_SUCCESS;
}

wddr_return_t dram_prepare_wrfifo_sequence(dram_dev_t *dram,
                                           dfi_tx_packet_buffer_t *buffer,
                                           burst_length_t burst_length,
                                           chipselect_t cs,
                                           uint16_t wrdata_en_offset,
                                           command_data_t *data)
{
    uint16_t initial_dram_offset = 0;
    uint8_t time_offset = 1;
    packet_item_t *packet;
    command_t command = {0};
    packet_group_info_t wrdata_en_info, wrdata_info;

    // Calculate timestamp of first CA packet (time 0).
    // Shifted by ratio to be measured in DRAM CLK cycles.
    initial_dram_offset = (buffer->ts_last_packet + time_offset) << dram->cfg->ratio;

    // Calculate wr latency required for wr packet from time 0
    // time 0 is identified as first CA packet ts
    // wrdata offset is measured in DRAM CLKs
    uint8_t wrdata_en_dram_clk = initial_dram_offset + CA_CLK_LEN + dram->cfg->phy_wr_lat + wrdata_en_offset;
    uint8_t wrdata_dram_clk = wrdata_en_dram_clk + dram->cfg->phy_wr_en;
    uint8_t cycles_per_packet = 1 << dram->cfg->ratio;

    create_packet_group_info(&wrdata_en_info, dram->cfg->ratio, wrdata_en_dram_clk, burst_length + 4);
    create_packet_group_info(&wrdata_info, dram->cfg->ratio, wrdata_dram_clk, burst_length);

    // Create WRFIFO command sequence.
    create_wrfifo_command(&command, data, cs);
    dram_create_address_packet_sequence(buffer, dram->cfg->ratio, &command, time_offset);

    time_offset = wrdata_en_info.dfi_ts_start - buffer->ts_last_packet;

    // If enough delay is required, insert CKE packet to keep clocks cycling.
    if (time_offset > 1)
    {
        create_cke_packet_sequence(buffer, 1);
        time_offset--;
    }

    do
    {
        packet = create_cke_packet_sequence(buffer, time_offset);
        if (packet == NULL)
        {
            return WDDR_ERROR;
        }

        if (packet->packet.packet.time >= wrdata_en_info.dfi_ts_start)
        {
            fill_wrdata_en_packet(packet, &wrdata_en_info, cycles_per_packet);
        }

        if (packet->packet.packet.time >= wrdata_info.dfi_ts_start)
        {
            fill_wrdata_packet(packet, &wrdata_info, cs, data, cycles_per_packet, burst_length);
        }

        time_offset = 1;
    } while(wrdata_en_info.phase_remaining || wrdata_info.phase_remaining);

    create_cke_packet_sequence(buffer, 1);

    return WDDR_SUCCESS;
}

static wddr_return_t dram_prepare_read_sequence(dram_dev_t *dram,
                                                dfi_tx_packet_buffer_t *buffer,
                                                burst_length_t burst_length,
                                                chipselect_t cs,
                                                uint16_t rddata_en_offset,
                                                command_t *command)
{
    uint16_t initial_dram_offset = 0;
    packet_item_t *packet;
    packet_group_info_t rddata_info;
    uint16_t time_offset = 1;

    // Calculate timestamp of first CA packet (time 0)
    initial_dram_offset = (buffer->ts_last_packet + time_offset) << dram->cfg->ratio;

    // Calculate rddata offset required for rd packet from time 0
    // time 0 is identified as first CA packet ts
    // rddata offset is measured in DRAM CLKs
    uint8_t rddata_dram_clk = initial_dram_offset + CA_CLK_LEN + dram->cfg->phy_rd_en + rddata_en_offset;
    uint8_t cycles_per_packet = 1 << dram->cfg->ratio;

    create_packet_group_info(&rddata_info, dram->cfg->ratio, rddata_dram_clk, burst_length);

    dram_create_address_packet_sequence(buffer, dram->cfg->ratio, command, time_offset);

    // Need to add phy_rd_en to get next packet offset.
    time_offset = rddata_info.dfi_ts_start - buffer->ts_last_packet;

    // If enough delay is required, insert CKE packet to keep clocks cycling.
    if (time_offset > 1)
    {
        create_cke_packet_sequence(buffer, 1);
        time_offset--;
    }

    do
    {
        packet = create_cke_packet_sequence(buffer, time_offset);
        if (packet == NULL)
        {
            return WDDR_ERROR;
        }

        fill_rddata_packet(packet, &rddata_info, cs, cycles_per_packet);

        time_offset = 1;
    } while (rddata_info.phase_remaining);

    create_cke_packet_sequence(buffer, 1);

    return WDDR_SUCCESS;
}

wddr_return_t dram_prepare_rddq_sequence(dram_dev_t *dram,
                                         dfi_tx_packet_buffer_t *buffer,
                                         burst_length_t burst_length,
                                         chipselect_t cs,
                                         uint16_t rddata_en_offset)
{
    command_t command = {0};
    create_rddq_command(&command, cs);
    return dram_prepare_read_sequence(dram, buffer, burst_length, cs, rddata_en_offset, &command);
}

wddr_return_t dram_prepare_rdfifo_sequence(dram_dev_t *dram,
                                           dfi_tx_packet_buffer_t *buffer,
                                           burst_length_t burst_length,
                                           chipselect_t cs,
                                           uint16_t rddata_en_offset)
{
    command_t command = {0};
    create_rdfifo_command(&command, cs);
    return dram_prepare_read_sequence(dram, buffer, burst_length, cs, rddata_en_offset, &command);
}

wddr_return_t dram_prepare_wrlvl_sequence(dram_dev_t *dram,
                                          dfi_tx_packet_buffer_t *buffer)
{
    packet_item_t *packet;
    uint8_t time_offset = 1;
    packet_group_info_t wrdata_en_info;
    uint16_t initial_dram_clk = (buffer->ts_last_packet + time_offset) << dram->cfg->ratio;

    create_packet_group_info(&wrdata_en_info, dram->cfg->ratio, initial_dram_clk, 4);

    do
    {
        packet = create_cke_packet_sequence(buffer, time_offset);
        if (packet == NULL)
        {
            return WDDR_ERROR;
        }

        fill_wrdata_en_packet(packet, &wrdata_en_info, 4);

        time_offset = 1;
    } while (wrdata_en_info.phase_remaining);

    create_cke_packet_sequence(buffer, 1);

    return WDDR_SUCCESS;
}

void dram_prepare_mrw_update(dram_dev_t *dram,
                             dfi_tx_packet_buffer_t *packet_buffer,
                             dram_freq_cfg_t *dram_cfg)
{
    create_cke_packet_sequence(packet_buffer, 1);

    for (uint8_t rank = CS_0; rank < WDDR_PHY_RANK; rank++)
    {
        dram_create_mrw_packet_sequence(packet_buffer, dram->cfg->ratio, rank, 0x01, dram_cfg->mr1, 10);
        create_cke_packet_sequence(packet_buffer, 1);
        dram_create_mrw_packet_sequence(packet_buffer, dram->cfg->ratio, rank, 0x02, dram_cfg->mr2, 10);
        create_cke_packet_sequence(packet_buffer, 1);
        dram_create_mrw_packet_sequence(packet_buffer, dram->cfg->ratio, rank, 0x0B, dram_cfg->mr11, 10);
        create_cke_packet_sequence(packet_buffer, 1);
        dram_create_mrw_packet_sequence(packet_buffer, dram->cfg->ratio, rank, 0x0C, dram_cfg->mr12, 10);
        create_cke_packet_sequence(packet_buffer, 1);
        dram_create_mrw_packet_sequence(packet_buffer, dram->cfg->ratio, rank, 0x0E, dram_cfg->mr14, dram_cfg->t_vref_ca_long);
        create_cke_packet_sequence(packet_buffer, 1);
    }
}

static void dram_write_mode_register(dram_dev_t *dram,
                                     dfi_dev_t *dfi,
                                     uint8_t mr,
                                     uint8_t op)
{
    dfi_tx_packet_buffer_clear(&dfi->packet.buff);
    dram_create_mrw_packet_sequence(&dfi->packet.buff, dram->cfg->ratio, CS_0, mr, op, 1);
    create_cke_packet_sequence(&dfi->packet.buff, 1);
    dfi_buffer_fill_and_send_packets(dfi, &dfi->packet.buff.list);
}

static wddr_return_t dram_create_mrw_packet_sequence(dfi_tx_packet_buffer_t *buffer,
                                                wddr_freq_ratio_t ratio,
                                                chipselect_t cs,
                                                uint8_t mode_register,
                                                uint8_t op,
                                                uint16_t time_offset)
{
    command_t command = {0};
    create_write_mode_register_command(&command, cs, mode_register, op);
    return dram_create_address_packet_sequence(buffer, ratio, &command, time_offset);
}

static wddr_return_t dram_create_address_packet_sequence(dfi_tx_packet_buffer_t *buffer,
                                                    wddr_freq_ratio_t ratio,
                                                    command_t *command,
                                                    uint16_t time_offset)
{
    uint8_t phase_offset = 0;
    uint8_t num_packets = (MAX_COMMAND_FRAMES >> ratio);
    // Five packets required for this command
    packet_item_t *packet;

    for (uint8_t i = 0; i < num_packets; i++)
    {
        packet = create_cke_packet_sequence(buffer, time_offset);
        if (packet == NULL)
        {
            return WDDR_ERROR;
        }

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

        time_offset = 1;
    }

    return WDDR_SUCCESS;
}
