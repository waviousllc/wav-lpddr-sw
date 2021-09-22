/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <cmn/driver.h>
#include <kernel/io.h>

void cmn_ibias_set_state_reg_if(cmn_reg_t *cmn_reg, ibias_state_t state)
{
    uint32_t reg_val = cmn_reg->DDR_CMN_IBIAS_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_IBIAS_CFG_EN, state);
    cmn_reg->DDR_CMN_IBIAS_CFG = reg_val;
}
