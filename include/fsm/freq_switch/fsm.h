/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _FREQ_SWITCH_FSM_H_
#define _FREQ_SWITCH_FSM_H_

/* Standard includes. */
#include <stdint.h>

/* FreeRTOS includes. */
#include <FreeRTOS.h>
#include <timers.h>

/* Kernel includes. */
#include <kernel/fsm.h>

/* LPDDR includes. */
#include <pll/fsm.h>

// Forward Declaration
struct fs_fsm_t;

/** @brief  Frequency Switch Init Complete Callback Definition */
typedef void (*init_complete_cb_t)(struct fs_fsm_t *fsm, void *args);

/**
 * @brief   Frequency Switch FSM Stucture
 *
 * fsm                      Underlying FSM instance.
 * timer                    Watchdog timer handle.
 * init_complete_cb         Init Complete Callback Function.
 * init_complete_cb_args    Init Complete Callback Function Arguments.
 * hw_switch_only           Flag for not accepting SW switch requests.
 */
typedef struct fs_fsm_t
{
    fsm_t               fsm;
    TimerHandle_t       timer;
    init_complete_cb_t  init_complete_cb;
    void                *init_complete_cb_args;
    bool                hw_switch_only;
} fs_fsm_t;

/**
 * @brief   Frequency Switch FSM Prep Data Structure
 *
 * msr          Next desired MSR to switch to.
 * prep_data    Pointer to prep data to pass to PLL FSM.
 */
typedef struct fs_prep_data_t
{
    uint8_t         msr;
    pll_prep_data_t prep_data;
} fs_prep_data_t;

/**
 * @brief   Frequency Switch State Enumerations
 *
 * IDLE             Idle state (default).
 * FAIL             Fail state when switch fails.
 * PREP_SWITCH      State when prepping to switch.
 * WAIT_FOR_SWITCH  State when waiting for switch to happen.
 * SWITCH           State when switching (software switch).
 * POST_SWITCH      State when switching has completed.
 * WAIT_FOR_LOCK    State when waiting for PLL lock to happen.
 */
typedef enum freq_switch_state_t
{
    FS_STATE_IDLE,
    FS_STATE_FAIL,
    FS_STATE_PREP_SWITCH,
    FS_STATE_WAIT_FOR_SWITCH,
    FS_STATE_SWITCH,
    FS_STATE_POST_SWITCH,
    FS_STATE_WAIT_FOR_LOCK,
} freq_switch_state_t;

/**
 * @brief   Frequency Switch FSM Initialization
 *
 * @details Initializes the Frequency Switch finite state machine.
 *
 * @param[in]   fsm   pointer to Frequency Switch FSM.
 * @param[in]   pll   pointer to PLL device structure.
 *
 * @return      void.
 */
void freq_switch_fsm_init(fs_fsm_t *fsm, pll_fsm_t *pll);

/**
 * @brief   Frequency Switch Event Prep
 *
 * @details External event function for prepping for a frequency switch.
 *          NOTE: A succesful return value doesn't mean state machine moved
 *          to the FREQ_PREP state. The state machine implementation might
 *          decide it can't move to this state. The caller must check current
 *          state of the fsm to determine if call did what was intended.
 *
 * @param[in]   fsm       pointer to Frequency Switch FSM.
 * @param[in]   prep_data pointer to Frequency Switch prep data structure.
 *
 * @return      returns if Frequency Switch FSM successfully invoked.
 * @retval      WDDR_SUCCESS if FSM successfully invoked.
 * @retval      WDDR_ERROR if FSM not in correct state to invoke FSM.
 */
wddr_return_t freq_switch_event_prep(fs_fsm_t *fsm, fs_prep_data_t *prep_data);

/**
 * @brief   Frequency Switch Event Set
 *
 * @details External event function for a frequency switch (in software).
 *          NOTE: A succesful return value doesn't mean state machine moved
 *          to the SWITCH state. The state machine implementation might decide
 *          it can't move to this state. The caller must check current state
 *          of the fsm to determine if call did what was intended.
 *
 * @param[in]   fsm   pointer to Frequency Switch FSM.
 *
 * @return      returns if Frequency Switch FSM successfully invoked.
 * @retval      WDDR_SUCCESS if FSM successfully invoked.
 * @retval      WDDR_ERROR if FSM not in correct state to invoke FSM.
 */
wddr_return_t freq_switch_event_sw_switch(fs_fsm_t *fsm);

/**
 * @brief   Frequency Switch FSM HW Switch Mode
 *
 * @details Sets the Frequency Switch FSM to only accept HW switches.
 *          All SW switch requests will return WDDR_ERROR once this is called.
 *
 * @param[in]   fsm     pointer to Frequency Switch FSM.
 *
 * @return      returns if successfully switch to hw freq switch only mode.
 * @retval      WDDR_SUCCESS if FSM successfully completed switch mode.
 * @retval      WDDR_ERROR if hardware not in correct state to switch.
 */
wddr_return_t freq_switch_event_hw_switch_mode(fs_fsm_t *fsm);

/**
 * @brief   Frequency Switch FSM Register Init Complete Callback Function
 *
 * @details Sets callback function to be executed before releasing
 *          Init Complete acknowledge.
 *
 * @param[in]   fsm       pointer to Frequency Switch FSM.
 * @param[in]   cb        function to be executed before releasing init complete.
 * @param[in]   args      pointer to arguments to be passed to callback function.
 *
 * @return          void.
 */
void freq_switch_register_init_complete_callback(fs_fsm_t *fsm, const init_complete_cb_t cb, void *args);

#endif /* _FREQ_SWITCH_FSM_H_ */
