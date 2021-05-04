/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <dfi/buffer_driver.h>
#include <wddr/memory_map.h>
#include <kernel/io.h>

/**
 * @brief   DFI Buffer Push Packet Into FIFO Register Interface
 *
 * @details Pushes a packet from the Loader HW into the FIFO.
 *
 * @param[in]   dfi_buffer  pointer to DFI Buffer device.
 *
 * @return      void
 */
static void dfi_buffer_push_packet_into_fifo_reg_if(dfi_buffer_dev_t *dfi_buffer);

/**
 * @brief   DFI Buffer Pops Packet From FIFO Register Interface
 *
 * @details Pops a packet from the FIFO into the loader HW.
 *
 * @param[in]   dfi_buffer  pointer to DFI Buffer device.
 *
 * @return      returns whether data was popped from EG FIFO.
 * @retval      WDDR_SUCCESS if data was popped.
 * @retval      WDDR_ERROR_DFI_PACKET_FIFO_EMPTY if no data was popped.
 * @retval      WDDR_ERROR otherwise.
 */
static wddr_return_t dfi_buffer_pop_packet_from_fifo_reg_if(dfi_buffer_dev_t *dfi_buffer);

void dfi_buffer_init_reg_if(dfi_buffer_dev_t *buffer, uint32_t base)
{
    buffer->base = base;
    // TODO: Completion variable setup / initialization
}

void dfi_buffer_enable_clock_reg_if(dfi_buffer_dev_t *dfi_buffer)
{
    uint32_t reg_val;

    reg_val = reg_read(dfi_buffer->base + DDR_DFICH_TOP_1_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_BUF_CLK_EN, 0x1);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_TS_RESET, 0x1);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_RDATA_CLR, 0x1);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_WDATA_CLR, 0x1);
    reg_write(dfi_buffer->base + DDR_DFICH_TOP_1_CFG__ADR, reg_val);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_TS_RESET, 0x0);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_RDATA_CLR, 0x0);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_WDATA_CLR, 0x0);
    reg_write(dfi_buffer->base + DDR_DFICH_TOP_1_CFG__ADR, reg_val);
}

void dfi_buffer_set_mode_reg_if(dfi_buffer_dev_t *dfi_buffer, bool enable)
{
    uint32_t reg_val;

    reg_val = reg_read(dfi_buffer->base + DDR_DFICH_TOP_1_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_BUF_MODE, enable);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_TS_ENABLE, enable);
    reg_write(dfi_buffer->base + DDR_DFICH_TOP_1_CFG__ADR, reg_val);
}

void dfi_buffer_set_wdata_hold_reg_if(dfi_buffer_dev_t *dfi_buffer, bool enable)
{
    uint32_t reg_val;

    reg_val = reg_read(dfi_buffer->base + DDR_DFICH_TOP_1_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_WDATA_HOLD, enable);
    reg_write(dfi_buffer->base + DDR_DFICH_TOP_1_CFG__ADR, reg_val);
}

void dfi_buffer_send_packets_reg_if(dfi_buffer_dev_t *dfi_buffer)
{
    uint32_t reg_val;

    // Send packets
    dfi_buffer_set_mode_reg_if(dfi_buffer, true);

    // Wait for IG FIFO to empty (all packets sent)
    // TODO: Replace with completion variable instead.
    do
    {
        reg_val = reg_read(dfi_buffer->base + DDR_DFICH_TOP_STA__ADR);
    } while (GET_REG_FIELD(reg_val, DDR_DFICH_TOP_STA_IG_STATE) != DFI_FIFO_STATE_EMPTY);

    // Disable Timestamp comparison logic
    reg_val = reg_read(dfi_buffer->base + DDR_DFICH_TOP_1_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_TS_ENABLE, 0x0);
    reg_write(dfi_buffer->base + DDR_DFICH_TOP_1_CFG__ADR, reg_val);
}

