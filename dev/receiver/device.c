/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#include <receiver/device.h>
#include <receiver/driver.h>
#include <receiver/delay_driver.h>
#include <kernel/io.h>

#define REC_FB_RES_MAX      (7)
#define REC_CAL_CODE_MIN    (0)
#define REC_CAL_CODE_MAX    (15)
#define REC_HYSTERESIS      (4)
#define T_SIDE_VAL_MASK     (0xC)
#define C_SIDE_VAL_MASK     (0x3)

/**
 * @brief  Receiver Calibrate Single Ended Register Interface
 *
 * @details Performs calibration procedure on a single side of the Receiver
 *          device.
 *
 * @param[in]   receiver    pointer to Receiver device.
 * @param[out]  code        pointer to table to store calibrated codes.
 * @param[in]   side        the side to calibrate.
 * @param[in]   output_addr the address of where to read output feedback.
 *
 * @return      returns whether calibration was successful.
 * @retval      WDDR_SUCCESS if successful.
 * @retval      WDDR_ERROR otherwise.
 */
static wddr_return_t receiver_calibrate_single_ended(receiver_dev_t *receiver,
                                                     uint8_t code[REC_PN_SIDE_NUM][REC_TC_SIDE_NUM],
                                                     receiver_tc_side_t side,
                                                     uint32_t output_addr);

void receiver_init(receiver_dev_t *receiver, uint32_t base, wddr_rank_t rank)
{
    receiver_init_reg_if(receiver, base, rank);
    receiver_delay_init_reg_if(&receiver->rx_delay, base, rank);
}

void receiver_enable(receiver_dev_t  *receiver)
{
    receiver_set_state_reg_if(receiver, REC_STATE_ENABLE);
}

void receiver_disable(receiver_dev_t  *receiver)
{
    receiver_set_state_reg_if(receiver, REC_STATE_DISABLE);
}

wddr_return_t receiver_calibrate(receiver_dev_t *receiver, receiver_common_cal_t *cal, uint32_t output_addr)
{
    receiver_set_feedback_resistor_reg_if(receiver, REC_FB_RES_MAX);
    receiver_set_state_reg_if(receiver, REC_STATE_CAL);
    receiver_set_mode_reg_if(receiver, REC_MODE_DIFF, REC_PATH_AC);
    PROPAGATE_ERROR(receiver_calibrate_single_ended(receiver, cal->cal_code, REC_T_SIDE, output_addr));
    PROPAGATE_ERROR(receiver_calibrate_single_ended(receiver, cal->cal_code, REC_C_SIDE, output_addr));
    receiver_set_state_reg_if(receiver, REC_STATE_DISABLE);
    return WDDR_SUCCESS;
}

void receiver_set_delay(receiver_dev_t *receiver,
                        uint16_t delay,
                        receiver_tc_side_mask_t side)
{
    receiver_delay_set_delay_code_reg_if(&receiver->rx_delay, delay, side);
}


static wddr_return_t receiver_calibrate_single_ended(receiver_dev_t *receiver,
                                                     uint8_t code[REC_PN_SIDE_NUM][REC_TC_SIDE_NUM],
                                                     receiver_tc_side_t side,
                                                     uint32_t output_addr)
{
    uint8_t     res_val = 0;
    uint8_t     cal_code;
    uint8_t     cal_code_down;
    uint8_t     cal_code_up;
    uint16_t    cal_code_avg;
    uint8_t     rec_hysteresis;
    uint8_t     res_val_mask = side == REC_T_SIDE ? T_SIDE_VAL_MASK : C_SIDE_VAL_MASK;

    // Up direction
    for (cal_code = REC_CAL_CODE_MIN; cal_code <= REC_CAL_CODE_MAX; cal_code++)
    {
        code[REC_N_SIDE][side] = REC_CAL_CODE_MIN + cal_code;
        code[REC_P_SIDE][side] = REC_CAL_CODE_MAX - cal_code;

        receiver_set_code_reg_if(receiver, code);
        res_val = reg_read(output_addr) & res_val_mask;

        if (res_val == 1)
        {
            break;
        }
    }

    // Validate final result
    if (cal_code == REC_CAL_CODE_MIN || cal_code == REC_CAL_CODE_MAX)
    {
        return WDDR_ERROR;
    }

    cal_code_up = cal_code;

    // Down direction
    cal_code = REC_CAL_CODE_MAX;
    do
    {
        code[REC_N_SIDE][side] = REC_CAL_CODE_MIN + cal_code;
        code[REC_P_SIDE][side] = REC_CAL_CODE_MAX - cal_code;

        receiver_set_code_reg_if(receiver, code);
        res_val = reg_read(output_addr) & res_val_mask;

        if (res_val == 0)
        {
            break;
        }

    } while((cal_code--) > REC_CAL_CODE_MIN);

    // Validate final result
    if (cal_code == REC_CAL_CODE_MIN || cal_code == REC_CAL_CODE_MAX)
    {
        return WDDR_ERROR;
    }

    cal_code_down = cal_code;

    cal_code_avg = (cal_code_up + cal_code_down) / 2;

    rec_hysteresis = REC_HYSTERESIS;

    if (cal_code_avg + rec_hysteresis > REC_CAL_CODE_MAX)
    {
        rec_hysteresis = REC_CAL_CODE_MAX - cal_code_avg;
    }

    if (cal_code_avg < REC_CAL_CODE_MIN + rec_hysteresis)
    {
        rec_hysteresis = REC_CAL_CODE_MIN + cal_code_avg;
    }

    // Add hysteresis
    if (side == REC_T_SIDE)
    {
        code[REC_N_SIDE][side] = REC_CAL_CODE_MIN + cal_code_avg + rec_hysteresis;
        code[REC_P_SIDE][side] = REC_CAL_CODE_MAX - cal_code_avg + rec_hysteresis;
    }
    else
    {
        code[REC_N_SIDE][side] = REC_CAL_CODE_MIN + cal_code_avg - rec_hysteresis;
        code[REC_P_SIDE][side] = REC_CAL_CODE_MAX - cal_code_avg - rec_hysteresis;
    }

    receiver_set_code_reg_if(receiver, code);

    return WDDR_SUCCESS;
}
