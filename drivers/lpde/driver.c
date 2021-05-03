/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdbool.h>
#include <error.h>
#include <lpde/device.h>
#include <wddr/memory_map.h>
#include <kernel/io.h>

wddr_return_t lpde_set_state_reg_if(lpde_dev_t *lpde, lpde_state_t state)
{
    uint32_t reg_val;
    reg_val = reg_read(lpde->base);
    switch(state)
    {
        case LPDE_STATE_DISABLED:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_LPDE_M0_R0_CFG_0_EN, 0x0);
            break;
        case LPDE_STATE_ENABLED:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_LPDE_M0_R0_CFG_0_EN, 0x1);
            break;
        default:
            return WDDR_ERROR;
    }

    reg_write(lpde->base, reg_val);
    return WDDR_SUCCESS;
}

void lpde_set_delay_reg_if(lpde_dev_t *lpde, uint8_t delay)
{
    uint32_t reg_val;
    reg_val = reg_read(lpde->base);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_LPDE_M0_R0_CFG_0_CTRL_BIN, delay);
    reg_write(lpde->base, reg_val);
}

void lpde_set_gear_reg_if(lpde_dev_t *lpde, uint8_t gear)
{
    uint32_t reg_val;
    reg_val = reg_read(lpde->base);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_LPDE_M0_R0_CFG_0_GEAR, gear);
    reg_write(lpde->base, reg_val);
}

void lpde_set_reg_if(lpde_dev_t *lpde, uint8_t gear, uint8_t delay, bool enable)
{
    uint32_t reg_val = 0;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_LPDE_M0_R0_CFG_0_GEAR, gear);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_LPDE_M0_R0_CFG_0_CTRL_BIN, delay);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_LPDE_M0_R0_CFG_0_EN, enable);
    reg_write(lpde->base, reg_val);
}
