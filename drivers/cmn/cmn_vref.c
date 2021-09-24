/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <cmn/driver.h>
#include <kernel/io.h>

void cmn_vref_set_state_reg_if(cmn_reg_t *cmn_reg,
                               wddr_msr_t msr,
                               vref_state_t state)
{
    uint32_t reg_val = cmn_reg->DDR_CMN_VREF_CFG[msr];
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
            return;
    }
    cmn_reg->DDR_CMN_VREF_CFG[msr] = reg_val;
}

void cmn_vref_set_code_reg_if(cmn_reg_t *cmn_reg,
                              wddr_msr_t msr,
                              uint32_t code)
{
    uint32_t reg_val = cmn_reg->DDR_CMN_VREF_CFG[msr];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_VREF_M0_CFG_CTRL, code);
    cmn_reg->DDR_CMN_VREF_CFG[msr] = reg_val;
}

void cmn_vref_set_pwr_mode_reg_if(cmn_reg_t *cmn_reg,
                                  wddr_msr_t msr,
                                  vref_pwr_mode_t pwr_mode)
{
    uint32_t reg_val = cmn_reg->DDR_CMN_VREF_CFG[msr];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_VREF_M0_CFG_PWR, pwr_mode);
    cmn_reg->DDR_CMN_VREF_CFG[msr] = reg_val;
}
