/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#ifndef _GEARBOX_TABLE_H_
#define _GEARBOX_TABLE_H_

/**
 * @brief   Gearbox Data Mode Enumerations
 */
typedef enum gb_data_mode_t
{
    DGB_1TO1_HF,
    DGB_2TO1_HF,
    DGB_2TO1_IR,
    DGB_4TO1_LF,
    DGB_4TO1_HF,
    DGB_4TO1_IR,
    DGB_8TO1_LF,
    DGB_8TO1_HF
} gb_data_mode_t;

/**
 * @brief   Gearbox Write Mode Enumerations
 */
typedef enum gb_write_mode_t
{
    WGB_1TO1,
    WGB_2TO1,
    WGB_4TO1,
    WGB_8TO1,
    WGB_2TO2,
    WGB_4TO2,
    WGB_8TO2,
    WGB_4TO4,
    WGB_8TO4,
    WGB_8TO8,
    WGB_16TO8
} gb_write_mode_t;

/**
 * @brief   Gearbox FIFO Mode Enumerations
 */
typedef enum gb_fifo_mode_t
{
    FGB_1TO1,
    FGB_1TO2,
    FGB_1TO4,
    FGB_1TO8,
    FGB_2TO2,
    FGB_2TO4,
    FGB_2TO8,
    FGB_4TO4,
    FGB_4TO8,
    FGB_8TO8,
    FGB_8TO16
} gb_fifo_mode_t;

/**
 * @brief   Gearbox WCK Mode Enumerations
 */
typedef enum gb_wck_mode_t
{
    // TODO: What does 0 and 1 actually mean?
    GB_WCK_MODE_0,
    GB_WCK_MODE_1
} gb_wck_mode_t;

#endif /* _GEARBOX_TABLE_H_ */
