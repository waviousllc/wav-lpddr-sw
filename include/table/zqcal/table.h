/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _ZQCAL_TABLE_H_
#define _ZQCAL_TABLE_H_

#include <stdint.h>

/**
 * @brief   ZQCAL P/N Side Enumerations
 *
 * P_SIDE   ZQCAL P side index.
 * N_SIDE   ZQCAL N side index.
 * PN_SIDE  Max ZQCAL P/N index.
 */
typedef enum zqcal_pn_cal_t
{
    ZQCAL_P_CAL,
    ZQCAL_N_CAL,
    ZQCAL_PN_CAL_NUM,
} zqcal_pn_cal_t;

/**
 * @brief   ZQCAL VOH Mode Enumeration
 *
 * ZQCAL_VOH_0P5      VOH set to 0.5.
 * ZQCAL_VOH_0P6      VOH set to 0.6.
 */
typedef enum zqcal_voh_t
{
    ZQCAL_VOH_0P5,
    ZQCAL_VOH_0P6,
    ZQCAL_VOH_NUM
} zqcal_voh_t;

/**
 * @brief   ZQCAL Configuration Structure
 *
 * code    calibrated PCAL / NCAL code for each VOH setting.
 */
typedef struct zqcal_cfg_t
{
    uint8_t     code[ZQCAL_VOH_NUM][ZQCAL_PN_CAL_NUM];
} zqcal_cfg_t;

#endif /* _ZQCAL_TABLE_H_ */
