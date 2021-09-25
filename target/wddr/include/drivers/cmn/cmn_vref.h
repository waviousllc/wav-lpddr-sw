/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _CMN_VREF_H_
#define _CMN_VREF_H_

#include <cmn_reg.h>
#include <vref/table.h>
#include <wddr/phy_config.h>

/**
 * @brief   Voltage Reference State Enumeration
 *
 * DISABLED    Disabled state.
 * HIZ         Hi-Z state.
 * ENABLED     Enabled state.
 */
typedef enum vref_state_t
{
    VREF_STATE_DISABLED,
    VREF_STATE_HIZ,
    VREF_STATE_ENABLED
} vref_state_t;

/**
 * @brief   Common Voltage Reference Set State Register Interface
 *
 * @details Sets the state of the voltage reference.
 *
 * @param[in]   cmn_reg     pointer to CTRL register space.
 * @param[in]   msr         which msr register set to use.
 * @param[in]   state       state to set.
 *
 * @return      void.
 */
void cmn_vref_set_state_reg_if(cmn_reg_t *cmn_reg,
                               wddr_msr_t msr,
                               vref_state_t state);

/**
 * @brief   Common Voltage Reference Set Code Register Interface
 *
 * @details Sets the code (level) of the voltage reference.
 *
 * @param[in]   cmn_reg     pointer to CTRL register space.
 * @param[in]   msr         which msr register set to use.
 * @param[in]   code        code to set.
 *
 * @return      void.
 */
void cmn_vref_set_code_reg_if(cmn_reg_t *cmn_reg,
                              wddr_msr_t msr,
                              uint32_t code);

/**
 * @brief   Common Voltage Reference Set Power Mode Register Interface
 *
 * @details Sets the power mode of the voltage reference.
 *
 * @param[in]   cmn_reg     pointer to CTRL register space.
 * @param[in]   msr         which msr register set to use.
 * @param[in]   pwr_mode    power mode to set.
 *
 * @return      void.
 */
void cmn_vref_set_pwr_mode_reg_if(cmn_reg_t *cmn_reg,
                                  wddr_msr_t msr,
                                  vref_pwr_mode_t pwr_mode);

#endif /* _CMN_VREF_H_ */
