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
 * init_wait       number of refclks to wait before starting.
 * refclk_count    number of refclks to sammple for.
 * target          Target count (target freq/refclk freq).
 */
typedef struct process_monitor_cfg_t
{
    uint8_t  init_wait;
    uint16_t refclk_count;
    uint32_t target
} pmon_cfg_t;

#endif /* _PMON_TABLE_H_ */
