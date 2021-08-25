/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <sensamp/device.h>
#include <sensamp/driver.h>

#define WAV_SA_MIN_CAL_CODE (0)
#define WAV_SA_MAX_CAL_CODE (31)
#define WAV_SA_MID_CAL_CODE ((WAV_SA_MAX_CAL_CODE - WAV_SA_MIN_CAL_CODE + 1) / 2)
#define WAV_SA_CAL_DIR_NEG  (0)
#define WAV_SA_CAL_DIR_POS  (1)

/**
 * @brief   Sense Amp (Sensamp) DQ Byte Configure (Internal)
 *
 * @details Configures all Sensamp Devices in a DQ Byte.
 *
 * @param[in]   sa_dqbyte   pointer to Sensamp device.
 * @param[in]   calibrate   flag to indicate if data should be calibrated.
 * @param[out]  data        pointer to table to fill in with calibrated codes.
 *
 * @return      returns whether calibraton completed successfully.
 * @retval      WDDR_SUCCESS if successful.
 * @retval      WDDR_ERROR otherwise.
 */
static wddr_return_t __sensamp_dqbyte_configure(sensamp_dqbyte_dev_t *sa_dqbyte,
                                                bool calibrate,
                                                sensamp_dqbit_cal_t data[WDDR_PHY_RANK][WDDR_PHY_DQ_SLICE_NUM]);

void sensamp_dqbyte_init(sensamp_dqbyte_dev_t *sa_dqbyte, uint32_t base)
{
    sensamp_dqbyte_init_reg_if(sa_dqbyte, base);
}

wddr_return_t sensamp_dqbyte_configure(sensamp_dqbyte_dev_t *sa_dqbyte,
                                       bool calibrate,
                                       sensamp_dqbyte_common_cal_t *cal)

{
    PROPAGATE_ERROR(sensamp_dqbyte_set_state_reg_if(sa_dqbyte, SA_STATE_CAL_ENABLED));
    PROPAGATE_ERROR(__sensamp_dqbyte_configure(sa_dqbyte, calibrate, cal->dq));
    PROPAGATE_ERROR(sensamp_dqbyte_set_state_reg_if(sa_dqbyte, SA_STATE_DYNAMIC));
    return WDDR_SUCCESS;
}

static wddr_return_t __sensamp_dqbyte_configure(sensamp_dqbyte_dev_t *sa_dqbyte,
                                                bool calibrate,
                                                sensamp_dqbit_cal_t data[WDDR_PHY_RANK][WDDR_PHY_DQ_SLICE_NUM])
{
    uint8_t val;
    uint8_t code_down, code_up, code_avg, cal_code;
    uint8_t bit_index, sa_index, rank_index;
    sensamp_dqbit_dev_t *sensamp_dqbit;

    for (bit_index = 0; bit_index < WDDR_PHY_DQ_SLICE_NUM; bit_index++)
    {
        sensamp_dqbit = &sa_dqbyte->rank[WDDR_RANK_0].dq.bit[bit_index];

        for (sa_index = SA_0_INDEX; sa_index < WDDR_PHY_CFG; sa_index++)
        {
            // Calibrate before updating value
            if (calibrate)
            {
                for (cal_code = WAV_SA_MIN_CAL_CODE; cal_code <= WAV_SA_MAX_CAL_CODE; cal_code++)
                {
                    sensamp_dqbit_set_cal_code_reg_if(sensamp_dqbit, sa_index, cal_code, true);
                    sensamp_dqbyte_get_status_reg_if(sa_dqbyte, bit_index, (sensamp_index_t) sa_index, &val);

                    // val for first cal_code must be 1
                    if (cal_code == WAV_SA_MIN_CAL_CODE && val == 0 )
                    {
                        return WDDR_ERROR;
                    }
                    // val for last cal code must be 0
                    else if (cal_code == WAV_SA_MAX_CAL_CODE && val == 1)
                    {
                        return WDDR_ERROR;
                    }
                    else if (val == 0)
                    {
                        break;
                    }
                }
                code_up = cal_code;

                cal_code = WAV_SA_MAX_CAL_CODE;
                do
                {
                    sensamp_dqbit_set_cal_code_reg_if(sensamp_dqbit, sa_index, cal_code, true);
                    sensamp_dqbyte_get_status_reg_if(sa_dqbyte, bit_index, (sensamp_index_t) sa_index, &val);

                    // val for first cal_code must be 1
                    if (cal_code == WAV_SA_MIN_CAL_CODE && val == 0)
                    {
                        return WDDR_ERROR;
                    }
                    // val for last cal code must be 0
                    else if (cal_code == WAV_SA_MAX_CAL_CODE && val == 1 )
                    {
                        return WDDR_ERROR;
                    }
                    else if (val == 1)
                    {
                        break;
                    }
                } while ((cal_code--) > WAV_SA_MIN_CAL_CODE);

                code_down = cal_code;
                code_avg = (code_down + code_up) / 2;
                // save code into calibration table
                data[rank_index][bit_index].code[sa_index] = code_avg;
            } // if (calibrate)

            // Update value for RANK 0 and RANK 1
            for (rank_index = WDDR_RANK_0; rank_index < WDDR_PHY_RANK; rank_index++)
            {
                sensamp_dqbit_set_cal_code_reg_if(&sa_dqbyte->rank[rank_index].dq.bit[bit_index],
                                                  sa_index,
                                                  data[rank_index][bit_index].code[sa_index],
                                                  false);
            }
        }
    }
    return WDDR_SUCCESS;
}
