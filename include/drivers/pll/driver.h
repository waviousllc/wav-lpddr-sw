/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _PLL_DRIVER_H_
#define _PLL_DRIVER_H_
#include <stdbool.h>
#include <pll/device.h>

/**
 * @brief   Phase Lock Loop (PLL) Initialization Register Interface
 *
 * @details Initializes PLL device at register interface level.
 *
 * @param[in]   pll     pointer to PLL device.
 * @param[in]   base    base address of PLL device.
 *
 * @return      void
 */
void pll_init_reg_if(pll_dev_t *pll, uint32_t base);

/**
 * @brief   Phase Lock Loop (PLL) Reset Register Interface
 *
 * @details Takes PLL device out of reset.
 *
 * @param[in]   pll     pointer to PLL device.
 *
 * @return      void
 */
void pll_reset_reg_if(pll_dev_t *pll);

/**
 * @brief   Phase Lock Loop (PLL) Set VCO Selection Register Interface
 *
 * @details Sets the VCO that PLL should move to on next switch.
 *
 * @param[in]   pll     pointer to PLL device.
 * @param[in]   vco_id  id of the VCO device to select.
 *
 * @return      void
 */
void pll_set_vco_sel_reg_if(pll_dev_t *pll, vco_index_t vco_id);

/**
 * @brief   Phase Lock Loop (PLL) Switch VCO Register Interface
 *
 * @details Performs switch of PLL to next VCO configured via
 *          pll_set_vco_sel_reg_if.
 *
 * @param[in]   pll     pointer to PLL device.
 *
 * @return      void
 */
void pll_switch_vco_reg_if(pll_dev_t *pll);

#endif /* _PLL_DRIVER_H_ */
