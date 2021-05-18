/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _DFI_DEV_H_
#define _DFI_DEV_H_

#include <dfi/table.h>
#include <dfi/buffer_device.h>

/**
 * @brief   DFI Device Structure
 *
 * @details DFI Device structure that agregrates all DFI components.
 *
 * base         base address of DFI device.
 * msr          current MSR configuration of DFI device.
 * dfi_buffer   DFI Buffer device.
 */
typedef struct dfi_dev_t
{
    uint32_t            base;
    wddr_msr_t          msr;
    dfi_buffer_dev_t    dfi_buffer;
} dfi_dev_t;

/**
 * @brief   DFI Device Initialzation
 *
 * @details Initializes DFI Device at device level.
 *
 * @param[in]   dfi     pointer to DFI device.
 * @param[in]   base    base address of DFI device.
 *
 * @return      void
 */
void dfi_init(dfi_dev_t *dfi, uint32_t base);

#endif /* _DFI_DEV_H_ */
