/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _CMN_ZQCAL_H_
#define _CMN_ZQCAL_H_

#include "cmn_reg.h"
#include <zqcal/table.h>

/**
 * @brief   ZQCAL State Enumeration
 *
 * DISABLED    Disabled state.
 * ENABLED     Enabled state.
 */
typedef enum zqcal_state_t
{
    ZQCAL_STATE_DISABLED,
    ZQCAL_STATE_ENABLED
} zqcal_state_t;

/**
 * @brief   ZQCAL Mode Enumeration
 *
 * ZQCAL_MODE_PULL_UP      Pull up (p side).
 * ZQCAL_MODE_PULL_DOWN    Pull down (n side).
 */
typedef enum zqcal_mode_t
{
    ZQCAL_MODE_PULL_UP,
    ZQCAL_MODE_PULL_DOWN
} zqcal_mode_t;

/**
 * @brief   Common ZQCAL Set State Register Interface
 *
 * @details Sets the state of ZQCAL.
 *
 * @param[in]   cmn_reg     pointer to CTRL register space.
 * @param[in]   state       state to set.
 *
 * @return      void.
 */
void cmn_zqcal_set_state_reg_if(cmn_reg_t *cmn_reg, zqcal_state_t state);

/**
 * @brief   Common ZQCAL Set Mode Register Interface
 *
 * @details Sets the mode of ZQCAL.
 *
 * @param[in]   cmn_reg     pointer to CTRL register space.
 * @param[in]   mode        mode to set.
 *
 * @return      void.
 */
void cmn_zqcal_set_mode_reg_if(cmn_reg_t *cmn_reg, zqcal_mode_t mode);

/**
 * @brief   Common ZQCAL Set Code Register Interface
 *
 * @details Sets the code of ZQCAL given current mode.
 *
 * @param[in]   cmn_reg     pointer to CTRL register space.
 * @param[in]   mode        mode of zqcal. Determines if P or N code is set.
 * @param[in]   code        code to set.
 *
 * @return      void.
 */
void cmn_zqcal_set_code_reg_if(cmn_reg_t *cmn_reg, zqcal_mode_t mode, uint8_t code);

/**
 * @brief   Common ZQCAL Set VOH Register Interface
 *
 * @details Sets the VOH of ZQCAL.
 *
 * @param[in]   cmn_reg     pointer to CTRL register space.
 * @param[in]   voh         voh to set.
 *
 * @return      void.
 */
void cmn_zqcal_set_voh_reg_if(cmn_reg_t *cmn_reg, zqcal_voh_t voh);

/**
 * @brief   Common ZQCAL Get Comparator Ouptut Register Interface
 *
 * @details Gets the comparator output result of ZQCAL.
 *
 * @param[in]   cmn_reg     pointer to CTRL register space.
 * @param[out]  val         pointer of where to store comparator value.
 *
 * @return      void
 */
void cmn_zqcal_get_output_reg_if(cmn_reg_t *cmn_reg, uint8_t *val);

#endif /* _CMN_ZQCAL_H_ */
