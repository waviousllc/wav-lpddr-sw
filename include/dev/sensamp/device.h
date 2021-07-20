/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _SENSAMP_DEV_H_
#define _SENSAMP_DEV_H_
#include <stdint.h>
#include <error.h>
#include <sensamp/table.h>

/**
 * @brief   Sense Amp (Sensamp) State Enumerations
 *
 * @details Enumerations for valid states that Sensamp device an be in.
 *
 * DYNAMIC          sensamp device dynamically controlled by PHY logic.
 * ENABLED          sensamp device forced enable.
 * CAL_ENABELED     sensamp device forced enable and in calibration mode.
 */
typedef enum sensamp_state_t
{
    SA_STATE_DYNAMIC,
    SA_STATE_ENABLED,
    SA_STATE_CAL_ENABLED
} sensamp_state_t;

/**
 * @brief   Sense Amp (Sensamp) DQ Bit Device Structure
 *
 * base     base address of single DQ bit.
 */
typedef struct sensamp_dqbit_dev_t
{
    uint32_t    base;
} sensamp_dqbit_dev_t;

/**
 * @brief Sense Amp (Sensamp) DQ Byte Structure
 *
 * base   base address of DQByte.
 * state  state of sensamp device.
 * bit    array of Sensamp DQ Bit devices. 1 per DQ bit.
 */
typedef struct sensamp_dqbyte_dev_t
{
    uint32_t            base;
    sensamp_state_t     state;
    struct
    {
        struct
        {
            sensamp_dqbit_dev_t bit[WDDR_PHY_DQ_SLICE_NUM];
        } dq;
    } rank[WDDR_PHY_RANK];
} sensamp_dqbyte_dev_t;

/**
 * @brief  Sense Amp (Sensamp) DQ Byte Intialization
 *
 * @details Initializes Sensamp DQ Byte device at device level.
 *
 * @param[in]   sa_dqbyte   pointer to Sensamp device.
 * @param[in]   base        base address of Sensamp device.
 *
 * @return      void
 */
void sensamp_dqbyte_init(sensamp_dqbyte_dev_t *sa_dqbyte, uint32_t base);

/**
 * @brief   Sense Amp (Sensamp) DQ Byte Calibration
 *
 * @details Peforms calibration procedure for all DQ bits in the DQ Byte.
 *
 * @param[in]   sa_dqbyte   pointer to Sensamp device..
 * @param[out]  cal         pointer to calibration table to fill in with
 *                          calibrated codes.
 *
 * @return      returns whether calibraton completed successfully.
 * @retval      WDDR_SUCCESS if successful.
 * @retval      WDDR_ERROR otherwise.
 */
wddr_return_t sensamp_dqbyte_cal(sensamp_dqbyte_dev_t *sa_dqbyte,
                                 sensamp_dqbyte_common_cal_t *cal);

#endif /* _SENSAMP_DEV_H_ */
