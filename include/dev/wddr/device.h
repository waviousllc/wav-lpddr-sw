/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _WDDR_DEV_H_
#define _WDDR_DEV_H_

#include <error.h>
#include <kernel/completion.h>
#include <kernel/notification.h>
#include <dfi/device.h>
#include <dfi_master/fsm.h>
#include <dfi_update/fsm.h>
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
 * fsw_event    private completion variable for use with FSW FSM.
 *              Internal use only.
 * endpoint     Notification endpoint.
 * channel      all channels.
 * cmn          all devices in common path.
 * dfi          DFI device.
 * dram         DRAM device.
 * pll          PLL device.
 * fsm.pll      PLL FSM.
 * fsm.fsw      Frequency Switch FSM.
 * fsm.dfiupd   DFI Update FSM.
 * fsm.dfimstr  DFI Master FSM.
 * table        pointer to calibration and configuration table for all
 *              frequencies.
 */
typedef struct wddr_dev_t
{
    uint32_t                base;
    Completion_t            fsw_event;
    NotificationEndpoint_t  endpoint;
    channel_t               channel[WDDR_PHY_CHANNEL_NUM];
    common_path_t           cmn;
    dfi_dev_t               dfi;
    dram_dev_t              dram;
    pll_dev_t               pll;
    struct
    {
        pll_fsm_t           pll;
        fs_fsm_t            fsw;
        dfi_update_fsm_t    dfiupd;
        dfi_master_fsm_t    dfimstr;
    } fsm;
    wddr_table_t            *table;
} wddr_dev_t;

/**
 * @brief   Wavious DDR (WDDR) Initilization
 *
 * @details Initailizes WDDR Device at device level.
 *
 * @note    PLL and Common block devices aren't initialized in this function.
 *          It is expected those are done prior so that MCU can run at MCUCLK
 *          rate and not REFCLK rate.
 *
 * @param[in]   wddr    pointer to WDDR device.
 * @param[in]   base    base address of the WDDR device.
 * @param[in]   table   pointer to WDDR table that should be used for
 *                      PHY configuration.
 *
 * @return      void
 */
void wddr_init(wddr_dev_t *wddr, uint32_t base, wddr_table_t *table);

/**
 * @brief   Wavious DDR (WDDR) Boot
 *
 * @details Boots WDDR Device. This calibrates all analog devices, and
 *          configures PHY for boot frequency.
 *
 * @param[in]   wddr    pointer to WDDR device.
 *
 * @return      returns whether boot completed successfully.
 * @retval      WDDR_SUCCESS if successful.
 * @retval      WDDR_ERROR otherwise.
 */
wddr_return_t wddr_boot(wddr_dev_t *wddr);

/**
 * @brief   Wavious DDR (WDDR) Prep Switch
 *
 * @details Prepares WDDR device for a frequency switch.
 *
 * @param[in]   wddr    pointer to WDDR device.
 * @param[in]   freq_id ID of frequency to prepare.
 *
 * @return      returns whether prep completed successfully.
 * @retval      WDDR_SUCCESS if successful.
 * @retval      WDDR_ERROR otherwise.
 */
wddr_return_t wddr_prep_switch(wddr_dev_t *wddr, uint8_t freq_id);

#endif /* _WDDR_DEV_H_ */
