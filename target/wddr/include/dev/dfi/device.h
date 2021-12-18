/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DFI_DEV_H_
#define _DFI_DEV_H_

#include <dfi/packet.h>
#include <dfi/driver.h>
#include <dfi/table.h>

// TODO: This should be defined by each WDDR IMPL
#define PACKET_STORAGE_SIZE     (64)

/**
 * @brief   DFI Device Structure
 *
 * @details DFI Device structure that agregrates all DFI components.
 *
 * dfi_reg      DFI register space.
 * dfich_reg    DFI Channel register space.
 * packet.buff  buffer that stores current sequence of packets.
 * packet.pool  underlying storage of packets used to create sequence.
 */
typedef struct dfi_dev_t
{
    dfi_reg_t   *dfi_reg;
    dfich_reg_t *dfich_reg;
    struct
    {
        dfi_tx_packet_buffer_t buff;
        packet_item_t pool[PACKET_STORAGE_SIZE];
    } packet;
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
