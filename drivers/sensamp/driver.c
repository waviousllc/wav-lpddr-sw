/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#include <sensamp/driver.h>
#include <wddr/memory_map.h>
#include <kernel/io.h>

#define SA_ENABLE       (0x1)
#define SA_DISABLE      (0x0)
#define WAV_SA_MIN_CAL_CODE (0)
#define WAV_SA_MAX_CAL_CODE (31)
#define WAV_SA_MID_CAL_CODE ((WAV_SA_MAX_CAL_CODE - WAV_SA_MIN_CAL_CODE + 1) / 2)
#define WAV_SA_CAL_DIR_NEG  (0)
#define WAV_SA_CAL_DIR_POS  (1)

// Base
#define SA_DQ_BIT_BASE__ADDR           (DDR_DQ_DQ_RX_SA_M0_R0_CFG_0__ADR)
#define SA_DQ_BIT_STA_BASE__ADDR       (DDR_DQ_DQ_RX_SA_STA_0__ADR)

// Mode, rank, bit offsets
// DQ
#define SA_DQ_BIT__OFFSET           (DDR_DQ_DQ_RX_SA_M0_R0_CFG_1__ADR - DDR_DQ_DQ_RX_SA_M0_R0_CFG_0__ADR)
#define SA_DQ_RANK__OFFSET          (DDR_DQ_DQ_RX_SA_M0_R1_CFG_0__ADR - DDR_DQ_DQ_RX_SA_M0_R0_CFG_0__ADR)
#define SA_DQ_MODE__OFFSET          (DDR_DQ_DQ_RX_SA_M1_R0_CFG_0__ADR - DDR_DQ_DQ_RX_SA_M0_R0_CFG_0__ADR)

#define SA_DQ_STA_BIT__OFFSET       (DDR_DQ_DQ_RX_SA_STA_1__ADR       - DDR_DQ_DQ_RX_SA_STA_0__ADR)

#define SA_DQS_CMN_BASE__ADDR       (DDR_DQ_DQS_RX_SA_CMN_CFG__ADR)

/**
 * @brief   Sensamp DQ Bit Initialization Register Interface
 *
 * @details Initializes Sensamp DQ Bit device at register interface level.
 *
 * @param[in]   sa_dqbit    pointer to Sensamp DQ Bit device.
 * @param[in]   base        base address of Sensamp DQ Bit device.
 * @param[in]   bit         bit index of Sensamp DQ bit device.
 * @param[in]   rank        rank of Sensamp DQ Bit device.
 *
 * @return      void
 */
static void sensamp_dqbit_init_reg_if(sensamp_dqbit_dev_t *sa_dqbit,
                                      uint32_t base,
                                      uint8_t bit,
                                      wddr_rank_t rank);

/**
 * @brief   Sensamp DQ Bit Set MSR Register Interface
 *
 * @details Configures Sensamp DQ Bit for given MSR value.
 *
 * @param[in]   sa_dqbit    pointer to Sensamp DQ Bit device.
 * @param[in]   msr         MSR to configure.
 * @param[in]   bit         bit index of Sensamp DQ bit device.
 * @param[in]   rank        rank of Sensamp DQ Bit device.
 *
 * @return      void
 */
static void sensamp_dqbit_set_msr_reg_if(sensamp_dqbit_dev_t *sa_dqbit,
                                         wddr_msr_t msr,
                                         uint8_t bit,
                                         wddr_rank_t rank);

void sensamp_dqbyte_set_msr_reg_if(sensamp_dqbyte_dev_t *sa_dqbyte,
                                   wddr_msr_t msr)
{
    uint8_t bit_index, rank_index;
    for (rank_index = 0; rank_index < WDDR_PHY_RANK; rank_index++)
    {
        for (bit_index = 0; bit_index < WDDR_PHY_DQ_SLICE_NUM; bit_index++)
        {
            sensamp_dqbit_set_msr_reg_if(&sa_dqbyte->rank[rank_index].dq.bit[bit_index],
                                         msr,
                                         bit_index,
                                         rank_index);
        }
    }

}

void sensamp_dqbyte_init_reg_if(sensamp_dqbyte_dev_t *sa_dqbyte,
                                uint32_t base)
{
    uint8_t rank_index, bit_index;

    sa_dqbyte->base = base;

    for (rank_index = 0; rank_index < WDDR_PHY_RANK; rank_index++)
    {
        for (bit_index = 0; bit_index < WDDR_PHY_DQ_SLICE_NUM; bit_index++)
        {
            sensamp_dqbit_init_reg_if(&sa_dqbyte->rank[rank_index].dq.bit[bit_index],
                                      base,
                                      bit_index,
                                      rank_index);
        }
    }
}

void sensamp_dqbyte_get_status_reg_if(sensamp_dqbyte_dev_t *sa_dqbyte,
                                      uint8_t bit,
                                      sensamp_index_t sa_index,
                                      uint8_t *status)
{
    uint32_t reg_val;
    reg_val = reg_read(sa_dqbyte->base + SA_DQ_BIT_STA_BASE__ADDR +
                       bit * SA_DQ_STA_BIT__OFFSET);
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

wddr_return_t sensamp_dqbyte_set_state_reg_if(sensamp_dqbyte_dev_t *sa_dqbyte,
                                              sensamp_state_t state)
{
    uint32_t reg_val;

    reg_val = reg_read(sa_dqbyte->base + DDR_DQ_DQS_RX_SA_CMN_CFG__ADR);

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
            return WDDR_ERROR;
    }

    reg_write(sa_dqbyte->base + DDR_DQ_DQS_RX_SA_CMN_CFG__ADR, reg_val);
    return WDDR_SUCCESS;
}

void sensamp_dqbit_set_cal_code_reg_if(sensamp_dqbit_dev_t *sa_dqbit,
                                       sensamp_index_t sa_index,
                                       uint8_t code,
                                       bool clear)
{
    uint32_t reg_val = 0;
    uint32_t cal_code;
    uint8_t cal_dir;

    if (!clear)
    {
        reg_val = reg_read(sa_dqbit->base);
    }

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

    reg_write(sa_dqbit->base, reg_val);
}

static void sensamp_dqbit_init_reg_if(sensamp_dqbit_dev_t *sa_dqbit,
                                      uint32_t base,
                                      uint8_t bit,
                                      wddr_rank_t rank)
{
    sa_dqbit->base = base;
    sensamp_dqbit_set_msr_reg_if(sa_dqbit, WDDR_MSR_0, bit, rank);
}

static void sensamp_dqbit_set_msr_reg_if(sensamp_dqbit_dev_t *sa_dqbit,
                                         wddr_msr_t msr,
                                         uint8_t bit,
                                         wddr_rank_t rank)
{
    sa_dqbit->base = (sa_dqbit->base & ~WDDR_MEMORY_MAP_REL_MASK) +
                      SA_DQ_BIT_BASE__ADDR + bit * SA_DQ_BIT__OFFSET +
                      rank * SA_DQ_RANK__OFFSET + msr * SA_DQ_MODE__OFFSET;
}
