/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _LPDE_TABLE_H_
#define _LPDE_TABLE_H_

#include <stdint.h>

/**
 * @brief   LPDE Configuration Structure
 *
 * delay  configured delay value.
 * gear   Configured gear value.
 */
typedef union lpde_cfg_t
{
    struct
    {
        uint32_t delay: 6;
        const uint32_t gear: 3;
    };
    uint32_t val;
} lpde_cfg_t;

#endif /* _LPDE_TABLE_H_ */
