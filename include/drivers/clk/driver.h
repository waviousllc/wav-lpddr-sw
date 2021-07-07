/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#ifndef _CLK_DRIVER_H_
#define _CLK_DRIVER_H_

#include <stdbool.h>
#include <stdint.h>

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
 * @brief   Clock Control Set PLL Clock Enable Register Interface
 *
 * @details Sets PLL Clock enable via CSR.
 *
 * @param[in]   enable  desired PLL clock enable value.
 *
 * @return      void
 */
void clk_ctrl_set_pll_clk_en_reg_if(bool enable);

/**
 * @brief   Clock Control Set MCU GFM Select Register Interface
 *
 * @details Sets MCU GFM Select via CSR.
 *
 * @param[in]   sel     desired MCU GFM clock select value.
 *
 * @return      void
 */
void clk_ctrl_set_mcu_gfm_sel_reg_if(clk_mcu_gfm_sel_t sel);

/**
 * @brief   Clock Common Control Set PLL0 Clock Divider Reset Register Interface
 *
 * @details Sets PLL0 Clock Divider Reset via CSR.
 *
 * @param[in]   reset   desired PLL0 clock divider reset value.
 *                      true indicates PLL0 clock divider is held in reset.
 *                      false indicates PLL0 clock divider is out of reset.
 *
 * @return      void
 */
void clk_cmn_ctrl_set_pll0_div_clk_rst_reg_if(bool reset);

/**
 * @brief   Clock Common Control Set GFCM Enable Register Interface
 *
 * @details Sets GFCM Enable via CSR.
 *
 * @param[in]   enable  desired GFCM enable value.
 *
 * @return      void
 */
void clk_cmn_ctrl_set_gfcm_en_reg_if(bool enable);

/**
 * @brief   Clock Common Control Set PLL0 Clock Divider Enable Register Interface
 *
 * @details Sets PLL0 Clock Divider Enable via CSR.
 *
 * @param[in]   enable  desired PLL0 clock divider enable value.
 *
 * @return      void
 */
void clk_cmn_ctrl_set_pll0_div_clk_en_reg_if(bool enable);

#endif /* _CLK_DRIVER_H_ */
