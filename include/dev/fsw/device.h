/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _FSW_DEV_H_
#define _FSW_DEV_H_

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
 * mode     The current Frequency Switch mode of the FSW device. After boot,
 *          mode is always FSW_MODE_DFI and all switches have to be initaited
 *          via the DFI interface.
 */
typedef struct fsw_device
{
    fsw_mode_t mode;
} fsw_dev_t;

/**
 * @brief   Frequency Switch Device Initialization
 *
 * @details Initializes the Frequency Switch Device.
 *
 * @param[in]   dev     pointer to Frequency Switch device to init.
 *
 * @return  void.
 */
void fsw_init(fsw_dev_t *dev);

/**
 * @brief   Frequency Switch Switch To DFI Mode.
 *
 * @details Switches the Frequency Switch device to DFI mode. This means that
 *          frequency switches are initiated via the DFI interface (INIT_START).
 *          Should be called after PHY boots.
 *
 * @param[in]   dev     pointer to Frequency Switch device to init.
 *
 * @return  void.
 */
void fsw_switch_to_dfi_mode(fsw_dev_t *dev);

#endif /* _FSW_DEV_H_ */
