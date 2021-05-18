/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _PLL_FSM_H_
#define _PLL_FSM_H_

#include <kernel/fsm.h>
#include <pll/device.h>

/** @brief  PLL FSM Structure */
typedef fsm_t pll_fsm_t;

/**
 * @brief   Possible PLL FSM states
 *
 * NOT_LOCKED       PLL is not locked.
 * PREP             PLL in prep process.
 * PREP_DONE        PLL prep completed.
 * SWITCH           PLL in switch process.
 * INIT_SWITCH_DONE PLL switch done.
 * LOCKED           PLL is phase locked.
 */
typedef enum pll_state_t
{
    PLL_STATE_NOT_LOCKED,
    PLL_STATE_PREP,
    PLL_STATE_PREP_DONE,
    PLL_STATE_SWITCH,
    PLL_STATE_INIT_SWITCH_DONE,
    PLL_STATE_LOCKED,
} pll_state_t;

/**
 * @brief   PLL FSM Prep Data Structure
 *
 * freq_id  Next desired frequency to prep.
 * cal      Pointer to PLL calibration data.
 * cfg      Pointer to PLL configuration data.
 */
typedef struct pll_prep_data_t
{
    uint8_t freq_id;
    pll_freq_cal_t *cal;
    pll_freq_cfg_t *cfg;
} pll_prep_data_t;

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
 * @brief   PLL FSM Prep Event
 *
 * @details Preparess the PLL for a switch. Only valid to call if FSM is in
 *          NOT_LOCKED or LOCKED state.
 *
 * @param[in]   fsm     pointer to PLL FSM.
 * @param[in]   data    pointer to data needed for Prep event.
 *
 * @return      void
 */
void pll_fsm_prep_event(pll_fsm_t *fsm, pll_prep_data_t *data);

/**
 * @brief   PLL FSM Switch Event
 *
 * @details Switches PLL VCO. Only valid to call if FSM is in PREP state.
 *
 * @param[in]   fsm         pointer to PLL FSM.
 *
 * @return      void
 */
void pll_fsm_switch_event(pll_fsm_t *fsm);

/**
 * @brief   PLL FSM Current Frequency
 *
 * @details Returns the current operating frequency of the DDR PHY.
 *
 * @param fsm       pointer to PLL FSM.
 * @param freq_id   pointer to fill in with current Frequency.
 *
 * @return          void.
 */
void pll_fsm_get_current_freq(pll_fsm_t *fsm, uint8_t *freq_id);

/**
 * @brief   PLL FSM Next Frequency
 *
 * @details Returns the next operating frequency of the DDR PHY.
 *
 * @param fsm       pointer to PLL FSM.
 * @param freq_id   pointer to fill in with next Frequency.
 *                  freq_id will be UNDEFINED_FREQ_ID if next VCO is
 *                  not prepped.
 *
 * @return          void.
 */
void pll_fsm_get_next_freq(pll_fsm_t *fsm, uint8_t *freq_id);

/**
 * @brief   PLL FSM Current VCO ID
 *
 * @details Returns the current VCO ID of the DDR PHY.
 *
 * @param fsm       pointer to PLL FSM.
 * @param vco_id    pointer to fill in with current VCO ID.
 *
 * @return          void.
 */
void pll_fsm_get_current_vco_id(pll_fsm_t *fsm, uint8_t *vco_id);

/**
 * @brief   PLL FSM Next VCO ID
 *
 * @details Returns the next VCO ID of the DDR PHY.
 *
 * @param fsm       pointer to PLL FSM.
 * @param vco_id    pointer to fill in with next VCO ID.
 *                  vco_id will be UNDEFINED_VCO_ID if next VCO is
 *                  not prepped.
 *
 * @return          void.
 */
void pll_fsm_get_next_vco_id(pll_fsm_t *fsm, uint8_t *vco_id);

#endif /* _PLL_FSM_H_ */
