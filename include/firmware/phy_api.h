/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _FIRMWARE_PHY_API_H_
#define _FIRMWARE_PHY_API_H_

#include <FreeRTOS.h>

/**
 * @brief   Firmware PHY Initialization
 *
 * @details API to initialize the PHY Firmware. Should be the first thing
 *          called in main.
 *
 * @return  void.
 */
void firmware_phy_init(void);

/**
 * @brief   Firmware PHY Start
 *
 * @details Starts the PHY Firmware. Must be called after FreeRTOS threads
 *          are running. Typically, this would be called by the Main Task.
 *
 * @return  returns whether PHY Firmware was started successfully.
 * @retval  pdPASS if started successfully.
 * @retval  pdFAIL otherwise.
 */
UBaseType_t firmware_phy_start(void);

/**
 * @brief   Firmware PHY Prepare Frequency Switch
 *
 * @details Requests the firmware to begin prepping PHY for a frequency
 *          switch.
 *
 * @param[in]   freq_id     The frequency to prepare.
 *
 * @return  returns whether PHY was prepared successfully.
 * @retval  pdPASS if prepared successfully.
 * @retval  pdFAIL otherwise.
 */
UBaseType_t firmware_phy_prep_switch(uint8_t freq_id);

#endif /* _FIRMWARE_PHY_API_H_ */
