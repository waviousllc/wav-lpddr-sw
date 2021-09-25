/**
* Copyright (c) 2021 Wavious LLC.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef _CMN_REG_H_
#define _CMN_REG_H_

/* Standard includes. */
#include <stdint.h>
#include "ddr_cmn_csr.h"

/**
 * @brief   Common Register Layout
 *
 * @details Layout of register address space of CMN CSRs. This structure should
 *          be "overlayed" by pointing to the base address of CMN register
 *          space in the memory map.
 *
 * @note    See hardware documentation for information about these registers.
 */
typedef struct cmn_reg
{
    volatile uint32_t reserved0[2];
    volatile uint32_t DDR_CMN_VREF_CFG[2];
    volatile uint32_t DDR_CMN_ZQCAL_CFG;
    volatile uint32_t DDR_CMN_ZQCAL_STA;
    volatile uint32_t DDR_CMN_IBIAS_CFG;
    volatile uint32_t DDR_CMN_TEST_CFG;
    volatile uint32_t DDR_CMN_LDO_CFG[2];
    volatile uint32_t DDR_CMN_CLK_CTRL_CFG;
    volatile uint32_t reserved1[3];
    volatile uint32_t DDR_CMN_PMON_ANA_CFG;
    volatile uint32_t DDR_CMN_PMON_DIG_CFG;
    volatile uint32_t DDR_CMN_PMON_DIG_NAND_CFG;
    volatile uint32_t DDR_CMN_PMON_DIG_NOR_CFG;
    volatile uint32_t DDR_CMN_PMON_NAND_STA;
    volatile uint32_t DDR_CMN_PMON_NOR_STA;
    volatile uint32_t DDR_CMN_CLK_STA;
    volatile uint32_t DDR_CMN_RSTN_CFG;
    volatile uint32_t DDR_CMN_RSTN_STA;
} cmn_reg_t;

#endif /* _CMN_REG_H_ */
