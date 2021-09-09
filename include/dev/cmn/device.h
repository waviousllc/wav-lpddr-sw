/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _CMN_DEV_H_
#define _CMN_DEV_H_

#include <error.h>
#include <cmn/driver.h>
#include <cmn/table.h>

/**
 * @brief   Common Device Structure
 *
 * @details Common device structure that aggegrates components common to all
 *          channels.
 *
 * cmn_reg  Common Register Space
 */
typedef struct common_device
{
    cmn_reg_t   *cmn_reg;
} cmn_dev_t;

/**
 * @biref   Common Device Initialization
 *
 * @details Initializes Common Device.
 *
 * @param[in]   cmn_dev     pointer to common device.
 * @param[in]   base        base address of cmn_dev register space.
 *
 * @return  void.
 */
void cmn_init(cmn_dev_t *cmn_dev, uint32_t base);

/**
 * @brief   Common Device Enable
 *
 * @details Enables entire common device (vref and bias current)
 *
 * @param[in]   cmn_dev     pointer to common device.
 *
 * @return void.
 */
void cmn_enable(cmn_dev_t *cmn_dev);

/**
 * @brief   Common Bias Current Enable
 *
 * @details Enables the Bias current.
 *
 * @param[in]   cmn_dev     pointer to common device.
 *
 * @return  void.
 */
void cmn_ibias_enable(cmn_dev_t *cmn_dev);

/**
 * @brief   Common Bias Current Disable
 *
 * @details Disables the Bias current.
 *
 * @param[in]   cmn_dev     pointer to common device.
 *
 * @return  void.
 */
void cmn_ibias_disable(cmn_dev_t *cmn_dev);

/**
 * @brief   Process Monitor Run
 *
 * @details Runs Process Monitor Estimation.
 *
 * @param[in]   cmn_dev     pointer to common device.
 * @param[out]  count       pointer to where to store measured estimation.
 *
 * @return      void
 */
void cmn_pmon_run(cmn_dev_t *cmn_dev, uint32_t *count);

/**
 * @brief   Common Process Monitor Configure
 *
 * @details Configures Process Monitor.
 *
 * @param[in]   cmn_dev     pointer to common device.
 * @param[in]   cfg         pointer to process monitor configuration to use.
 *
 * @return      void
 */
void cmn_pmon_configure(cmn_dev_t *cmn_dev, pmon_cfg_t *cfg);

/**
 * @brief   Common Voltage Reference Set Mode Enable
 *
 * @details Places the Voltage Reference device into Enabled mode.
 *
 * @param[in]   cmn_dev     pointer to common device.
 *
 * @return      void
 */
void cmn_vref_set_mode_enable(cmn_dev_t *cmn_dev, wddr_msr_t msr);

/**
 * @brief   Common Voltage Reference Set Mode Disable
 *
 * @details Places the Voltage Reference device into Disabled mode.
 *
 * @param[in]   cmn_dev     pointer to common device.
 *
 * @return      void
 */
void cmn_vref_set_mode_disable(cmn_dev_t *cmn_dev, wddr_msr_t msr);

/**
 * @brief   Common Voltage Reference Set Mode HIZ
 *
 * @details Places the Voltage Reference device into HI-Z mode.
 *
 * @param[in]   cmn_dev     pointer to common device.
 *
 * @return      void
 */
void cmn_vref_set_mode_hiz(cmn_dev_t *cmn_dev, wddr_msr_t msr);

/**
 * @brief   ZQCAL Calibrate
 *
 * @details Perform ZQCAL calibration procedure.
 *
 * @param[in]   cmn_dev     pointer to common device.
 * @param[out]  cfg         pointer to ZQCAL configuration structure to fill in
 *                          with calibrated values.
 *
 * @return      returns whether calibraton completed successfully.
 * @retval      WDDR_SUCCESS if successful.
 * @retval      WDDR_ERROR_ZQCAL_NCAL_AT_MIN if minimum code is used for NCAL.
 * @retval      WDDR_ERROR_ZQCAL_NCAL_AT_MAX if maximum code is used for NCAL.
 * @retval      WDDR_ERROR_ZQCAL_PCAL_AT_MIN if minimum code is used for PCAL.
 * @retval      WDDR_ERROR_ZQCAL_PCAL_AT_MAX if maximum code is used for PCAL.
 */
wddr_return_t cmn_zqcal_calibrate(cmn_dev_t *cmn_dev, zqcal_cfg_t *cfg);

#endif /* _CMN_DEV_H_ */
