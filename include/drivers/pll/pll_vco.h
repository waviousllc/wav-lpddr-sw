/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _PLL_VCO_H_
#define _PLL_VCO_H_

#include <stdbool.h>
#include "pll_reg.h"

/**
 * @brief   VCO Set Band Register Interface
 *
 * @details Sets band, fine band, and mux settings for the given VCO.
 *
 * @param[in]   pll_reg     pointer to the PLL register space.
 * @param[in]   vco_id      id of the VCO to set.
 * @param[in]   band        band value to set.
 * @param[in]   fine        fine band value to set.
 * @param[in]   mux         mux value to set.
 *
 * @return      void
 */
void vco_set_band_reg_if(pll_reg_t *pll_reg,
                         uint8_t vco_id,
                         uint8_t band,
                         uint8_t fine,
                         bool mux);

/**
 * @brief   VCO Set Integer Fractional Register Interface
 *
 * @details Sets integer comparison and proportional gain settings for the VCO
 *          device.
 *
 * @param[in]   pll_reg     pointer to the PLL register space.
 * @param[in]   vco_id      id of the VCO to set.
 * @param[in]   int_comp    integer comparison value to set.
 * @param[in]   prop_gain   proportional gain value to set.
 *
 * @return      void
 */
void vco_set_int_frac_reg_if(pll_reg_t *pll_reg,
                             uint8_t vco_id,
                             uint8_t int_comp,
                             uint8_t prop_gain);

/**
 * @brief   VCO Set Post Divider Register Interface
 *
 * @details Sets post-divider settings for the VCO device.
 *
 * @param[in]   pll_reg     pointer to the PLL register space.
 * @param[in]   vco_id      id of the VCO to set.
 * @param[in]   post_div    post divider value to set.
 *
 * @return      void
 */
void vco_set_post_div_reg_if(pll_reg_t *pll_reg,
                             uint8_t vco_id,
                             uint8_t post_div);

/**
 * @brief   VCO Set FLL Control1 Register Interface
 *
 * @details Sets FLL control1 settings for the VCO device.
 *
 * @param[in]   pll_reg                 pointer to the PLL register space.
 * @param[in]   vco_id                  id of the VCO to set.
 * @param[in]   band_start              band start value to set.
 *                                      Used for calibration.
 * @param[in]   fine_start              fine band start value to set.
 *                                      Used for calibration.
 * @param[in]   lock_count_threshold    threshold value to set.
 *                                      Used for calibration.
 *
 * @return      void
 */
void vco_set_fll_control1_reg_if(pll_reg_t *pll_reg,
                                 uint8_t vco_id,
                                 uint8_t band_start,
                                 uint8_t fine_start,
                                 uint8_t lock_count_threshold);

/**
 * @brief   VCO Set FLL Control2 Register Interface
 *
 * @details Sets FLL control2 settings for the VCO device.
 *
 * @param[in]   pll_reg                 pointer to the PLL register space.
 * @param[in]   vco_id                  id of the VCO to set.
 * @param[in]   fll_refclk_count        refclk_count value to set.
 * @param[in]   fll_range               range value to set.
 * @param[in]   fll_vco_count_target    target value to set.
 *
 * @return      void
 */
void vco_set_fll_control2_reg_if(pll_reg_t *pll_reg,
                                 uint8_t vco_id,
                                 uint8_t fll_refclk_count,
                                 uint8_t fll_range,
                                 uint16_t fll_vco_count_target);

/**
 * @brief   VCO Set FLL Enable Register Interface
 *
 * @details Enables / Disables FLL of VCO device.
 *
 * @param[in]   pll_reg     pointer to the PLL register space.
 * @param[in]   vco_id      id of the VCO to set.
 * @param[in]   enable      flag to indicate if FLL should be enabled.
 *
 * @return      void
 */
void vco_set_fll_enable_reg_if(pll_reg_t *pll_reg,
                               uint8_t vco_id,
                               bool enable);

/**
 * @brief   VCO Get FLL Band Status Register Interface
 *
 * @details Gets the Band and Fine Band settings for the VCO device. This is
 *          used to get calibrated band and fine band settings.
 *
 * @param[in]   pll_reg     pointer to the PLL register space.
 * @param[in]   vco_id      id of the VCO to set.
 * @param[out]  band        pointer to store band value.
 * @param[out]  fine        pointer to store fine band value.
 *
 * @return      void
 */
void vco_get_fll_band_status_reg_if(pll_reg_t *pll_reg,
                                    uint8_t vco_id,
                                    uint8_t *band,
                                    uint8_t *fine);

/**
 * @brief   VCO IS FLL Locked Register Interface
 *
 * @details Returns whether VCO is in FLL lock state. Used during calibration.
 *
 * @param[in]   pll_reg     pointer to the PLL register space.
 * @param[in]   vco_id      id of the VCO to get status from.
 *
 * @return      Returns whether VCO device is FLL locked.
 * @retval      true, if locked.
 * @retval      false if not.
 */
bool vco_is_fll_locked_reg_if(pll_reg_t *pll_reg, uint8_t vco_id);

/**
 * @brief   VCO Set Enable Register Interface
 *
 * @details Enables / Disables VCO device.
 *
 * @param[in]   pll_reg     pointer to the PLL register space.
 * @param[in]   vco_id      id of the VCO to set.
 * @param[in]   enable      flag to indicate if VCO should be enabled.
 *
 * @return      void
 */
void vco_set_enable_reg_if(pll_reg_t *pll_reg,
                           uint8_t vco_id,
                           bool enable);

#endif /* _PLL_VCO_H_ */
