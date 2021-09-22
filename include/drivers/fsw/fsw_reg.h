/**
* Copyright (c) 2021 Wavious LLC.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef _FSW_REG_H_
#define _FSW_REG_H_

/* Standard includes. */
#include <stdint.h>

/**
 * @brief   FSW Register Layout
 *
 * @details Layout of register address space of FSW CSRs. This structure should
 *          be "overlayed" by pointing to the base address of FSW register space
 *          in the memory map.
 *
 * @note    See hardware documentation for information about these registers.
 */
typedef struct fsw_reg
{
    volatile uint32_t DDR_FSW_CTRL_CFG;
    volatile uint32_t DDR_FSW_CTRL_STA;
    volatile uint32_t DDR_FSW_DEBUG_CFG;
    volatile uint32_t reserved0[8];
    volatile uint32_t DDR_FSW_CSP_0_CFG;
    volatile uint32_t DDR_FSW_CSP_1_CFG;
    volatile uint32_t DDR_FSW_CSP_STA;
} fsw_reg_t;

#endif /* _FSW_REG_H_ */
