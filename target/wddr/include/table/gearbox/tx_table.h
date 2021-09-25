/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _GEARBOX_TX_TABLE_H_
#define _GEARBOX_TX_TABLE_H_

#include <stdint.h>
#include <gearbox/table.h>

/**
 * @brief   Transmit (TX) Gearbox Configuration Structure
 *
 * @details Structure for storing Transmit Gearbox configuration.
 *
 * data_mode    Data Mode Configuration.
 * write_mode   Write Mode Configuration.
 * ck2wck_ratio Ratio of WCK to CK.
 */
typedef struct tx_gb_cfg_t
{
    const gb_data_mode_t data_mode;
    const gb_write_mode_t write_mode;
    const uint8_t ck2wck_ratio;
} tx_gb_cfg_t;

#endif /* _GEARBOX_TX_TABLE_H_ */
