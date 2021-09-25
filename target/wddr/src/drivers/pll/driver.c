/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <pll/driver.h>
#include <kernel/io.h>
#include <wddr/memory_map.h>

void pll_init_reg_if(pll_dev_t *pll, uint32_t base)
{
    pll->base = base;
}

void pll_reset_reg_if(pll_dev_t *pll)
{
    uint32_t reg_val;
    reg_val = reg_read(pll->base + DDR_MVP_PLL_CORE_OVERRIDES__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_OVERRIDES_CORE_RESET_MUX, 0x1);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_OVERRIDES_CORE_RESET, 0x0);
    reg_write(pll->base + DDR_MVP_PLL_CORE_OVERRIDES__ADR, reg_val);
}

void pll_set_vco_sel_reg_if(pll_dev_t *pll, vco_index_t vco_id)
{
    uint32_t reg_val;
    reg_val = reg_read(pll->base + DDR_MVP_PLL_CORE_OVERRIDES__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_OVERRIDES_CORE_VCO_SEL_MUX, 0x1);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_OVERRIDES_CORE_VCO_SEL, vco_id);
    reg_write(pll->base + DDR_MVP_PLL_CORE_OVERRIDES__ADR, reg_val);
}

void pll_switch_vco_reg_if(pll_dev_t *pll)
{
    uint32_t reg_val;
    // Switch VCO
    reg_val = UPDATE_REG_FIELD(0, DDR_MVP_PLL_CORE_SWTICH_VCO_CORE_SWITCH_VCO, 0x1);
    reg_write(pll->base + DDR_MVP_PLL_CORE_SWTICH_VCO__ADR, reg_val);
}
