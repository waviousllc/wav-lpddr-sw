/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <dfi/device.h>
#include <dfi/driver.h>
#include <dfi/buffer_device.h>
#include <kernel/irq.h>
#include <kernel/io.h>
#include <wddr/memory_map.h>
#include <wddr/irq_map.h>
#include <firmware/phy_task.h>

/** @brief  IRQ Handler for all phymstr ACK events */
static void handle_dfi_phymstr_ack_irq(int irq_num, void *args);

/** @brief  IRQ Handler for all ctrlupd events */
static void handle_dfi_ctrlupd_req_irq(int irq_num, void *args);

/** @brief  IRQ Handler for all phyupd ACK events */
static void handle_dfi_phyupd_ack_irq(int irq_num, void *args);

void dfi_init(dfi_dev_t *dfi, uint32_t base)
{
    uint32_t mask;

    dfi->base = base;
    dfi_set_msr_reg_if(dfi, WDDR_MSR_0);
    dfi_buffer_init(&dfi->dfi_buffer, base);

    // Request IRQs
    request_irq(MCU_FAST_IRQ_PHYMSTR_ACK, handle_dfi_phymstr_ack_irq, NULL);
    request_irq(MCU_FAST_IRQ_PHYUPD_ACK, handle_dfi_phyupd_ack_irq, NULL);
    request_irq(MCU_FAST_IRQ_CTRLUPD_REQ, handle_dfi_ctrlupd_req_irq, NULL);

    // Turn off interrupts until needed
    disable_irq(MCU_FAST_IRQ_PHYMSTR_ACK);
    disable_irq(MCU_FAST_IRQ_PHYUPD_ACK);

    // Turn on sticky bit for IRQ
    mask = reg_read(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_STICKY_CFG__ADR);
    mask |= FAST_IRQ_STICKY_MASK(DDR_IRQ_PHYUPD_ACK) |
            FAST_IRQ_STICKY_MASK(DDR_IRQ_PHYMSTR_ACK) |
            FAST_IRQ_STICKY_MASK(DDR_IRQ_CTRLUPD_REQ_ASSERTION) |
            FAST_IRQ_STICKY_MASK(DDR_IRQ_CTRLUPD_REQ_DEASSERTION);
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_STICKY_CFG__ADR, mask);
}

void dfi_phymstr_req(dfi_dev_t *dfi, dfi_phymstr_req_t *req)
{
    dfi_phymstr_req_assert_reg_if(req);
}

void dfi_phymstr_exit(dfi_dev_t *dfi)
{
    dfi_phymstr_req_deassert_reg_if();
}

void dfi_ctrlupd_deassert_ack(dfi_dev_t *dfi)
{
    dfi_ctrlupd_deassert_ack_reg_if();
}

void dfi_ctrlupd_enable(dfi_dev_t *dfi)
{
    dfi_ctrlupd_ack_override_reg_if(false, 0);
    enable_irq(MCU_FAST_IRQ_CTRLUPD_REQ);
}

void dfi_ctrlupd_disable(dfi_dev_t *dfi)
{
    dfi_ctrlupd_ack_override_reg_if(true, 0);
    disable_irq(MCU_FAST_IRQ_CTRLUPD_REQ);
}

void dfi_phyupd_req(dfi_dev_t *dfi, dfi_phyupd_type_t type)
{
    dfi_phyupd_req_assert_reg_if(type);
}

void dfi_phyupd_exit(dfi_dev_t *dfi)
{
    dfi_phyupd_req_deassert_reg_if();
}

static void handle_dfi_phymstr_ack_irq(__UNUSED__ int irq_num, __UNUSED__ void *args)
{
    BaseType_t xHigherPriorityTaskWoken;
    fw_msg_t msg = {
        .event = FW_PHY_EVENT_PHYMSTR_ACK,
        .data = NULL,
    };

    disable_irq(MCU_FAST_IRQ_PHYMSTR_ACK);

    // Clear interrupt
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_CLR_CFG__ADR,
              FAST_IRQ_STICKY_MASK(DDR_IRQ_PHYMSTR_ACK));
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_CLR_CFG__ADR, 0x0);

    // Send FW Event
    fw_phy_task_notify_isr(&msg, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void handle_dfi_phyupd_ack_irq(__UNUSED__ int irq_num, __UNUSED__ void *args)
{
    BaseType_t xHigherPriorityTaskWoken;
    fw_msg_t msg = {
        .event = FW_PHY_EVENT_PHYUPD_ACK,
        .data = NULL,
    };

    // Disable interrupt to prevent from firing again
    disable_irq(MCU_FAST_IRQ_PHYUPD_ACK);

    // Clear interrupt
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_CLR_CFG__ADR,
              FAST_IRQ_STICKY_MASK(DDR_IRQ_PHYUPD_ACK));
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_CLR_CFG__ADR, 0x0);

    // Send FW Event
    fw_phy_task_notify_isr(&msg, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void handle_dfi_ctrlupd_req_irq(__UNUSED__ int irq_num, __UNUSED__ void *args)
{
    uint32_t reg_val;
    BaseType_t xHigherPriorityTaskWoken;
    fw_msg_t msg = {
        .data = NULL,
    };

    // Determine if ASSERT or DEASSERT
    reg_val = reg_read(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_STA__ADR);

    // Clear interrupt
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_CLR_CFG__ADR,
                FAST_IRQ_STICKY_MASK(DDR_IRQ_CTRLUPD_REQ_ASSERTION) |
                FAST_IRQ_STICKY_MASK(DDR_IRQ_CTRLUPD_REQ_DEASSERTION));
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_CLR_CFG__ADR, 0x0);

    if (reg_val & FAST_IRQ_STICKY_MASK(DDR_IRQ_CTRLUPD_REQ_ASSERTION))
    {
        msg.event = FW_PHY_EVENT_CTRLUPD_ASSERT;
    }
    else if (reg_val & FAST_IRQ_STICKY_MASK(DDR_IRQ_CTRLUPD_REQ_DEASSERTION))
    {
        msg.event = FW_PHY_EVENT_CTRLUPD_DEASSERT;
    }
    // TODO: ERROR handling
    else
    {
        return;
    }

    // Send FW Event
    fw_phy_task_notify_isr(&msg, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
