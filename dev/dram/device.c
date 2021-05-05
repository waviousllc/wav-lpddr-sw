/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <string.h>
#include <compiler.h>
#include <dram/device.h>

#define CA_LAST         (3)
#define VRCG__SHFT      (3)
#define VRCG__MSK       (1 << VRCG__SHFT)
#define CBT__SHFT       (0)
#define CBT__MSK        (1 << CBT__SHFT)

enum init_command_idx
{
    INIT_CK_IDX,
    INIT_CKE_IDX,
    INIT_MR1_IDX,
    INIT_MR2_IDX,
    INIT_MR11_IDX,
    INIT_MR12_IDX,
    INIT_MR14_IDX,
    INIT_TOTAL_IDX,
};

enum cbt_command_idx
{
    CBT_CK_IDX,
    CBT_CKE_IDX,
    CBT_MR13_IDX,
    CBT_CBTC_IDX,
    CBT_TOTAL_IDX
};

void dram_init(dram_dev_t *dram)
{
    dram->mr13 = 0;
    dram->mr2 = 0;
}

void dram_power_down(__UNUSED__ dram_dev_t *dram,
                     dfi_buffer_dev_t *dfi_buffer,
                     dfi_command_cfg_t *cfg)
{
    command_t command = {0};
    command.command_type = COMMAND_TYPE_CK;

     // Initialize TX Packet Buffer
    dfi_tx_packet_buffer_init(&dfi_buffer->tx_packet_buffer, &dfi_buffer->tx_packets[0], PACKET_BUFFER_DEPTH);

    // Get DRAM into Power Down state
    // Force CK to toggle for at least 10 cycles without CKE
    dfi_tx_packet_buffer_fill(&command, &dfi_buffer->tx_packet_buffer, cfg, 1);
    dfi_tx_packet_buffer_fill(&command, &dfi_buffer->tx_packet_buffer, cfg, 10);
    dfi_buffer_fill_and_send_packets(dfi_buffer, &dfi_buffer->tx_packet_buffer);
}

void dram_idle(__UNUSED__ dram_dev_t *dram,
               dfi_buffer_dev_t *dfi_buffer,
               dfi_command_cfg_t *cfg)
{
    command_t command[2] = {0};
    command[0].command_type = COMMAND_TYPE_CK;
    command[1].command_type = COMMAND_TYPE_CKE;

    // Initialize TX Packet Buffer
    dfi_tx_packet_buffer_init(&dfi_buffer->tx_packet_buffer, &dfi_buffer->tx_packets[0], PACKET_BUFFER_DEPTH);

    // Get DRAM into IDLE state
    dfi_tx_packet_buffer_fill(&command[0], &dfi_buffer->tx_packet_buffer, cfg, 1);
    dfi_tx_packet_buffer_fill(&command[1], &dfi_buffer->tx_packet_buffer, cfg, 30);
    dfi_buffer_fill_and_send_packets(dfi_buffer, &dfi_buffer->tx_packet_buffer);
}


