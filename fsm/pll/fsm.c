/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#include <compiler.h>
#include <pll/fsm.h>
#include <kernel/irq.h>
#include <kernel/io.h>
#include <wddr/irq_map.h>
#include <wddr/memory_map.h>

/** @brief  PLL State Handler for NOT_LOCKED state */
static void pll_fsm_not_locked_state_handler(fsm_t *fsm, void *data);

/** @brief  PLL State Handler for PREP state */
static void pll_fsm_prep_state_handler(fsm_t *fsm, void *data);

/** @brief  PLL State Handler for PREP_DONE state */
static void pll_fsm_prep_done_state_handler(fsm_t *fsm, void *data);

/** @brief  PLL State Handler for SWITCH state */
static void pll_fsm_switch_state_handler(fsm_t *fsm, void *data);

/** @brief  PLL State Handler for INIT_SWITCH_DONE state */
static void pll_fsm_init_switch_done_state_handler(fsm_t *fsm, void *data);

/** @brief  PLL State Handler for LOCKED state */
static void pll_fsm_locked_state_handler(fsm_t *fsm, void *data);

/** @brief  Guard Function for INIT_SWITCH_DONE state */
static bool pll_init_switch_done_guard(fsm_t *fsm, void *data);

/** @brief  Guard Function for PREP state */
static bool pll_prep_guard(fsm_t *fsm, void *data);

/** @brief  Guard Function for SWITCH state */
static bool pll_switch_guard(fsm_t *fsm, void *data);

/** @brief  IRQ Handler for all PLL IRQ events */
static void pll_irq_handler(int irq, void *args);

static const state_func_t state_table[] =
{
    // State Function                           // Guard Function           // Exit Function
    {pll_fsm_not_locked_state_handler,          NULL,                       NULL},
    {pll_fsm_prep_state_handler,                pll_prep_guard,             NULL},
    {pll_fsm_prep_done_state_handler,           NULL,                       NULL},
    {pll_fsm_switch_state_handler,              pll_switch_guard,           NULL},
    {pll_fsm_init_switch_done_state_handler,    pll_init_switch_done_guard, NULL},
    {pll_fsm_locked_state_handler,              NULL,                       NULL},
};

void pll_fsm_init(pll_fsm_t *fsm, pll_dev_t *pll)
{
    uint32_t reg_val;
    fsm_init(fsm, pll, state_table);

    disable_irq(MCU_FAST_IRQ_PLL);

    /**
     * @note PLL might have been used outside of the state machine so
     *       interrupts state might be in wrong state. Interrupts must
     *       be cleared.
     */
    // Enable all interrupts
    reg_val = reg_read(pll->base + DDR_MVP_PLL_CORE_STATUS_INT_EN__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_STATUS_INT_EN_INITIAL_SWITCH_DONE_INT_EN, 0x1);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_STATUS_INT_EN_CORE_LOCKED_INT_EN, 0x1);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_STATUS_INT_EN_LOSS_OF_LOCK_INT_EN, 0x1);
    reg_write(pll->base + DDR_MVP_PLL_CORE_STATUS_INT_EN__ADR, reg_val);

    // Wait until PLL has seen INT_EN updates
    do
    {
        reg_val = reg_read(pll->base + DDR_MVP_PLL_CORE_STATUS_INT_EN__ADR);
    } while (!(reg_val & DDR_MVP_PLL_CORE_STATUS_INT_EN_CORE_LOCKED_INT_EN__MSK));

    // Read interrupt status and clear
    reg_val = reg_read(pll->base + DDR_MVP_PLL_CORE_STATUS_INT__ADR);
    reg_write(pll->base + DDR_MVP_PLL_CORE_STATUS_INT__ADR, reg_val);

    // Disable all interrupts
    reg_val = reg_read(pll->base + DDR_MVP_PLL_CORE_STATUS_INT_EN__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_STATUS_INT_EN_INITIAL_SWITCH_DONE_INT_EN, 0x0);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_STATUS_INT_EN_CORE_LOCKED_INT_EN, 0x0);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_STATUS_INT_EN_LOSS_OF_LOCK_INT_EN, 0x0);
    reg_write(pll->base + DDR_MVP_PLL_CORE_STATUS_INT_EN__ADR, reg_val);

    // Request IRQ
    request_irq(MCU_FAST_IRQ_PLL, pll_irq_handler, fsm);
}

