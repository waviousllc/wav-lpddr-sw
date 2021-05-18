/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _VCO_DEV_H_
#define _VCO_DEV_H_

#include <stdint.h>

/**
 * @brief   VCO Device Structure
 *
 * @details Structure used to store state of VCO device.
 *
 * base         base address of the VCO device.
 * vco_id       id of the VCO.
 * freq_id      current frequency that VCO is configured to output.
 */
typedef struct vco_dev_t
{
    uint32_t    base;
    uint8_t     vco_id;
    uint8_t     freq_id;
} vco_dev_t;

#endif /* _VCO_DEV_H_ */
