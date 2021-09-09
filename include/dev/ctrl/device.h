/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _CTRL_DEV_H_
#define _CTRL_DEV_H_

#include <error.h>
#include <ctrl/driver.h>

/**
 * @brief   Control Device Structure
 *
 * @details Control device structure that aggegrates components common to all
 *          channels.
 *
 * ctrl_reg  Control Register Space
 */
typedef struct control_device
{
    ctrl_reg_t   *ctrl_reg;
} ctrl_dev_t;

/**
 * @biref   Control Device Initialization
 *
 * @details Initializes Control Device.
 *
 * @param[in]   ctrl_dev     pointer to control device.
 * @param[in]   base        base address of control register space.
 *
 * @return  void.
 */
void ctrl_init(ctrl_dev_t *ctrl_dev, uint32_t base);

#endif /* _CTRL_DEV_H_ */
