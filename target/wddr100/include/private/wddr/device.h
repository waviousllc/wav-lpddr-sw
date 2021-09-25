/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _WDDR_PRV_DEVICE_H_
#define _WDDR_PRV_DEVICE_H_

#include <channel/device.h>
#include <cmn/device.h>
#include <ctrl/device.h>
#include <dram/device.h>
#include <fsw/device.h>
#include <pll/device.h>
#include <wddr/table.h>

/**
 * @brief   WDDR Structure
 *
 * @details WDDR structure that aggegrates all devices of
 *          the WDDR device.
 *
 * channel      channel register space.
 * cmn          common register space.
 * ctrl         ctrl register space.
 * dfi          DFI device.
 * dram         DRAM device.
 * fsw          Frequency Switch device.
 * pll          PLL device.
 * table        pointer to calibration and configuration table for all
 *              frequencies.
 */
struct wddr_device_t
{
    channel_dev_t   channel[WDDR_PHY_CHANNEL_NUM];
    cmn_dev_t       cmn;
    ctrl_dev_t      ctrl;
    dfi_dev_t       dfi;
    dram_dev_t      dram;
    pll_dev_t       pll;
    fsw_dev_t       fsw;
    wddr_table_t    *table;
};

#endif /* _WDDR_PRV_DEVICE_H_ */
