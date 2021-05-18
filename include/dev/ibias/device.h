/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _IBIAS_DEV_H_
#define _IBIAS_DEV_H_

#include <stdint.h>
#include <error.h>

/**
 * @brief   Bias Current Device State
 *
 * DISABLE     Bias Current Device disabled.
 * ENABLE      Bias Current Device enabled.
 */
typedef enum ibias_state_t
{
    IBIAS_STATE_DISABLE,
    IBIAS_STATE_ENABLE,
} ibias_state_t;

/**
 * @brief Bias Current Device Structure
 *
 * @details Structure for bias current device.
 *
 * base    base address of the bias current device.
 */
typedef struct ibias_dev_t
{
    uint32_t base;
} ibias_dev_t;

/**
 * @brief   Bias Current Initialization
 *
 * @details Intiializes Bias Current Device at the device level.
 *
 * @param[in]   bias      pointer to Bias Current device to initialize.
 * @param[in]   base      base address of the Bias Current device.
 *
 * @return      void
 */
void ibias_init(ibias_dev_t *ibias, uint32_t base);

/**
 * @brief   Bias Current Enable
 *
 * @details Enables the Bias Current device.
 *
 * @param[in]   bias      pointer to Bias Current device to enable.
 *
 * @return      returns whether device was enabled successfully.
 * @retval      WDDR_SUCCESS if device was enabled.
 * @retval      WDDR_ERROR otherwise.
 */
wddr_return_t ibias_enable(ibias_dev_t *ibias);

/**
 * @brief   Bias Current Disable
 *
 * @details Disables the Bias Current device.
 *
 * @param[in]   bias      pointer to Bias Current device to disable.
 *
 * @return      returns whether device was disabled successfully.
 * @retval      WDDR_SUCCESS if device was disabled.
 * @retval      WDDR_ERROR otherwise.
 */
wddr_return_t ibias_disable(ibias_dev_t *ibias);

#endif /* _IBIAS_DEV_H_ */
