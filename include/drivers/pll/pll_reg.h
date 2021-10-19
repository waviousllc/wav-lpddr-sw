/**
* Copyright (c) 2021 Wavious LLC.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef _MVP_PLL_REG_H_
#define _MVP_PLL_REG_H_

/* Standard includes. */
#include <stdint.h>

/**
 * @brief   PLL Register Layout
 *
 * @details Layout of register address space of PLL CSRs. This structure
 *          should be "overlayed" by pointing to the base address of PLL
 *          register space in the memory map.
 *
 * @note    See hardware documentation for information about these registers.
 */
typedef struct pll_reg
{
    volatile uint32_t DDR_MVP_PLL_CORE_OVERRIDES;
    volatile uint32_t DDR_MVP_PLL_CORE_SWTICH_VCO;
    volatile uint32_t DDR_MVP_PLL_CORE_SWTICH_VCO_HW;
    volatile uint32_t DDR_MVP_PLL_CORE_STATUS;
    volatile uint32_t DDR_MVP_PLL_CORE_STATUS_INT;
    volatile uint32_t DDR_MVP_PLL_CORE_STATUS_INT_EN;
    volatile uint32_t DDR_MVP_PLL_VCO0_BAND;
    volatile uint32_t DDR_MVP_PLL_VCO0_CONTROL;
    volatile uint32_t DDR_MVP_PLL_VCO0_FLL_CONTROL1;
    volatile uint32_t DDR_MVP_PLL_VCO0_FLL_CONTROL2;
    volatile uint32_t DDR_MVP_PLL_VCO0_FLL_CONTROL3;
    volatile uint32_t DDR_MVP_PLL_VCO0_FLL_BAND_STATUS;
    volatile uint32_t DDR_MVP_PLL_VCO0_FLL_COUNT_STATUS;
    volatile uint32_t DDR_MVP_PLL_VCO0_INT_FRAC_SETTINGS;
    volatile uint32_t DDR_MVP_PLL_VCO0_PROP_GAINS;
    volatile uint32_t DDR_MVP_PLL_VCO1_BAND;
    volatile uint32_t DDR_MVP_PLL_VCO1_CONTROL;
    volatile uint32_t DDR_MVP_PLL_VCO1_FLL_CONTROL1;
    volatile uint32_t DDR_MVP_PLL_VCO1_FLL_CONTROL2;
    volatile uint32_t DDR_MVP_PLL_VCO1_FLL_CONTROL3;
    volatile uint32_t DDR_MVP_PLL_VCO1_FLL_BAND_STATUS;
    volatile uint32_t DDR_MVP_PLL_VCO1_FLL_COUNT_STATUS;
    volatile uint32_t DDR_MVP_PLL_VCO1_INT_FRAC_SETTINGS;
    volatile uint32_t DDR_MVP_PLL_VCO1_PROP_GAINS;
    volatile uint32_t DDR_MVP_PLL_VCO1_SSC_CONTROLS;
    volatile uint32_t DDR_MVP_PLL_VCO2_BAND;
    volatile uint32_t DDR_MVP_PLL_VCO2_CONTROL;
    volatile uint32_t DDR_MVP_PLL_VCO2_FLL_CONTROL1;
    volatile uint32_t DDR_MVP_PLL_VCO2_FLL_CONTROL2;
    volatile uint32_t DDR_MVP_PLL_VCO2_FLL_CONTROL3;
    volatile uint32_t DDR_MVP_PLL_VCO2_FLL_BAND_STATUS;
    volatile uint32_t DDR_MVP_PLL_VCO2_FLL_COUNT_STATUS;
    volatile uint32_t DDR_MVP_PLL_VCO2_INT_FRAC_SETTINGS;
    volatile uint32_t DDR_MVP_PLL_VCO2_PROP_GAINS;
    volatile uint32_t DDR_MVP_PLL_VCO2_SSC_CONTROLS;
    volatile uint32_t DDR_MVP_PLL_STATE_MACHINE_CONTROLS;
    volatile uint32_t DDR_MVP_PLL_STATE_MACHINE_CONTROLS2;
    volatile uint32_t DDR_MVP_PLL_INTR_GAINS;
    volatile uint32_t DDR_MVP_PLL_INTR_PROP_FL_GAINS;
    volatile uint32_t DDR_MVP_PLL_INTR_PROP_GAINS_OVERRIDE;
    volatile uint32_t DDR_MVP_PLL_LOCK_DET_SETTINGS;
    volatile uint32_t DDR_MVP_PLL_FASTLOCK_DET_SETTINGS;
    volatile uint32_t DDR_MVP_PLL_ANALOG_EN_RESET;
    volatile uint32_t DDR_MVP_PLL_MODE_DTST_MISC;
    volatile uint32_t DDR_MVP_PLL_PROP_CTRLS;
    volatile uint32_t DDR_MVP_PLL_REFCLK_CONTROLS;
    volatile uint32_t DDR_MVP_PLL_CLKGATE_DISABLES;
} pll_reg_t;

#endif /* _PLL_REG_H_ */
