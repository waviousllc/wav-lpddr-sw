/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _CMN_CLK_H_
#define _CMN_CLK_H_

#include <stdbool.h>
#include <cmn_reg.h>

/**
 * @brief   Common Clock Control Set PLL0 Clock Divider Reset Enable Register Interface
 *
 * @details Sets whether the PLL0 clock divider reset is enabled.
 *
 * @param[in]   cmn_reg     pointer to CTRL register space.
 * @param[in]   enable      flag to indicate if pll clock divider
 *                          reset is enabled.
 *
 * @return      void
 */
void cmn_clk_ctrl_set_pll0_div_clk_rst_reg_if(cmn_reg_t *cmn_reg, bool enable);

/**
 * @brief   Clock Common Control Set GFCM Enable Register Interface
 *
 * @details Sets GFCM Enable via CSR.
 *
 * @param[in]   cmn_reg     pointer to CTRL register space.
 * @param[in]   enable      flag to indicate if GFM should be enabled.
 *
 * @return      void
 */
void cmn_clk_ctrl_set_gfcm_en_reg_if(cmn_reg_t *cmn_reg, bool enable);

/**
 * @brief   Clock Common Control Set PLL0 Clock Divider Enable Register Interface
 *
 * @details Sets PLL0 Clock Divider Enable via CSR.
 *
 * @param[in]   cmn_reg     pointer to CTRL register space.
 * @param[in]   enable      flag to indicate if pll clock divider should be
 *                          enabled.
 *
 * @return      void
 */
void cmn_clk_ctrl_set_pll0_div_clk_en_reg_if(cmn_reg_t *cmn_reg, bool enable);

#endif /* _CMN_CLK_H_ */