void dram_frequency_init(__UNUSED__ dram_dev_t *dram,
                         dfi_buffer_dev_t *dfi_buffer,
                         dfi_command_cfg_t *cfg,
                         dram_freq_cfg_t *dram_cfg,
                         dram_freq_cal_t *dram_cal)
{
    command_t command[INIT_TOTAL_IDX] = {0};
    command[INIT_CK_IDX].command_type = COMMAND_TYPE_CK;
    command[INIT_CKE_IDX].command_type = COMMAND_TYPE_CKE;

    create_write_mode_register_command(&command[INIT_MR1_IDX], CS_0, 0x1, dram_cfg->mr1);       // MR1
    create_write_mode_register_command(&command[INIT_MR2_IDX], CS_0, 0x2, dram_cfg->mr2);       // MR2
    create_write_mode_register_command(&command[INIT_MR11_IDX], CS_0, 0xB, dram_cfg->mr11);     // MR11
    create_write_mode_register_command(&command[INIT_MR12_IDX], CS_0, 0xC, dram_cal->mr12);     // MR12
    create_write_mode_register_command(&command[INIT_MR14_IDX], CS_0, 0xE, dram_cal->mr14);     // MR14

    // Initialize TX Packet Buffer
    dfi_tx_packet_buffer_init(&dfi_buffer->tx_packet_buffer, &dfi_buffer->tx_packets[0], PACKET_BUFFER_DEPTH);

    // Issue MR0/1/11/12/13/14
    dfi_tx_packet_buffer_fill(&command[INIT_MR1_IDX], &dfi_buffer->tx_packet_buffer, cfg, 1);
    dfi_tx_packet_buffer_fill(&command[INIT_MR2_IDX], &dfi_buffer->tx_packet_buffer, cfg, 15);
    dfi_tx_packet_buffer_fill(&command[INIT_MR11_IDX], &dfi_buffer->tx_packet_buffer, cfg, 15);
    dfi_tx_packet_buffer_fill(&command[INIT_MR12_IDX], &dfi_buffer->tx_packet_buffer, cfg, 15);
    dfi_tx_packet_buffer_fill(&command[INIT_MR14_IDX], &dfi_buffer->tx_packet_buffer, cfg, 15);
    dfi_tx_packet_buffer_fill(&command[INIT_CKE_IDX], &dfi_buffer->tx_packet_buffer, cfg, 1);
    dfi_buffer_fill_and_send_packets(dfi_buffer, &dfi_buffer->tx_packet_buffer);
}

void dram_cbt_enter(dram_dev_t *dram,
                    dfi_buffer_dev_t *dfi_buffer,
                    dfi_command_cfg_t *cfg)
{
    command_t command[CBT_TOTAL_IDX] = {0};
    command[CBT_CK_IDX].command_type = COMMAND_TYPE_CK;
    command[CBT_CKE_IDX].command_type = COMMAND_TYPE_CKE;

    dram->mr13 |= CBT__MSK;
    create_write_mode_register_command(&command[CBT_MR13_IDX], CS_0, 0xD, dram->mr13);

    dfi_tx_packet_buffer_init(&dfi_buffer->tx_packet_buffer, &dfi_buffer->tx_packets[0], PACKET_BUFFER_DEPTH);
    dfi_tx_packet_buffer_fill(&command[CBT_MR13_IDX], &dfi_buffer->tx_packet_buffer, cfg, 15);
    dfi_tx_packet_buffer_fill(&command[CBT_CKE_IDX], &dfi_buffer->tx_packet_buffer, cfg, 1);
    dfi_tx_packet_buffer_fill(&command[CBT_CK_IDX], &dfi_buffer->tx_packet_buffer, cfg, 15);
    dfi_buffer_fill_and_send_packets(dfi_buffer, &dfi_buffer->tx_packet_buffer);
}

void dram_cbt_exit(dram_dev_t *dram,
                   dfi_buffer_dev_t *dfi_buffer,
                   dfi_command_cfg_t *cfg)
{
    command_t command[CBT_TOTAL_IDX] = {0};
    command[CBT_CK_IDX].command_type = COMMAND_TYPE_CK;
    command[CBT_CKE_IDX].command_type = COMMAND_TYPE_CKE;

    dram->mr13 &= ~CBT__MSK;
    create_write_mode_register_command(&command[CBT_MR13_IDX], CS_0, 0xD, dram->mr13);

    dfi_tx_packet_buffer_init(&dfi_buffer->tx_packet_buffer, &dfi_buffer->tx_packets[0], PACKET_BUFFER_DEPTH);
    dfi_tx_packet_buffer_fill(&command[CBT_CK_IDX], &dfi_buffer->tx_packet_buffer, cfg, 15);
    dfi_tx_packet_buffer_fill(&command[CBT_CKE_IDX], &dfi_buffer->tx_packet_buffer, cfg, 1);
    dfi_tx_packet_buffer_fill(&command[CBT_MR13_IDX], &dfi_buffer->tx_packet_buffer, cfg, 15);
    dfi_tx_packet_buffer_fill(&command[CBT_CKE_IDX], &dfi_buffer->tx_packet_buffer, cfg, 1);
    dfi_buffer_fill_and_send_packets(dfi_buffer, &dfi_buffer->tx_packet_buffer);
}

