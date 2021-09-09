/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <ctrl/driver.h>
#include <kernel/io.h>

void ctrl_clk_set_pll_clk_en_reg_if(ctrl_reg_t *ctrl_reg, bool enable)
{
    uint32_t reg_val = ctrl_reg->DDR_CTRL_CLK_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CTRL_CLK_CFG_PLL_CLK_EN, 0x1);
    ctrl_reg->DDR_CTRL_CLK_CFG = reg_val;
}

void ctrl_clk_set_mcu_gfm_sel_reg_if(ctrl_reg_t *ctrl_reg,
                                     clk_mcu_gfm_sel_t sel)
{
    uint32_t reg_val = ctrl_reg->DDR_CTRL_CLK_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CTRL_CLK_CFG_MCU_GFM_SEL, sel);
    ctrl_reg->DDR_CTRL_CLK_CFG = reg_val;
}

void ctrl_clk_get_mcu_gfm_sel_status_reg_if(ctrl_reg_t *ctrl_reg,
                                            uint8_t *gfm_sel0,
                                            uint8_t *gfm_sel1)
{
    uint32_t reg_val = ctrl_reg->DDR_CTRL_CLK_STA;
    *gfm_sel0 = GET_REG_FIELD(reg_val, DDR_CTRL_CLK_STA_MCU_GFM_SEL0);
    *gfm_sel1 = GET_REG_FIELD(reg_val, DDR_CTRL_CLK_STA_MCU_GFM_SEL1);
}

void ctrl_clk_get_dfi_clk_status_reg_if(ctrl_reg_t *ctrl_reg,
                                        uint8_t *dfi_clk_on)
{
    uint32_t reg_val = ctrl_reg->DDR_CTRL_CLK_STA;
    *dfi_clk_on = GET_REG_FIELD(reg_val, DDR_CTRL_CLK_STA_DFI_CLK_ON);
}
