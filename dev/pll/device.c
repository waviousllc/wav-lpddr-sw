/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stddef.h>
#include <compiler.h>
#include <kernel/irq.h>
#include <kernel/io.h>
#include <firmware/phy_task.h>
#include <pll/device.h>
#include <pll/driver.h>
#include <wddr/memory_map.h>
#include <wddr/irq_map.h>

// MCU VCO values
#define MCU_BAND                    (0x3)
#define MCU_BAND_FINE               (0x1F)
#define MCU_FLL_REFCLK_COUNT        (63)
#define MCU_FLL_RANGE               (2)
#define MCU_FLL_VCO_COUNT_TARGET    (320)
#define MCU_LOCK_COUNT_THRESHOLD    (2)

static void pll_irq_handler(int irq, void *args);

void pll_init(pll_dev_t *pll, uint32_t base)
{
    vco_dev_t *p_vco;

    // Initialize all VCOs
    for (uint8_t vco_id = 0; vco_id < VCO_INDEX_NUM; vco_id++)
    {
        p_vco = &pll->vco[vco_id];
        p_vco->id = vco_id;
        p_vco->freq = UNDEFINED_FREQ_ID;
    }

    // Intialize PLL
    pll->pll_reg = (pll_reg_t *)(base + WDDR_MEMORY_MAP_PLL);
    pll->p_vco_current = NULL;
    pll->p_vco_next = NULL;
    pll->p_vco_prev = NULL;

    /**
     * @note PLL interrupts might be enabled at boot so need to clear.
     */
    // Enable all interrupts
    pll_set_loss_lock_interrupt_state(pll, true);
    pll_set_lock_interrupt_state(pll, true);
    pll_set_init_lock_interrupt_state(pll, true);

    // Clear Interrupt status
    pll_clear_interrupt_reg_if(pll->pll_reg);

    // Disable all interrupts
    pll_set_loss_lock_interrupt_state(pll, false);
    pll_set_lock_interrupt_state(pll, false);
    pll_set_init_lock_interrupt_state(pll, false);

    // Register IRQ
    request_irq(MCU_FAST_IRQ_PLL, pll_irq_handler, pll);
}

void pll_boot(pll_dev_t *pll)
{
    vco_dev_t *p_vco;

    // Only run once
    if (pll->p_vco_current != NULL)
    {
        return;
    }

    // TODO: VCO0 calibration?
    // Configure VCO0
    p_vco = &pll->vco[VCO_INDEX_MCU];
    pll->p_vco_current = p_vco;

    // Setup VCO0 for MCU Frequency
    vco_set_band_reg_if(pll->pll_reg, p_vco->id, MCU_BAND, MCU_BAND_FINE, true);
    vco_set_fll_control2_reg_if(pll->pll_reg,
                                p_vco->id,
                                MCU_FLL_REFCLK_COUNT,
                                MCU_FLL_RANGE,
                                MCU_FLL_VCO_COUNT_TARGET);
    vco_set_fll_control1_reg_if(pll->pll_reg,
                                p_vco->id,
                                MCU_BAND,
                                MCU_BAND_FINE,
                                MCU_LOCK_COUNT_THRESHOLD);

    // Take PLL out of reset
    pll_set_vco_sel_reg_if(pll->pll_reg, p_vco->id);
    pll_reset_reg_if(pll->pll_reg);
}

void pll_prepare_vco_switch(pll_dev_t *pll, uint8_t freq_id, pll_freq_cfg_t *cfg)
{
    vco_dev_t *p_vco;
    vco_cfg_t *p_vco_cfg;

    // Find available PHY VCO
    for (uint8_t vco_id = VCO_INDEX_PHY_START; vco_id < VCO_INDEX_PHY_END; vco_id++)
    {
        // If not current, then it's free.
        if (vco_id != pll->p_vco_current->id)
        {
            p_vco_cfg = &cfg->vco_cfg[vco_id - VCO_INDEX_PHY_START];
            p_vco = &pll->vco[vco_id];

            //  Configure VCO for given frequency
            vco_set_enable_reg_if(pll->pll_reg, p_vco->id, true);
            vco_set_post_div_reg_if(pll->pll_reg, p_vco->id, p_vco_cfg->post_div);
            vco_set_int_frac_reg_if(pll->pll_reg, p_vco->id, p_vco_cfg->int_comp, p_vco_cfg->prop_gain);
            vco_set_band_reg_if(pll->pll_reg, p_vco->id, p_vco_cfg->band, p_vco_cfg->fine, true);

            // Set as next VCO
            pll_set_vco_sel_reg_if(pll->pll_reg, p_vco->id);
            pll->p_vco_next = p_vco;
            p_vco->freq = freq_id;
            break;
        }
    }
}

wddr_return_t pll_switch_vco(pll_dev_t *pll, bool is_sw_switch)
{
    if (pll->p_vco_next == NULL)
    {
        return WDDR_ERROR;
    }

    // Perform switch via SW
    if (is_sw_switch)
    {
        pll_switch_vco_reg_if(pll->pll_reg);
    }

    // Switch VCO pointers
    pll->p_vco_prev = pll->p_vco_current;
    pll->p_vco_current = pll->p_vco_next;
    pll->p_vco_next = NULL;

    return WDDR_SUCCESS;
}