void dram_vrcg_enable(dram_dev_t *dram,
                      dfi_buffer_dev_t *dfi_buffer,
                      dfi_command_cfg_t *cfg)
{
    dram->mr13 |= VRCG__MSK;
    dram_write_mode_register_13(dram, dfi_buffer, cfg, dram->mr13);
}

void dram_vrcg_disable(dram_dev_t *dram,
                       dfi_buffer_dev_t *dfi_buffer,
                       dfi_command_cfg_t *cfg)
{
    dram->mr13 &= ~VRCG__MSK;
    dram_write_mode_register_13(dram, dfi_buffer, cfg, dram->mr13);
}

static void dram_write_mode_register(__UNUSED__ dram_dev_t *dram,
                                 dfi_buffer_dev_t *dfi_buffer,
                                 dfi_command_cfg_t *cfg,
                                 uint8_t mr,
                                 uint8_t op)
{
    command_t command[2] = {0};
    command[0].command_type = COMMAND_TYPE_CKE;

    create_write_mode_register_command(&command[1], CS_0, mr, op);

    dfi_tx_packet_buffer_init(&dfi_buffer->tx_packet_buffer, &dfi_buffer->tx_packets[0], PACKET_BUFFER_DEPTH);
    dfi_tx_packet_buffer_fill(&command[1], &dfi_buffer->tx_packet_buffer, cfg, 1);
    dfi_tx_packet_buffer_fill(&command[0], &dfi_buffer->tx_packet_buffer, cfg, 1);
    dfi_buffer_fill_and_send_packets(dfi_buffer, &dfi_buffer->tx_packet_buffer);
}

void dram_write_mode_register_13(dram_dev_t *dram,
                                 dfi_buffer_dev_t *dfi_buffer,
                                 dfi_command_cfg_t *cfg,
                                 uint8_t mr13)
{
    dram->mr13 = mr13;
    return dram_write_mode_register(dram, dfi_buffer, cfg, 0xD, dram->mr13);
}

void dram_write_mode_register_2(dram_dev_t *dram,
                                dfi_buffer_dev_t *dfi_buffer,
                                dfi_command_cfg_t *cfg,
                                uint8_t mr2)
{
    dram->mr2 = mr2;
    return dram_write_mode_register(dram, dfi_buffer, cfg, 0x2, dram->mr2);
}


void dram_get_read_enable_offset_timing(dram_freq_cfg_t *dram_cfg,
                                        uint8_t *offset)
{
    *offset = (dram_cfg->phy_rd_en + CA_LAST);
}

void dram_get_write_enable_offset_timing(dram_freq_cfg_t *dram_cfg,
                                         uint8_t *offset)
{
    *offset = (dram_cfg->phy_wr_lat + CA_LAST);
}

void dram_get_write_data_offset_timing(dram_freq_cfg_t *dram_cfg,
                                       uint8_t *offset)
{
    *offset = (dram_cfg->phy_wr_lat + dram_cfg->phy_wr_en + CA_LAST);
}

