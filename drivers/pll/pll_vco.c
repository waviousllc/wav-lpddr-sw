/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <pll/driver.h>
#include <kernel/io.h>
#include <wddr/memory_map.h>

#define VCO_0   (0x0)
#define VCO_1   (0x1)
#define VCO_2   (0x2)

void vco_set_band_reg_if(pll_reg_t *pll_reg, uint8_t vco_id, uint8_t band, uint8_t fine, bool mux)
{

    uint32_t reg_val = 0x0;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_BAND_VCO0_BAND_MUX, mux);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_BAND_VCO0_BAND, band);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_BAND_VCO0_FINE_MUX, mux);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_BAND_VCO0_FINE, fine);

    switch(vco_id)
    {
        case VCO_0:
            pll_reg->DDR_MVP_PLL_VCO0_BAND = reg_val;
            break;
        case VCO_1:
            pll_reg->DDR_MVP_PLL_VCO1_BAND = reg_val;
            break;
        case VCO_2:
            pll_reg->DDR_MVP_PLL_VCO2_BAND = reg_val;
            break;
        default:
            return;
    }
}

void vco_set_int_frac_reg_if(pll_reg_t *pll_reg, uint8_t vco_id, uint8_t int_comp, uint8_t prop_gain)
{
    uint32_t prop_gain_reg = UPDATE_REG_FIELD(0x0, DDR_MVP_PLL_VCO0_PROP_GAINS_VCO0_PROP_GAIN, prop_gain);
    uint32_t int_frac_reg;

    switch(vco_id)
    {
        case VCO_0:
            pll_reg->DDR_MVP_PLL_VCO0_PROP_GAINS = prop_gain_reg;
            int_frac_reg = pll_reg->DDR_MVP_PLL_VCO0_INT_FRAC_SETTINGS;
            int_frac_reg = UPDATE_REG_FIELD(int_frac_reg, DDR_MVP_PLL_VCO0_INT_FRAC_SETTINGS_VCO0_INT, int_comp);
            pll_reg->DDR_MVP_PLL_VCO0_INT_FRAC_SETTINGS = int_frac_reg;
            break;
        case VCO_1:
            pll_reg->DDR_MVP_PLL_VCO1_PROP_GAINS = prop_gain_reg;
            int_frac_reg = pll_reg->DDR_MVP_PLL_VCO1_INT_FRAC_SETTINGS;
            int_frac_reg = UPDATE_REG_FIELD(int_frac_reg, DDR_MVP_PLL_VCO1_INT_FRAC_SETTINGS_VCO1_INT, int_comp);
            pll_reg->DDR_MVP_PLL_VCO1_INT_FRAC_SETTINGS = int_frac_reg;
            break;
        case VCO_2:
            pll_reg->DDR_MVP_PLL_VCO2_PROP_GAINS = prop_gain_reg;
            int_frac_reg = pll_reg->DDR_MVP_PLL_VCO2_INT_FRAC_SETTINGS;
            int_frac_reg = UPDATE_REG_FIELD(int_frac_reg, DDR_MVP_PLL_VCO2_INT_FRAC_SETTINGS_VCO2_INT, int_comp);
            pll_reg->DDR_MVP_PLL_VCO2_INT_FRAC_SETTINGS = int_frac_reg;
            break;
        default:
            return;
    }
}
void vco_set_post_div_reg_if(pll_reg_t *pll_reg, uint8_t vco_id, uint8_t post_div)
{
    uint32_t reg_val;

    switch(vco_id)
    {
        case VCO_1:
            reg_val = pll_reg->DDR_MVP_PLL_VCO1_CONTROL;
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO1_CONTROL_VCO1_POST_DIV, post_div);
            pll_reg->DDR_MVP_PLL_VCO1_CONTROL = reg_val;
            break;
        case VCO_2:
            reg_val = pll_reg->DDR_MVP_PLL_VCO2_CONTROL;
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO2_CONTROL_VCO2_POST_DIV, post_div);
            pll_reg->DDR_MVP_PLL_VCO2_CONTROL = reg_val;
            break;
        default:
            return;
    }
}

