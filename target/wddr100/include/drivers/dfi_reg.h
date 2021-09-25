/**
* Copyright (c) 2021 Wavious LLC.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef _DFI_REG_H_
#define _DFI_REG_H_

/* Standard includes. */
#include <stdint.h>
#include "ddr_dfi_csr.h"

/**
 * @brief   DFI Register Layout
 *
 * @details Layout of register address space of DFI CSRs. This structure should
 *          be "overlayed" by pointing to the base address of DFI register
 *          space in the memory map.
 *
 * @note    See hardware documentation for information about these registers.
 */
typedef struct dfi_reg
{
    volatile uint32_t DDR_DFI_TOP_0_CFG;
    volatile uint32_t DDR_DFI_DATA_BIT_ENABLE_CFG;
    volatile uint32_t DDR_DFI_PHYFREQ_RANGE_CFG;
    volatile uint32_t DDR_DFI_STATUS_IF_CFG;
    volatile uint32_t DDR_DFI_STATUS_IF_STA;
    volatile uint32_t DDR_DFI_STATUS_IF_EVENT_0_CFG;
    volatile uint32_t DDR_DFI_STATUS_IF_EVENT_1_CFG;
    volatile uint32_t DDR_DFI_CTRLUPD_IF_CFG;
    volatile uint32_t DDR_DFI_CTRLUPD_IF_STA;
    volatile uint32_t DDR_DFI_CTRLUPD_IF_EVENT_0_CFG;
    volatile uint32_t DDR_DFI_CTRLUPD_IF_EVENT_1_CFG;
    volatile uint32_t DDR_DFI_LP_CTRL_IF_CFG;
    volatile uint32_t DDR_DFI_LP_CTRL_IF_STA;
    volatile uint32_t DDR_DFI_LP_CTRL_IF_EVENT_0_CFG;
    volatile uint32_t DDR_DFI_LP_CTRL_IF_EVENT_1_CFG;
    volatile uint32_t DDR_DFI_LP_DATA_IF_CFG;
    volatile uint32_t DDR_DFI_LP_DATA_IF_STA;
    volatile uint32_t DDR_DFI_LP_DATA_IF_EVENT_0_CFG;
    volatile uint32_t DDR_DFI_LP_DATA_IF_EVENT_1_CFG;
    volatile uint32_t DDR_DFI_PHYUPD_IF_CFG;
    volatile uint32_t DDR_DFI_PHYUPD_IF_STA;
    volatile uint32_t DDR_DFI_PHYMSTR_IF_CFG;
    volatile uint32_t DDR_DFI_PHYMSTR_IF_STA;
    volatile uint32_t DDR_DFI_DEBUG_CFG;
} dfi_reg_t;

#endif /* _DFI_REG_H_ */
