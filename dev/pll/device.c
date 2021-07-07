/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#include <stddef.h>
#include <pll/device.h>
#include <pll/driver.h>
#include <vco/driver.h>

// MCU VCO values
#define MCU_BAND                    (0x3)
#define MCU_BAND_FINE               (0x1F)
#define MCU_FLL_REFCLK_COUNT        (63)
#define MCU_FLL_RANGE               (2)
#define MCU_FLL_VCO_COUNT_TARGET    (320)
#define MCU_LOCK_COUNT_THRESHOLD    (2)

void pll_init(pll_dev_t *pll, uint32_t base)
{
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

void pll_prepare_vco_switch(pll_dev_t *pll, uint8_t freq_id, pll_freq_cal_t *cal, pll_freq_cfg_t *cfg)
{
    vco_dev_t *p_vco;
    vco_cfg_t *p_vco_cfg;
    vco_cal_t *p_vco_cal;

    // Find available PHY VCO
    for (uint8_t vco_id = VCO_INDEX_PHY_START; vco_id < VCO_INDEX_PHY_END; vco_id++)
    {
        // If not current, then it's free.
        if (vco_id != pll->p_vco_current->vco_id)
        {
            p_vco_cal = &cal->vco_cal[vco_id - VCO_INDEX_PHY_START];
            p_vco_cfg = &cfg->vco_cfg[vco_id - VCO_INDEX_PHY_START];
            p_vco = &pll->vco[vco_id];

            //  Configure VCO for given frequency
            vco_set_enable_reg_if(p_vco, true);
            vco_set_post_div_reg_if(p_vco, p_vco_cfg->post_div);
            vco_set_int_frac_reg_if(p_vco, p_vco_cfg->int_comp, p_vco_cfg->prop_gain);
            vco_set_band_reg_if(p_vco, p_vco_cal->band, p_vco_cal->fine, true);

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

void pll_calibrate_vco(pll_dev_t *pll, pll_freq_cal_t *cal, pll_freq_cfg_t *cfg)
{
    vco_dev_t *p_vco;
    vco_cfg_t *p_vco_cfg;
    vco_cal_t *p_vco_cal;

    // Can only calibrate prior to switching to a VCO used for PHY clock
    if (pll->p_vco_current != NULL && pll->p_vco_current->vco_id != VCO_INDEX_MCU)
    {
        return;
    }

    for (uint8_t vco_id = VCO_INDEX_PHY_START; vco_id < VCO_INDEX_PHY_END; vco_id++)
    {
        p_vco_cal = &cal->vco_cal[vco_id - VCO_INDEX_PHY_START];
        p_vco_cfg = &cfg->vco_cfg[vco_id - VCO_INDEX_PHY_START];
        p_vco = &pll->vco[vco_id];

        // Configure VCO for given frequency
        vco_set_fll_control2_reg_if(p_vco,
                                    p_vco_cfg->fll_refclk_count,
                                    p_vco_cfg->fll_range,
                                    p_vco_cfg->fll_vco_count_target);
        vco_set_fll_control1_reg_if(p_vco, p_vco_cal->band, p_vco_cal->fine, p_vco_cfg->lock_count_threshold);

        // Enable VCO FLL
        vco_set_fll_enable_reg_if(p_vco, true);

        // Wait until locked
        while (!vco_is_fll_locked(p_vco));

        // Disable VCO FLL
        vco_set_fll_enable_reg_if(p_vco, false);

        // Get calibrated VCO values
        vco_get_fll_band_status_reg_if(p_vco, &p_vco_cal->band, &p_vco_cal->fine);
    }
}