void vco_set_fll_control1_reg_if(pll_reg_t *pll_reg,
                                 uint8_t vco_id,
                                 uint8_t band_start,
                                 uint8_t fine_start,
                                 uint8_t lock_count_threshold)
{
    uint32_t reg_val;
    switch(vco_id)
    {
        case VCO_0:
            reg_val = pll_reg->DDR_MVP_PLL_VCO0_FLL_CONTROL1;
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_FLL_CONTROL1_VCO0_BAND_START, band_start);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_FLL_CONTROL1_VCO0_FINE_START, fine_start);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_FLL_CONTROL1_VCO0_LOCKED_COUNT_THRESHOLD, lock_count_threshold);
            pll_reg->DDR_MVP_PLL_VCO0_FLL_CONTROL1 = reg_val;
            break;
        case VCO_1:
            reg_val = pll_reg->DDR_MVP_PLL_VCO1_FLL_CONTROL1;
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO1_FLL_CONTROL1_VCO1_BAND_START, band_start);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO1_FLL_CONTROL1_VCO1_FINE_START, fine_start);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO1_FLL_CONTROL1_VCO1_LOCKED_COUNT_THRESHOLD, lock_count_threshold);
            pll_reg->DDR_MVP_PLL_VCO1_FLL_CONTROL1 = reg_val;
            break;
        case VCO_2:
            reg_val = pll_reg->DDR_MVP_PLL_VCO2_FLL_CONTROL1;
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO2_FLL_CONTROL1_VCO2_BAND_START, band_start);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO2_FLL_CONTROL1_VCO2_FINE_START, fine_start);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO2_FLL_CONTROL1_VCO2_LOCKED_COUNT_THRESHOLD, lock_count_threshold);
            pll_reg->DDR_MVP_PLL_VCO2_FLL_CONTROL1 = reg_val;
            break;
        default:
            return;
    }
}

void vco_set_fll_control2_reg_if(pll_reg_t *pll_reg,
                                 uint8_t vco_id,
                                 uint8_t fll_refclk_count,
                                 uint8_t fll_range,
                                 uint16_t fll_vco_count_target)
{
    uint32_t reg_val;
    switch(vco_id)
    {
        case VCO_0:
            reg_val = pll_reg->DDR_MVP_PLL_VCO0_FLL_CONTROL2;
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_FLL_CONTROL2_VCO0_FLL_REFCLK_COUNT, fll_refclk_count);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_FLL_CONTROL2_VCO0_FLL_VCO_COUNT_TARGET, fll_vco_count_target);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_FLL_CONTROL2_VCO0_FLL_RANGE, fll_range);
            pll_reg->DDR_MVP_PLL_VCO0_FLL_CONTROL2 = reg_val;
            break;
        case VCO_1:
            reg_val = pll_reg->DDR_MVP_PLL_VCO1_FLL_CONTROL2;
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO1_FLL_CONTROL2_VCO1_FLL_REFCLK_COUNT, fll_refclk_count);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO1_FLL_CONTROL2_VCO1_FLL_VCO_COUNT_TARGET, fll_vco_count_target);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO1_FLL_CONTROL2_VCO1_FLL_RANGE, fll_range);
            pll_reg->DDR_MVP_PLL_VCO1_FLL_CONTROL2 = reg_val;
            break;
        case VCO_2:
            reg_val = pll_reg->DDR_MVP_PLL_VCO2_FLL_CONTROL2;
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO2_FLL_CONTROL2_VCO2_FLL_REFCLK_COUNT, fll_refclk_count);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO2_FLL_CONTROL2_VCO2_FLL_VCO_COUNT_TARGET, fll_vco_count_target);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO2_FLL_CONTROL2_VCO2_FLL_RANGE, fll_range);
            pll_reg->DDR_MVP_PLL_VCO2_FLL_CONTROL2 = reg_val;
            break;
        default:
            return;
    }
}

