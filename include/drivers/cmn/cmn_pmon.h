/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _CMN_PMON_H_
#define _CMN_PMON_H_

#include "cmn_reg.h"

/**
 * @brief PMON State Enumerations
 *
 * DISABLED     PMON Disabled.
 * ENABLED      PMON Enabled.
 */
typedef enum pmon_state_t
{
    PMON_STATE_DISABLED,
    PMON_STATE_ENABLED,
} pmon_state_t;

/**
 * @brief   Common Process Monitor Set State Register Interface
 *
 * @details Sets the process monitor state.
 *
 * @param[in]   cmn_reg     pointer to CTRL register space.
 * @param[in]   state       state requested to change to.
 *
 * @return  void.
 */
void cmn_pmon_set_state_reg_if(cmn_reg_t *cmn_reg, pmon_state_t state);

/**
 * @brief   Common Process Monitor Configure Register Interface
 *
 * @details Configures process monitor.
 *
 * @param[in]   cmn_reg     pointer to CTRL register space.
 * @param[in]   refcount    number of refclks to sample for.
 * @param[in]   init_wait   number of refclks to wait before starting.
 *
 * @return      void
 */
void cmn_pmon_configure_reg_if(cmn_reg_t *cmn_reg,
                               uint16_t refcount,
                               uint8_t init_wait);

/**
 * @brief   Common Process Monitor Get Status Count Register Interface
 *
 * @details Gets status count of process monitor.
 *
 * @param[in]   cmn_reg     pointer to CTRL register space.
 * @param[out]  count       pointer to store current status count.
 *
 * @return  void
 */
void cmn_pmon_get_status_count_reg_if(cmn_reg_t *cmn_reg, uint32_t *count);

#endif /* _CMN_PMON_H_ */