void pll_disable_vco(pll_dev_t *pll)
{
    // Nothing to do if previous is NULL
    if (pll->p_vco_prev == NULL)
    {
        return;
    }

    // PHY VCOs should be disabled
    if (pll->p_vco_prev->id != VCO_INDEX_MCU)
    {
        vco_set_enable_reg_if(pll->pll_reg, pll->p_vco_prev->id, false);
    }
    else
    {
        // Enable FLL for VCO0. Peristent mode is used by default for VCO0.
        vco_set_fll_enable_reg_if(pll->pll_reg, pll->p_vco_prev->id, true);

        // Disable coarse and fine band overrides so VCO is adjusted
        // as PLL adjusts voltage
        vco_set_band_reg_if(pll->pll_reg, pll->p_vco_prev->id, MCU_BAND, MCU_BAND_FINE, false);
    }

    pll->p_vco_prev = NULL;
}

void pll_calibrate_vco(pll_dev_t *pll, pll_freq_cfg_t *cfg)
{
    vco_dev_t *p_vco;
    vco_cfg_t *p_vco_cfg;

    // Can only calibrate prior to switching to a VCO used for PHY clock
    if (pll->p_vco_current != NULL && pll->p_vco_current->id != VCO_INDEX_MCU)
    {
        return;
    }

    for (uint8_t vco_id = VCO_INDEX_PHY_START; vco_id < VCO_INDEX_PHY_END; vco_id++)
    {
        p_vco_cfg = &cfg->vco_cfg[vco_id - VCO_INDEX_PHY_START];
        p_vco = &pll->vco[vco_id];

        // Configure VCO for given frequency
        vco_set_fll_control2_reg_if(pll->pll_reg,
                                    p_vco->id,
                                    p_vco_cfg->fll_refclk_count,
                                    p_vco_cfg->fll_range,
                                    p_vco_cfg->fll_vco_count_target);
        vco_set_fll_control1_reg_if(pll->pll_reg,
                                    p_vco->id,
                                    p_vco_cfg->band,
                                    p_vco_cfg->fine,
                                    p_vco_cfg->lock_count_threshold);

        // Enable VCO FLL
        vco_set_fll_enable_reg_if(pll->pll_reg, p_vco->id, true);

        // Wait until locked
        while (!vco_is_fll_locked_reg_if(pll->pll_reg, p_vco->id));

        // Disable VCO FLL
        vco_set_fll_enable_reg_if(pll->pll_reg, p_vco->id, false);

        // Get calibrated VCO values
        vco_get_fll_band_status_reg_if(pll->pll_reg, p_vco->id, &p_vco_cfg->band, &p_vco_cfg->fine);
    }
}

void pll_set_loss_lock_interrupt_state(pll_dev_t *pll, bool enable)
{
    pll_enable_loss_lock_interrupt_reg_if(pll->pll_reg, enable);
}

void pll_set_lock_interrupt_state(pll_dev_t *pll, bool enable)
{
    pll_enable_lock_interrupt_reg_if(pll->pll_reg, enable);
}

void pll_set_init_lock_interrupt_state(pll_dev_t *pll, bool enable)
{
    pll_enable_switch_interrupt_reg_if(pll->pll_reg, enable);
}

void pll_get_current_vco(pll_dev_t *pll, uint8_t *vco_id)
{
    *vco_id = pll->p_vco_current->id;
}

void pll_get_next_vco(pll_dev_t *pll, uint8_t *vco_id)
{
    configASSERT(pll->p_vco_next != NULL);

    if (pll->p_vco_next == NULL)
    {
        *vco_id = UNDEFINED_VCO_ID;
        return;
    }

    *vco_id = pll->p_vco_next->id;
}

void pll_get_current_freq(pll_dev_t *pll, uint8_t *freq_id)
{
    *freq_id = pll->p_vco_current->freq;
}

static void pll_irq_handler(__UNUSED__ int irq, void *args)
{
    uint32_t reg_val;
    BaseType_t xHigherPriorityTaskWoken;
    fw_msg_t msg;
    pll_dev_t *pll = (pll_dev_t *) args;

    // Clear PLL's IRQ status
    reg_val = pll_clear_interrupt_reg_if(pll->pll_reg);
    do
    {
        // Loss of lock (highest priority)
        if (reg_val & DDR_MVP_PLL_CORE_STATUS_INT_LOSS_OF_LOCK__MSK)
        {
            msg.event = FW_PHY_EVENT_PLL_LOSS_LOCK;
            break;
        }

        // Full Lock (Phase lock loop)
        else if (reg_val & DDR_MVP_PLL_CORE_STATUS_INT_CORE_LOCKED__MSK)
        {
            msg.event = FW_PHY_EVENT_PLL_LOCK;
            break;
        }

        // Initial Lock (Frequency Lock Loop)
        else if (reg_val & DDR_MVP_PLL_CORE_STATUS_INT_INITIAL_SWITCH_DONE__MSK)
        {
            msg.event = FW_PHY_EVENT_PLL_INIT_LOCK;
            break;
        }
        // Invalid interrupt
        return;
    } while (0);

    // Send FW Event
    fw_phy_task_notify_isr(&msg, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
