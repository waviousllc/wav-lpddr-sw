/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <clk/driver.h>
#include <wddr/memory_map.h>
#include <kernel/io.h>

void clk_ctrl_set_pll_clk_en_reg_if(bool enable)
{
    uint32_t reg_val;

    reg_val = reg_read(WDDR_MEMORY_MAP_CTRL + DDR_CTRL_CLK_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CTRL_CLK_CFG_PLL_CLK_EN, 0x1);
    reg_write(WDDR_MEMORY_MAP_CTRL + DDR_CTRL_CLK_CFG__ADR, reg_val);
}

void clk_ctrl_set_mcu_gfm_sel_reg_if(clk_mcu_gfm_sel_t sel)
{
    uint32_t reg_val;

    reg_val = reg_read(WDDR_MEMORY_MAP_CTRL + DDR_CTRL_CLK_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CTRL_CLK_CFG_MCU_GFM_SEL, sel);
    reg_write(WDDR_MEMORY_MAP_CTRL + DDR_CTRL_CLK_CFG__ADR, reg_val);
}

void clk_cmn_ctrl_set_pll0_div_clk_rst_reg_if(bool reset)
{
    uint32_t reg_val;

    reg_val = reg_read(WDDR_MEMORY_MAP_CMN + DDR_CMN_CLK_CTRL_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_CLK_CTRL_CFG_PLL0_DIV_CLK_RST, reset);
    reg_write(WDDR_MEMORY_MAP_CMN + DDR_CMN_CLK_CTRL_CFG__ADR, reg_val);
}

void clk_cmn_ctrl_set_gfcm_en_reg_if(bool enable)
{
    uint32_t reg_val;

    reg_val = reg_read(WDDR_MEMORY_MAP_CMN + DDR_CMN_CLK_CTRL_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_CLK_CTRL_CFG_GFCM_EN, enable);
    reg_write(WDDR_MEMORY_MAP_CMN + DDR_CMN_CLK_CTRL_CFG__ADR, reg_val);
}

void clk_cmn_ctrl_set_pll0_div_clk_en_reg_if(bool enable)
{
    uint32_t reg_val;

    reg_val = reg_read(WDDR_MEMORY_MAP_CMN + DDR_CMN_CLK_CTRL_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_CLK_CTRL_CFG_PLL0_DIV_CLK_EN, enable);
    reg_write(WDDR_MEMORY_MAP_CMN + DDR_CMN_CLK_CTRL_CFG__ADR, reg_val);
}
