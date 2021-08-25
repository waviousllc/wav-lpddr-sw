/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _WDDR_DEV_H_
#define _WDDR_DEV_H_

#include <error.h>
#include <dfi/device.h>
#include <dram/device.h>
#include <fsw/device.h>
#include <path/ca.h>
#include <path/common.h>
#include <path/dq.h>
#include <pll/device.h>
#include <wddr/phy_config.h>
#include <wddr/table.h>
#include "boot_options.h"

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
 * xHandle      private reference to task block by prep / switch request.
 * endpoint     Notification endpoint.
 * channel      all channels.
 * cmn          all devices in common path.
 * dfi          DFI device.
 * dram         DRAM device.
 * fsw          Frequency Switch device.
 * pll          PLL device.
 * table        pointer to calibration and configuration table for all
 *              frequencies.
 */
typedef struct wddr_dev_t
{
    uint32_t        base;
    bool            is_booted;
    channel_t       channel[WDDR_PHY_CHANNEL_NUM];
    common_path_t   cmn;
    dfi_dev_t       dfi;
    dram_dev_t      dram;
    pll_dev_t       pll;
    fsw_dev_t       fsw;
    wddr_table_t    *table;
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
 * @note    Should only be called by firmware.
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

 * @note    Should only be called by firmware.
 *
 * @param[in]   wddr    pointer to WDDR device.
 * @param[in]   cfg     boot configuration.
 *
 * @return      returns whether boot completed successfully.
 * @retval      WDDR_SUCCESS if successful.
 * @retval      WDDR_ERROR otherwise.
 */
wddr_return_t wddr_boot(wddr_dev_t *wddr, wddr_boot_cfg_t cfg);

/**
 * @brief   Wavious DDR (WDDR) Prep Switch
 *
 * @details Prepares WDDR device for a frequency switch.
 *
 * @note    Should only be called by firmware.
 *
 * @param[in]   wddr    pointer to WDDR device.
 * @param[in]   freq_id ID of frequency to prepare.
 * @param[in]   msr     mode switch register to prepare.
 *
 * @return      returns whether prep completed successfully.
 * @retval      WDDR_SUCCESS if successful.
 * @retval      WDDR_ERROR otherwise.
 */
wddr_return_t wddr_prep_switch(wddr_dev_t *wddr,
                               uint8_t freq_id,
                               wddr_msr_t msr);

/**
 * @brief   Wavious DDR (WDDR) Software Frequency Switch
 *
 * @details Prepares and switches WDDR PHY Frequency.
 *
 * @note    This function can only be used prior to boot. It's publically
 *          available so that it can be used externally during PHY Training.
 *
 * @param[in]   wddr    pointer to WDDR device.
 * @param[in]   freq_id ID of frequency to prepare and switch to.
 * @param[in]   msr     MSR to switch to.
 *
 * @return      returns whether switch completed sucessfully.
 * @retval      WDDR_SUCCESS if successful.
 * @retval      WDDR_ERROR if switch cannot be called.
 */
wddr_return_t wddr_sw_freq_switch(wddr_dev_t *wddr,
                                  uint8_t freq_id,
                                  wddr_msr_t msr);

/**
 * @brief   WDDR Enable Loopback
 *
 * @details Enables Loopback mode for WDDR device.
 *
 * @note    This is only used for training modes. It should never be used
 *          otherwise.
 *
 * @note    Perform a frequency prep and switch to reset loopback state.
 *
 * @param[in]   wddr    pointer to WDDR device.
 *
 * @return  void.
 */
void wddr_enable_loopback(wddr_dev_t *wddr);

/**
 * @brief   WDDR IOCAL Update PHY
 *
 * @details Updates IOCAL CSRs in the PHY with latest calibrated values.
 *          Can only be called when in PHYUPD or CTRLUPD states.
 *          Do not call directly.
 *
 * @note    Should only be called by firmware.
 *
 * @param[in]   wddr    pointer to WDDR device.
 *
 * @return      void.
 */
void wddr_iocal_update_phy(wddr_dev_t *wddr);

/**
 * @brief   WDDR IOCAL Calibrate
 *
 * @details Calibrates IOCAL values. Calibration can be done regardless of
 *          DFI states. See wddr_iocal_update_phy for information about how
 *          to apply calibrated values to the PHY. Do not call directly.
 *
 * @note    Should only be called by firmware.
 *
 * @param[in]   wddr    pointer to WDDR device.
 *
 * @return      void.
 */
void wddr_iocal_calibrate(wddr_dev_t *wddr);

#endif /* _WDDR_DEV_H_ */
