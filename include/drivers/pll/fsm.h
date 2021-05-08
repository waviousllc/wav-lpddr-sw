/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _PLL_FSM_H_
#define _PLL_FSM_H_

#include <pll/device.h>

/**
 * @brief   Possible PLL FSM states
 *
 * NOT_LOCKED   PLL is not locked.
 * INIT_LOCKED  PLL is frequency locked.
 * LOCKED       PLL is phase locked.
 */
typedef enum pll_state_t
{
    PLL_STATE_NOT_LOCKED,
    PLL_STATE_INIT_LOCKED,
    PLL_STATE_LOCKED,
} pll_state_t;

/**
 * @brief   PLL FSM Initialization
 *
 * @details Initializes PLL FSM.
 *
 * @param[in]   fsm     pointer to PLL FSM.
 * @param[in]   pll     pointer to PLL device.
 *
 * @return      void
 */
void pll_fsm_init(pll_fsm_t *fsm, pll_dev_t *pll);

/**
 * @brief   PLL FSM Reset Event
 *
 * @details Resets the PLL FSM. Typically used when a software switch of VCOs
 *          is performed. The PLL logic doesn't indicate a loss of lock event
 *          when VCOs are switched so this function can be used to model that
 *          event in software.
 *
 * @param[in]   fsm     pointer to PLL FSM.
 *
 * @return      void
 */
void pll_fsm_reset_event(pll_fsm_t *fsm);

#endif /* _PLL_FSM_H_ */
