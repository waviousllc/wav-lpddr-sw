/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DRIVER_TABLE_H_
#define _DRIVER_TABLE_H_

#include <stdint.h>

/**
 * @brief   Driver Mode Enumerations
 *
 * DIFF     Differential mode.
 * SE       Single-ended mode.
 */
typedef enum driver_mode_t
{
    DRIVER_MODE_DIFF,
    DRIVER_MODE_SE,
} driver_mode_t;

/**
 * @brief   Driver Loopback Mode Enumerations
 *
 * DISABLE  Driver is not in loopback mode.
 * ENABLE   Driver is in loopback mode.
 */
typedef enum driver_loopback_mode_t
{
    DRIVER_LOOPBACK_MODE_DISABLE,
    DRIVER_LOOPBACK_MODE_ENABLE,
} driver_loopback_mode_t;

/**
 * @brief   Driver P/N Calibration Enumerations
 *
 * @details Indices for P and N calibration codes.
 *
 * P_CAL        Driver P cal index.
 * N_CAL        Driver N cal index.
 * PN_CAL_NUM   Max Driver P/N indices.
 */
typedef enum driver_pn_side_t
{
    DRVR_P_CAL,
    DRVR_N_CAL,
    DRVR_PN_CAL_NUM,
} driver_pn_side_t;

/**
 * @brief Driver Impedance Enumerations
 *
 * HIZ     High-Z impedance.
 * 240     240 ohm impedance.
 * 120     120 ohm impedance.
 * 80      80 ohm impedance.
 * 60      60 ohm impedance.
 * 48      48 ohm impedance.
 * 40      40 ohm impedance.
 */
typedef enum driver_impedance_t
{
    DRIVER_IMPEDANCE_HIZ,
    DRIVER_IMPEDANCE_240,
    DRIVER_IMPEDANCE_120,
    DRIVER_IMPEDANCE_80,
    DRIVER_IMPEDANCE_60,
    DRIVER_IMPEDANCE_48,
    DRIVER_IMPEDANCE_40
} driver_impedance_t;

/**
 * @brief   Driver Common Calibration Structure
 *
 * @details Calibration data for Driver common to both slices in TX path.
 *
 * code     pointer to calibrated P/N codes. This points to ZQCAL table.
 */
typedef struct driver_cmn_cal_t
{
    uint8_t *code;
} driver_cmn_cal_t;

/**
 * @brief   Driver Common Configuration Structure
 *
 * @details Configuration data for Driver common to both slice in TX path.
 *
 * mode     driver mode.
 */
typedef struct driver_cmn_cfg_t
{
    driver_mode_t mode;
} driver_cmn_cfg_t;

/**
 * @brief   Driver Configuration Structure
 *
 * @details Configuration data for Driver for a specific slice in TX Path.
 *
 * tx_impd      transmit impedance configuration.
 * rx_impd      receive impdedance configuration.
 * override     driver override configuration.
 */
typedef struct driver_cfg_t
{
    driver_impedance_t tx_impd;
    driver_impedance_t rx_impd;
    struct
    {
        uint8_t sel     : 3;
        uint8_t val_t   : 1;
        uint8_t val_c   : 1;
    } override;
} driver_cfg_t;

#endif /* _DRIVER_TABLE_H_ */
