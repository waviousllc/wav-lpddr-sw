/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#include <string.h>
#include <compiler.h>
#include <dram/device.h>

#define CA_LAST         (3)
#define VRCG__SHFT      (3)
#define VRCG__MSK       (1 << VRCG__SHFT)
#define CBT__SHFT       (0)
#define CBT__MSK        (1 << CBT__SHFT)

/** @brief  Internal Function for updating DRAM Mode Register */
static void dram_write_mode_register(__UNUSED__ dram_dev_t *dram,
                                     dfi_buffer_dev_t *dfi_buffer,
                                     dram_freq_cfg_t *cfg,
                                     uint8_t mr,
                                     uint8_t op);

void dram_init(dram_dev_t *dram)
{
    dram->mr13 = 0;
    dram->mr2 = 0;
}

void dram_power_down(__UNUSED__ dram_dev_t *dram, dfi_buffer_dev_t *dfi_buffer)
{
    dfi_tx_packet_buffer_t packet_buffer;

    dfi_tx_packet_buffer_init(&packet_buffer);
    create_ck_packet_sequence(&packet_buffer, 1);
    create_ck_packet_sequence(&packet_buffer, 10);
    dfi_buffer_fill_and_send_packets(dfi_buffer, &packet_buffer.list);
    dfi_tx_packet_buffer_free(&packet_buffer);
}

void dram_idle(__UNUSED__ dram_dev_t *dram, dfi_buffer_dev_t *dfi_buffer)
{
    dfi_tx_packet_buffer_t packet_buffer;
    dfi_tx_packet_buffer_init(&packet_buffer);
    create_ck_packet_sequence(&packet_buffer, 1);
    create_cke_packet_sequence(&packet_buffer, 30);
    dfi_buffer_fill_and_send_packets(dfi_buffer, &packet_buffer.list);
    dfi_tx_packet_buffer_free(&packet_buffer);
}

void dram_frequency_init(__UNUSED__ dram_dev_t *dram,
                         dfi_buffer_dev_t *dfi_buffer,
                         dram_freq_cfg_t *dram_cfg,
                         dram_freq_cal_t *dram_cal)
{
    dfi_tx_packet_buffer_t packet_buffer;

    // Initialize TX Packet Buffer
    dfi_tx_packet_buffer_init(&packet_buffer);
    create_mrw_packet_sequence(&packet_buffer, dram_cfg->ratio, CS_0, 0x1, dram_cfg->mr1, 1);
    create_mrw_packet_sequence(&packet_buffer, dram_cfg->ratio, CS_0, 0x2, dram_cfg->mr2, 15);
    create_mrw_packet_sequence(&packet_buffer, dram_cfg->ratio, CS_0, 0xB, dram_cfg->mr11, 15);
    create_mrw_packet_sequence(&packet_buffer, dram_cfg->ratio, CS_0, 0xC, dram_cal->mr12, 15);
    create_mrw_packet_sequence(&packet_buffer, dram_cfg->ratio, CS_0, 0xE, dram_cal->mr14, 15);
    create_cke_packet_sequence(&packet_buffer, 1);
    dfi_buffer_fill_and_send_packets(dfi_buffer, &packet_buffer.list);
    dfi_tx_packet_buffer_free(&packet_buffer);
}

void dram_cbt_enter(dram_dev_t *dram,
                    dfi_buffer_dev_t *dfi_buffer,
                    dram_freq_cfg_t *cfg)
{
    dfi_tx_packet_buffer_t packet_buffer;

    dram->mr13 |= CBT__MSK;

    dfi_tx_packet_buffer_init(&packet_buffer);
    create_mrw_packet_sequence(&packet_buffer, cfg->ratio, CS_0, 0xD, dram->mr13, 15);
    create_cke_packet_sequence(&packet_buffer, 1);
    create_ck_packet_sequence(&packet_buffer, 15);
    dfi_buffer_fill_and_send_packets(dfi_buffer, &packet_buffer.list);
    dfi_tx_packet_buffer_free(&packet_buffer);
}

void dram_cbt_exit(dram_dev_t *dram,
                   dfi_buffer_dev_t *dfi_buffer,
                   dram_freq_cfg_t *cfg)
{
    dfi_tx_packet_buffer_t packet_buffer;

    dram->mr13 &= ~CBT__MSK;

    dfi_tx_packet_buffer_init(&packet_buffer);
    create_ck_packet_sequence(&packet_buffer, 15);
    create_cke_packet_sequence(&packet_buffer, 1);
    create_mrw_packet_sequence(&packet_buffer, cfg->ratio, CS_0, 0xD, dram->mr13, 15);
    create_cke_packet_sequence(&packet_buffer, 1);
    dfi_buffer_fill_and_send_packets(dfi_buffer, &packet_buffer.list);
    dfi_tx_packet_buffer_free(&packet_buffer);
}

void dram_vrcg_enable(dram_dev_t *dram,
                      dfi_buffer_dev_t *dfi_buffer,
                      dram_freq_cfg_t *cfg)
{
    dram->mr13 |= VRCG__MSK;
    dram_write_mode_register_13(dram, dfi_buffer, cfg, dram->mr13);
}

void dram_vrcg_disable(dram_dev_t *dram,
                       dfi_buffer_dev_t *dfi_buffer,
                       dram_freq_cfg_t *cfg)
{
    dram->mr13 &= ~VRCG__MSK;
    dram_write_mode_register_13(dram, dfi_buffer, cfg, dram->mr13);
}

void dram_write_mode_register_13(dram_dev_t *dram,
                                 dfi_buffer_dev_t *dfi_buffer,
                                 dram_freq_cfg_t *cfg,
                                 uint8_t mr13)
{
    dram->mr13 = mr13;
    return dram_write_mode_register(dram, dfi_buffer, cfg, 0xD, dram->mr13);
}

void dram_write_mode_register_2(dram_dev_t *dram,
                                dfi_buffer_dev_t *dfi_buffer,
                                dram_freq_cfg_t *cfg,
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

static void dram_write_mode_register(__UNUSED__ dram_dev_t *dram,
                                 dfi_buffer_dev_t *dfi_buffer,
                                 dram_freq_cfg_t *cfg,
                                 uint8_t mr,
                                 uint8_t op)
{
    dfi_tx_packet_buffer_t packet_buffer;

    dfi_tx_packet_buffer_init(&packet_buffer);
    create_mrw_packet_sequence(&packet_buffer, cfg->ratio, CS_0, mr, op, 1);
    create_cke_packet_sequence(&packet_buffer, 1);
    dfi_buffer_fill_and_send_packets(dfi_buffer, &packet_buffer.list);
    dfi_tx_packet_buffer_free(&packet_buffer);
}
