/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _GEARBOX_DEV_H_
#define _GEARBOX_DEV_H_

#include <stdint.h>

/**
 * @brief   Gearbox Device Structure
 *
 * @details Gearbox device structure.
 *
 * @base    base address of Gearbox device.
 */
typedef struct gearbox_dev_t
{
    uint32_t base;
} gearbox_dev_t;

#endif /* _GEARBOX_DEV_H_ */
