/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <dq/driver.h>
#include <kernel/io.h>
#include <FreeRTOS.h>

/*******************************************************************************
**                            DQ
*******************************************************************************/
void dq_dq_rt_set_pipe_en_reg_if(dq_reg_t *dq_reg,
                                 wddr_msr_t msr,
                                 wddr_rank_t rank,
                                 uint32_t pipe_en)
{
    dq_reg->DDR_DQ_DQ_TX_RT_CFG[msr][rank] = pipe_en;
}

void dq_dq_sdr_set_fc_delay_reg_if(dq_reg_t *dq_reg,
                                   wddr_msr_t msr,
                                   wddr_rank_t rank,
                                   uint8_t bit,
                                   uint32_t fc_delay)
{
    configASSERT(bit < WDDR_PHY_DQ_SLICE_NUM);
    dq_reg->DDR_DQ_DQ_TX_SDR_FC_DLY_CFG[msr][rank][bit] = fc_delay;
}

void dq_dq_sdr_set_pipe_en_reg_if(dq_reg_t *dq_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  uint8_t bit,
                                  uint32_t pipe_en)
{
    configASSERT(bit < WDDR_PHY_DQ_SLICE_NUM);
    dq_reg->DDR_DQ_DQ_TX_SDR_CFG[msr][rank][bit] = pipe_en;
}

void dq_dq_sdr_set_x_sel_reg_if(dq_reg_t *dq_reg,
                                wddr_msr_t msr,
                                wddr_rank_t rank,
                                uint8_t bit,
                                uint32_t x_sel)
{
    configASSERT(bit < WDDR_PHY_DQ_SLICE_NUM);
    dq_reg->DDR_DQ_DQ_TX_SDR_X_SEL_CFG[msr][rank][bit] = x_sel;
}

void dq_dq_ddr_set_pipe_en_reg_if(dq_reg_t *dq_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  uint8_t bit,
                                  uint32_t pipe_en)
{
    configASSERT(bit < WDDR_PHY_DQ_SLICE_NUM);
    dq_reg->DDR_DQ_DQ_TX_DDR_CFG[msr][rank][bit] = pipe_en;
}

void dq_dq_ddr_set_x_sel_reg_if(dq_reg_t *dq_reg,
                                wddr_msr_t msr,
                                wddr_rank_t rank,
                                uint8_t bit,
                                uint32_t x_sel)
{
    configASSERT(bit < WDDR_PHY_DQ_SLICE_NUM);
    dq_reg->DDR_DQ_DQ_TX_DDR_X_SEL_CFG[msr][rank][bit] = x_sel;
}

void dq_dq_qdr_set_pipe_en_reg_if(dq_reg_t *dq_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  uint8_t bit,
                                  uint32_t pipe_en)
{
    configASSERT(bit < WDDR_PHY_DQ_SLICE_NUM);
    dq_reg->DDR_DQ_DQ_TX_QDR_CFG[msr][rank][bit] = pipe_en;
}

void dq_dq_qdr_set_x_sel_reg_if(dq_reg_t *dq_reg,
                                wddr_msr_t msr,
                                wddr_rank_t rank,
                                uint8_t bit,
                                uint32_t x_sel)
{
    configASSERT(bit < WDDR_PHY_DQ_SLICE_NUM);
    dq_reg->DDR_DQ_DQ_TX_QDR_X_SEL_CFG[msr][rank][bit] = x_sel;
}

/*******************************************************************************
**                            DQS
*******************************************************************************/
void dq_dqs_rt_set_pipe_en_reg_if(dq_reg_t *dq_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  uint32_t pipe_en)
{
    dq_reg->DDR_DQ_DQS_TX_RT_CFG[msr][rank] = pipe_en;
}

void dq_dqs_sdr_set_fc_delay_reg_if(dq_reg_t *dq_reg,
                                    wddr_msr_t msr,
                                    wddr_rank_t rank,
                                    uint8_t bit,
                                    uint32_t fc_delay)
{
    configASSERT(bit < WDDR_PHY_DQS_SLICE_NUM);
    dq_reg->DDR_DQ_DQS_TX_SDR_FC_DLY_CFG[msr][rank][bit] = fc_delay;
}

void dq_dqs_sdr_set_pipe_en_reg_if(dq_reg_t *dq_reg,
                                   wddr_msr_t msr,
                                   wddr_rank_t rank,
                                   uint8_t bit,
                                   uint32_t pipe_en)
{
    configASSERT(bit < WDDR_PHY_DQS_SLICE_NUM);
    dq_reg->DDR_DQ_DQS_TX_SDR_CFG[msr][rank][bit] = pipe_en;
}

void dq_dqs_sdr_set_x_sel_reg_if(dq_reg_t *dq_reg,
                                 wddr_msr_t msr,
                                 wddr_rank_t rank,
                                 uint8_t bit,
                                 uint32_t x_sel)
{
    configASSERT(bit < WDDR_PHY_DQS_SLICE_NUM);
    dq_reg->DDR_DQ_DQS_TX_SDR_X_SEL_CFG[msr][rank][bit] = x_sel;
}

void dq_dqs_ddr_set_pipe_en_reg_if(dq_reg_t *dq_reg,
                                   wddr_msr_t msr,
                                   wddr_rank_t rank,
                                   uint8_t bit,
                                   uint32_t pipe_en)
{
    configASSERT(bit < WDDR_PHY_DQS_SLICE_NUM);
    dq_reg->DDR_DQ_DQS_TX_DDR_CFG[msr][rank][bit] = pipe_en;
}

void dq_dqs_ddr_set_x_sel_reg_if(dq_reg_t *dq_reg,
                                 wddr_msr_t msr,
                                 wddr_rank_t rank,
                                 uint8_t bit,
                                 uint32_t x_sel)
{
    configASSERT(bit < WDDR_PHY_DQS_SLICE_NUM);
    dq_reg->DDR_DQ_DQS_TX_DDR_X_SEL_CFG[msr][rank][bit] = x_sel;
}

void dq_dqs_qdr_set_pipe_en_reg_if(dq_reg_t *dq_reg,
                                   wddr_msr_t msr,
                                   wddr_rank_t rank,
                                   uint8_t bit,
                                   uint32_t pipe_en)
{
    configASSERT(bit < WDDR_PHY_DQS_SLICE_NUM);
    dq_reg->DDR_DQ_DQS_TX_QDR_CFG[msr][rank][bit] = pipe_en;
}

void dq_dqs_qdr_set_x_sel_reg_if(dq_reg_t *dq_reg,
                                 wddr_msr_t msr,
                                 wddr_rank_t rank,
                                 uint8_t bit,
                                 uint32_t x_sel)
{
    configASSERT(bit < WDDR_PHY_DQS_SLICE_NUM);
    dq_reg->DDR_DQ_DQS_TX_QDR_X_SEL_CFG[msr][rank][bit] = x_sel;
}
