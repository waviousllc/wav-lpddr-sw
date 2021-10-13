/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DFI_INTF_H_
#define _DFI_INTF_H_

#include "dfi_reg.h"
#include <wddr/interface.h>

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
