/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DFI_UPDATE_FSM_H_
#define _DFI_UPDATE_FSM_H_

/* Kernel includes. */
#include <stdint.h>

/* FreeRTOS includes. */
#include <FreeRTOS.h>
#include <timers.h>

/* Kernel includes. */
#include <kernel/fsm.h>

/* WDDR includes. */
#include <error.h>

/** @brief IOCAL Update PHY Interface Function */
typedef void (*iocal_update_phy_fn_t)(void *dev);

/** @brief IOCAL calibrate Interface Function */
typedef void (*iocal_calibrate_fn_t)(void *dev);

/**
 * @brief   DFI Update State Enumerations
 *
 * IDLE             Idle State (default).
 * REQ              Signal to PHY that IOCAL needs to be updated.
 * CTRLUPD_WAIT     Waiting for CTRLUPD REQ deassertion.
 * PHYUPD_WAIT      Waiting for PHYUPD ACK response from MC to proceed.
 * CAL              Memory Controller initiated Update Request. Update IOCAL value.
 * UPDATE           Update IOCAL.
 * UPDATE_EXIT      Indicate through PHY Update complete.
 */
typedef enum dfi_update_state_t
{
    DFI_UPDATE_STATE_IDLE,
    DFI_UPDATE_STATE_REQ,
    DFI_UPDATE_STATE_CTRLUPD_WAIT,
    DFI_UPDATE_STATE_PHYUPD_WAIT,
    DFI_UPDATE_STATE_CAL,
    DFI_UPDATE_STATE_UPDATE,
    DFI_UPDATE_STATE_UPDATE_EXIT
} dfi_update_state_t;

/**
 * @brief   DFI PHYUPD Type Enumerations
 *
 * TYPE_0   Value corresponding to type 0 PHYUPD timer.
 * TYPE_1   Value corresponding to type 1 PHYUPD timer.
 * TYPE_2   Value corresponding to type 2 PHYUPD timer.
 * TYPE_3   Value corresponding to type 3 PHYUPD timer.
 */
typedef enum dfi_phyupd_type_t
{
    DFI_PHYUPD_TYPE_0,
    DFI_PHYUPD_TYPE_1,
    DFI_PHYUPD_TYPE_2,
    DFI_PHYUPD_TYPE_3
} dfi_phyupd_type_t;

/**
 * @brief   DFI Update FSM Stucture
 *
 * fsm                      Underlying FSM instance.
 * intf.iocal_update_phy    Function that will update IOCAL values in the PHY.
 * intf.iocal_calibrate     Function that will perform IOCAL calibration.
 * intf.dev                 Generic pointer to IOCAL callback data.
 */
typedef struct dfi_update_fsm_t
{
    fsm_t               fsm;
    struct
    {
        iocal_update_phy_fn_t   iocal_update_phy;
        iocal_calibrate_fn_t    iocal_calibrate;
        void                    *dev;
    } intf;
} dfi_update_fsm_t;

/**
 * @brief   DFI Update FSM Intialization
 *
 * @details Initializes the DFI Update finite state machine.
 *
 * @param[in]   fsm                 pointer to DFI Update FSM instance.
 * @param[in]   dev                 Generic pointer to IOCAL function callback data.
 * @param[in]   iocal_update_phy    Function that will update IOCAL values in the PHY.
 * @param[in]   iocal_calibrate     Function that will perform IOCAL calibration.
 *
 * @return      void
 */
void dfi_update_fsm_init(dfi_update_fsm_t *fsm,
                         void *dev,
                         const iocal_update_phy_fn_t iocal_update_phy,
                         const iocal_calibrate_fn_t iocal_calibrate);

/**
 * @brief   DFI Update Event Request Update
 *
 * @details External event function for requesting an update to the IOCAL value
 *          in the PHY.
 *          NOTE: A succesful return value doesn't mean state machine moved
 *          to the correct state. The state machine implementation might decide
 *          it can't move to this state. The caller must check current state of
 *          the fsm to determine if call did what was intended.
 *
 * @param[in]   fsm     pointer to DFI Update FSM instance.
 * @param[in]   type    DFI PHYUPD type enumeration value to be used.
 *
 * @return      returns if DFI Update FSM successfully invoked.
 * @retval      WDDR_SUCCESS if FSM successfully invoked.
 * @retval      WDDR_ERROR if FSM not in correct state to invoke FSM.
 */
wddr_return_t dfi_update_event_req_update(dfi_update_fsm_t *fsm, dfi_phyupd_type_t type);

#endif /* _DFI_UPDATE_FSM_H_ */
