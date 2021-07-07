/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#ifndef _RECEIVER_TABLE_H_
#define _RECEIVER_TABLE_H_

#include <stdint.h>

/**
 * @brief   Receiver T/C Side Enumerations
 *
 * T_SIDE   Receiver T side index.
 * C_SIDE   Receiver C side index.
 * TC_SIDE  Max receiver T/C index.
 */
typedef enum receiver_tc_side_t
{
    REC_T_SIDE,
    REC_C_SIDE,
    REC_TC_SIDE_NUM
} receiver_tc_side_t;

/**
 * @brief   Receiver T/C Side Enumerations Mask
 *
 * T_SIDE_MASK     Receiver T side mask.
 * C_SIDE_MASK     Receiver C side mask.
 * BOTH_SIDE_MASK  Max receiver T/C mask.
 */
typedef enum receiver_tc_side_mask_t
{
    REC_T_SIDE_MASK      = (0x1 << REC_T_SIDE),
    REC_C_SIDE_MASK      = (0x1 << REC_C_SIDE),
    REC_BOTH_SIDE_MASK   = (REC_T_SIDE_MASK | REC_C_SIDE_MASK)
} receiver_tc_side_mask_t;
/**
 * @brief   Receiver P/N Side Enumerations
 *
 * P_SIDE   Receiver P side index.
 * N_SIDE   Receiver N side index.
 * PN_SIDE  Max receiver P/N index.
 */
typedef enum receiver_pn_side_t
{
    REC_P_SIDE,
    REC_N_SIDE,
    REC_PN_SIDE_NUM
} receiver_pn_side_t;

/**
 * @brief   Receiver Mode Enumerations
 *
 * DIFF     differential mode.
 * SE       single-ended mode.
 */
typedef enum receiver_mode_t
{
    REC_MODE_DIFF,
    REC_MODE_SE
} receiver_mode_t;

/**
 * @brief   Receiver Path State
 *
 * AC       AC path configuration.
 * DC       DC path configuration.
 */
typedef enum receiver_path_state_t
{
    REC_PATH_AC = 0,
    REC_PATH_DC = 1,
} receiver_path_state_t;

/**
 * @brief   Receiver Frequency Calibration Structure
 *
 * @details Stores frequency specific calibration data.
 *
 * rx_delay    RX delay on reciever delay element.
 */
typedef struct receiver_freq_cal_t
{
    uint16_t    rx_delay[REC_TC_SIDE_NUM];
} receiver_freq_cal_t;

/**
 * @brief   Receiver Frequency Configuration Structure
 *
 * @details Stores frequency specific configuration data.
 *
 * feedback_resistor    Feeback resistor value.
 * mode                 Receive Mode.
 * path_state           Path state.
 */
typedef struct receiver_freq_cfg_t
{
    uint8_t                     feedback_resistor;
    receiver_mode_t             mode;
    receiver_path_state_t       path_state;
} receiver_freq_cfg_t;

/**
 * @brief   Receiver Common Calibration Structure
 *
 * @details Stores frequency independent calibration data
 *
 * cal_code    Receiver calibration codes.
 */
typedef struct receiver_common_cal_t
{
    uint8_t     cal_code[REC_PN_SIDE_NUM][REC_TC_SIDE_NUM];
} receiver_common_cal_t;

#endif /* _RECEIVER_TABLE_H_ */
