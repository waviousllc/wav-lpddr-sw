/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _FSW_DEV_H_
#define _FSW_DEV_H_

#include <wddr/phy_defs.h>
#include <fsw/driver.h>
#include <dfi/device.h>

/**
 * @brief   Frequency Switch Modes
 *
 * SW       Mode where software initiates frequency switch.
 * DFI      Mode where DFI interface initiates frequency switch.
 */
typedef enum fsw_mode
{
    FSW_MODE_SW,
    FSW_MODE_DFI,
} fsw_mode_t;

/**
 * @brief   Frequency Switch Device Structure
 *
 * fsw_reg  Poniter to Frequency Switch register space.
 * mode     The current Frequency Switch mode of the FSW device. After boot,
 *          mode is always FSW_MODE_DFI and all switches have to be initaited
 *          via the DFI interface.
 */
typedef struct fsw_device
{
    fsw_reg_t  *fsw_reg;
    fsw_mode_t mode;
} fsw_dev_t;

/**
 * @brief   Frequency Switch Device Initialization
 *
 * @details Initializes the Frequency Switch Device.
 *
 * @param[in]   dev     pointer to Frequency Switch device to init.
 * @param[in]   base    base address of fsw register space.
 *
 * @return  void.
 */
void fsw_init(fsw_dev_t *dev, uint32_t base);

/**
 * @brief   Frequency Switch Switch To DFI Mode.
 *
 * @details Switches the Frequency Switch device to DFI mode. This means that
 *          frequency switches are initiated via the DFI interface (INIT_START).
 *          Should be called after PHY boots.
 *
 * @param[in]   fsw_dev     pointer to Frequency Switch device.
 * @param[in]   dfi_dev     pointer to DFI device.
 *
 * @return  void.
 */
void fsw_switch_to_dfi_mode(fsw_dev_t *fsw_dev, dfi_dev_t *dfi_dev);

/**
 * @brief   Frequency Switch Get Current Mode Switch Register (MSR)
 *
 * @details Returns the current Mode Switch Register configured in the PHY.
 *          PHY supports two concurrent configurations. The MSR indicates which
 *          of the configurations the PHY uses currently.
 *
 * @return  returns current Mode Switch Register value.
 * @retval  possible values are WDDR_MSR_0 and WDDR_MSR_1.
 */
wddr_msr_t fsw_get_current_msr(fsw_dev_t *dev);

/**
 * @brief   Frequency Switch Get Next Mode Switch Register (MSR)
 *
 * @details Returns the next Mode Switch Register configured in the PHY.
 *          PHY supports two concurrent configurations. The MSR indicates which
 *          of the configurations the PHY uses currently. This function returns
 *          the next MSR not in use.
 *
 * @return  returns current Mode Switch Register value.
 * @retval  possible values are WDDR_MSR_0 and WDDR_MSR_1.
 */
wddr_msr_t fsw_get_next_msr(fsw_dev_t *dev);

#endif /* _FSW_DEV_H_ */