static void dfi_buffer_push_packet_into_fifo_reg_if(dfi_buffer_dev_t *dfi_buffer)
{
    uint32_t reg_val;
    uint8_t upd_bit;
    reg_val = reg_read(dfi_buffer->base + DDR_DFICH_TOP_1_CFG__ADR);
    upd_bit = GET_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_WDATA_UPDATE) ^ 0x1;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_WDATA_UPDATE, upd_bit);
    reg_write(dfi_buffer->base + DDR_DFICH_TOP_1_CFG__ADR, reg_val);
}

wddr_return_t dfi_buffer_write_ig_fifo_reg_if(dfi_buffer_dev_t *dfi_buffer,
                                              uint32_t data[DFI_IG_FIFO_LOAD_NUM])
{
    uint32_t reg_val;
    uint8_t en_bit;

    reg_val = reg_read(dfi_buffer->base + DDR_DFICH_TOP_STA__ADR);

    // Check if packet fits before loading
    if (GET_REG_FIELD(reg_val, DDR_DFICH_TOP_STA_IG_STATE) == DFI_FIFO_STATE_FULL)
    {
        return WDDR_ERROR_DFI_PACKET_FIFO_FULL;
    }

    // Write entire packet and push
    for (uint8_t ii = DFI_IG_FIFO_LOAD_NUM; ii > 0; ii--)
    {
        reg_write(dfi_buffer->base + DDR_DFICH_IG_DATA_CFG__ADR, data[ii - 1]);
        reg_val = reg_read(dfi_buffer->base + DDR_DFICH_TOP_1_CFG__ADR);
        en_bit = GET_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_WDATA_ENABLE) ^ 0x1;
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_WDATA_ENABLE, en_bit);
        reg_write(dfi_buffer->base + DDR_DFICH_TOP_1_CFG__ADR, reg_val);
    }

    dfi_buffer_push_packet_into_fifo_reg_if(dfi_buffer);
    return WDDR_SUCCESS;
}

wddr_return_t dfi_buffer_read_eg_fifo_reg_if(dfi_buffer_dev_t *dfi_buffer,
                                             uint32_t data[DFI_EG_FIFO_LOAD_NUM])
{
    uint32_t reg_val;
    uint8_t en_bit;

    // Pop a packet
    PROPAGATE_ERROR(dfi_buffer_pop_packet_from_fifo_reg_if(dfi_buffer));

    // Write into data buffer
    for (uint8_t ii = 0; ii < DFI_EG_FIFO_LOAD_NUM; ii++)
    {
        data[ii] = reg_read(dfi_buffer->base + DDR_DFICH_EG_DATA_STA__ADR);
        reg_val = reg_read(dfi_buffer->base + DDR_DFICH_TOP_1_CFG__ADR);
        en_bit = GET_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_RDATA_ENABLE) ^ 0x1;
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_RDATA_ENABLE, en_bit);
        reg_write(dfi_buffer->base + DDR_DFICH_TOP_1_CFG__ADR, reg_val);
    }

    return WDDR_SUCCESS;
}

static wddr_return_t dfi_buffer_pop_packet_from_fifo_reg_if(dfi_buffer_dev_t *dfi_buffer)
{
    uint32_t reg_val;
    uint8_t upd_bit;

    reg_val = reg_read(dfi_buffer->base + DDR_DFICH_TOP_STA__ADR);
    if (GET_REG_FIELD(reg_val, DDR_DFICH_TOP_STA_EG_STATE) == DFI_FIFO_STATE_EMPTY)
    {
        return WDDR_ERROR_DFI_PACKET_FIFO_EMPTY;
    }

    reg_val = reg_read(dfi_buffer->base + DDR_DFICH_TOP_1_CFG__ADR);
    upd_bit = GET_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_RDATA_UPDATE) ^ 0x1;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_RDATA_UPDATE, upd_bit);
    reg_write(dfi_buffer->base + DDR_DFICH_TOP_1_CFG__ADR, reg_val);

    return WDDR_SUCCESS;
}
