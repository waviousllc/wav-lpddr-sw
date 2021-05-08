/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <vco/driver.h>
#include <kernel/io.h>
#include <wddr/memory_map.h>

#define VCO_0   (0x0)
#define VCO_1   (0x1)
#define VCO_2   (0x2)

#define VCO0_BASE_ADDR      (DDR_MVP_PLL_VCO0_BAND__ADR)
#define VCO1_BASE_ADDR      (DDR_MVP_PLL_VCO1_BAND__ADR)
#define VCO2_BASE_ADDR      (DDR_MVP_PLL_VCO2_BAND__ADR)

#define VCO_BAND_OFFSET                         (DDR_MVP_PLL_VCO0_BAND__ADR - VCO0_BASE_ADDR)
#define VCO_PROP_GAIN_OFFSET                    (DDR_MVP_PLL_VCO0_PROP_GAINS__ADR - VCO0_BASE_ADDR)
#define VCO_INT_FRAC_SETTINGS_OFFSET            (DDR_MVP_PLL_VCO0_INT_FRAC_SETTINGS__ADR - VCO0_BASE_ADDR)
#define VCO_CONTROL_OFFSET                      (DDR_MVP_PLL_VCO0_CONTROL__ADR - VCO0_BASE_ADDR)
#define VCO_FLL_CONTROL1_OFFSET                 (DDR_MVP_PLL_VCO0_FLL_CONTROL1__ADR - VCO0_BASE_ADDR)
#define VCO_FLL_CONTROL2_OFFSET                 (DDR_MVP_PLL_VCO0_FLL_CONTROL2__ADR - VCO0_BASE_ADDR)
#define VCO_FLL_BAND_STATUS_OFFSET              (DDR_MVP_PLL_VCO0_FLL_BAND_STATUS__ADR - VCO0_BASE_ADDR)

void vco_init_reg_if(vco_dev_t *vco, uint32_t base)
{
    switch(vco->vco_id)
    {
        case VCO_0:
            vco->base = base + VCO0_BASE_ADDR;
            break;
        case VCO_1:
            vco->base = base + VCO1_BASE_ADDR;
            break;
        case VCO_2:
            vco->base = base + VCO2_BASE_ADDR;
            break;
        default:
            break;
    }
}

void vco_set_band_reg_if(vco_dev_t *vco, uint8_t band, uint8_t fine, bool mux)
{
    uint32_t reg_val = 0x0;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_BAND_VCO0_BAND_MUX, mux);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_BAND_VCO0_BAND, band);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_BAND_VCO0_FINE_MUX, mux);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_BAND_VCO0_FINE, fine);
    reg_write(vco->base + VCO_BAND_OFFSET, reg_val);
}

void vco_set_int_frac_reg_if(vco_dev_t *vco, uint8_t int_comp, uint8_t prop_gain)
{
    uint32_t reg_val;

    // Prop Gain
    reg_val = UPDATE_REG_FIELD(0x0, DDR_MVP_PLL_VCO0_PROP_GAINS_VCO0_PROP_GAIN, prop_gain);
    reg_write(vco->base + VCO_PROP_GAIN_OFFSET, reg_val);

    // Int / Fraction
    reg_val = reg_read(vco->base + VCO_INT_FRAC_SETTINGS_OFFSET);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_INT_FRAC_SETTINGS_VCO0_INT, int_comp);
    reg_write(vco->base + VCO_INT_FRAC_SETTINGS_OFFSET, reg_val);
}

void vco_set_post_div_reg_if(vco_dev_t *vco, uint8_t post_div)
{
    uint32_t reg_val;

    if (vco->vco_id != VCO_0)
    {
        reg_val = reg_read(vco->base + VCO_CONTROL_OFFSET);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO1_CONTROL_VCO1_POST_DIV, post_div);
        reg_write(vco->base + VCO_CONTROL_OFFSET, reg_val);
    }
}

void vco_set_fll_control1_reg_if(vco_dev_t *vco,
                                 uint8_t band_start,
                                 uint8_t fine_start,
                                 uint8_t lock_count_threshold)
{
    uint32_t reg_val;
    reg_val = reg_read(vco->base + VCO_FLL_CONTROL1_OFFSET);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_FLL_CONTROL1_VCO0_BAND_START, band_start);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_FLL_CONTROL1_VCO0_FINE_START, fine_start);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_FLL_CONTROL1_VCO0_LOCKED_COUNT_THRESHOLD, lock_count_threshold);
    reg_write(vco->base + VCO_FLL_CONTROL1_OFFSET, reg_val);
}

void vco_set_fll_control2_reg_if(vco_dev_t *vco,
                                 uint8_t fll_refclk_count,
                                 uint8_t fll_range,
                                 uint16_t fll_vco_count_target)
{
    uint32_t reg_val;
    reg_val = reg_read(vco->base + VCO_FLL_CONTROL2_OFFSET);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_FLL_CONTROL2_VCO0_FLL_REFCLK_COUNT, fll_refclk_count);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_FLL_CONTROL2_VCO0_FLL_VCO_COUNT_TARGET, fll_vco_count_target);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_FLL_CONTROL2_VCO0_FLL_RANGE, fll_range);
    reg_write(vco->base + VCO_FLL_CONTROL2_OFFSET, reg_val);
}

void vco_set_fll_enable_reg_if(vco_dev_t *vco, bool enable)
{
    uint32_t reg_val;
    reg_val = reg_read(vco->base + VCO_FLL_CONTROL1_OFFSET);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_FLL_CONTROL1_VCO0_FLL_ENABLE, enable);
    reg_write(vco->base + VCO_FLL_CONTROL1_OFFSET, reg_val);
}

void vco_get_fll_band_status_reg_if(vco_dev_t *vco, uint8_t *band, uint8_t *fine)
{
    uint32_t reg_val;
    reg_val = reg_read(vco->base + VCO_FLL_BAND_STATUS_OFFSET);
    *band = GET_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_FLL_BAND_STATUS_VCO0_BAND_STATUS);
    *fine = GET_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_FLL_BAND_STATUS_VCO0_FINE_STATUS);
}

bool vco_is_fll_locked(vco_dev_t *vco)
{
    uint32_t reg_val;
    reg_val = reg_read(vco->base + VCO_FLL_CONTROL1_OFFSET);
    return GET_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_FLL_CONTROL1_VCO0_LOCKED) == 0x1;
}

void vco_set_enable_reg_if(vco_dev_t *vco, bool enable)
{
    uint32_t reg_val;
    reg_val = reg_read(vco->base + VCO_CONTROL_OFFSET);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_CONTROL_VCO0_ENA, enable);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_MVP_PLL_VCO0_CONTROL_VCO0_ENA_MUX, 0x1);
    reg_write(vco->base + VCO_CONTROL_OFFSET, reg_val);
}
