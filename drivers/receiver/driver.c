/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <compiler.h>
#include <receiver/driver.h>
#include <wddr/memory_map.h>
#include <kernel/io.h>

#define REC_FB_RES_MAX      (7)
#define REC_CAL_CODE_MIN    (0)
#define REC_CAL_CODE_MAX    (15)
#define REC_HYSTERESIS      (4)
#define RECEIVER_DISABLE    (0x0)
#define RECEIVER_ENABLE     (0x1)

#define RX_DQS_BASE__ADDR       (DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG__ADR)

// Rank / Mode Offsets
#define RX_DQS_RANK__OFFSET     (DDR_DQ_DQS_RX_IO_CMN_M0_R1_CFG__ADR - DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG__ADR)
#define RX_DQS_MODE__OFFSET     (DDR_DQ_DQS_RX_IO_CMN_M1_R0_CFG__ADR - DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG__ADR)

/**
 * @brief  Receiver Calibrate Single Ended Register Interface
 *
 * @details Performs calibration procedure on a single side of the Receiver
 *          device.
 *
 * @param[in]   receiver    pointer to Receiver device.
 * @param[out]  code        pointer to table to store calibrated codes.
 * @param[in]   side        the side to calibrate.
 *
 * @return      returns whether calibration was successful.
 * @retval      WDDR_SUCCESS if successful.
 * @retval      WDDR_ERROR otherwise.
 */
static wddr_return_t receiver_calibrate_single_ended_reg_if(receiver_dev_t *receiver,
                                                            uint8_t code[REC_PN_SIDE_NUM][REC_TC_SIDE_NUM],
                                                            receiver_tc_side_t side);

/**
 * @brief   Receiver Get Ouput Value Register Interface
 *
 * @details Gets the current output of the Receiver device.
 *
 * @param[in]   receiver    pointer to Receiver device.
 * @param[out]  val         pointer to store output value.
 *
 * @return      void.
 */
static void receiver_get_output_value_reg_if(receiver_dev_t *receiver,
                                             uint8_t *val);

void receiver_set_msr_reg_if(receiver_dev_t *receiver,
                             wddr_msr_t msr,
                             wddr_rank_t rank)
{
    uint32_t base = receiver->base & ~WDDR_MEMORY_MAP_REL_MASK;
    receiver->base = base + RX_DQS_BASE__ADDR + rank * RX_DQS_RANK__OFFSET +
                     msr * RX_DQS_MODE__OFFSET;
}

void receiver_init_reg_if(receiver_dev_t *receiver,
                          uint32_t base,
                          wddr_rank_t rank)
{
    receiver->base = base;
    receiver_set_msr_reg_if(receiver, WDDR_MSR_0, rank);
}

void receiver_set_state_reg_if(receiver_dev_t *receiver,
                               receiver_state_t state)
{
    uint32_t reg_val;
    reg_val = reg_read(receiver->base);

    switch(state)
    {
        case REC_STATE_DISABLE:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_EN, RECEIVER_DISABLE);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_RXCAL_EN, RECEIVER_DISABLE);
            break;
        case REC_STATE_ENABLE:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_EN, RECEIVER_ENABLE);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_RXCAL_EN, RECEIVER_DISABLE);
            break;
        case REC_STATE_CAL:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_EN, RECEIVER_ENABLE);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_RXCAL_EN, RECEIVER_ENABLE);
            break;
        default:
            return;
    }

    reg_write(receiver->base, reg_val);
}

void receiver_set_mode_reg_if(receiver_dev_t *receiver,
                              receiver_mode_t mode,
                              receiver_path_state_t path_state)
{
    uint32_t reg_val;
    reg_val = reg_read(receiver->base);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_SE_MODE, mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_DCPATH_EN, path_state);
    reg_write(receiver->base, reg_val);
}

void receiver_set_code_reg_if(receiver_dev_t *receiver,
                              uint8_t code[REC_PN_SIDE_NUM][REC_TC_SIDE_NUM])
{
    uint32_t reg_val;
    reg_val = reg_read(receiver->base);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_CAL_P_C, code[REC_P_SIDE][REC_C_SIDE]);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_CAL_P_T, code[REC_P_SIDE][REC_T_SIDE]);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_CAL_N_C, code[REC_N_SIDE][REC_C_SIDE]);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_CAL_N_T, code[REC_N_SIDE][REC_T_SIDE]);
    reg_write(receiver->base, reg_val);
}

void receiver_set_feedback_resistor_reg_if(receiver_dev_t *receiver,
                                           uint8_t feedback_resistor)
{
    uint32_t reg_val;
    reg_val = reg_read(receiver->base);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_FB_EN, feedback_resistor);
    reg_write(receiver->base, reg_val);
}

wddr_return_t receiver_calibrate_reg_if(receiver_dev_t *receiver,
                                        uint8_t code[REC_PN_SIDE_NUM][REC_TC_SIDE_NUM])
{
    receiver_set_state_reg_if(receiver, REC_STATE_CAL);
    receiver_set_mode_reg_if(receiver, REC_MODE_DIFF, REC_PATH_AC);
    PROPAGATE_ERROR(receiver_calibrate_single_ended_reg_if(receiver, code, REC_T_SIDE));
    PROPAGATE_ERROR(receiver_calibrate_single_ended_reg_if(receiver, code, REC_C_SIDE));
    receiver_set_state_reg_if(receiver, REC_STATE_DISABLE);
    return WDDR_SUCCESS;
}

static wddr_return_t receiver_calibrate_single_ended_reg_if(receiver_dev_t *receiver,
                                                            uint8_t code[REC_PN_SIDE_NUM][REC_TC_SIDE_NUM],
                                                            receiver_tc_side_t side)
{
    uint8_t     res_val = 0;
    uint8_t     cal_code;
    uint8_t     cal_code_down;
    uint8_t     cal_code_up;
    uint8_t     cal_code_avg;
    uint8_t     rec_hysteresis;

    // Up direction
    for (cal_code = REC_CAL_CODE_MIN; cal_code <= REC_CAL_CODE_MAX; cal_code++)
    {
        code[REC_N_SIDE][side] = REC_CAL_CODE_MIN + cal_code;
        code[REC_P_SIDE][side] = REC_CAL_CODE_MAX - cal_code;

        receiver_set_code_reg_if(receiver, code);
        receiver_get_output_value_reg_if(receiver, &res_val);

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
        receiver_get_output_value_reg_if(receiver, &res_val);

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

static void receiver_get_output_value_reg_if(__UNUSED__ receiver_dev_t *receiver,
                                             uint8_t *val)
{
    // TODO: Read output
    *val = 0;
}
