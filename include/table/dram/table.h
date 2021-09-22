/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DRAM_TABLE_H_
#define _DRAM_TABLE_H_

#include <stdint.h>
#include <wddr/phy_defs.h>

/**
 * @brief   DRAM Configuration Structure
 *
 * @details Per-Frequency DRAM configuration data.
 *
 * ratio            DFI to DRAM Clock ratio.
 * t_vref_ca_long   CA VREF LONG in cycles.
 * txsr             Self-Refresh Exit timing.
 * mr1              Mode Register 1 (Toggle Preamble).
 * mr2              Mode Register 2 (Read / Write Latency).
 * mr11             Mode Register 11 (CA / DQ ODT).
 * mr12             Mode Register 12 (CA VREF).
 * mr14             Mode Register 14 (DQ VREF).
 * phy_wr_lat       Write Enable Offset (DFI).
 * phy_wr_en        Write Data Offset (DFI).
 * phy_rd_en        Read Enable Offset (DFI).
 * t_sh_train       Setup / Hold cycles for CBT.
 */
typedef struct dram_freq_cfg_t
{
    const wddr_freq_ratio_t ratio;
    const uint16_t t_vref_ca_long;
    const uint16_t txsr;
    const uint8_t mr1;
    uint8_t mr2;
    const uint8_t mr11;
    uint8_t mr12;
    uint8_t mr14;
    const uint8_t phy_wr_lat;
    const uint8_t phy_wr_en;
    const uint8_t phy_rd_en;
    const uint8_t t_sh_train;
} dram_freq_cfg_t;

#endif /* _DRAM_TABLE_H_ */
