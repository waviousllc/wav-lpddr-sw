/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _CMN_IBIAS_H_
#define _CMN_IBIAS_H_

#include "cmn_reg.h"

/**
 * @brief   Bias Current Device State
 *
 * DISABLE     Bias Current Device disabled.
 * ENABLE      Bias Current Device enabled.
 */
typedef enum ibias_state_t
{
    IBIAS_STATE_DISABLE,
    IBIAS_STATE_ENABLE,
} ibias_state_t;

/**
 * @brief   Common Bias Current Set State Register Interface
 *
 * @details Sets the state of the bias current.
 *
 * @param[in]   cmn_reg     pointer to CTRL register space.
 * @param[in]   state       state requested to change to.
 *
 * @return  void
 */
void cmn_ibias_set_state_reg_if(cmn_reg_t *cmn_reg, ibias_state_t state);

#endif /* _CMN_IBIAS_H_ */
