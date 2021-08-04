/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DFI_DEV_H_
#define _DFI_DEV_H_

#include <dfi/table.h>
#include <dfi/buffer_device.h>

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
 * @brief   DFI Device Structure
 *
 * @details DFI Device structure that agregrates all DFI components.
 *
 * base         base address of DFI device.
 * msr          current MSR configuration of DFI device.
 * dfi_buffer   DFI Buffer device.
 */
typedef struct dfi_dev_t
{
    uint32_t            base;
    wddr_msr_t          msr;
    dfi_buffer_dev_t    dfi_buffer;
} dfi_dev_t;

/**
 * @brief   DFI Device Initialzation
 *
 * @details Initializes DFI Device at device level.
 *
 * @param[in]   dfi     pointer to DFI device.
 * @param[in]   base    base address of DFI device.
 *
 * @return      void
 */
void dfi_init(dfi_dev_t *dfi, uint32_t base);

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

#endif /* _DFI_DEV_H_ */
