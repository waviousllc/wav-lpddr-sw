/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <gearbox/tx_driver.h>
#include <wddr/memory_map.h>
#include <kernel/io.h>

// DQS
#define TX_GB_DQS_BASE__ADDR    (DDR_DQ_DQS_TX_M0_CFG__ADR)
#define TX_GB_DQS_MODE__OFFSET  (DDR_DQ_DQS_TX_M1_CFG__ADR - DDR_DQ_DQS_TX_M0_CFG__ADR)
// CK
#define TX_GB_CK_BASE__ADDR     (DDR_CA_DQS_TX_M0_CFG__ADR)
#define TX_GB_CK_MODE__OFFSET   (DDR_CA_DQS_TX_M1_CFG__ADR - DDR_CA_DQS_TX_M0_CFG__ADR)

void tx_gb_set_msr_reg_if(tx_gearbox_dev_t *gb,
                          wddr_msr_t msr,
                          wddr_slice_type_t slice_type)
{
    uint32_t base = 0, mode_offset;
    switch(slice_type)
    {
        case WDDR_SLICE_TYPE_DQS:
            base = TX_GB_DQS_BASE__ADDR;
            mode_offset = TX_GB_DQS_MODE__OFFSET;
            break;
        case WDDR_SLICE_TYPE_CK:
            base = TX_GB_CK_BASE__ADDR;
            mode_offset = TX_GB_CK_MODE__OFFSET;
            break;
        case WDDR_SLICE_TYPE_DQ:
        case WDDR_SLICE_TYPE_CA:
            return;
    }

    gb->base = (gb->base & ~WDDR_MEMORY_MAP_REL_MASK) + base + msr * mode_offset;

}

void tx_gb_init_reg_if(tx_gearbox_dev_t *gb,
                       uint32_t base,
                       wddr_slice_type_t slice_type)
{
    gb->base = base;
    tx_gb_set_msr_reg_if(gb, WDDR_MSR_0, slice_type);
}

void tx_gb_set_mode_reg_if(tx_gearbox_dev_t *gb,
                           wddr_slice_type_t slice_type,
                           tx_gb_cfg_t *cfg)
{
    uint32_t reg_val;
    switch (slice_type)
    {
        case WDDR_SLICE_TYPE_DQS:
            reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQS_TX_M0_CFG_TGB_MODE, cfg->data_mode);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_M0_CFG_WGB_MODE, cfg->write_mode);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_M0_CFG_CK2WCK_RATIO, cfg->ck2wck_ratio);
            break;
        case WDDR_SLICE_TYPE_CK:
            reg_val = UPDATE_REG_FIELD(0x0, DDR_CA_DQS_TX_M0_CFG_TGB_MODE, cfg->data_mode);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_M0_CFG_WGB_MODE, cfg->write_mode);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_M0_CFG_CK2WCK_RATIO, cfg->ck2wck_ratio);
            break;
        default:
            return;
    }

    reg_write(gb->base, reg_val);
}
