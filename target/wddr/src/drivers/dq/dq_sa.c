
/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <dq/driver.h>
#include <kernel/io.h>
#include <FreeRTOS.h>

#define SA_ENABLE                   (0x1)
#define SA_DISABLE                  (0x0)
#define WAV_SA_MIN_CAL_CODE         (0)
#define WAV_SA_MAX_CAL_CODE         (31)
#define WAV_SA_MID_CAL_CODE         ((WAV_SA_MAX_CAL_CODE - WAV_SA_MIN_CAL_CODE + 1) / 2)
#define WAV_SA_CAL_DIR_NEG          (0)
#define WAV_SA_CAL_DIR_POS          (1)

/*******************************************************************************
**                            DQ
*******************************************************************************/
void dq_dq_sa_get_status_reg_if(dq_reg_t *dq_reg,
                                uint8_t bit,
                                sensamp_index_t sa_index,
                                uint8_t *status)
{
    configASSERT(bit < WDDR_PHY_DQ_SLICE_NUM);
    uint32_t reg_val = dq_reg->DDR_DQ_DQ_RX_SA_STA[bit];
    switch (sa_index)
    {
        case SA_0_INDEX:
            *status = GET_REG_FIELD(reg_val, DDR_DQ_DQ_RX_SA_STA_0_SA_OUT_0);
            break;
        case SA_90_INDEX:
            *status = GET_REG_FIELD(reg_val, DDR_DQ_DQ_RX_SA_STA_0_SA_OUT_90);
            break;
        case SA_180_INDEX:
            *status = GET_REG_FIELD(reg_val, DDR_DQ_DQ_RX_SA_STA_0_SA_OUT_180);
            break;
        case SA_270_INDEX:
            *status = GET_REG_FIELD(reg_val, DDR_DQ_DQ_RX_SA_STA_0_SA_OUT_270);
            break;
    }
}

void dq_dq_sa_clear_cal_code_reg_if(dq_reg_t *dq_reg,
                                    wddr_msr_t msr,
                                    wddr_rank_t rank,
                                    uint8_t bit)
{
    configASSERT(bit < WDDR_PHY_DQ_SLICE_NUM);
    dq_reg->DDR_DQ_DQ_RX_SA_CFG[msr][rank][bit] = 0x0;
}

void dq_dq_sa_set_cal_code_reg_if(dq_reg_t *dq_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  uint8_t bit,
                                  sensamp_index_t sa_index,
                                  uint8_t code)
{
    configASSERT(bit < WDDR_PHY_DQ_SLICE_NUM);
    uint32_t cal_code;
    uint8_t cal_dir;
    uint32_t reg_val = dq_reg->DDR_DQ_DQ_RX_SA_CFG[msr][rank][bit];

    if (code < WAV_SA_MID_CAL_CODE)
    {
        cal_dir = WAV_SA_CAL_DIR_NEG;
        cal_code = WAV_SA_MID_CAL_CODE - (code + 1);
    }
    else
    {
        cal_dir = WAV_SA_CAL_DIR_POS;
        cal_code = code - WAV_SA_MID_CAL_CODE;
    }

    switch (sa_index)
    {
        case SA_0_INDEX:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_RX_SA_M0_R0_CFG_0_CAL_DIR_0, cal_dir);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_RX_SA_M0_R0_CFG_0_CAL_CODE_0, cal_code);
            break;
        case SA_90_INDEX:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_RX_SA_M0_R0_CFG_0_CAL_DIR_90, cal_dir);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_RX_SA_M0_R0_CFG_0_CAL_CODE_90, cal_code);
            break;
        case SA_180_INDEX:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_RX_SA_M0_R0_CFG_0_CAL_DIR_180, cal_dir);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_RX_SA_M0_R0_CFG_0_CAL_CODE_180, cal_code);
            break;
        case SA_270_INDEX:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_RX_SA_M0_R0_CFG_0_CAL_DIR_270, cal_dir);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_RX_SA_M0_R0_CFG_0_CAL_CODE_270, cal_code);
            break;
        default:
            return;
    }

    dq_reg->DDR_DQ_DQ_RX_SA_CFG[msr][rank][bit] = reg_val;
}

/*******************************************************************************
**                            DQS
*******************************************************************************/
void dq_dqs_sa_cmn_set_state_reg_if(dq_reg_t *dq_reg,
                                    sensamp_state_t state)
{
    uint32_t reg_val = dq_reg->DDR_DQ_DQS_RX_SA_CMN_CFG;

    switch (state)
    {
        case SA_STATE_ENABLED:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_SA_CMN_CFG_OVR_EN_0_180, SA_ENABLE);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_SA_CMN_CFG_CAL_EN_0_180, SA_DISABLE);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_SA_CMN_CFG_SW_OVR, 0x1);
            if (WDDR_PHY_CFG == WDDR_PHY_CFG_QDR)
            {
                reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_SA_CMN_CFG_OVR_EN_90_270, SA_ENABLE);
                reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_SA_CMN_CFG_CAL_EN_90_270, SA_DISABLE);
            }
            break;
        case SA_STATE_DYNAMIC:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_SA_CMN_CFG_OVR_EN_0_180, SA_DISABLE);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_SA_CMN_CFG_CAL_EN_0_180, SA_DISABLE);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_SA_CMN_CFG_SW_OVR, 0x0);
            if (WDDR_PHY_CFG == WDDR_PHY_CFG_QDR)
            {
                reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_SA_CMN_CFG_OVR_EN_90_270, SA_DISABLE);
                reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_SA_CMN_CFG_CAL_EN_90_270, SA_DISABLE);
            }
            break;
        case SA_STATE_CAL_ENABLED:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_SA_CMN_CFG_CAL_EN_0_180, SA_ENABLE);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_SA_CMN_CFG_OVR_EN_0_180, SA_ENABLE);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_SA_CMN_CFG_SW_OVR, 0x1);
            if (WDDR_PHY_CFG == WDDR_PHY_CFG_QDR)
            {
                reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_SA_CMN_CFG_CAL_EN_90_270, SA_ENABLE);
                reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_SA_CMN_CFG_OVR_EN_90_270, SA_ENABLE);
            }
            break;
        default:
            return;
    }

    dq_reg->DDR_DQ_DQS_RX_SA_CMN_CFG = reg_val;
}
