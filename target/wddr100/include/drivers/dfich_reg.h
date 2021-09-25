/**
* Copyright (c) 2021 Wavious LLC.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef _DFICH_REG_H_
#define _DFICH_REG_H_

/* Standard includes. */
#include <stdint.h>
#include "ddr_dfich_csr.h"

/**
 * @brief   DFICH Register Layout
 *
 * @details Layout of register address space of DFICH CSRs. This structure should
 *          be "overlayed" by pointing to the base address of DFICH register
 *          space in the memory map.
 *
 * @note    See hardware documentation for information about these registers.
 */
typedef struct dfich_reg
{
    volatile uint32_t DDR_DFICH_TOP_1_CFG;
    volatile uint32_t DDR_DFICH_TOP_2_CFG;
    volatile uint32_t DDR_DFICH_TOP_3_CFG;
    volatile uint32_t DDR_DFICH_TOP_STA;
    volatile uint32_t DDR_DFICH_IG_DATA_CFG;
    volatile uint32_t DDR_DFICH_EG_DATA_STA;
    volatile uint32_t DDR_DFICH_WRC_CFG[2];
    volatile uint32_t DDR_DFICH_WRCCTRL_CFG[2];
    volatile uint32_t DDR_DFICH_CKCTRL_CFG[2];
    volatile uint32_t DDR_DFICH_RDC_CFG[2];
    volatile uint32_t DDR_DFICH_RCTRL_CFG[2];
    volatile uint32_t DDR_DFICH_WCTRL_CFG[2];
    volatile uint32_t DDR_DFICH_WENCTRL_CFG[2];
    volatile uint32_t DDR_DFICH_WCKCTRL_CFG[2];
    volatile uint32_t DDR_DFICH_WRD_CFG[2];
    volatile uint32_t DDR_DFICH_RDD_CFG[2];
    volatile uint32_t DDR_DFICH_CTRL0_CFG[2];
    volatile uint32_t DDR_DFICH_CTRL1_CFG[2];
    volatile uint32_t DDR_DFICH_CTRL2_CFG[2];
    volatile uint32_t DDR_DFICH_CTRL3_CFG[2];
    volatile uint32_t DDR_DFICH_CTRL4_CFG[2];
    volatile uint32_t DDR_DFICH_CTRL5_CFG[2];
} dfich_reg_t;

#endif /* _DFICH_REG_H_ */
