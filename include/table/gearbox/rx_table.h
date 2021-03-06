/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _GEARBOX_RX_TABLE_H_
#define _GEARBOX_RX_TABLE_H_

#include <stdint.h>
#include <gearbox/table.h>

/**
 * @brief   Receive (RX) Gearbox Configuration Structure
 *
 * @details Structure for storing Receive Gearbox configuration.
 *
 * data_mode        Data Mode Configuration.
 * fifo_mode        FIFO Mode Configuration.
 * wck_mode         WCK Mode Configuration.
 * pre_filter_sel   Sets if preamble should be filtered in PHY.
 */
typedef struct rx_gb_cfg_t
{
    const gb_data_mode_t data_mode;
    const gb_fifo_mode_t fifo_mode;
    const gb_wck_mode_t  wck_mode;
    const uint8_t pre_filter_sel;
} rx_gb_cfg_t;

#endif /* _GEARBOX_RX_TABLE_H_ */
