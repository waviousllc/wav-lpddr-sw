/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#ifndef _PI_DEV_H_
#define _PI_DEV_H_

#include <stdint.h>

/**
 * @brief   PI State Enumeration
 *
 * DISABLED    Disabled state.
 * ENABLED     Enabled state.
 */
typedef enum pi_state_t
{
    PI_STATE_DISABLED,
    PI_STATE_ENABLED,
} pi_state_t;

/**
 * @brief Phase Interpolator Structure
 *
 * base    base address of the Phase Interpolator.
 */
typedef struct phase_interpolator_dev_t
{
    uint32_t    base;
} pi_dev_t;

#endif /* _PI_DEV_H_ */
