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
#include <vco/driver.h>

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
    uint32_t reg_val;
    vco_dev_t *p_vco;

    // Initialize all VCOs
    for (uint8_t vco_id = 0; vco_id < VCO_INDEX_NUM; vco_id++)
    {
        p_vco = &pll->vco[vco_id];
        p_vco->vco_id = vco_id;
        p_vco->freq_id = UNDEFINED_FREQ_ID;
        vco_init_reg_if(p_vco, base);
    }

    // Intialize PLL
    pll->p_vco_current = NULL;
    pll->p_vco_next = NULL;
    pll->p_vco_prev = NULL;
    pll_init_reg_if(pll, base);

    /**
     * @note PLL interrupts might be enabled at boot so need to clear.
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
    vco_set_band_reg_if(p_vco, MCU_BAND, MCU_BAND_FINE, true);
    vco_set_fll_control2_reg_if(p_vco,
                                MCU_FLL_REFCLK_COUNT,
                                MCU_FLL_RANGE,
                                MCU_FLL_VCO_COUNT_TARGET);
    vco_set_fll_control1_reg_if(p_vco,
                                MCU_BAND,
                                MCU_BAND_FINE,
                                MCU_LOCK_COUNT_THRESHOLD);

    // Take PLL out of reset
    pll_set_vco_sel_reg_if(pll, VCO_INDEX_MCU);
    pll_reset_reg_if(pll);
}

void pll_prepare_vco_switch(pll_dev_t *pll, uint8_t freq_id, pll_freq_cfg_t *cfg)
{
    vco_dev_t *p_vco;
    vco_cfg_t *p_vco_cfg;

    // Find available PHY VCO
    for (uint8_t vco_id = VCO_INDEX_PHY_START; vco_id < VCO_INDEX_PHY_END; vco_id++)
    {
        // If not current, then it's free.
        if (vco_id != pll->p_vco_current->vco_id)
        {
            p_vco_cfg = &cfg->vco_cfg[vco_id - VCO_INDEX_PHY_START];
            p_vco = &pll->vco[vco_id];

            //  Configure VCO for given frequency
            vco_set_enable_reg_if(p_vco, true);
            vco_set_post_div_reg_if(p_vco, p_vco_cfg->post_div);
            vco_set_int_frac_reg_if(p_vco, p_vco_cfg->int_comp, p_vco_cfg->prop_gain);
            vco_set_band_reg_if(p_vco, p_vco_cfg->band, p_vco_cfg->fine, true);

            // Set as next VCO
            pll_set_vco_sel_reg_if(pll, vco_id);
            pll->p_vco_next = p_vco;
            p_vco->freq_id = freq_id;
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
        pll_switch_vco_reg_if(pll);
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
    if (pll->p_vco_prev->vco_id != VCO_INDEX_MCU)
    {
        vco_set_enable_reg_if(pll->p_vco_prev, false);
    }
    else
    {
        // Enable FLL for VCO0. Peristent mode is used by default for VCO0.
        vco_set_fll_enable_reg_if(pll->p_vco_prev, true);

        // Disable coarse and fine band overrides so VCO is adjusted
        // as PLL adjusts voltage
        vco_set_band_reg_if(pll->p_vco_prev, MCU_BAND, MCU_BAND_FINE, false);
    }

    pll->p_vco_prev = NULL;
}

void pll_calibrate_vco(pll_dev_t *pll, pll_freq_cfg_t *cfg)
{
    vco_dev_t *p_vco;
    vco_cfg_t *p_vco_cfg;

    // Can only calibrate prior to switching to a VCO used for PHY clock
    if (pll->p_vco_current != NULL && pll->p_vco_current->vco_id != VCO_INDEX_MCU)
    {
        return;
    }

    for (uint8_t vco_id = VCO_INDEX_PHY_START; vco_id < VCO_INDEX_PHY_END; vco_id++)
    {
        p_vco_cfg = &cfg->vco_cfg[vco_id - VCO_INDEX_PHY_START];
        p_vco = &pll->vco[vco_id];

        // Configure VCO for given frequency
        vco_set_fll_control2_reg_if(p_vco,
                                    p_vco_cfg->fll_refclk_count,
                                    p_vco_cfg->fll_range,
                                    p_vco_cfg->fll_vco_count_target);
        vco_set_fll_control1_reg_if(p_vco, p_vco_cfg->band, p_vco_cfg->fine, p_vco_cfg->lock_count_threshold);

        // Enable VCO FLL
        vco_set_fll_enable_reg_if(p_vco, true);

        // Wait until locked
        while (!vco_is_fll_locked(p_vco));

        // Disable VCO FLL
        vco_set_fll_enable_reg_if(p_vco, false);

        // Get calibrated VCO values
        vco_get_fll_band_status_reg_if(p_vco, &p_vco_cfg->band, &p_vco_cfg->fine);
    }
}

void pll_set_loss_lock_interrupt_state(pll_dev_t *pll, bool enable)
{
    uint32_t reg_val;
    reg_val = reg_read(pll->base + DDR_MVP_PLL_CORE_STATUS_INT_EN__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_STATUS_INT_EN_LOSS_OF_LOCK_INT_EN, enable);
    reg_write(pll->base + DDR_MVP_PLL_CORE_STATUS_INT_EN__ADR, reg_val);
}

void pll_set_lock_interrupt_state(pll_dev_t *pll, bool enable)
{
    uint32_t reg_val;
    reg_val = reg_read(pll->base + DDR_MVP_PLL_CORE_STATUS_INT_EN__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_STATUS_INT_EN_CORE_LOCKED_INT_EN, enable);
    reg_write(pll->base + DDR_MVP_PLL_CORE_STATUS_INT_EN__ADR, reg_val);
}

void pll_set_init_lock_interrupt_state(pll_dev_t *pll, bool enable)
{
    uint32_t reg_val;
    reg_val = reg_read(pll->base + DDR_MVP_PLL_CORE_STATUS_INT_EN__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_STATUS_INT_EN_INITIAL_SWITCH_DONE_INT_EN, enable);
    reg_write(pll->base + DDR_MVP_PLL_CORE_STATUS_INT_EN__ADR, reg_val);
}

void pll_get_current_vco(pll_dev_t *pll, uint8_t *vco_id)
{
    *vco_id = pll->p_vco_current->vco_id;
}

void pll_get_next_vco(pll_dev_t *pll, uint8_t *vco_id)
{
    configASSERT(pll->p_vco_next != NULL);

    if (pll->p_vco_next == NULL)
    {
        *vco_id = UNDEFINED_VCO_ID;
        return;
    }

    *vco_id = pll->p_vco_next->vco_id;
}

void pll_get_current_freq(pll_dev_t *pll, uint8_t *freq_id)
{
    *freq_id = pll->p_vco_current->freq_id;
}

static void pll_irq_handler(__UNUSED__ int irq, void *args)
{
    uint32_t reg_val;
    BaseType_t xHigherPriorityTaskWoken;
    fw_msg_t msg;
    pll_dev_t *pll = (pll_dev_t *) args;

    // Read PLL's IRQ status
    reg_val = reg_read(pll->base + DDR_MVP_PLL_CORE_STATUS_INT__ADR);

    // clear PLL's IRQ status
    reg_write(pll->base + DDR_MVP_PLL_CORE_STATUS_INT__ADR, reg_val);

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
