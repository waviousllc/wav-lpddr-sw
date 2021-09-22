/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <cmn/driver.h>
#include <kernel/io.h>

void cmn_pmon_set_state_reg_if(cmn_reg_t *cmn_reg, pmon_state_t state)
{
    uint32_t reg_val;

    reg_val = UPDATE_REG_FIELD(0x0, DDR_CMN_PMON_ANA_CFG_NAND_EN, state);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_PMON_ANA_CFG_NOR_EN,  state);
    cmn_reg->DDR_CMN_PMON_ANA_CFG = reg_val;

    if (state == PMON_STATE_ENABLED)
    {
        // reset digital values of PMONs
        reg_val = cmn_reg->DDR_CMN_PMON_DIG_CFG;
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_PMON_DIG_CFG_REFCLK_RST, 0x1);
        cmn_reg->DDR_CMN_PMON_DIG_CFG = reg_val;
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_PMON_DIG_CFG_REFCLK_RST, 0x0);
        cmn_reg->DDR_CMN_PMON_DIG_CFG = reg_val;
    }

    reg_val = cmn_reg->DDR_CMN_PMON_DIG_NAND_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val,  DDR_CMN_PMON_DIG_NAND_CFG_COUNT_EN, state);
    cmn_reg->DDR_CMN_PMON_DIG_NAND_CFG = reg_val;

    reg_val = cmn_reg->DDR_CMN_PMON_DIG_NOR_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val,  DDR_CMN_PMON_DIG_NOR_CFG_COUNT_EN, state);
    cmn_reg->DDR_CMN_PMON_DIG_NOR_CFG = reg_val;
}

void cmn_pmon_configure_reg_if(cmn_reg_t *cmn_reg,
                               uint16_t refcount,
                               uint8_t init_wait)
{
    uint32_t reg_val = cmn_reg->DDR_CMN_PMON_DIG_NAND_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_PMON_DIG_NAND_CFG_REFCOUNT, refcount);
    cmn_reg->DDR_CMN_PMON_DIG_NAND_CFG = reg_val;

    reg_val = cmn_reg->DDR_CMN_PMON_DIG_NOR_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_PMON_DIG_NOR_CFG_REFCOUNT, refcount);
    cmn_reg->DDR_CMN_PMON_DIG_NOR_CFG = reg_val;

    reg_val = cmn_reg->DDR_CMN_PMON_DIG_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_PMON_DIG_CFG_INITWAIT, init_wait);
    cmn_reg->DDR_CMN_PMON_DIG_CFG = reg_val;
}

static void __cmn_pmon_get_status_count_reg_if(volatile uint32_t *status_addr,
                                               volatile uint32_t *cfg_addr,
                                               uint32_t *count)
{
    uint32_t reg_val;
    uint32_t refcount;
    uint8_t done = 0;

    do
    {
        reg_val = *status_addr;
        done = GET_REG_FIELD(reg_val, DDR_CMN_PMON_NAND_STA_DONE);
    } while (done == 0x0);
    *count = GET_REG_FIELD(reg_val, DDR_CMN_PMON_NAND_STA_COUNT);

    reg_val = *cfg_addr;
    refcount = GET_REG_FIELD(reg_val, DDR_CMN_PMON_DIG_NAND_CFG_REFCOUNT);
    *count = *count / refcount;
}

void cmn_pmon_get_status_count_reg_if(cmn_reg_t *cmn_reg, uint32_t *count)
{
    uint32_t nand_count;
    uint32_t nor_count;
    __cmn_pmon_get_status_count_reg_if(&cmn_reg->DDR_CMN_PMON_NAND_STA, &cmn_reg->DDR_CMN_PMON_DIG_NAND_CFG, &nand_count);
    __cmn_pmon_get_status_count_reg_if(&cmn_reg->DDR_CMN_PMON_NOR_STA, &cmn_reg->DDR_CMN_PMON_DIG_NOR_CFG, &nor_count);
    *count = (nand_count >> 1) + (nor_count >> 1);
}
