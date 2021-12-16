/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <dfi/driver.h>
#include <kernel/io.h>
#include <wddr/irq_map.h>
#include <wddr/memory_map.h>

void dfi_fifo_enable_ca_rdata_loopback_reg_if(dfich_reg_t *dfich_reg, bool enable)
{
    uint32_t reg_val = dfich_reg->DDR_DFICH_TOP_1_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_CA_RDDATA_EN, enable);
    dfich_reg->DDR_DFICH_TOP_1_CFG = reg_val;
}

void dfi_fifo_rdout_en_ovr_reg_if(dfich_reg_t *dfich_reg, bool ovr_sel, bool ovr)
{
    uint32_t reg_val = dfich_reg->DDR_DFICH_TOP_1_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_RDOUT_EN_OVR_SEL, ovr_sel);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_RDOUT_EN_OVR, ovr);
    dfich_reg->DDR_DFICH_TOP_1_CFG = reg_val;
}

void dfi_fifo_enable_clock_reg_if(dfich_reg_t *dfich_reg)
{
    uint32_t reg_val = dfich_reg->DDR_DFICH_TOP_1_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_BUF_CLK_EN, 0x1);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_TS_RESET, 0x1);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_RDATA_CLR, 0x1);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_WDATA_CLR, 0x1);
    dfich_reg->DDR_DFICH_TOP_1_CFG = reg_val;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_TS_RESET, 0x0);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_RDATA_CLR, 0x0);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_WDATA_CLR, 0x0);
    dfich_reg->DDR_DFICH_TOP_1_CFG = reg_val;
}

void dfi_fifo_set_mode_reg_if(dfich_reg_t *dfich_reg, bool enable)
{
    uint32_t reg_val = dfich_reg->DDR_DFICH_TOP_1_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_BUF_MODE, enable);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_TS_ENABLE, enable);
    dfich_reg->DDR_DFICH_TOP_1_CFG = reg_val;
}

void dfi_fifo_set_wdata_hold_reg_if(dfich_reg_t *dfich_reg, bool enable)
{
    uint32_t reg_val = dfich_reg->DDR_DFICH_TOP_1_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_WDATA_HOLD, enable);
    dfich_reg->DDR_DFICH_TOP_1_CFG = reg_val;
}

void dfi_fifo_send_packets_reg_if(dfich_reg_t *dfich_reg)
{
    uint32_t reg_val;

    // Reset TS
    reg_val = dfich_reg->DDR_DFICH_TOP_1_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_TS_RESET, 0x1);
    dfich_reg->DDR_DFICH_TOP_1_CFG = reg_val;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_TS_RESET, 0x0);
    dfich_reg->DDR_DFICH_TOP_1_CFG = reg_val;

    // Send packets
    dfi_fifo_set_mode_reg_if(dfich_reg, true);

    do
    {
        reg_val = dfich_reg->DDR_DFICH_TOP_STA;
    } while (GET_REG_FIELD(reg_val, DDR_DFICH_TOP_STA_IG_STATE) != DFI_FIFO_STATE_EMPTY);

    // Need to clear interrupt to ensure it doesn't fire early when using blocking method
    /** NOTE: Only CH0 is enabled */
    reg_val = FAST_IRQ_STICKY_MASK(DDR_IRQ_CH0_IBUF_EMPTY) |
              FAST_IRQ_STICKY_MASK(DDR_IRQ_CH0_IBUF_FULL);
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_CLR_CFG__ADR, reg_val);
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_CLR_CFG__ADR, 0x0);

    // Disable Timestamp comparison logic
    reg_val = dfich_reg->DDR_DFICH_TOP_1_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_TS_ENABLE, 0x0);
    dfich_reg->DDR_DFICH_TOP_1_CFG = reg_val;
}

dfi_return_t dfi_fifo_write_ig_reg_if(dfich_reg_t *dfich_reg,
                                      const uint32_t data[DFI_IG_FIFO_LOAD_NUM])
{
    uint32_t reg_val;
    uint8_t en_bit, upd_bit;

    reg_val = dfich_reg->DDR_DFICH_TOP_STA;

    // Check if packet fits before loading
    if (GET_REG_FIELD(reg_val, DDR_DFICH_TOP_STA_IG_STATE) == DFI_FIFO_STATE_FULL)
    {
        return DFI_ERROR_FIFO_FULL;
    }

    // Get WDATA ENABLE status
    reg_val = dfich_reg->DDR_DFICH_TOP_1_CFG;

    // Write entire packet and push
    for (uint8_t ii = DFI_IG_FIFO_LOAD_NUM; ii > 0; ii--)
    {
        dfich_reg->DDR_DFICH_IG_DATA_CFG = data[ii - 1];
        en_bit = GET_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_WDATA_ENABLE) ^ 0x1;
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_WDATA_ENABLE, en_bit);
        dfich_reg->DDR_DFICH_TOP_1_CFG = reg_val;
    }

    // Push packet into the FIFO
    upd_bit = GET_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_WDATA_UPDATE) ^ 0x1;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_WDATA_UPDATE, upd_bit);
    dfich_reg->DDR_DFICH_TOP_1_CFG = reg_val;

    return DFI_SUCCESS;
}

dfi_return_t dfi_fifo_read_eg_reg_if(dfich_reg_t *dfich_reg,
                                     uint32_t data[DFI_EG_FIFO_LOAD_NUM])
{
    uint32_t reg_val;
    uint8_t en_bit, upd_bit;

    reg_val = dfich_reg->DDR_DFICH_TOP_STA;

    // FIFO can't be empty
    if (GET_REG_FIELD(reg_val, DDR_DFICH_TOP_STA_EG_STATE) == DFI_FIFO_STATE_EMPTY)
    {
        return DFI_ERROR_FIFO_EMPTY;
    }

    // Pop a packet
    reg_val = dfich_reg->DDR_DFICH_TOP_1_CFG;
    upd_bit = GET_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_RDATA_UPDATE) ^ 0x1;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_RDATA_UPDATE, upd_bit);
    dfich_reg->DDR_DFICH_TOP_1_CFG = reg_val;

    // Write into data buffer
    for (uint8_t ii = 0; ii < DFI_EG_FIFO_LOAD_NUM; ii++)
    {
        data[ii] = dfich_reg->DDR_DFICH_EG_DATA_STA;
        en_bit = GET_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_RDATA_ENABLE) ^ 0x1;
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_RDATA_ENABLE, en_bit);
        dfich_reg->DDR_DFICH_TOP_1_CFG = reg_val;
    }

    return DFI_SUCCESS;
}
