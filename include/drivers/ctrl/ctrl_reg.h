/**
* Copyright (c) 2021 Wavious LLC.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef _CTRL_REG_H_
#define _CTRL_REG_H_

/* Standard includes. */
#include <stdint.h>

/**
 * @brief   Control Register Layout
 *
 * @details Layout of register address space of CTRL CSRs. This structure should
 *          be "overlayed" by pointing to the base address of CTRL register
 *          space in the memory map.
 *
 * @note    See hardware documentation for information about these registers.
 */
typedef struct ctrl_reg
{
    volatile uint32_t DDR_CTRL_CLK_CFG;
    volatile uint32_t DDR_CTRL_CLK_STA;
    volatile uint32_t DDR_CTRL_AHB_SNOOP_CFG;
    volatile uint32_t DDR_CTRL_AHB_SNOOP_STA;
    volatile uint32_t DDR_CTRL_AHB_SNOOP_DATA_STA;
    volatile uint32_t DDR_CTRL_AHB_SNOOP_PATTERN_CFG;
    volatile uint32_t DDR_CTRL_AHB_SNOOP_PATTERN_0_CFG;
    volatile uint32_t DDR_CTRL_AHB_SNOOP_PATTERN_1_CFG;
    volatile uint32_t DDR_CTRL_AHB_SNOOP_PATTERN_STA;
    volatile uint32_t DDR_CTRL_DEBUG_CFG;
    volatile uint32_t DDR_CTRL_DEBUG1_CFG;
} ctrl_reg_t;

#endif /* _CTRL_REG_H_ */
