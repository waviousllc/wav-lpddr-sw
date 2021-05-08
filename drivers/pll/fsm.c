/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <compiler.h>
#include <pll/fsm.h>
#include <kernel/irq.h>
#include <kernel/io.h>
#include <wddr/irq_map.h>
#include <wddr/memory_map.h>

/** @brief  PLL State Handler for all states */
static void pll_fsm_passtrough_state_handler(__UNUSED__ fsm_t *fsm, __UNUSED__ void *data);

/** @brief  Guard Function for PLL Init Lock State */
static bool pll_init_lock_guard(fsm_t *fsm, __UNUSED__ void *data);

/** @brief  IRQ Handler for all PLL IRQ events */
static void pll_irq_handler(__UNUSED__ int irq, void *args);

static const state_func_t state_table[] =
{
    // State Function                   // Guard Function       // Exit Function
    {pll_fsm_passtrough_state_handler,  NULL,                   NULL},
    {pll_fsm_passtrough_state_handler,  pll_init_lock_guard,    NULL},
    {pll_fsm_passtrough_state_handler,  NULL,                   NULL},
};

void pll_fsm_init(pll_fsm_t *fsm, pll_dev_t *pll)
{
    uint32_t reg_val;
    fsm_init(fsm, pll, state_table);

    // Request IRQ
    request_irq(MCU_FAST_IRQ_PLL, pll_irq_handler, fsm);

    // Turn PLL interrupts
    reg_val = reg_read(pll->base + DDR_MVP_PLL_CORE_STATUS_INT_EN__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_STATUS_INT_EN_INITIAL_SWITCH_DONE_INT_EN, 0x1);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_STATUS_INT_EN_LOSS_OF_LOCK_INT_EN, 0x1);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_STATUS_INT_EN_CORE_LOCKED_INT_EN, 0x1);
    reg_write(pll->base + DDR_MVP_PLL_CORE_STATUS_INT_EN__ADR, reg_val);

    // Enable interrupt in CPU
    enable_irq(MCU_FAST_IRQ_PLL);
}

void pll_fsm_reset_event(pll_fsm_t *fsm)
{
    fsm_handle_external_event(fsm, PLL_STATE_NOT_LOCKED, NULL);
}

static void pll_fsm_passtrough_state_handler(__UNUSED__ fsm_t *fsm, __UNUSED__ void *data)
{
    // Do Nothing
}

static bool pll_init_lock_guard(fsm_t *fsm, __UNUSED__ void *data)
{
    // Only allowed if not locked already
    return fsm->current_state == PLL_STATE_NOT_LOCKED;
}

static void pll_irq_handler(__UNUSED__ int irq, void *args)
{
    fsm_t *fsm = (fsm_t *) args;
    pll_dev_t *pll = (pll_dev_t *) fsm->instance;

    uint32_t reg_val;

    // Read PLL's IRQ status
    reg_val = reg_read(pll->base + DDR_MVP_PLL_CORE_STATUS_INT__ADR);

    // clear PLL's IRQ status
    reg_write(pll->base + DDR_MVP_PLL_CORE_STATUS_INT__ADR, reg_val);

    // Loss of lock (highest priority)
    if (reg_val & DDR_MVP_PLL_CORE_STATUS_INT_LOSS_OF_LOCK__MSK)
    {
        fsm_handle_interrupt_event(fsm, PLL_STATE_NOT_LOCKED, NULL);
    }

    // Full Lock (Phase lock loop)
    else if (reg_val & DDR_MVP_PLL_CORE_STATUS_INT_CORE_LOCKED__MSK)
    {
        fsm_handle_interrupt_event(fsm, PLL_STATE_LOCKED, NULL);
    }

    // Initial Lock (Frequency Lock Loop)
    else if (reg_val & DDR_MVP_PLL_CORE_STATUS_INT_INITIAL_SWITCH_DONE__MSK)
    {
        fsm_handle_interrupt_event(fsm, PLL_STATE_INIT_LOCKED, NULL);
    }
}