void dfi_command_config_init(dram_freq_cfg_t *dram_cfg,
                             dfi_command_cfg_t *cfg,
                             burst_length_t bl)
{
    cfg->ratio = dram_cfg->ratio;

    uint8_t burst_length = bl == BL_16 ? 16 : 32;

    // READ
    cfg->group_offsets[COMMAND_TYPE_READ][PACKET_GROUP_RDDATA_EN] = (dram_cfg->phy_rd_en + CA_LAST);
    cfg->group_lengths[COMMAND_TYPE_READ][PACKET_GROUP_RDDATA_EN] = burst_length;
    cfg->group_offsets[COMMAND_TYPE_READ][PACKET_GROUP_RDDATA_CS] = (dram_cfg->phy_rd_en + CA_LAST);
    cfg->group_lengths[COMMAND_TYPE_READ][PACKET_GROUP_RDDATA_CS] = burst_length;
    cfg->group_lengths[COMMAND_TYPE_READ][PACKET_GROUP_COMMAND_CKE] = burst_length + dram_cfg->phy_rd_en + CA_LAST;
    cfg->group_lengths[COMMAND_TYPE_READ][PACKET_GROUP_COMMAND_DCE] = burst_length + dram_cfg->phy_rd_en + CA_LAST;

    // WRITE
    cfg->group_offsets[COMMAND_TYPE_WRITE][PACKET_GROUP_WRDATA_EN] = (dram_cfg->phy_wr_lat + CA_LAST);
    cfg->group_lengths[COMMAND_TYPE_WRITE][PACKET_GROUP_WRDATA_EN] = burst_length + 4; // Why??
    cfg->group_offsets[COMMAND_TYPE_WRITE][PACKET_GROUP_WRDATA] = (dram_cfg->phy_wr_lat + dram_cfg->phy_wr_en + CA_LAST);
    cfg->group_lengths[COMMAND_TYPE_WRITE][PACKET_GROUP_WRDATA] = burst_length;
    cfg->group_lengths[COMMAND_TYPE_WRITE][PACKET_GROUP_COMMAND_CKE] = burst_length + dram_cfg->phy_wr_lat +
                                                                       dram_cfg->phy_wr_en + CA_LAST;
    cfg->group_lengths[COMMAND_TYPE_WRITE][PACKET_GROUP_COMMAND_DCE] = burst_length + dram_cfg->phy_wr_lat +
                                                                       dram_cfg->phy_wr_en + CA_LAST;

    // CBT
    // TODO: Need to update these magic numbers. Timing is wrong here anyway.
    // NOTE: Length is function of phases so need to double length based on ratio
    cfg->group_offsets[COMMAND_TYPE_CBT][PACKET_GROUP_WRDATA_EN] = dram_cfg->t_sh_train;
    cfg->group_lengths[COMMAND_TYPE_CBT][PACKET_GROUP_WRDATA_EN] = 2 * cfg->ratio; // Pulses of DQS
    cfg->group_offsets[COMMAND_TYPE_CBT][PACKET_GROUP_WRDATA] = dram_cfg->phy_wr_en - 2; // subtract Preamble
    cfg->group_lengths[COMMAND_TYPE_CBT][PACKET_GROUP_WRDATA] = (2 * dram_cfg->t_sh_train + 4) * cfg->ratio; // Pulses of DQ
    // Now CA / CS is after for training purposes
    cfg->group_offsets[COMMAND_TYPE_CBT][PACKET_GROUP_COMMAND] = dram_cfg->phy_wr_en - 2 + dram_cfg->t_vref_ca_long +
                                                                 (2 * dram_cfg->t_sh_train + 4) * cfg->ratio;
    cfg->group_offsets[COMMAND_TYPE_CBT][PACKET_GROUP_COMMAND_CS] = dram_cfg->phy_wr_en - 2 + dram_cfg->t_vref_ca_long +
                                                                    (2 * dram_cfg->t_sh_train + 4) * cfg->ratio;
    cfg->group_lengths[COMMAND_TYPE_CBT][PACKET_GROUP_COMMAND] = 4;
    cfg->group_lengths[COMMAND_TYPE_CBT][PACKET_GROUP_COMMAND_CS] = 4;
    cfg->group_lengths[COMMAND_TYPE_CBT][PACKET_GROUP_COMMAND_DCE] = dram_cfg->phy_wr_en - 2 +
                                                                     (2 * dram_cfg->t_sh_train + 4) * cfg->ratio +
                                                                     dram_cfg->t_vref_ca_long + 4;
}
