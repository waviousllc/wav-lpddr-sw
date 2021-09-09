/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <compiler.h>
#include <fsw/device.h>
#include <kernel/irq.h>
#include <kernel/io.h>
#include <pll/device.h>
#include <wddr/irq_map.h>
#include <wddr/memory_map.h>
#include <firmware/phy_task.h>

/** @brief  IRQ Handler for INIT_START IRQ */
static void handle_init_start_irq(int irq_num, void *args);

/** @brief  IRQ Handler for INIT_COMPLETE IRQ */
static void handle_init_complete_irq(int irq_num, void *args);

void fsw_init(fsw_dev_t *dev, uint32_t base)
{
    uint32_t mask;

    dev->fsw_reg = (fsw_reg_t *)(base + WDDR_MEMORY_MAP_FSW);
    dev->mode = FSW_MODE_SW;

    // Mask interrupts
    mask = reg_read(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_MSK_CFG__ADR);
    mask |= FAST_IRQ_STICKY_MASK(DDR_IRQ_INIT_START) | FAST_IRQ_STICKY_MASK(DDR_IRQ_INIT_COMPLETE);
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_MSK_CFG__ADR, mask);

    // Register interrupt handlers
    request_irq(MCU_FAST_IRQ_INIT_START, handle_init_start_irq, NULL);
    request_irq(MCU_FAST_IRQ_INIT_COMPLETE, handle_init_complete_irq, NULL);

    // TURN on sticky bit for IRQs
    mask = reg_read(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_STICKY_CFG__ADR);
    mask |= FAST_IRQ_STICKY_MASK(DDR_IRQ_INIT_START) | FAST_IRQ_STICKY_MASK(DDR_IRQ_INIT_COMPLETE);
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_STICKY_CFG__ADR, mask);
}

void fsw_switch_to_dfi_mode(fsw_dev_t *fsw_dev, dfi_dev_t *dfi_dev)
{
    uint32_t init_start, reg_val;

    if (fsw_dev->mode == FSW_MODE_DFI)
    {
        return;
    }

    // Set override values to MSR0 and VCO1
    fsw_ctrl_set_msr_vco_ovr_val_reg_if(fsw_dev->fsw_reg, WDDR_MSR_0, VCO_INDEX_PHY_1);

    // Disable overrides to allow hardware to switch MSR and VCO.
    fsw_ctrl_set_msr_vco_ovr_reg_if(fsw_dev->fsw_reg, false);

    // Release init_complete override indicating dfi interface is ready.
    dfi_set_init_complete_ovr_reg_if(dfi_dev->dfi_reg, false, 0x0);

    // init_start must be low before proceeding, wait for this to happen.
    do
    {
        init_start = dfi_get_init_start_status_reg_if(dfi_dev->dfi_reg);
    } while (init_start != 0x0);

    // Release init_start override (shouldn't be set)
    dfi_set_init_start_ovr_reg_if(dfi_dev->dfi_reg, false, 0x0);

    // Disable interrupts
    disable_irq(MCU_FAST_IRQ_INIT_START);
    disable_irq(MCU_FAST_IRQ_INIT_COMPLETE);

    // Unmask interrupts
    reg_val = reg_read(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_MSK_CFG__ADR);
    reg_val &= ~(FAST_IRQ_STICKY_MASK(DDR_IRQ_INIT_START) | FAST_IRQ_STICKY_MASK(DDR_IRQ_INIT_COMPLETE));
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_MSK_CFG__ADR, reg_val);

    // Clear interrupts
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_CLR_CFG__ADR,
              FAST_IRQ_STICKY_MASK(DDR_IRQ_INIT_START) | FAST_IRQ_STICKY_MASK(DDR_IRQ_INIT_COMPLETE));
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_CLR_CFG__ADR, 0x0);

    fsw_dev->mode = FSW_MODE_DFI;
}

wddr_msr_t fsw_get_current_msr(fsw_dev_t *dev)
{
    return fsw_ctrl_get_current_msr_reg_if(dev->fsw_reg);
}

wddr_msr_t fsw_get_next_msr(fsw_dev_t *dev)
{
    return !fsw_get_current_msr(dev);
}

static void handle_init_start_irq(__UNUSED__ int irq_num, __UNUSED__ void *args)
{
    BaseType_t xHigherPriorityTaskWoken;
    fw_msg_t msg = {
        .event = FW_PHY_EVENT_INIT_START,
        .data = NULL,
    };

    // Mask interrupt
    disable_irq(MCU_FAST_IRQ_INIT_START);

    // Clear interrupt
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_CLR_CFG__ADR,
              FAST_IRQ_STICKY_MASK(DDR_IRQ_INIT_START));
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_CLR_CFG__ADR, 0x0);

    fw_phy_task_notify_isr(&msg, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void handle_init_complete_irq(__UNUSED__ int irq_num, __UNUSED__ void *args)
{
    BaseType_t xHigherPriorityTaskWoken;
    fw_msg_t msg = {
        .event = FW_PHY_EVENT_INIT_COMPLETE,
        .data = NULL,
    };

    // Mask interrupt
    disable_irq(MCU_FAST_IRQ_INIT_COMPLETE);

    // Clear interrupt
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_CLR_CFG__ADR,
              FAST_IRQ_STICKY_MASK(DDR_IRQ_INIT_COMPLETE));
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_CLR_CFG__ADR, 0x0);

    // Send
    fw_phy_task_notify_isr(&msg, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
