/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <cmn/driver.h>
#include <kernel/io.h>

void cmn_clk_ctrl_set_pll0_div_clk_rst_reg_if(cmn_reg_t *cmn_reg, bool reset)
{
    uint32_t reg_val = cmn_reg->DDR_CMN_CLK_CTRL_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_CLK_CTRL_CFG_PLL0_DIV_CLK_RST, reset);
    cmn_reg->DDR_CMN_CLK_CTRL_CFG = reg_val;
}

void cmn_clk_ctrl_set_gfcm_en_reg_if(cmn_reg_t *cmn_reg, bool enable)
{
    uint32_t reg_val = cmn_reg->DDR_CMN_CLK_CTRL_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_CLK_CTRL_CFG_GFCM_EN, enable);
    cmn_reg->DDR_CMN_CLK_CTRL_CFG = reg_val;
}

void cmn_clk_ctrl_set_pll0_div_clk_en_reg_if(cmn_reg_t *cmn_reg, bool enable)
{
    uint32_t reg_val = cmn_reg->DDR_CMN_CLK_CTRL_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_CLK_CTRL_CFG_PLL0_DIV_CLK_EN, enable);
    cmn_reg->DDR_CMN_CLK_CTRL_CFG = reg_val;
}
