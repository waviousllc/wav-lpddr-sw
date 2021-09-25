/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <ca/driver.h>
#include <kernel/io.h>
#include <FreeRTOS.h>

/*******************************************************************************
**                            CA
*******************************************************************************/
void ca_dq_rt_set_pipe_en_reg_if(ca_reg_t *ca_reg,
                                 wddr_msr_t msr,
                                 wddr_rank_t rank,
                                 uint32_t pipe_en)
{
    ca_reg->DDR_CA_DQ_TX_RT_CFG[msr][rank] = pipe_en;
}

void ca_dq_sdr_set_fc_delay_reg_if(ca_reg_t *ca_reg,
                                   wddr_msr_t msr,
                                   wddr_rank_t rank,
                                   uint8_t bit,
                                   uint32_t fc_delay)
{
    configASSERT(bit < WDDR_PHY_CA_SLICE_NUM);
    ca_reg->DDR_CA_DQ_TX_SDR_FC_DLY_CFG[msr][rank][bit] = fc_delay;
}

void ca_dq_sdr_set_pipe_en_reg_if(ca_reg_t *ca_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  uint8_t bit,
                                  uint32_t pipe_en)
{
    configASSERT(bit < WDDR_PHY_CA_SLICE_NUM);
    ca_reg->DDR_CA_DQ_TX_SDR_CFG[msr][rank][bit] = pipe_en;
}

void ca_dq_sdr_set_x_sel_reg_if(ca_reg_t *ca_reg,
                                wddr_msr_t msr,
                                wddr_rank_t rank,
                                uint8_t bit,
                                uint32_t x_sel)
{
    configASSERT(bit < WDDR_PHY_CA_SLICE_NUM);
    ca_reg->DDR_CA_DQ_TX_SDR_X_SEL_CFG[msr][rank][bit] = x_sel;
}

void ca_dq_ddr_set_pipe_en_reg_if(ca_reg_t *ca_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  uint8_t bit,
                                  uint32_t pipe_en)
{
    configASSERT(bit < WDDR_PHY_CA_SLICE_NUM);
    ca_reg->DDR_CA_DQ_TX_DDR_CFG[msr][rank][bit] = pipe_en;
}

void ca_dq_ddr_set_x_sel_reg_if(ca_reg_t *ca_reg,
                                wddr_msr_t msr,
                                wddr_rank_t rank,
                                uint8_t bit,
                                uint32_t x_sel)
{
    configASSERT(bit < WDDR_PHY_CA_SLICE_NUM);
    ca_reg->DDR_CA_DQ_TX_DDR_X_SEL_CFG[msr][rank][bit] = x_sel;
}

void ca_dq_qdr_set_pipe_en_reg_if(ca_reg_t *ca_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  uint8_t bit,
                                  uint32_t pipe_en)
{
    configASSERT(bit < WDDR_PHY_CA_SLICE_NUM);
    ca_reg->DDR_CA_DQ_TX_QDR_CFG[msr][rank][bit] = pipe_en;
}

void ca_dq_qdr_set_x_sel_reg_if(ca_reg_t *ca_reg,
                                wddr_msr_t msr,
                                wddr_rank_t rank,
                                uint8_t bit,
                                uint32_t x_sel)
{
    configASSERT(bit < WDDR_PHY_CA_SLICE_NUM);
    ca_reg->DDR_CA_DQ_TX_QDR_X_SEL_CFG[msr][rank][bit] = x_sel;
}

/*******************************************************************************
**                            CK
*******************************************************************************/
void ca_dqs_rt_set_pipe_en_reg_if(ca_reg_t *ca_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  uint32_t pipe_en)
{
    ca_reg->DDR_CA_DQS_TX_RT_CFG[msr][rank] = pipe_en;
}

void ca_dqs_sdr_set_fc_delay_reg_if(ca_reg_t *ca_reg,
                                    wddr_msr_t msr,
                                    wddr_rank_t rank,
                                    uint8_t bit,
                                    uint32_t fc_delay)
{
    configASSERT(bit < WDDR_PHY_CK_SLICE_NUM);
    ca_reg->DDR_CA_DQS_TX_SDR_FC_DLY_CFG[msr][rank][bit] = fc_delay;
}

void ca_dqs_sdr_set_pipe_en_reg_if(ca_reg_t *ca_reg,
                                   wddr_msr_t msr,
                                   wddr_rank_t rank,
                                   uint8_t bit,
                                   uint32_t pipe_en)
{
    configASSERT(bit < WDDR_PHY_CK_SLICE_NUM);
    ca_reg->DDR_CA_DQS_TX_SDR_CFG[msr][rank][bit] = pipe_en;
}

void ca_dqs_sdr_set_x_sel_reg_if(ca_reg_t *ca_reg,
                                 wddr_msr_t msr,
                                 wddr_rank_t rank,
                                 uint8_t bit,
                                 uint32_t x_sel)
{
    configASSERT(bit < WDDR_PHY_CK_SLICE_NUM);
    ca_reg->DDR_CA_DQS_TX_SDR_X_SEL_CFG[msr][rank][bit] = x_sel;
}

void ca_dqs_ddr_set_pipe_en_reg_if(ca_reg_t *ca_reg,
                                   wddr_msr_t msr,
                                   wddr_rank_t rank,
                                   uint8_t bit,
                                   uint32_t pipe_en)
{
    configASSERT(bit < WDDR_PHY_CK_SLICE_NUM);
    ca_reg->DDR_CA_DQS_TX_DDR_CFG[msr][rank][bit] = pipe_en;
}

void ca_dqs_ddr_set_x_sel_reg_if(ca_reg_t *ca_reg,
                                 wddr_msr_t msr,
                                 wddr_rank_t rank,
                                 uint8_t bit,
                                 uint32_t x_sel)
{
    configASSERT(bit < WDDR_PHY_CK_SLICE_NUM);
    ca_reg->DDR_CA_DQS_TX_DDR_X_SEL_CFG[msr][rank][bit] = x_sel;
}

void ca_dqs_qdr_set_pipe_en_reg_if(ca_reg_t *ca_reg,
                                   wddr_msr_t msr,
                                   wddr_rank_t rank,
                                   uint8_t bit,
                                   uint32_t pipe_en)
{
    configASSERT(bit < WDDR_PHY_CK_SLICE_NUM);
    ca_reg->DDR_CA_DQS_TX_QDR_CFG[msr][rank][bit] = pipe_en;
}

void ca_dqs_qdr_set_x_sel_reg_if(ca_reg_t *ca_reg,
                                 wddr_msr_t msr,
                                 wddr_rank_t rank,
                                 uint8_t bit,
                                 uint32_t x_sel)
{
    configASSERT(bit < WDDR_PHY_CK_SLICE_NUM);
    ca_reg->DDR_CA_DQS_TX_QDR_X_SEL_CFG[msr][rank][bit] = x_sel;
}
