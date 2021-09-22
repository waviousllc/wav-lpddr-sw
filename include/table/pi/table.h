/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _PI_TABLE_H_
#define _PI_TABLE_H_

#include <stdint.h>

/**
 * @brief   Phase Interpolator Configuration Structure
 *
 * code     Configured code value.
 * gear     Configured gear value.
 * xcpl     Configured xcpl value.
 */
typedef union pi_cfg_t
{
    struct
    {
        uint32_t code:6;
        const uint32_t gear:4;
        const uint32_t xcpl:1;
    };
    uint32_t val;
} pi_cfg_t;

#endif /* _PI_TABLE_H_ */