void vco_set_fll_enable_reg_if(pll_reg_t *pll_reg, uint8_t vco_id, bool enable)
{
    uint32_t reg_val;
    switch(vco_id)
    {
        case VCO_0:
            reg_val = pll_reg->DDR_MVP_PLL_VCO0_FLL_CONTROL1;
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_FLL_CONTROL1_VCO0_FLL_ENABLE, enable);
            pll_reg->DDR_MVP_PLL_VCO0_FLL_CONTROL1 = reg_val;
            break;
        case VCO_1:
            reg_val = pll_reg->DDR_MVP_PLL_VCO1_FLL_CONTROL1;
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO1_FLL_CONTROL1_VCO1_FLL_ENABLE, enable);
            pll_reg->DDR_MVP_PLL_VCO1_FLL_CONTROL1 = reg_val;
            break;
        case VCO_2:
            reg_val = pll_reg->DDR_MVP_PLL_VCO2_FLL_CONTROL1;
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO2_FLL_CONTROL1_VCO2_FLL_ENABLE, enable);
            pll_reg->DDR_MVP_PLL_VCO2_FLL_CONTROL1 = reg_val;
            break;
        default:
            return;
    }
}

void vco_get_fll_band_status_reg_if(pll_reg_t *pll_reg, uint8_t vco_id, uint8_t *band, uint8_t *fine)
{
    uint32_t reg_val;
    switch(vco_id)
    {
        case VCO_0:
            reg_val = pll_reg->DDR_MVP_PLL_VCO0_FLL_BAND_STATUS;
            break;
        case VCO_1:
            reg_val = pll_reg->DDR_MVP_PLL_VCO1_FLL_BAND_STATUS;
            break;
        case VCO_2:
            reg_val = pll_reg->DDR_MVP_PLL_VCO2_FLL_BAND_STATUS;
            break;
        default:
            return;
    }

    *band = GET_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_FLL_BAND_STATUS_VCO0_BAND_STATUS);
    *fine = GET_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_FLL_BAND_STATUS_VCO0_FINE_STATUS);
}

bool vco_is_fll_locked_reg_if(pll_reg_t *pll_reg, uint8_t vco_id)
{
    uint32_t reg_val;

    switch(vco_id)
    {
        case VCO_0:
            reg_val = pll_reg->DDR_MVP_PLL_VCO0_FLL_CONTROL1;
            break;
        case VCO_1:
            reg_val = pll_reg->DDR_MVP_PLL_VCO1_FLL_CONTROL1;
            break;
        case VCO_2:
            reg_val = pll_reg->DDR_MVP_PLL_VCO2_FLL_CONTROL1;
            break;
        default:
            return false;
    }

    return !!GET_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_FLL_CONTROL1_VCO0_LOCKED);
}

void vco_set_enable_reg_if(pll_reg_t *pll_reg, uint8_t vco_id, bool enable)
{
    uint32_t reg_val;
    switch(vco_id)
    {
        case VCO_0:
            reg_val = pll_reg->DDR_MVP_PLL_VCO0_CONTROL;
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_CONTROL_VCO0_ENA, enable);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_CONTROL_VCO0_ENA_MUX, 0x1);
            pll_reg->DDR_MVP_PLL_VCO0_CONTROL = reg_val;
            break;
        case VCO_1:
            reg_val = pll_reg->DDR_MVP_PLL_VCO1_CONTROL;
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO1_CONTROL_VCO1_ENA, enable);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO1_CONTROL_VCO1_ENA_MUX, 0x1);
            pll_reg->DDR_MVP_PLL_VCO1_CONTROL = reg_val;
            break;
        case VCO_2:
            reg_val = pll_reg->DDR_MVP_PLL_VCO2_CONTROL;
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO2_CONTROL_VCO2_ENA, enable);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO2_CONTROL_VCO2_ENA_MUX, 0x1);
            pll_reg->DDR_MVP_PLL_VCO2_CONTROL = reg_val;
            break;
        default:
            return;
    }
}
