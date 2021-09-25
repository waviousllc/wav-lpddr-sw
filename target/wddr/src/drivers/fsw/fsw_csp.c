/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <fsw/driver.h>
#include <kernel/io.h>

void fsw_csp_set_clk_disable_over_val_reg_if(fsw_reg_t *fsw_reg,
                                            bool enable)
{
    uint32_t reg_val = fsw_reg->DDR_FSW_CSP_1_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CSP_1_CFG_CLK_DISABLE_OVR_VAL, enable);
    fsw_reg->DDR_FSW_CSP_1_CFG = reg_val;
}

void fsw_csp_sync_reg_if(fsw_reg_t *fsw_reg)
{
    uint32_t csp_status;

    // Sync Request
    uint32_t reg_val = fsw_reg->DDR_FSW_CSP_1_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CSP_1_CFG_REQ_OVR, 0x1);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CSP_1_CFG_REQ_OVR_VAL, 0x1);
    fsw_reg->DDR_FSW_CSP_1_CFG = reg_val;
    do
    {
        csp_status = fsw_reg->DDR_FSW_CSP_STA;
    } while (GET_REG_FIELD(csp_status, DDR_FSW_CSP_STA_REQ_COMPLETE) != 1);

    // Turn off Override
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CSP_1_CFG_REQ_OVR_VAL, 0x0);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CSP_1_CFG_REQ_COMPLETE_STA_CLR, 0x1);
    fsw_reg->DDR_FSW_CSP_1_CFG = reg_val;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CSP_1_CFG_REQ_OVR, 0x0);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CSP_1_CFG_REQ_COMPLETE_STA_CLR, 0x0);
    fsw_reg->DDR_FSW_CSP_1_CFG = reg_val;
}
