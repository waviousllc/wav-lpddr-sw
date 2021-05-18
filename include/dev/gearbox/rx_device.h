/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _GEARBOX_RX_DEV_H_
#define _GEARBOX_RX_DEV_H_

#include <stdint.h>
#include <gearbox/device.h>

/**
 * @brief   Receive (rX) Gearbox Data Structure
 *
 * @details RX Gearbox structure for configuring DQ and DQS receive gearbox.
 *
 * dq_gb   DQ Gearbox
 * dqs_gb  DQS Garbox
 */
typedef struct rx_gearbox_dev_t
{
    gearbox_dev_t dq_gb;
    gearbox_dev_t dqs_gb;
} rx_gearbox_dev_t;

#endif /* _GEARBOX_RX_DEV_H_ */
