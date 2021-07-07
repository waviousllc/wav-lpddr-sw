/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#ifndef _DRAM_TABLE_H_
#define _DRAM_TABLE_H_

#include <stdint.h>
#include <wddr/phy_defs.h>

/**
 * @brief   DRAM Calibration Structure
 *
 * @details Per-Frequency DRAM calibration data.
 *
 * mr12    Mode Register 12 (CA VREF).
 * mr14    Mode Register 14 (DQ VREF).
 */
typedef struct dram_freq_cal_t
{
    uint8_t mr12;
    uint8_t mr14;
} dram_freq_cal_t;

/**
 * @brief   DRAM Configuration Structure
 *
 * @details Per-Frequency DRAM configuration data.
 *
 * mr1              Mode Register 1 (Toggle Preamble).
 * mr2              Mode Register 2 (Read / Write Latency).
 * mr11             Mode Register 11 (CA / DQ ODT).
 * phy_wr_lat       Write Enable Offset (DFI).
 * phy_wr_en        Write Data Offset (DFI).
 * phy_rd_en        Read Enable Offset (DFI).
 * t_sh_train       Setup / Hold cycles for CBT.
 * ratio            DFI to DRAM Clock ratio.
 * t_vref_ca_long   CA VREF LONG in cycles.
 * txsr             Self-Refresh Exit timing.
 */
typedef struct dram_freq_cfg_t
{
    uint8_t mr1;
    uint8_t mr2;
    uint8_t mr11;
    uint8_t phy_wr_lat;
    uint8_t phy_wr_en;
    uint8_t phy_rd_en;
    uint8_t t_sh_train;
    wddr_freq_ratio_t ratio;
    uint16_t t_vref_ca_long;
    uint16_t txsr;
} dram_freq_cfg_t;

#endif /* _DRAM_TABLE_H_ */
