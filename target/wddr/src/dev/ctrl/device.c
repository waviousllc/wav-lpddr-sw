/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <ctrl/device.h>
#include <wddr/memory_map.h>

void ctrl_init(ctrl_dev_t *ctrl_dev, uint32_t base)
{
    ctrl_dev->ctrl_reg = (ctrl_reg_t *)(base + WDDR_MEMORY_MAP_CTRL);
}
