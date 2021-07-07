/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#include <vref/driver.h>
#include <wddr/memory_map.h>
#include <kernel/io.h>

#define VREF_BASE__ADDR     (DDR_CMN_VREF_M0_CFG__ADR)
#define VREF_MODE__OFFSET   (DDR_CMN_VREF_M1_CFG__ADR - DDR_CMN_VREF_M0_CFG__ADR)

void vref_init_reg_if(vref_dev_t *vref, uint32_t base)
{
    vref->base = base;
    vref_set_msr_reg_if(vref, WDDR_MSR_0);
}

void vref_set_msr_reg_if(vref_dev_t *vref, wddr_msr_t msr)
{
    uint32_t base = vref->base & ~WDDR_MEMORY_MAP_REL_MASK;
    vref->base = base + VREF_BASE__ADDR +  msr * VREF_MODE__OFFSET;
}

wddr_return_t vref_set_state_reg_if(vref_dev_t *vref, vref_state_t state)
{
    uint32_t reg_val;

    reg_val = reg_read(vref->base);
    switch(state)
    {
        case VREF_STATE_DISABLED:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_VREF_M0_CFG_EN, 0x0);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_VREF_M0_CFG_HIZ, 0x0);
            break;
        case VREF_STATE_ENABLED:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_VREF_M0_CFG_EN, 0x1);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_VREF_M0_CFG_HIZ, 0x0);
            break;
        case VREF_STATE_HIZ:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_VREF_M0_CFG_EN, 0x0);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_VREF_M0_CFG_HIZ, 0x1);
            break;
        default:
            return WDDR_ERROR;
    }

    reg_write(vref->base , reg_val);
    return WDDR_SUCCESS;
}

void vref_set_code_reg_if(vref_dev_t *vref, uint32_t code)
{
    uint32_t reg_val;
    reg_val = reg_read(vref->base);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_VREF_M0_CFG_CTRL, code);
    reg_write(vref->base, reg_val);
}

void vref_set_pwr_mode_reg_if(vref_dev_t *vref, vref_pwr_mode_t pwr_mode)
{
    uint32_t reg_val;
    reg_val = reg_read(vref->base);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_VREF_M0_CFG_PWR, pwr_mode);
    reg_write(vref->base, reg_val);
}
