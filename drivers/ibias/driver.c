/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#include <ibias/driver.h>
#include <kernel/io.h>
#include <wddr/memory_map.h>

#define IBIAS_BASE__ADDR    (DDR_CMN_IBIAS_CFG__ADR)

void ibias_init_reg_if(ibias_dev_t *ibias, uint32_t base)
{
    ibias->base = base + IBIAS_BASE__ADDR;
}

wddr_return_t ibias_set_state_reg_if(ibias_dev_t *ibias, ibias_state_t state)
{
    uint32_t reg_val;
    switch(state)
    {
        case IBIAS_STATE_DISABLE:
        case IBIAS_STATE_ENABLE:
            reg_val = reg_read(ibias->base);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_IBIAS_CFG_EN, state);
            reg_write(ibias->base, reg_val);
            break;
        default:
            return WDDR_ERROR;
    }
    return WDDR_SUCCESS;
}
