/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <misr/driver.h>
#include <wddr/memory_map.h>
#include <kernel/io.h>

#define MISR_READ_START__ADDR   DDR_DFICH_DQ0_W3_TO_W0_MISR_LO_STA__ADR
#define MISR_READ_OFFSET        DDR_DFICH_DQ0_W7_TO_W4_MISR_LO_STA__ADR - \
                                DDR_DFICH_DQ0_W3_TO_W0_MISR_LO_STA__ADR

void misr_update_enable_reg_if(misr_en_t misr_en, bool enable)
{
    uint32_t reg_val;
    uint32_t en_val;

    reg_val = reg_read(WDDR_MEMORY_MAP_DFI_CH0 + DDR_DFICH_MISR_0_CFG__ADR);

    if (enable)
    {
        en_val = misr_en | GET_REG_FIELD(reg_val, DDR_DFICH_MISR_0_CFG_MISR_ENABLE);
    }
    else
    {
        en_val = (~misr_en) & GET_REG_FIELD(reg_val, DDR_DFICH_MISR_0_CFG_MISR_ENABLE);
    }

    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_MISR_0_CFG_MISR_ENABLE, en_val);
    reg_write(WDDR_MEMORY_MAP_DFI_CH0 + DDR_DFICH_MISR_0_CFG__ADR, reg_val);
}

void misr_clear_lfsr_reg_if(void)
{
    uint32_t reg_val;

    reg_val = reg_read(WDDR_MEMORY_MAP_DFI_CH0 + DDR_DFICH_MISR_0_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_MISR_0_CFG_LFSR_CLEAR, 0x1);
    reg_write(WDDR_MEMORY_MAP_DFI_CH0 + DDR_DFICH_MISR_0_CFG__ADR, reg_val);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_MISR_0_CFG_LFSR_CLEAR, 0x0);
    reg_write(WDDR_MEMORY_MAP_DFI_CH0 + DDR_DFICH_MISR_0_CFG__ADR, reg_val);
}

void misr_load_lfsr_seed_reg_if(uint32_t low, uint8_t high)
{
    uint32_t reg_val;

    // Set LFSR SEED value
    // NOTE: HIGH value is only 4 bits
    reg_write(WDDR_MEMORY_MAP_DFI_CH0 + DDR_DFICH_MISR_1_CFG__ADR, low);
    reg_write(WDDR_MEMORY_MAP_DFI_CH0 + DDR_DFICH_MISR_2_CFG__ADR, high & 0xF);

    // Load SEED into MISRs
    reg_val = reg_read(WDDR_MEMORY_MAP_DFI_CH0 + DDR_DFICH_MISR_0_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_MISR_0_CFG_LFSR_LD_SEED, 0x1);
    reg_write(WDDR_MEMORY_MAP_DFI_CH0 + DDR_DFICH_MISR_0_CFG__ADR, reg_val);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_MISR_0_CFG_LFSR_LD_SEED, 0x0);
    reg_write(WDDR_MEMORY_MAP_DFI_CH0 + DDR_DFICH_MISR_0_CFG__ADR, reg_val);
}

void misr_read_reg_if(misr_t misr, uint32_t *low, uint8_t *high)
{
    uint32_t lo_reg_addr = WDDR_MEMORY_MAP_DFI_CH0 + MISR_READ_START__ADDR +
                           MISR_READ_OFFSET * misr;

    *low = reg_read(lo_reg_addr);
    // HI reg address is always LO + 4
    // NOTE: HI value is 8 bits
    *high = reg_read(lo_reg_addr + 4) & 0xFF;
}
