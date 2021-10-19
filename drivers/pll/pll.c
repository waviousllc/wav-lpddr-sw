/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <pll/driver.h>
#include <kernel/io.h>
#include <wddr/memory_map.h>

void pll_reset_reg_if(pll_reg_t *pll_reg)
{
    uint32_t reg_val;
    reg_val = pll_reg->DDR_MVP_PLL_CORE_OVERRIDES;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_OVERRIDES_CORE_RESET_MUX, 0x1);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_OVERRIDES_CORE_RESET, 0x0);
    pll_reg->DDR_MVP_PLL_CORE_OVERRIDES = reg_val;
}

void pll_set_vco_sel_reg_if(pll_reg_t *pll_reg, uint8_t vco_id)
{
    uint32_t reg_val;
    reg_val = pll_reg->DDR_MVP_PLL_CORE_OVERRIDES;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_OVERRIDES_CORE_VCO_SEL_MUX, 0x1);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_OVERRIDES_CORE_VCO_SEL, vco_id);
    pll_reg->DDR_MVP_PLL_CORE_OVERRIDES = reg_val;
}

void pll_switch_vco_reg_if(pll_reg_t *pll_reg)
{
    uint32_t reg_val;
    // Switch VCO
    reg_val = UPDATE_REG_FIELD(0, DDR_MVP_PLL_CORE_SWTICH_VCO_CORE_SWITCH_VCO, 0x1);
    pll_reg->DDR_MVP_PLL_CORE_SWTICH_VCO = reg_val;
}

void pll_enable_loss_lock_interrupt_reg_if(pll_reg_t *pll_reg, bool enable)
{
    uint32_t reg_val;
    reg_val = pll_reg->DDR_MVP_PLL_CORE_STATUS_INT_EN;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_STATUS_INT_EN_LOSS_OF_LOCK_INT_EN, enable);
    pll_reg->DDR_MVP_PLL_CORE_STATUS_INT_EN = reg_val;
}

void pll_enable_lock_interrupt_reg_if(pll_reg_t *pll_reg, bool enable)
{
    uint32_t reg_val;
    reg_val = pll_reg->DDR_MVP_PLL_CORE_STATUS_INT_EN;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_STATUS_INT_EN_CORE_LOCKED_INT_EN, enable);
    pll_reg->DDR_MVP_PLL_CORE_STATUS_INT_EN = reg_val;
}

void pll_enable_switch_interrupt_reg_if(pll_reg_t *pll_reg, bool enable)
{
    uint32_t reg_val;
    reg_val = pll_reg->DDR_MVP_PLL_CORE_STATUS_INT_EN;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_STATUS_INT_EN_INITIAL_SWITCH_DONE_INT_EN, enable);
    pll_reg->DDR_MVP_PLL_CORE_STATUS_INT_EN = reg_val;
}

uint32_t pll_clear_interrupt_reg_if(pll_reg_t *pll_reg)
{
    uint32_t reg_val;
    reg_val = pll_reg->DDR_MVP_PLL_CORE_STATUS_INT;
    pll_reg->DDR_MVP_PLL_CORE_STATUS_INT = reg_val;
    return reg_val;
}

void pll_wait_until_core_ready_reg_if(pll_reg_t *pll_reg)
{
    uint32_t reg_val;

    do
    {
        reg_val = pll_reg->DDR_MVP_PLL_CORE_STATUS;
    } while (!GET_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_STATUS_CORE_READY));
}
