/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#include <zqcal/driver.h>
#include <wddr/memory_map.h>
#include <kernel/io.h>

void zqcal_init_reg_if(zqcal_dev_t *zqcal, uint32_t base)
{
    zqcal->base = base;
}

wddr_return_t zqcal_set_state_reg_if(zqcal_dev_t *zqcal, zqcal_state_t state)
{
    uint32_t reg_val;

    reg_val = reg_read(zqcal->base + DDR_CMN_ZQCAL_CFG__ADR);
    switch(state)
    {
        case ZQCAL_STATE_DISABLED:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_ZQCAL_CFG_CAL_EN, 0x0);
            break;
        case ZQCAL_STATE_ENABLED:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_ZQCAL_CFG_CAL_EN, 0x1);
            break;
        default:
            return WDDR_ERROR;
    }

    reg_write(zqcal->base + DDR_CMN_ZQCAL_CFG__ADR , reg_val);
    return WDDR_SUCCESS;
}

wddr_return_t zqcal_set_mode_reg_if(zqcal_dev_t *zqcal, zqcal_mode_t mode)
{
    uint32_t reg_val;

    reg_val = reg_read(zqcal->base + DDR_CMN_ZQCAL_CFG__ADR);
    switch(mode)
    {
        case ZQCAL_MODE_PULL_UP:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_ZQCAL_CFG_PD_SEL, 0x0);
            break;
        case ZQCAL_MODE_PULL_DOWN:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_ZQCAL_CFG_PD_SEL, 0x1);
            break;
        default:
            return WDDR_ERROR;
    }

    reg_write(zqcal->base + DDR_CMN_ZQCAL_CFG__ADR , reg_val);
    return WDDR_SUCCESS;
}

wddr_return_t zqcal_set_code_reg_if(zqcal_dev_t *zqcal, zqcal_mode_t mode, uint8_t code)
{
    uint32_t reg_val;

    reg_val = reg_read(zqcal->base + DDR_CMN_ZQCAL_CFG__ADR);
    switch(mode)
    {
        case ZQCAL_MODE_PULL_UP:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_ZQCAL_CFG_PCAL, code);
            break;
        case ZQCAL_MODE_PULL_DOWN:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_ZQCAL_CFG_NCAL, code);
            break;
        default:
            return WDDR_ERROR;
    }

    reg_write(zqcal->base + DDR_CMN_ZQCAL_CFG__ADR , reg_val);
    return WDDR_SUCCESS;
}

wddr_return_t zqcal_set_voh_reg_if(zqcal_dev_t *zqcal, zqcal_voh_t voh)
{
    uint32_t reg_val;

    reg_val = reg_read(zqcal->base + DDR_CMN_ZQCAL_CFG__ADR);
    switch(voh)
    {
        case ZQCAL_VOH_0P5:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_ZQCAL_CFG_VOL_0P6_SEL, 0x0);
            break;
        case ZQCAL_VOH_0P6:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_ZQCAL_CFG_VOL_0P6_SEL, 0x1);
            break;
        default:
            return WDDR_ERROR;
    }

    reg_write(zqcal->base + DDR_CMN_ZQCAL_CFG__ADR , reg_val);
    return WDDR_SUCCESS;
}

void zqcal_get_output_reg_if(zqcal_dev_t *zqcal, uint8_t *val)
{
    *val = GET_REG_FIELD(reg_read(zqcal->base + DDR_CMN_ZQCAL_STA__ADR), DDR_CMN_ZQCAL_STA_COMP);
}
