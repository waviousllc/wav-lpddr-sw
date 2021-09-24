/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DFI_INTF_H_
#define _DFI_INTF_H_

#include "dfi_reg.h"

/**
 * @brief   DFI PHYMSTR Type Enumerations
 *
 * @details Signals to MC which type of PHYMSTR REQ is being performed.
 *          This is system defined and signals expected amount of time that
 *          MC will relinquish control of DRAM to PHY.
 *
 * TYPE_0   Value corresponding to type 0 PHYMSTR timer.
 * TYPE_1   Value corresponding to type 1 PHYMSTR timer.
 * TYPE_2   Value corresponding to type 2 PHYMSTR timer.
 * TYPE_3   Value corresponding to type 3 PHYMSTR timer.
 */
typedef enum dfi_phymstr_type
{
    DFI_MASTER_TYPE_0,
    DFI_MASTER_TYPE_1,
    DFI_MASTER_TYPE_2,
    DFI_MASTER_TYPE_3,
} dfi_phymstr_type_t;

/**
 * @brief   DFI PHYMSTR Chipselect State Enumerations
 *
 * @details Indicates which ranks are active / inactive for PHYMSTR
 *          control.
 *
 * ACTIVE       active while PHY is master.
 * INACTIVE     inactive while PHY is master.
 */
typedef enum dfi_phymstr_cs_state
{
    DFI_MASTER_CS_STATE_ACTIVE = 0,
    DFI_MASTER_CS_STATE_INACTIVE = 1,
} dfi_phymstr_cs_state_t;

/**
 * @brief   DFI PHYMSTR State Select Enumerations
 *
 * @details Indicates which state the MC should place DRAM
 *          into. Only valid for Ranks where cs_state=0.
 *
 * IDLE         MC should place DRAM in IDLE state.
 * REFRESH      MC should place DRAM in REFRESH state.
 */
typedef enum dfi_phymstr_state_sel
{
    DFI_MASTER_STATE_SEL_IDLE = 0,
    DFI_MASTER_STATE_SEL_REFRESH = 1,
} dfi_phymstr_state_sel_t;

/**
 * @brief   DFI PHYMSTR Request Structure
 *
 * type         request type indicating requested time.
 * state_sel    request state DRAM should be placed in.
 * cs_state     request rank active/inactive state.
 */
typedef struct dfi_phymstr_req
{
    dfi_phymstr_type_t type;
    dfi_phymstr_state_sel_t state_sel;
    dfi_phymstr_cs_state_t cs_state;
} dfi_phymstr_req_t;

/**
 * @brief   DFI PHYUPD Type Enumerations
 *
 * TYPE_0   Value corresponding to type 0 PHYUPD timer.
 * TYPE_1   Value corresponding to type 1 PHYUPD timer.
 * TYPE_2   Value corresponding to type 2 PHYUPD timer.
 * TYPE_3   Value corresponding to type 3 PHYUPD timer.
 */
typedef enum dfi_phyupd_type
{
    DFI_PHYUPD_TYPE_0,
    DFI_PHYUPD_TYPE_1,
    DFI_PHYUPD_TYPE_2,
    DFI_PHYUPD_TYPE_3
} dfi_phyupd_type_t;

/**
 * @brief   DFI Set Init Complete Override Register Interface
 *
 * @details Set init_complete interface override.
 *
 *
 * @param[in]   dfi_reg     pointer to DFI register space.
 * @param[in]   override    flag to indicate if SW should override.
 * @param[in]   val         value to override.
 *
 * @return      void
 */
void dfi_set_init_complete_ovr_reg_if(dfi_reg_t *dfi_reg,
                                      bool override,
                                      uint8_t val);

/**
 * @brief   DFI Set Init Start Override Register Interface
 *
 * @details Set init_start interface override.
 *
 *
 * @param[in]   dfi_reg     pointer to DFI register space.
 * @param[in]   override    flag to indicate if SW should override.
 * @param[in]   val         value to override.
 *
 * @return      void
 */
void dfi_set_init_start_ovr_reg_if(dfi_reg_t *dfi_reg,
                                   bool override,
                                   uint8_t val);

/**
 * @brief   DFI Get Init Start Status Register Interface
 *
 * @details Get init_start status
 *
 * @param[in]   dfi_reg     pointer to DFI register space.
 *
 * @return  returns status of init_start.
 * @retval  0 if init_start is deasserted (low).
 * @retval  1 if init_start is asserted (high).
 */
uint8_t dfi_get_init_start_status_reg_if(dfi_reg_t *dfi_reg);

/**
 * @brief   DFI Get Init Complete Status Register Interface
 *
 * @details Get init_complete status
 *
 * @param[in]   dfi_reg     pointer to DFI register space.
 *
 * @return  returns status of init_start.
 * @retval  0 if init_start is deasserted (low).
 * @retval  1 if init_start is asserted (high).
 */
uint8_t dfi_get_init_complete_status_reg_if(dfi_reg_t *dfi_reg);

/**
 * @brief   DFI PHYMSTR Request Assert Register Interface
 *
 * @details Asserts PHYMSTR REQ to the Memory Controller.
 *
 * @param[in]   dfi_reg     pointer to DFI register space.
 * @param[in]   req         pointer to PHYMSTR request parameters.
 *
 * @return      void.
 */
void dfi_phymstr_req_assert_reg_if(dfi_reg_t *dfi_reg,
                                   dfi_phymstr_req_t *req);
/**
 * @brief   DFI PHYMSTR Request Deassert Register Interface
 *
 * @details Deasserts PHYMSTR REQ to the Memory Controller.
 *
 * @param[in]   dfi_reg     pointer to DFI register space.
 *
 * @return      void.
 */
void dfi_phymstr_req_deassert_reg_if(dfi_reg_t *dfi_reg);

/**
 * @brief   DFI CTRLUPD Deassert Acknowledge Register Interface
 *
 * @details Deasserts CTRLUPD ACK to the Memory Controller.
 *
 * @param[in]   dfi_reg     pointer to DFI register space.
 *
 * @return      void.
 */
void dfi_ctrlupd_deassert_ack_reg_if(dfi_reg_t *dfi_reg);

/**
 * @brief   DFI CTRLUPD ACK Override Register Interface
 *
 * @details Overrides acknowledge signal of DFI CTRLUPD interface.
 *
 * @param[in]   dfi_reg     pointer to DFI register space.
 * @param[in]   override    flag to indicate if SW should override.
 * @param[in]   val         value to override.
 *
 * @return      void
 */
void dfi_ctrlupd_ack_override_reg_if(dfi_reg_t *dfi_reg,
                                     bool override,
                                     uint8_t val);

/**
 * @brief   DFI PHYUPD Request Assert Register Interface
 *
 * @details Asserts REQ signal of DFI PHYUPD interface. Signals to Memory
 *          Controller that IOCAL Update is required.
 *
 * @param[in]   dfi_reg     pointer to DFI register space.
 * @param[in]   type        type of PHYUPD requested.
 *
 * @return      void.
 */
void dfi_phyupd_req_assert_reg_if(dfi_reg_t *dfi_reg,
                                  dfi_phyupd_type_t type);

/**
 * @brief   DFI PHYUPD Request Deassert Register Interface
 *
 * @details Deasserts PHYUPD REQ to the Memory Controller.
 *
 * @param[in]   dfi_reg     pointer to DFI register space.
 *
 * @return      void.
 */
void dfi_phyupd_req_deassert_reg_if(dfi_reg_t *dfi_reg);

#endif /* _DFI_INTF_H_ */
