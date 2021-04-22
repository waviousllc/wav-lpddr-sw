/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#ifndef _ZQCAL_DEV_H_
#define _ZQCAL_DEV_H_

#include <stdint.h>
#include <error.h>
#include <zqcal/table.h>

/**
 * @brief   ZQCAL State Enumeration
 *
 * DISABLED    Disabled state.
 * ENABLED     Enabled state.
 */
typedef enum zqcal_state_t
{
    ZQCAL_STATE_DISABLED,
    ZQCAL_STATE_ENABLED
} zqcal_state_t;

/**
 * @brief   ZQCAL Mode Enumeration
 *
 * ZQCAL_MODE_PULL_UP      Pull up (p side).
 * ZQCAL_MODE_PULL_DOWN    Pull down (n side).
 */
typedef enum zqcal_mode_t
{
    ZQCAL_MODE_PULL_UP,
    ZQCAL_MODE_PULL_DOWN
} zqcal_mode_t;

/**
 * @brief ZQCAL Device Structure
 *
 * base    base address of zqcal device.
 */
typedef struct zqcal_dev_t
{
    uint32_t base;
} zqcal_dev_t;

/**
 * @brief   ZQCAL Initilization
 *
 * @details Initailizes ZQCAL Device at device level.
 *
 * @param[in]   zqcal   pointer to ZQCAL device.
 * @param[in]   base    base address of the ZQCAL device.
 *
 * @return      void
 */
void zqcal_init(zqcal_dev_t *zqcal, uint32_t base);

/**
 * @brief   ZQCAL Calibrate
 *
 * @details Perform ZQCAL calibration procedure.
 *
 * @param[in]   zqcal   pointer to ZQCAL device.
 * @param[out]  cal     pointer to ZQCAL Calibration Structure to fill in with
 *                      calibrated values.
 *
 * @return      returns whether calibraton completed successfully.
 * @retval      WDDR_SUCCESS if successful.
 * @retval      WDDR_ERROR_ZQCAL_NCAL_AT_MIN if minimum code is used for NCAL.
 * @retval      WDDR_ERROR_ZQCAL_NCAL_AT_MAX if maximum code is used for NCAL.
 * @retval      WDDR_ERROR_ZQCAL_PCAL_AT_MIN if minimum code is used for PCAL.
 * @retval      WDDR_ERROR_ZQCAL_PCAL_AT_MAX if maximum code is used for PCAL.
 */
wddr_return_t zqcal_calibrate(zqcal_dev_t *zqcal, zqcal_cal_t *cal);

#endif /* _ZQCAL_DEV_H_ */
