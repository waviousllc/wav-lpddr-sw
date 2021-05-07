/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _WDDR_MEMORY_MAP_H_
#define _WDDR_MEMORY_MAP_H_

#include <board/board.h>
#include "ddr_ca_csr.h"
#include "ddr_cmn_csr.h"
#include "ddr_ctrl_csr.h"
#include "ddr_dfi_csr.h"
#include "ddr_dfich_csr.h"
#include "ddr_dq_csr.h"
#include "ddr_mvp_pll_csr.h"
#include "ddr_fsw_csr.h"

// Common MCU registers (same for all Wavious MCUs)
#define WDDR_MEMORY_MAP_MCU_INTF        (MEMORY_MAP_WAV_MCU_INTF)
#define WDDR_MEMORY_MAP_MCU             (MEMORY_MAP_WAV_MCU)
#define WDDR_MEMORY_MAP_MCU_ITCM        (MEMORY_MAP_WAV_MCU_ITCM)
#define WDDR_MEMORY_MAP_MCU_DTCM        (MEMORY_MAP_WAV_MCU_DTCM)

// DDR PHY Specific
// Mask used for MSR Switching (PHY offsets only)
#define WDDR_MEMORY_MAP_REL_MASK        (0x0000FFFF)

// Common Analog / Common Clock Analog
#define WDDR_MEMORY_MAP_CMN             (0x00090000)

// MVP PLL
#define WDDR_MEMORY_MAP_PLL             (0x00098000)

// CTRL / Debug / Test
#define WDDR_MEMORY_MAP_FSW             (0x000A0000)
#define WDDR_MEMORY_MAP_CTRL            (0x000B0000)

// DFI
#define WDDR_MEMORY_MAP_DFI             (0x000C0000)
#define WDDR_MEMORY_MAP_DFI_CH0         (0x000D0000)

// PHY Channel 0
#define WDDR_MEMORY_MAP_CH0_DQ0         (0x000F0000)
#define WDDR_MEMORY_MAP_CH0_DQ1         (0x00100000)
#define WDDR_MEMORY_MAP_CH0_CA          (0x00110000)

// PHY Channel 1
#define WDDR_MEMORY_MAP_CH1_DQ0         (0x00120000)
#define WDDR_MEMORY_MAP_CH1_DQ1         (0x00130000)
#define WDDR_MEMORY_MAP_CH1_CA          (0x00140000)

// RESERVED
#define WDDR_MEMORY_MAP_PHY_SLV_RSVD    (0x00150000)

// Relative Offsets
#define WDDR_MEMORY_MAP_PHY_CH_START    (WDDR_MEMORY_MAP_CH0_DQ0)
#define WDDR_MEMORY_MAP_PHY_CH_OFFSET   (WDDR_MEMORY_MAP_CH1_DQ0 - WDDR_MEMORY_MAP_PHY_CH_START)
#define WDDR_MEMORY_MAP_PHY_DQ_OFFSET   (WDDR_MEMORY_MAP_CH0_DQ1 - WDDR_MEMORY_MAP_PHY_CH_START)
#define WDDR_MEMORY_MAP_PHY_CA_OFFSET   (WDDR_MEMORY_MAP_CH0_CA  - WDDR_MEMORY_MAP_PHY_CH_START)

#endif /* _WDDR_MEMORY_MAP_H_ */
