/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <pmon/driver.h>
#include <wddr/memory_map.h>

/**
 * @breif   Process Monitor Get Status Count Commong Register Interface
 *
 * @details Common function for getting the NAND / NOR statu of Process Monitor
 *          via CSR.
 *
 * @param[in]   pmon            pointer to Processs Monitor device.
 * @param[in]   status_addr     address of status CSR.
 * @param[in]   cfg_addrr       address of config CSR.
 * @param[out]  count   pointer to store current status count.
 *
 * @return      void
 */
static void pmon_get_status_count_common_reg_if(pmon_dev_t *pmon,
                                                uint32_t status_addr,
                                                uint32_t cfg_addr,
                                                uint32_t *count);

void pmon_init_reg_if(pmon_dev_t *pmon, uint32_t base)
{
    pmon->base = base;
}

wddr_return_t pmon_set_state_reg_if(pmon_dev_t *pmon, pmon_state_t state)
{
    uint32_t reg_val;

    if ((state != PMON_STATE_ENABLED) && (state != PMON_STATE_DISABLED))
    {
        return WDDR_ERROR;
    }

    reg_val = UPDATE_REG_FIELD(0x0, DDR_CMN_PMON_ANA_CFG_NAND_EN, state);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_PMON_ANA_CFG_NOR_EN,  state);
    reg_write(pmon->base + DDR_CMN_PMON_ANA_CFG__ADR, reg_val);

    if (state == PMON_STATE_ENABLED)
    {
        // reset digital values of PMONs
        reg_val = reg_read(pmon->base + DDR_CMN_PMON_DIG_CFG__ADR);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_PMON_DIG_CFG_REFCLK_RST, 0x1);
        reg_write(pmon->base + DDR_CMN_PMON_DIG_CFG__ADR, reg_val);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_PMON_DIG_CFG_REFCLK_RST, 0x0);
        reg_write(pmon->base + DDR_CMN_PMON_DIG_CFG__ADR, reg_val);
    }

    reg_val = reg_read(pmon->base + DDR_CMN_PMON_DIG_NAND_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val,  DDR_CMN_PMON_DIG_NAND_CFG_COUNT_EN, state);
    reg_write(pmon->base + DDR_CMN_PMON_DIG_NAND_CFG__ADR, reg_val);

    reg_val = reg_read(pmon->base + DDR_CMN_PMON_DIG_NOR_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val,  DDR_CMN_PMON_DIG_NOR_CFG_COUNT_EN, state);
    reg_write(pmon->base + DDR_CMN_PMON_DIG_NOR_CFG__ADR, reg_val);

    return WDDR_SUCCESS;
}

void pmon_configure_reg_if(pmon_dev_t *pmon, uint16_t refcount, uint8_t init_wait)
{
    uint32_t reg_val;

    reg_val = reg_read(pmon->base + DDR_CMN_PMON_DIG_NAND_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_PMON_DIG_NAND_CFG_REFCOUNT, refcount);
    reg_write(pmon->base + DDR_CMN_PMON_DIG_NAND_CFG__ADR, reg_val);

    reg_val = reg_read(pmon->base + DDR_CMN_PMON_DIG_NOR_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_PMON_DIG_NOR_CFG_REFCOUNT, refcount);
    reg_write(pmon->base + DDR_CMN_PMON_DIG_NOR_CFG__ADR, reg_val);

    reg_val = reg_read(pmon->base + DDR_CMN_PMON_DIG_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_PMON_DIG_CFG_INITWAIT, initwait);
    reg_write(pmon->base + DDR_CMN_PMON_DIG_CFG__ADR, reg_val);
}

static void pmon_get_status_count_common_reg_if(pmon_dev_t *pmon,
                                                uint32_t status_addr,
                                                uint32_t cfg_addr,
                                                uint32_t *count)

{
    uint32_t reg_val;
    uint32_t refcount;
    uint8_t done = 0;

    do
    {
        reg_val = reg_read(pmon->base + status_addr);
        done = GET_REG_FIELD(reg_val, DDR_CMN_PMON_NAND_STA_DONE);
    } while (done == 0x0);
    *count = GET_REG_FIELD(reg_val, DDR_CMN_PMON_NAND_STA_COUNT);

    reg_val = reg_read(pmon->base + cfg_addr);
    refcount = GET_REG_FIELD(reg_val, DDR_CMN_PMON_DIG_NAND_CFG_REFCOUNT);
    *count = *count / refcount;
}

void pmon_get_status_count_reg_if(pmon_dev_t *pmon, uint32_t *count)
{
    uint32_t nand_count;
    uint32_t nor_count;
    pmon_get_status_count_common_reg_if(pmon, DDR_CMN_PMON_NAND_STA__ADR, DDR_CMN_PMON_DIG_NAND_CFG__ADR, &nand_count);
    pmon_get_status_count_common_reg_if(pmon, DDR_CMN_PMON_NOR_STA__ADR, DDR_CMN_PMON_DIG_NOR_CFG__ADR, &nor_count);
    *count = (nand_count / 2) + (nor_count / 2);
}
