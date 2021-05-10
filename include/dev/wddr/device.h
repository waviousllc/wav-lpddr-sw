/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _WDDR_DEV_H_
#define _WDDR_DEV_H_

#include <dfi/device.h>
#include <dram/device.h>
#include <freq_switch/fsm.h>
#include <path/ca.h>
#include <path/common.h>
#include <path/dq.h>
#include <pll/device.h>
#include <pll/fsm.h>
#include <wddr/phy_config.h>
#include <wddr/table.h>

/**
 * @brief   Channel Structure
 *
 * @details Channel structure that aggegrates DQ and CA paths.
 *
 * dq    DQ Bytes
 * ca    CA Bytes
 */
typedef struct channel_t
{
    dq_path_t      dq[WDDR_PHY_DQ_BYTE_NUM];
    ca_path_t      ca;
} channel_t;

/**
 * @brief   WDDR Structure
 *
 * @details WDDR structure that aggegrates all devices of
 *          the WDDR device.
 *
 * base         base address of WDDR (top-level).
 * channel      all channels.
 * cmn          all devices in common path.
 * dfi          DFI device.
 * dram         DRAM device.
 * pll          PLL device.
 * fsm.pll      PLL FSM.
 * fsm.fsw      Frequency Switch FSM.
 * table        pointer to calibration and configuration table for all
 *              frequencies.
 */
typedef struct wddr_dev_t
{
    uint32_t        base;
    channel_t       channel[WDDR_PHY_CHANNEL_NUM];
    common_path_t   cmn;
    dfi_dev_t       dfi;
    dram_dev_t      dram;
    pll_dev_t       pll;
    struct
    {
        pll_fsm_t   pll;
        fs_fsm_t    fsw;
    } fsm;
    wddr_table_t    *table;
} wddr_dev_t;

#endif /* _WDDR_DEV_H_ */
