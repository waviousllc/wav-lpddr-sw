/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#include <dfi/device.h>
#include <dfi/driver.h>
#include <dfi/buffer_device.h>

void dfi_init(dfi_dev_t *dfi, uint32_t base)
{
    dfi->base = base;
    dfi_set_msr_reg_if(dfi, WDDR_MSR_0);
    dfi_buffer_init(&dfi->dfi_buffer, base);
}
