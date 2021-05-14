/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <fsw/driver.h>
#include <wddr/memory_map.h>
#include <kernel/io.h>

void fsw_ctrl_set_msr_vco_ovr_val_reg_if(uint8_t msr, uint8_t vco_id)
{
    uint32_t reg_val;

    reg_val = reg_read(WDDR_MEMORY_MAP_FSW + DDR_FSW_CTRL_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CTRL_CFG_MSR_OVR_VAL, msr);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CTRL_CFG_VCO_SEL_OVR_VAL, vco_id);
    reg_write(WDDR_MEMORY_MAP_FSW + DDR_FSW_CTRL_CFG__ADR, reg_val);
}

void fsw_ctrl_set_msr_vco_ovr_reg_if(bool enable)
{
    uint32_t reg_val;

    reg_val = reg_read(WDDR_MEMORY_MAP_FSW + DDR_FSW_CTRL_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CTRL_CFG_VCO_SEL_OVR, enable);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CTRL_CFG_MSR_OVR, enable);
    reg_write(WDDR_MEMORY_MAP_FSW + DDR_FSW_CTRL_CFG__ADR, reg_val);
}

void fsw_csp_set_clk_disable_ovr_val_reg_if(bool enable)
{
    uint32_t reg_val;

    reg_val = reg_read(WDDR_MEMORY_MAP_FSW + DDR_FSW_CSP_1_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CSP_1_CFG_CLK_DISABLE_OVR_VAL, enable);
    reg_write(WDDR_MEMORY_MAP_FSW + DDR_FSW_CSP_1_CFG__ADR, reg_val);
}
