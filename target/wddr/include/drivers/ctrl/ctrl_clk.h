/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _CTRL_CLK_H_
#define _CTRL_CLK_H_

#include <stdbool.h>
#include <ctrl_reg.h>

/**
 * @brief   MCU Clock Glitch Free Mux Select Enumeration
 *
 * REFCLK       MCU Clock Source is Refclk.
 * PLL_VCO0     MCU Clock Source is PLL VCO0.
 */
typedef enum clk_mcu_gfm_sel_t
{
    CLK_MCU_GFM_SEL_REFCLK,
    CLK_MCU_GFM_SEL_PLL_VCO0,
} clk_mcu_gfm_sel_t;

/**
 * @brief   Control Clock Set PLL Clock Enable Register Interface
 *
 * @details Sets PLL clock enable.
 *
 * @param[in]   ctrl_reg    pointer to CTRL register space.
 * @param[in]   enable      flag to indicate if pll clock should be enabled.
 *
 * @return      void
 */
void ctrl_clk_set_pll_clk_en_reg_if(ctrl_reg_t *ctrl_reg, bool enable);

/**
 * @brief   Control Clock Set MCU GFM Select Register Interface
 *
 * @details Sets MCU GFM Select.
 *
 * @param[in]   ctrl_reg    pointer to CTRL register space.
 * @param[in]   sel         desired MCU GFM clock select value.
 *
 * @return      void
 */
void ctrl_clk_set_mcu_gfm_sel_reg_if(ctrl_reg_t *ctrl_reg,
                                     clk_mcu_gfm_sel_t sel);

/**
 * @brief   Control Clock Get MCU GFM Select Status Register Interface
 *
 * @details Gets MCU GFM selection.
 *
 * @param[in]   ctrl_reg    pointer to CTRL register space.
 * @param[in]   gfm_sel0    pointer to store gfm_sel0 value.
 * @param[in]   gfm_sel1    pointer to store gfm_sel1 value.
 *
 * @return      void
 */
void ctrl_clk_get_mcu_gfm_sel_status_reg_if(ctrl_reg_t *ctrl_reg,
                                            uint8_t *gfm_sel0,
                                            uint8_t *gfm_sel1);
/**
 * @brief   Control Clock Get DFI Clock Status Register Interface
 *
 * @details Gets DFI Clock status.
 *
 * @param[in]   ctrl_reg    pointer to CTRL register space.
 * @param[in]   dfi_clk_on  pointer to store status of DFI clock.
 *
 * @return      void
 */
void ctrl_clk_get_dfi_clk_status_reg_if(ctrl_reg_t *ctrl_reg,
                                        uint8_t *dfi_clk_on);

#endif /* _CTRL_CLK_H_ */
