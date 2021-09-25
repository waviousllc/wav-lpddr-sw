/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <dq/driver.h>
#include <kernel/io.h>

void dq_top_set_chip_select_reg_if(dq_reg_t *dq_reg,
                                   wddr_rank_t rank_sel,
                                   bool override)
{
    uint32_t reg_val = dq_reg->DDR_DQ_TOP_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_TOP_CFG_WCS_SW_OVR_VAL, rank_sel);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_TOP_CFG_RCS_SW_OVR_VAL, rank_sel);
    dq_reg->DDR_DQ_TOP_CFG = reg_val;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_TOP_CFG_WCS_SW_OVR, override);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_TOP_CFG_RCS_SW_OVR, override);
    dq_reg->DDR_DQ_TOP_CFG = reg_val;
}

void dq_top_clear_fifo_reg_if(dq_reg_t *dq_reg)
{
    uint32_t reg_val = dq_reg->DDR_DQ_TOP_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_TOP_CFG_FIFO_CLR, 0x1);
    dq_reg->DDR_DQ_TOP_CFG = reg_val;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_TOP_CFG_FIFO_CLR, 0x0);
    dq_reg->DDR_DQ_TOP_CFG = reg_val;
}

void dq_top_get_chip_select_status_reg_if(dq_reg_t *dq_reg,
                                          uint8_t *wcs,
                                          uint8_t *rcs)
{
    uint32_t reg_val = dq_reg->DDR_DQ_TOP_STA;
    *wcs = GET_REG_FIELD(reg_val, DDR_DQ_TOP_STA_WCS);
    *rcs = GET_REG_FIELD(reg_val, DDR_DQ_TOP_STA_RCS);
}
