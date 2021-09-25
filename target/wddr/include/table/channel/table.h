/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _CHANNEL_TABLE_H_
#define _CHANNEL_TABLE_H_

#include <wddr/phy_defs.h>
#include <dq/table.h>
#include <ca/table.h>

/**
 * @brief   Channel Frequency Configuration Structure
 *
 * @details Stores all configuration data for entire channel for a specific
 *          frequency.
 *
 * dq       per-frequency configuration data for both DQ bytes.
 * ca       per-frequency configuration data for CA.
 */
typedef struct channel_freq_cfg_t
{
    dq_path_freq_cfg_t dq[WDDR_PHY_DQ_BYTE_NUM];
    ca_path_freq_cfg_t ca;
} channel_freq_cfg_t;

/**
 * @brief   Channel Common Frequency Configuration Structure
 *
 * @details Stores all configuration data for entire channel that is common
 *          across all frequencies.
 *
 * dq       frequency independent configuration data for both DQ bytes.
 */
typedef struct channel_common_cfg_t
{
    dq_path_common_cfg_t dq[WDDR_PHY_DQ_BYTE_NUM];
} channel_common_cfg_t;

#endif /* _CHANNEL_TABLE_H_ */
