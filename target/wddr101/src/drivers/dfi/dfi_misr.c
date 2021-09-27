/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <kernel/io.h>
#include <dfi/driver.h>

void misr_set_enable_reg_if(dfich_reg_t *dfich_reg, misr_en_t misr_en, bool enable)
{
    uint32_t reg_val;
    uint32_t en_val;

    reg_val = dfich_reg->DDR_DFICH_MISR_0_CFG;

    if (enable)
    {
        en_val = misr_en | GET_REG_FIELD(reg_val, DDR_DFICH_MISR_0_CFG_MISR_ENABLE);
    }
    else
    {
        en_val = (~misr_en) & GET_REG_FIELD(reg_val, DDR_DFICH_MISR_0_CFG_MISR_ENABLE);
    }

    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_MISR_0_CFG_MISR_ENABLE, en_val);
    dfich_reg->DDR_DFICH_MISR_0_CFG = reg_val;
}

void misr_clear_lfsr_reg_if(dfich_reg_t *dfich_reg)
{
    uint32_t reg_val;

    reg_val = dfich_reg->DDR_DFICH_MISR_0_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_MISR_0_CFG_LFSR_CLEAR, 0x1);
    dfich_reg->DDR_DFICH_MISR_0_CFG = reg_val;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_MISR_0_CFG_LFSR_CLEAR, 0x0);
    dfich_reg->DDR_DFICH_MISR_0_CFG = reg_val;
}

void misr_load_lfsr_seed_reg_if(dfich_reg_t *dfich_reg, uint32_t low, uint32_t high)
{
    uint32_t reg_val;

    // Set LFSR SEED value
    // NOTE: HIGH value is only 4 bits
    dfich_reg->DDR_DFICH_MISR_1_CFG  = low;
    dfich_reg->DDR_DFICH_MISR_2_CFG  = high & 0xF;

    // Load SEED into MISRs
    reg_val = dfich_reg->DDR_DFICH_MISR_0_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_MISR_0_CFG_LFSR_LD_SEED, 0x1);
    dfich_reg->DDR_DFICH_MISR_0_CFG = reg_val;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_MISR_0_CFG_LFSR_LD_SEED, 0x0);
    dfich_reg->DDR_DFICH_MISR_0_CFG = reg_val;
}

void misr_read_reg_if(dfich_reg_t *dfich_reg, misr_t misr, uint32_t *low, uint32_t *high)
{

    *low =  dfich_reg->DDR_DFICH_MISR_STA[misr][0];
    *high = dfich_reg->DDR_DFICH_MISR_STA[misr][1];
}
