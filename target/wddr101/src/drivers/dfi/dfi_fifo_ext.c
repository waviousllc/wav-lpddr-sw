/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <dfi/driver.h>
#include <kernel/io.h>
#include <wddr/memory_map.h>

void dfi_fifo_set_ptr_reg_if(dfich_reg_t *dfich_reg,
                             uint8_t start_ptr,
                             uint8_t stop_ptr)
{
    uint32_t reg_val;
    reg_val = dfich_reg->DDR_DFICH_TOP_2_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_2_CFG_IG_START_PTR, start_ptr);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_2_CFG_IG_STOP_PTR, stop_ptr);
    dfich_reg->DDR_DFICH_TOP_2_CFG = reg_val;
}

void dfi_fifo_load_ptr_reg_if(dfich_reg_t *dfich_reg)
{
    uint32_t reg_val;
    reg_val = dfich_reg->DDR_DFICH_TOP_2_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_2_CFG_IG_LOAD_PTR, 0x1);
    dfich_reg->DDR_DFICH_TOP_2_CFG = reg_val;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_2_CFG_IG_LOAD_PTR, 0x0);
    dfich_reg->DDR_DFICH_TOP_2_CFG = reg_val;
}

void dfi_fifo_set_loop_mode_reg_if(dfich_reg_t *dfich_reg,
                                   bool enable,
                                   uint8_t num_loops)
{
    uint32_t reg_val;
    reg_val = dfich_reg->DDR_DFICH_TOP_2_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_2_CFG_IG_LOOP_MODE, enable);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_2_CFG_IG_NUM_LOOPS, num_loops);
    dfich_reg->DDR_DFICH_TOP_2_CFG = reg_val;
}
