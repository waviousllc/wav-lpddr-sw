/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _PMON_TABLE_H_
#define _PMON_TABLE_H_

#include <stdint.h>

/**
 * @brief   Process Monitor Configuration Structure
 *
 * target          Target count (target freq/refclk freq).
 * refclk_count    number of refclks to sammple for.
 * init_wait       number of refclks to wait before starting.
 */
typedef struct process_monitor_cfg_t
{
    const uint32_t target;
    const uint16_t refclk_count;
    const uint8_t  init_wait;
} pmon_cfg_t;

#endif /* _PMON_TABLE_H_ */
