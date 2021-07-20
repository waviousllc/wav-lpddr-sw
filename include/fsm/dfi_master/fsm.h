/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DFI_MASTER_FSM_H_
#define _DFI_MASTER_FSM_H_

/* Standard includes. */
#include <stdint.h>

/* FreeRTOS includes. */
#include <FreeRTOS.h>
#include <timers.h>

/* Kernel includes. */
#include <kernel/fsm.h>

/* LPDDR includes. */
#include <error.h>

/**
 * @brief   DFI Master State Enumerations
 *
 * IDLE     Idle State (default).
 * REQ      Signal to MC (via PHY) that PHY wants master control of DRAM.
 * WAIT     Waiting for ack from MC.
 * MASTER   PHY has master control of DRAM.
 * EXIT     Signal to MC (via PHY) that PHY is giving control back to MC.
 */
typedef enum dfi_master_state_t
{
    DFI_MASTER_STATE_IDLE,
    DFI_MASTER_STATE_REQ,
    DFI_MASTER_STATE_WAIT,
    DFI_MASTER_STATE_MASTER,
    DFI_MASTER_STATE_EXIT,
} dfi_master_state_t;

/**
 * @brief   DFI Master Type Enumerations
 *
 * @details Signals to MC which type of MASTER REQ is being performed.
 *          This is system defined and signals expected amount of time that
 *          MC will relinquish control of DRAM to PHY.
 *
 * TYPE_0   Value corresponding to type 0 PHYMSTR timer.
 * TYPE_1   Value corresponding to type 1 PHYMSTR timer.
 * TYPE_2   Value corresponding to type 2 PHYMSTR timer.
 * TYPE_3   Value corresponding to type 3 PHYMSTR timer.
 */
typedef enum dfi_master_type_t
{
    DFI_MASTER_TYPE_0,
    DFI_MASTER_TYPE_1,
    DFI_MASTER_TYPE_2,
    DFI_MASTER_TYPE_3,
} dfi_master_type_t;

/**
 * @brief   DFI Master Chipselect State Enumerations
 *
 * @details Indicates which ranks are active / inactive for PHY Master
 *          control.
 *
 * ACTIVE       active while PHY is master.
 * INACTIVE     inactive while PHY is master.
 */
typedef enum dfi_master_cs_state_t
{
    DFI_MASTER_CS_STATE_ACTIVE = 0,
    DFI_MASTER_CS_STATE_INACTIVE = 1,
} dfi_master_cs_state_t;

/**
 * @brief   DFI Master State Select Enumerations
 *
 * @details Indicates which state the MC should place DRAM
 *          into. Only valid for Ranks where cs_state=0.
 *
 * IDLE         MC should place DRAM in IDLE state.
 * REFRESH      MC should place DRAM in REFRESH state.
 */
typedef enum dfi_master_state_sel_t
{
    DFI_MASTER_STATE_SEL_IDLE = 0,
    DFI_MASTER_STATE_SEL_REFRESH = 1,
} dfi_master_state_sel_t;

/**
 * @brief   DFI Master Request Structure
 *
 * type         request type indicating requested time.
 * state_sel    request state DRAM should be placed in.
 * cs_state     request rank active/inactive state.
 */
typedef struct dfi_master_req_t
{
    dfi_master_type_t type;
    dfi_master_state_sel_t state_sel;
    uint8_t cs_state;
} dfi_master_req_t;

/**
 * @brief   DFI Master FSM Structure
 *
 * fsm      underlying generic FSM structure.
 * timer    Watchdog timer handle.
 */
typedef struct dfi_master_fsm_t
{
    fsm_t           fsm;
    TimerHandle_t   timer;
} dfi_master_fsm_t;

/**
 * @brief   DFI Master FSM Intialization
 *
 * @details Initializes the DFI Master finite state machine.
 *
 * @param[in]   fsm     pointer to DFI Master FSM instance.
 *
 * @return      void
 */
void dfi_master_fsm_init(dfi_master_fsm_t *fsm);

/**
 * @brief   DFI Master Event Request
 *
 * @details External event to issue a PHY Master request to the MC.
 *          NOTE: A succesful return value doesn't mean state machine moved
 *          to the correct state. The state machine implementation might decide
 *          it can't move to this state. The caller must check current state of
 *          the fsm to determine if call did what was intended.
 *
 * @param[in]   fsm     pointer to DFI Master FSM instance.
 * @param[in]   req     DFI Master request data for interface signals.
 *
 * @return      returns if DFI Master FSM successfully invoked.
 * @retval      WDDR_SUCCESS if FSM successfully invoked.
 * @retval      WDDR_ERROR if FSM not in correct state to invoke FSM.
 */
wddr_return_t dfi_master_event_request(dfi_master_fsm_t *fsm, dfi_master_req_t *req);

/**
 * @brief   DFI Master Event Exit
 *
 * @details External relinquish control of DRAM back to MC.
 *          NOTE: A succesful return value doesn't mean state machine moved
 *          to the correct state. The state machine implementation might decide
 *          it can't move to this state. The caller must check current state of
 *          the fsm to determine if call did what was intended.
 *
 * @param[in]   fsm     pointer to DFI Master FSM instance.
 *
 * @return      returns if DFI Master FSM successfully invoked.
 * @retval      WDDR_SUCCESS if FSM successfully invoked.
 * @retval      WDDR_ERROR if FSM not in correct state to invoke FSM.
 */
wddr_return_t dfi_master_event_exit(dfi_master_fsm_t *fsm);

#endif /* _DFI_MASTER_FSM_H_ */
