/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <gearbox/rx_driver.h>
#include <wddr/memory_map.h>
#include <kernel/io.h>

// DQ
#define RX_DQ_GB_BASE__ADDR    (DDR_DQ_DQ_RX_M0_CFG__ADR)
#define RX_DQ_GB_MODE__OFFSET  (DDR_DQ_DQ_RX_M1_CFG__ADR - DDR_DQ_DQ_RX_M0_CFG__ADR)

// DQS
#define RX_DQS_GB_BASE__ADDR    (DDR_DQ_DQS_RX_M0_CFG__ADR)
#define RX_DQS_GB_MODE__OFFSET  (DDR_DQ_DQS_RX_M1_CFG__ADR - DDR_DQ_DQS_RX_M0_CFG__ADR)

// CA
#define RX_CA_GB_BASE__ADDR    (DDR_CA_DQ_RX_M0_CFG__ADR)
#define RX_CA_GB_MODE__OFFSET  (DDR_CA_DQ_RX_M1_CFG__ADR - DDR_CA_DQ_RX_M0_CFG__ADR)

// CK
#define RX_CK_GB_BASE__ADDR    (DDR_CA_DQS_RX_M0_CFG__ADR)
#define RX_CK_GB_MODE__OFFSET  (DDR_CA_DQS_RX_M1_CFG__ADR - DDR_CA_DQS_RX_M0_CFG__ADR)

void rx_gb_set_msr_reg_if(rx_gearbox_dev_t *gb,
                          wddr_msr_t msr,
                          wddr_slice_type_t slice_type)
{
    uint32_t dq_base, dq_mode_offset;
    uint32_t dqs_base, dqs_mode_offset;

    switch (slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
        case WDDR_SLICE_TYPE_DQS:
            // DQ
            dq_base = RX_DQ_GB_BASE__ADDR;
            dq_mode_offset = RX_DQ_GB_MODE__OFFSET;
            // DQS
            dqs_base = RX_DQS_GB_BASE__ADDR;
            dqs_mode_offset = RX_DQS_GB_MODE__OFFSET;
            break;
        case WDDR_SLICE_TYPE_CA:
        case WDDR_SLICE_TYPE_CK:
            // DQ
            dq_base = RX_CA_GB_BASE__ADDR;
            dq_mode_offset = RX_CA_GB_MODE__OFFSET;
            // DQS
            dqs_base = RX_CK_GB_BASE__ADDR;
            dqs_mode_offset = RX_CK_GB_MODE__OFFSET;
            break;
        default:
            return;
    }

    gb->dq_gb.base = (gb->dq_gb.base & ~WDDR_MEMORY_MAP_REL_MASK) + dq_base + msr * dq_mode_offset;
    gb->dqs_gb.base = (gb->dqs_gb.base & ~WDDR_MEMORY_MAP_REL_MASK) + dqs_base + msr * dqs_mode_offset;
}

void rx_gb_init_reg_if(rx_gearbox_dev_t *gb,
                       uint32_t base,
                       wddr_slice_type_t slice_type)
{
    gb->dq_gb.base = base;
    gb->dqs_gb.base = base;
    rx_gb_set_msr_reg_if(gb, WDDR_MSR_0, slice_type);
}

void rx_gb_set_mode_reg_if(rx_gearbox_dev_t *gb,
                           wddr_slice_type_t slice_type,
                           rx_gb_cfg_t *cfg)
{
    uint32_t reg_val;

    switch (slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            // DQ
            reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQ_RX_M0_CFG_RGB_MODE, cfg->data_mode);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_RX_M0_CFG_FGB_MODE, cfg->fifo_mode);
            reg_write(gb->dq_gb.base, reg_val);
            break;
        case WDDR_SLICE_TYPE_DQS:
            // DQS
            reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQS_RX_M0_CFG_RGB_MODE, cfg->data_mode);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_M0_CFG_FGB_MODE, cfg->fifo_mode);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_M0_CFG_WCK_MODE, cfg->wck_mode);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_M0_CFG_PRE_FILTER_SEL, cfg->pre_filter_sel);
            reg_write(gb->dqs_gb.base, reg_val);
            break;
        case WDDR_SLICE_TYPE_CA:
            // CA
            reg_val = UPDATE_REG_FIELD(0x0, DDR_CA_DQ_RX_M0_CFG_RGB_MODE, cfg->data_mode);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_RX_M0_CFG_FGB_MODE, cfg->fifo_mode);
            reg_write(gb->dq_gb.base, reg_val);
            break;
        case WDDR_SLICE_TYPE_CK:
            // CK
            reg_val = UPDATE_REG_FIELD(0x0, DDR_CA_DQS_RX_M0_CFG_RGB_MODE, cfg->data_mode);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_RX_M0_CFG_FGB_MODE, cfg->fifo_mode);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_RX_M0_CFG_WCK_MODE, cfg->wck_mode);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_RX_M0_CFG_PRE_FILTER_SEL, cfg->pre_filter_sel);
            reg_write(gb->dqs_gb.base, reg_val);
            break;
    }
}