void pll_fsm_prep_event(pll_fsm_t *fsm, pll_prep_data_t *data)
{
    uint32_t reg_val;
    pll_dev_t *pll = (pll_dev_t *) fsm->instance;

    // Disable interrupts
    disable_irq(MCU_FAST_IRQ_PLL);

    // Disable PLL interrupts
    reg_val = reg_read(pll->base + DDR_MVP_PLL_CORE_STATUS_INT_EN__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_STATUS_INT_EN_INITIAL_SWITCH_DONE_INT_EN, 0x0);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_STATUS_INT_EN_CORE_LOCKED_INT_EN, 0x0);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_STATUS_INT_EN_LOSS_OF_LOCK_INT_EN, 0x0);
    reg_write(pll->base + DDR_MVP_PLL_CORE_STATUS_INT_EN__ADR, reg_val);

    fsm_handle_external_event(fsm, PLL_STATE_PREP, data);
}

void pll_fsm_switch_event(pll_fsm_t *fsm, bool is_sw_switch)
{
    fsm_handle_external_event(fsm, PLL_STATE_SWITCH, (void *) is_sw_switch);
}

void pll_fsm_get_current_freq(pll_fsm_t *fsm, uint8_t *freq_id)
{
    pll_dev_t *pll = (pll_dev_t *) fsm->instance;
    *freq_id = pll->p_vco_current->freq_id;
}

void pll_fsm_get_next_freq(pll_fsm_t *fsm, uint8_t *freq_id)
{
    pll_dev_t *pll = (pll_dev_t *) fsm->instance;
    if (pll->p_vco_next != NULL)
    {
        *freq_id = pll->p_vco_next->freq_id;
    }
    else
    {
        *freq_id = UNDEFINED_FREQ_ID;
    }
}

void pll_fsm_get_current_vco_id(pll_fsm_t *fsm, uint8_t *vco_id)
{
    pll_dev_t *pll = (pll_dev_t *) fsm->instance;
    *vco_id = pll->p_vco_current->vco_id;
}

void pll_fsm_get_next_vco_id(pll_fsm_t *fsm, uint8_t *vco_id)
{
    pll_dev_t *pll = (pll_dev_t *) fsm->instance;
    if(pll->p_vco_next != NULL)
    {
        *vco_id = pll->p_vco_next->vco_id;
    }
    else
    {
        *vco_id = UNDEFINED_VCO_ID;
    }
}

static void pll_fsm_not_locked_state_handler(fsm_t *fsm, __UNUSED__ void *data)
{
    // Enable interrupt in CPU
    enable_irq(MCU_FAST_IRQ_PLL);
}

static void pll_fsm_prep_state_handler(fsm_t *fsm, void *data)
{
    pll_dev_t *pll = (pll_dev_t *) fsm->instance;
    pll_prep_data_t *prep_data = (pll_prep_data_t *) data;

    // Prepare PLL and VCO for switch
    pll_prepare_vco_switch(pll, prep_data->freq_id, prep_data->cal, prep_data->cfg);

    // Go to PREP_DONE state
    fsm_handle_internal_event(fsm, PLL_STATE_PREP_DONE, NULL);
}

static void pll_fsm_prep_done_state_handler(fsm_t *fsm, void *data)
{
    uint32_t reg_val;
    pll_dev_t *pll = (pll_dev_t *) fsm->instance;

    // Enable Initial Lock interrupt
    // This should be here so don't miss initial switch doen event when HW / SW switch happens
    reg_val = reg_read(pll->base + DDR_MVP_PLL_CORE_STATUS_INT_EN__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_STATUS_INT_EN_INITIAL_SWITCH_DONE_INT_EN, 0x1);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_STATUS_INT_EN_CORE_LOCKED_INT_EN, 0x0);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_STATUS_INT_EN_LOSS_OF_LOCK_INT_EN, 0x0);
    reg_write(pll->base + DDR_MVP_PLL_CORE_STATUS_INT_EN__ADR, reg_val);
}

