/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _LPDE_DEV_H_
#define _LPDE_DEV_H_

#include <stdint.h>

/**
 * @brief   LPDE State Enumeration
 *
 * DISABLED    Disabled state.
 * ENABLED     Enabled state.
 */
typedef enum lpde_state_t
{
    LPDE_STATE_DISABLED,
    LPDE_STATE_ENABLED,
} lpde_state_t;

/**
 * @brief   LP Delay Element Structure
 *
 * base    Base address of the LPDE device.
 */
typedef struct lp_delay_element_dev_t
{
    uint32_t    base;
} lpde_dev_t;

#endif /* _LPDE_DEV_H_ */
