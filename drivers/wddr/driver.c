/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <wddr/driver.h>
#include <wddr/memory_map.h>
#include <kernel/io.h>

/** @brief  Common Function to write TOP CFG CSR for all channels */
static void wddr_set_channel_top_cfg_reg_if(wddr_dev_t *wddr,
                                            wddr_channel_t channel,
                                            uint32_t ca_reg_val,
                                            uint32_t dq_reg_val);

void wddr_init_reg_if(wddr_dev_t *wddr, uint32_t base)
{
    wddr->base = base;
}

void wddr_clear_fifo_reg_if(wddr_dev_t *wddr, wddr_channel_t channel)
{
    uint32_t ca_reg_val, dq_reg_val;

    // CA
    ca_reg_val = reg_read(wddr->base +
                       WDDR_MEMORY_MAP_PHY_CH_START +
                       WDDR_MEMORY_MAP_PHY_CH_OFFSET * channel +
                       WDDR_MEMORY_MAP_PHY_CA_OFFSET +
                       DDR_CA_TOP_CFG__ADR);

    // DQ
    dq_reg_val = reg_read(wddr->base +
                       WDDR_MEMORY_MAP_PHY_CH_START +
                       WDDR_MEMORY_MAP_PHY_CH_OFFSET * channel +
                       DDR_DQ_TOP_CFG__ADR);

    ca_reg_val = UPDATE_REG_FIELD(ca_reg_val, DDR_CA_TOP_CFG_FIFO_CLR, 0x1);
    dq_reg_val = UPDATE_REG_FIELD(dq_reg_val, DDR_DQ_TOP_CFG_FIFO_CLR, 0x1);
    wddr_set_channel_top_cfg_reg_if(wddr, channel, ca_reg_val, dq_reg_val);

    ca_reg_val = UPDATE_REG_FIELD(ca_reg_val, DDR_CA_TOP_CFG_FIFO_CLR, 0x0);
    dq_reg_val = UPDATE_REG_FIELD(dq_reg_val, DDR_DQ_TOP_CFG_FIFO_CLR, 0x0);
    wddr_set_channel_top_cfg_reg_if(wddr, channel, ca_reg_val, dq_reg_val);
}

void wddr_set_dram_resetn_pin_reg_if(wddr_dev_t *wddr, bool override, bool high)
{
    uint32_t reg_val = UPDATE_REG_FIELD(0x0, DDR_CMN_RSTN_CFG_RSTN_OVR_VAL, high);
    reg_write(wddr->base + WDDR_MEMORY_MAP_CMN + DDR_CMN_RSTN_CFG__ADR, reg_val);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CMN_RSTN_CFG_RSTN_OVR_SEL, override);
    reg_write(wddr->base + WDDR_MEMORY_MAP_CMN + DDR_CMN_RSTN_CFG__ADR, reg_val);
}

void wddr_set_chip_select_reg_if(wddr_dev_t *wddr, wddr_channel_t channel, wddr_rank_t rank, bool override)
{
    uint32_t ca_reg_val, dq_reg_val;

    // CA
    ca_reg_val = reg_read(wddr->base +
                       WDDR_MEMORY_MAP_PHY_CH_START +
                       WDDR_MEMORY_MAP_PHY_CH_OFFSET * channel +
                       WDDR_MEMORY_MAP_PHY_CA_OFFSET +
                       DDR_CA_TOP_CFG__ADR);

    // DQ
    dq_reg_val = reg_read(wddr->base +
                       WDDR_MEMORY_MAP_PHY_CH_START +
                       WDDR_MEMORY_MAP_PHY_CH_OFFSET * channel +
                       DDR_DQ_TOP_CFG__ADR);

    ca_reg_val = UPDATE_REG_FIELD(ca_reg_val, DDR_CA_TOP_CFG_WCS_SW_OVR_VAL, rank);
    ca_reg_val = UPDATE_REG_FIELD(ca_reg_val, DDR_CA_TOP_CFG_RCS_SW_OVR_VAL, rank);
    dq_reg_val = UPDATE_REG_FIELD(dq_reg_val, DDR_DQ_TOP_CFG_WCS_SW_OVR_VAL, rank);
    dq_reg_val = UPDATE_REG_FIELD(dq_reg_val, DDR_DQ_TOP_CFG_RCS_SW_OVR_VAL, rank);
    wddr_set_channel_top_cfg_reg_if(wddr, channel, ca_reg_val, dq_reg_val);

    ca_reg_val = UPDATE_REG_FIELD(ca_reg_val, DDR_CA_TOP_CFG_WCS_SW_OVR, override);
    ca_reg_val = UPDATE_REG_FIELD(ca_reg_val, DDR_CA_TOP_CFG_RCS_SW_OVR, override);
    dq_reg_val = UPDATE_REG_FIELD(dq_reg_val, DDR_DQ_TOP_CFG_WCS_SW_OVR, override);
    dq_reg_val = UPDATE_REG_FIELD(dq_reg_val, DDR_DQ_TOP_CFG_RCS_SW_OVR, override);
    wddr_set_channel_top_cfg_reg_if(wddr, channel, ca_reg_val, dq_reg_val);
}

static void wddr_set_channel_top_cfg_reg_if(wddr_dev_t *wddr,
                                            wddr_channel_t channel,
                                            uint32_t ca_reg_val,
                                            uint32_t dq_reg_val)
{
    // CA
    reg_write(wddr->base +
              WDDR_MEMORY_MAP_PHY_CH_START +
              WDDR_MEMORY_MAP_PHY_CH_OFFSET * channel +
              WDDR_MEMORY_MAP_PHY_CA_OFFSET +
              DDR_CA_TOP_CFG__ADR,
              ca_reg_val);

    // DQ0
    reg_write(wddr->base +
              WDDR_MEMORY_MAP_PHY_CH_START +
              WDDR_MEMORY_MAP_PHY_CH_OFFSET * channel +
              DDR_DQ_TOP_CFG__ADR,
              dq_reg_val);

    // DQ1
    reg_write(wddr->base +
              WDDR_MEMORY_MAP_PHY_CH_START +
              WDDR_MEMORY_MAP_PHY_CH_OFFSET * channel +
              WDDR_MEMORY_MAP_PHY_DQ_OFFSET +
              DDR_DQ_TOP_CFG__ADR,
              dq_reg_val);
}
