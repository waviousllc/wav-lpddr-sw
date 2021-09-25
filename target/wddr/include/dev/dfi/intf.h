/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DFI_INTF_DEV_H_
#define _DFI_INTF_DEV_H_

#include <dfi/device.h>

/**
 * @brief   DFI PHY Master Request
 *
 * @details Request Memory Controller through DFI interface for PHY to have
 *          Master control of the DRAM.
 *
 * @param[in]   dfi     pointer to DFI device.
 * @param[in]   req     PHYMSTR request parameters.
 *
 * @return      void.
 */
void dfi_phymstr_req(dfi_dev_t *dfi, dfi_phymstr_req_t *req);

/**
 * @brief   DFI PHY Master Exit
 *
 * @details Release PHY control of DRAM back to the Memory Controller.
 *
 * @param[in]   dfi     pointer to DFI device.
 *
 * @return      void.
 */
void dfi_phymstr_exit(dfi_dev_t *dfi);

/**
 * @brief   DFI Control Update Deassert Acknowledge
 *
 * @details Deassert Acknowledge indicating that IOCAL Update has been performed
 *          via CTRLUPD ACK interface.
 *
 * @param[in]   dfi     pointer to DFI device.
 *
 * @return      void.
 */
void dfi_ctrlupd_deassert_ack(dfi_dev_t *dfi);

/**
 * @brief   DFI Control Update Enable
 *
 * @details Allows the PHY to handle all CTRLUPD Requests from the Memory
 *          Controller.
 *
 * @param[in]   dfi     pointer to DFI device.
 *
 * @return      void.
 */
void dfi_ctrlupd_enable(dfi_dev_t *dfi);

/**
 * @brief   DFI Control Update Disable
 *
 * @details Forces the PHY to ignore all CTRLUPD Requests from the Memory
 *          Controller.
 *
 * @param[in]   dfi     pointer to DFI device.
 *
 * @return      void.
 */
void dfi_ctrlupd_disable(dfi_dev_t *dfi);

/**
 * @brief   DFI PHY Update Request
 *
 * @details Request Memory Controller through DFI PHYUPD interface to allow
 *          PHY to update PHY CSRs.
 *
 * @param[in]   dfi     pointer to DFI device.
 * @param[in]   type    PHYUPD type requested.
 *
 * @return      void.
 */
void dfi_phyupd_req(dfi_dev_t *dfi, dfi_phyupd_type_t type);

/**
 * @brief   DFI PHY Update Exit
 *
 * @details Signals to Memory Controller that PHY is done with previously
 *          requested update.
 *
 * @param[in]   dfi     pointer to DFI device.
 *
 * @return      void.
 */
void dfi_phyupd_exit(dfi_dev_t *dfi);

#endif /* _DFI_INTF_DEV_H_ */
