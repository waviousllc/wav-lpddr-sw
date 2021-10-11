/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <cmn/driver.h>
#include <kernel/io.h>

void cmn_rstn_set_pin_reg_if(cmn_reg_t *cmn_reg, bool override, bool high)
{
    uint32_t reg_val = cmn_reg->DDR_CMN_RSTN_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_RSTN_CFG_RSTN_OVR_VAL, high);
    cmn_reg->DDR_CMN_RSTN_CFG = reg_val;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_RSTN_CFG_RSTN_OVR_SEL, override);
    cmn_reg->DDR_CMN_RSTN_CFG = reg_val;
}

void cmn_rstn_get_rstn_loopback_reg_if(cmn_reg_t *cmn_reg,
                                       uint8_t *rstn_lb)
{
    *rstn_lb = GET_REG_FIELD(cmn_reg->DDR_CMN_RSTN_STA, DDR_CMN_RSTN_STA_RSTN_LPBK);
}
