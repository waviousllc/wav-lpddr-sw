
/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <fsw/driver.h>
#include <kernel/io.h>

void fsw_ctrl_set_msr_vco_ovr_val_reg_if(fsw_reg_t *fsw_reg,
                                         uint8_t msr,
                                         uint8_t vco_id)
{
    uint32_t reg_val = fsw_reg->DDR_FSW_CTRL_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CTRL_CFG_MSR_OVR_VAL, msr);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CTRL_CFG_VCO_SEL_OVR_VAL, vco_id);
    fsw_reg->DDR_FSW_CTRL_CFG = reg_val;
}

void fsw_ctrl_set_msr_vco_ovr_reg_if(fsw_reg_t *fsw_reg, bool enable)
{
    uint32_t reg_val = fsw_reg->DDR_FSW_CTRL_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CTRL_CFG_VCO_SEL_OVR, enable);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CTRL_CFG_MSR_OVR, enable);
    fsw_reg->DDR_FSW_CTRL_CFG = reg_val;
}

void fsw_ctrl_set_msr_toggle_en_reg_if(fsw_reg_t *fsw_reg, bool enable)
{
    uint32_t reg_val = fsw_reg->DDR_FSW_CTRL_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CTRL_CFG_MSR_TOGGLE_EN, enable);
    fsw_reg->DDR_FSW_CTRL_CFG = reg_val;
}

void fsw_ctrl_set_vco_toggle_en_reg_if(fsw_reg_t *fsw_reg, bool enable)
{
    uint32_t reg_val = fsw_reg->DDR_FSW_CTRL_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CTRL_CFG_VCO_TOGGLE_EN, enable);
    fsw_reg->DDR_FSW_CTRL_CFG = reg_val;
}

void fsw_ctrl_set_prep_done_reg_if(fsw_reg_t *fsw_reg, bool done)
{
    uint32_t reg_val = fsw_reg->DDR_FSW_CTRL_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CTRL_CFG_PREP_DONE, done);
    fsw_reg->DDR_FSW_CTRL_CFG = reg_val;
}

void fsw_ctrl_set_post_work_done_reg_if(fsw_reg_t *fsw_reg,
                                        bool override,
                                        bool done)
{
    uint32_t reg_val = fsw_reg->DDR_FSW_CTRL_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CTRL_CFG_PSTWORK_DONE, done);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CTRL_CFG_PSTWORK_DONE_OVR, override);
    fsw_reg->DDR_FSW_CTRL_CFG = reg_val;
}

uint8_t fsw_ctrl_get_current_msr_reg_if(fsw_reg_t *fsw_reg)
{
    // Double flip to ensure only read as one or zero
    return !!GET_REG_FIELD(fsw_reg->DDR_FSW_CTRL_STA, DDR_FSW_CTRL_STA_CMN_MSR);
}
