/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _PATH_COMMON_H_
#define _PATH_COMMON_H_
#include <path/common.h>
#include <ibias/device.h>
#include <vref/device.h>
#include <zqcal/device.h>
#include <pmon/device.h>

/**
 * @brief   Common Block Structure
 *
 * @details Aggregates all devices common to both CA and DQ paths.
 *
 * ibias    Bias Current device.
 * vref     Voltage Reference device.
 * zqcal    ZQCAL device.
 * pmon     Process Monitor device.
 */
typedef struct common_path_t
{
    ibias_dev_t ibias;
    vref_dev_t  vref;
    zqcal_dev_t zqcal;
    pmon_dev_t  pmon;
} common_path_t;

/**
 * @brief   Common Path Intalization
 *
 * @details Initializes all devices in the Common Path.
 *
 * @param[in]   cmn_path    pointer to common path structure.
 * @param[in]   base        base address of common path.
 *
 * @return      void
 */
void common_path_init(common_path_t *cmn_path, uint32_t base);

/**
 * @brief   Common Path Enable
 *
 * @details Enables all devices in the Common Path. This is for devices that
 *          need to be enabled.
 *
 * @param[in]   cmn_path    pointer to common path structure.
 *
 * @return      void
 */
void common_path_enable(common_path_t *cmn_path);

#endif /* _PATH_COMMON_H_ */