static void pll_fsm_switch_state_handler(fsm_t *fsm, __UNUSED__ void *data)
{
    pll_dev_t *pll = (pll_dev_t *) fsm->instance;
    bool is_sw_switch = (bool) data;

    // Switch PLL
    pll_switch_vco(pll, is_sw_switch);

    fsm_handle_internal_event(fsm, PLL_STATE_NOT_LOCKED, NULL);
}

static void pll_fsm_init_switch_done_state_handler(fsm_t *fsm, __UNUSED__ void *data)
{
    uint32_t reg_val;
    pll_dev_t *pll = (pll_dev_t *) fsm->instance;

    // Enable Loss of Lock and Core Lock interrupts
    reg_val = reg_read(pll->base + DDR_MVP_PLL_CORE_STATUS_INT_EN__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_STATUS_INT_EN_INITIAL_SWITCH_DONE_INT_EN, 0x0);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_STATUS_INT_EN_CORE_LOCKED_INT_EN, 0x1);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_STATUS_INT_EN_LOSS_OF_LOCK_INT_EN, 0x1);
    reg_write(pll->base + DDR_MVP_PLL_CORE_STATUS_INT_EN__ADR, reg_val);

    // Enable interrupt in CPU
    enable_irq(MCU_FAST_IRQ_PLL);
}

static void pll_fsm_locked_state_handler(fsm_t *fsm, __UNUSED__ void *data)
{
    uint32_t reg_val;
    pll_dev_t *pll = (pll_dev_t *) fsm->instance;

    // Disable other VCO
    pll_disable_vco(pll);

    // Enable Loss of Lock interrupt
    reg_val = reg_read(pll->base + DDR_MVP_PLL_CORE_STATUS_INT_EN__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_STATUS_INT_EN_INITIAL_SWITCH_DONE_INT_EN, 0x0);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_STATUS_INT_EN_CORE_LOCKED_INT_EN, 0x0);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_STATUS_INT_EN_LOSS_OF_LOCK_INT_EN, 0x1);
    reg_write(pll->base + DDR_MVP_PLL_CORE_STATUS_INT_EN__ADR, reg_val);

    // Enable interrupt in CPU
    enable_irq(MCU_FAST_IRQ_PLL);
}

static bool pll_init_switch_done_guard(fsm_t *fsm, __UNUSED__ void *data)
{
    return (fsm->current_state == PLL_STATE_NOT_LOCKED ||
            fsm->current_state == PLL_STATE_PREP_DONE);
}

static bool pll_prep_guard(fsm_t *fsm, __UNUSED__ void *data)
{
    return (fsm->current_state == PLL_STATE_LOCKED ||
            fsm->current_state == PLL_STATE_NOT_LOCKED ||
            fsm->current_state == PLL_STATE_PREP_DONE);
}

static bool pll_switch_guard(fsm_t *fsm, __UNUSED__ void *data)
{
    return fsm->current_state == PLL_STATE_PREP_DONE;
}

static void pll_irq_handler(__UNUSED__ int irq, void *args)
{
    uint32_t reg_val;
    fsm_t *fsm = (fsm_t *) args;
    pll_dev_t *pll = (pll_dev_t *) fsm->instance;

    // Read PLL's IRQ status
    reg_val = reg_read(pll->base + DDR_MVP_PLL_CORE_STATUS_INT__ADR);

    // clear PLL's IRQ status
    reg_write(pll->base + DDR_MVP_PLL_CORE_STATUS_INT__ADR, reg_val);

    // Disable interrupt
    disable_irq(MCU_FAST_IRQ_PLL);

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
        fsm_handle_interrupt_event(fsm, PLL_STATE_INIT_SWITCH_DONE, NULL);
    }
}
