/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <dfi/buffer_driver.h>
#include <wddr/memory_map.h>
#include <wddr/irq_map.h>
#include <kernel/io.h>
#include <kernel/irq.h>
#include <kernel/completion.h>

/**
 * @brief   DFI Ingress Buffer IRQ Handler
 *
 * @details Handles coordination of tasks related to DFI Ingress Buffer IRQ.
 *
 * @param[in]   irq_num     number cooresponding to current irq being handled.
 * @param[in]   args        pointer to IRQ handler arguments.
 *
 * @return      void.
 */
static void dfi_buffer_handle_ig_irq(int irq_num, void *args);

void dfi_buffer_init_reg_if(dfi_buffer_dev_t *buffer, uint32_t base)
{
    buffer->base = base;
    vInitCompletion(&buffer->ig_empty_completion);

    // Register interrupt functions for hw buffers
    request_irq(MCU_FAST_IRQ_IBUF, dfi_buffer_handle_ig_irq, buffer);
    uint32_t mask = reg_read(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_STICKY_CFG__ADR);
    mask |= FAST_IRQ_STICKY_MASK(DDR_IRQ_CH0_IBUF_EMPTY);
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_STICKY_CFG__ADR, mask);
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

    // Clear completion flag
    vReInitCompletion(&dfi_buffer->ig_empty_completion);

    // Send packets
    dfi_buffer_set_mode_reg_if(dfi_buffer, true);

    // Enable interrupt and wait for Buffer to empty
    enable_irq(MCU_FAST_IRQ_IBUF);
    vWaitForCompletion(&dfi_buffer->ig_empty_completion);
    disable_irq(MCU_FAST_IRQ_IBUF);

    // Disable Timestamp comparison logic
    reg_val = reg_read(dfi_buffer->base + DDR_DFICH_TOP_1_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_TS_ENABLE, 0x0);
    reg_write(dfi_buffer->base + DDR_DFICH_TOP_1_CFG__ADR, reg_val);
}

void dfi_buffer_send_packets_non_blocking_reg_if(dfi_buffer_dev_t *dfi_buffer)
{
    uint32_t reg_val;
    // Send packets
    dfi_buffer_set_mode_reg_if(dfi_buffer, true);

    do
    {
        reg_val = reg_read(dfi_buffer->base + DDR_DFICH_TOP_STA__ADR);
    } while (GET_REG_FIELD(reg_val, DDR_DFICH_TOP_STA_IG_STATE) != DFI_FIFO_STATE_EMPTY);

    // Need to clear interrupt to ensure it doesn't fire early when using blocking method
    /** NOTE: Only CH0 is enabled */
    reg_val = FAST_IRQ_STICKY_MASK(DDR_IRQ_CH0_IBUF_EMPTY) |
              FAST_IRQ_STICKY_MASK(DDR_IRQ_CH0_IBUF_FULL);
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_CLR_CFG__ADR, reg_val);
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_CLR_CFG__ADR, 0x0);

    // Disable Timestamp comparison logic
    reg_val = reg_read(dfi_buffer->base + DDR_DFICH_TOP_1_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_TS_ENABLE, 0x0);
    reg_write(dfi_buffer->base + DDR_DFICH_TOP_1_CFG__ADR, reg_val);
}

wddr_return_t dfi_buffer_write_ig_fifo_reg_if(dfi_buffer_dev_t *dfi_buffer,
                                              uint32_t data[DFI_IG_FIFO_LOAD_NUM])
{
    uint32_t reg_val;
    uint8_t en_bit, upd_bit;

    reg_val = reg_read(dfi_buffer->base + DDR_DFICH_TOP_STA__ADR);

    // Check if packet fits before loading
    if (GET_REG_FIELD(reg_val, DDR_DFICH_TOP_STA_IG_STATE) == DFI_FIFO_STATE_FULL)
    {
        return WDDR_ERROR_DFI_PACKET_FIFO_FULL;
    }

    // Get WDATA ENABLE status
    reg_val = reg_read(dfi_buffer->base + DDR_DFICH_TOP_1_CFG__ADR);

    // Write entire packet and push
    for (uint8_t ii = DFI_IG_FIFO_LOAD_NUM; ii > 0; ii--)
    {
        reg_write(dfi_buffer->base + DDR_DFICH_IG_DATA_CFG__ADR, data[ii - 1]);
        en_bit = GET_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_WDATA_ENABLE) ^ 0x1;
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_WDATA_ENABLE, en_bit);
        reg_write(dfi_buffer->base + DDR_DFICH_TOP_1_CFG__ADR, reg_val);
    }

    // Push packet into the FIFO
    upd_bit = GET_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_WDATA_UPDATE) ^ 0x1;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_WDATA_UPDATE, upd_bit);
    reg_write(dfi_buffer->base + DDR_DFICH_TOP_1_CFG__ADR, reg_val);

    return WDDR_SUCCESS;
}

wddr_return_t dfi_buffer_read_eg_fifo_reg_if(dfi_buffer_dev_t *dfi_buffer,
                                             uint32_t data[DFI_EG_FIFO_LOAD_NUM])
{
    uint32_t reg_val;
    uint8_t en_bit, upd_bit;

    // FIFO can't be empty
    reg_val = reg_read(dfi_buffer->base + DDR_DFICH_TOP_STA__ADR);
    if (GET_REG_FIELD(reg_val, DDR_DFICH_TOP_STA_EG_STATE) == DFI_FIFO_STATE_EMPTY)
    {
        return WDDR_ERROR_DFI_PACKET_FIFO_EMPTY;
    }

    // Pop a packet
    reg_val = reg_read(dfi_buffer->base + DDR_DFICH_TOP_1_CFG__ADR);
    upd_bit = GET_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_RDATA_UPDATE) ^ 0x1;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_RDATA_UPDATE, upd_bit);
    reg_write(dfi_buffer->base + DDR_DFICH_TOP_1_CFG__ADR, reg_val);

    // Write into data buffer
    for (uint8_t ii = 0; ii < DFI_EG_FIFO_LOAD_NUM; ii++)
    {
        data[ii] = reg_read(dfi_buffer->base + DDR_DFICH_EG_DATA_STA__ADR);
        en_bit = GET_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_RDATA_ENABLE) ^ 0x1;
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_TOP_1_CFG_RDATA_ENABLE, en_bit);
        reg_write(dfi_buffer->base + DDR_DFICH_TOP_1_CFG__ADR, reg_val);
    }

    return WDDR_SUCCESS;
}

static void dfi_buffer_handle_ig_irq(int irq_num, void *args)
{
    uint32_t reg_val;
    dfi_buffer_dev_t *dfi_buffer = (dfi_buffer_dev_t *) args;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Clear Interrupt
    /** NOTE: Only CH0 is enabled */
    reg_val = FAST_IRQ_STICKY_MASK(DDR_IRQ_CH0_IBUF_EMPTY) |
              FAST_IRQ_STICKY_MASK(DDR_IRQ_CH0_IBUF_FULL);
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_CLR_CFG__ADR, reg_val);
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_CLR_CFG__ADR, 0x0);

    reg_val = reg_read(dfi_buffer->base + DDR_DFICH_TOP_STA__ADR);
    if (GET_REG_FIELD(reg_val, DDR_DFICH_TOP_STA_IG_STATE) == DFI_FIFO_STATE_EMPTY)
    {
        vCompleteFromISR(&dfi_buffer->ig_empty_completion, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}
