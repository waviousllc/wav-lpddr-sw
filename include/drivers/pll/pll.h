/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _PLL_H_
#define _PLL_H_

#include <stdbool.h>
#include "pll_reg.h"

/**
 * @brief   Phase Lock Loop (PLL) Reset Register Interface
 *
 * @details Takes PLL device out of reset.
 *
 * @param[in]   pll_reg     pointer to the PLL register space.
 *
 * @return      void
 */
void pll_reset_reg_if(pll_reg_t *pll_reg);

/**
 * @brief   Phase Lock Loop (PLL) Set VCO Selection Register Interface
 *
 * @details Sets the VCO that PLL should move to on next switch.
 *
 * @param[in]   pll_reg     pointer to the PLL register space.
 * @param[in]   vco_id      id of the VCO device to select.
 *
 * @return      void
 */
void pll_set_vco_sel_reg_if(pll_reg_t *pll_reg, uint8_t vco_id);

/**
 * @brief   Phase Lock Loop (PLL) Switch VCO Register Interface
 *
 * @details Performs switch of PLL to next VCO configured via
 *          pll_set_vco_sel_reg_if.
 *
 * @param[in]   pll_reg     pointer to the PLL register space.
 *
 * @return      void
 */
void pll_switch_vco_reg_if(pll_reg_t *pll_reg);

/**
 * @brief   Phase Lock Loop (PLL) Enable Loss Lock Interrupt Register Interface
 *
 * @details Sets the interrupt state for Loss of Lock interrupt.
 *
 * @param[in]   pll_reg     pointer to the PLL register space.
 * @param[in]   enable      state of loss lock interrupt.
 *
 * @return      void.
 */
void pll_enable_loss_lock_interrupt_reg_if(pll_reg_t *pll_reg,
                                           bool enable);

/**
 * @brief   Phase Lock Loop (PLL) Enable Switch Interrupt Register Interface
 *
 * @details Sets the interrupt state for switch interrupt.
 *
 * @param[in]   pll_reg     pointer to the PLL register space.
 * @param[in]   enable      state of init lock interrupt.
 *
 * @return      void.
 */
void pll_enable_switch_interrupt_reg_if(pll_reg_t *pll_reg,
                                        bool enable);

/**
 * @brief   Phase Lock Loop (PLL) Enable Lock Interrupt Register Interface
 *
 * @details Sets the interrupt state for the Lock interrupt.
 *
 * @param[in]   pll_reg     pointer to the PLL register space.
 * @param[in]   enable      state of lock interrupt.
 *
 * @return      void.
 */
void pll_enable_lock_interrupt_reg_if(pll_reg_t *pll_reg,
                                      bool enable);

/**
 * @brief   Phase Lock Loop (PLL) Clear Interrupt Register Interface
 *
 * @details Clears the interrupt state of the PLL.
 *
 * @param[in]   pll_reg     pointer to the PLL register space.
 *
 * @return      PLL interrupt status.
 */
uint32_t pll_clear_interrupt_reg_if(pll_reg_t *pll_reg);

/**
 * @brief   Phase Lock Loop (PLL) Wait Until Core Ready Register Interface
 *
 * @details Blocks via polling until the PLL Core reports that it is ready.
 *
 * @param[in]   pll_reg     pointer to the PLL register space.
 *
 * @return      void.
 */
void pll_wait_until_core_ready_reg_if(pll_reg_t *pll_reg);

#endif /* _PLL_H_ */
