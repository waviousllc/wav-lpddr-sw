/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <cmn/driver.h>
#include <kernel/io.h>

void cmn_zqcal_set_state_reg_if(cmn_reg_t *cmn_reg, zqcal_state_t state)
{
    uint32_t reg_val = cmn_reg->DDR_CMN_ZQCAL_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_ZQCAL_CFG_CAL_EN, state);
    cmn_reg->DDR_CMN_ZQCAL_CFG = reg_val;
}

void cmn_zqcal_set_mode_reg_if(cmn_reg_t *cmn_reg, zqcal_mode_t mode)
{
    uint32_t reg_val = cmn_reg->DDR_CMN_ZQCAL_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_ZQCAL_CFG_PD_SEL, mode);
    cmn_reg->DDR_CMN_ZQCAL_CFG = reg_val;
}

void cmn_zqcal_set_code_reg_if(cmn_reg_t *cmn_reg, zqcal_mode_t mode, uint8_t code)
{
    uint32_t reg_val = cmn_reg->DDR_CMN_ZQCAL_CFG;

    switch(mode)
    {
        case ZQCAL_MODE_PULL_UP:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_ZQCAL_CFG_PCAL, code);
            break;
        case ZQCAL_MODE_PULL_DOWN:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_ZQCAL_CFG_NCAL, code);
            break;
        default:
            return;
    }

    cmn_reg->DDR_CMN_ZQCAL_CFG = reg_val;
}

void cmn_zqcal_set_voh_reg_if(cmn_reg_t *cmn_reg, zqcal_voh_t voh)
{
    uint32_t reg_val = cmn_reg->DDR_CMN_ZQCAL_CFG;
    switch(voh)
    {
        case ZQCAL_VOH_0P5:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_ZQCAL_CFG_VOL_0P6_SEL, 0x0);
            break;
        case ZQCAL_VOH_0P6:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_ZQCAL_CFG_VOL_0P6_SEL, 0x1);
            break;
        default:
            return;
    }

    cmn_reg->DDR_CMN_ZQCAL_CFG = reg_val;
}

void cmn_zqcal_get_output_reg_if(cmn_reg_t *cmn_reg, uint8_t *val)
{
    *val = GET_REG_FIELD(cmn_reg->DDR_CMN_ZQCAL_STA, DDR_CMN_ZQCAL_STA_COMP);
}
