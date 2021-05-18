/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _PMON_DEV_H_
#define _PMON_DEV_H_

#include <stdint.h>
#include <error.h>
#include <pmon/table.h>

/**
 * @brief Process Monitor Device Structure
 *
 * base    base address of Process Monitor Device.
 */
typedef struct process_monitor_dev_t
{
    uint32_t base;
} pmon_dev_t;

/**
 * @brief PMON State Enumerations
 *
 * DISABLED     PMON Disabled.
 * ENABLED      PMON Enabled.
 */
typedef enum pmon_state_t
{
    PMON_STATE_DISABLED,
    PMON_STATE_ENABLED,
} pmon_state_t;

/**
 * @brief   Process Monitor Initilization
 *
 * @details Initializes Process Monitor device at the device level.
 *
 * @param[in]   pmon    pointer to process monitor device to initialize.
 * @param[in]   base    pointer to base address of process monitor device.
 *
 * @return      void
 */
void pmon_init(pmon_dev_t *pmon, uint32_t base);

/**
 * @brief   Process Monitor Run
 *
 * @details Runs Process Monitor Estimation.
 *
 * @param[in]   pmon    pointer to process monitor device.
 * @param[out]  count   pointer to where to store measured estimation.
 *
 * @return      void
 */
void pmon_run(pmon_dev_t *pmon, uint32_t *count);

/**
 * @brief   Process Monitor Configure
 *
 * @details Configures Process Monitor.
 *
 * @param[in]   pmon    pointer to process monitor device.
 * @param[in]   cfg     pointer to process monitor configuration to use.
 *
 * @return      void
 */
void pmon_configure(pmon_dev_t *pmon, pmon_cfg_t *cfg);

#endif /* _PMON_DEV_H_ */
