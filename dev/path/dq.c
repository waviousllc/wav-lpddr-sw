/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <path/dq.h>
#include <gearbox/tx_driver.h>
#include <gearbox/rx_driver.h>
#include <pipeline/rt_driver.h>
#include <pi/driver.h>
#include <pi/tx_driver.h>
#include <pi/rx_driver.h>
#include <lpde/driver.h>
#include <lpde/rx_driver.h>
#include <lpde/tx_driver.h>

/** @brief  Initializes DQ TX Path for the given rank */
static void dq_tx_path_init_rank(dq_tx_path_t *dq_path, uint32_t base, wddr_rank_t rank);

/** @brief  Initializes DQ TX Path for rank independent devices */
static void dq_tx_path_init(dq_tx_path_t *dq_path, uint32_t base);

/** @brief  Initializes DQ RX Path for the given rank */
static void dq_rx_path_init_rank(dq_rx_path_t *dq_path, uint32_t base, wddr_rank_t rank);

/** @brief  Initializes DQ RX Path for rank independent devices */
static void dq_rx_path_init(dq_rx_path_t *dq_path, uint32_t base);

/** @brief  Sets state of all devices in the DQ Path */
static void dq_path_set_state(dq_path_t *dq_path, bool enable);

void dq_path_init(dq_path_t *dq_path, uint32_t base)
{
    // Rank dependent
    for (uint8_t rank = 0; rank < WDDR_PHY_RANK; rank++)
    {
        dq_tx_path_init_rank(&dq_path->tx, base, rank);
        dq_rx_path_init_rank(&dq_path->rx, base, rank);
    }

    // Rank independent
    dq_tx_path_init(&dq_path->tx, base);
    dq_rx_path_init(&dq_path->rx, base);
}

void dq_path_enable(dq_path_t *dq_path)
{
    dq_path_set_state(dq_path, true);
}

static void dq_tx_path_init_rank(dq_tx_path_t *dq_path, uint32_t base, wddr_rank_t rank)
{
    uint8_t bit_index;

    // DQ
    for (bit_index = 0; bit_index < WDDR_PHY_DQ_SLICE_NUM; bit_index++)
    {
        pipeline_bit_init(&dq_path->rank[rank].dq.pipeline[bit_index], base, WDDR_SLICE_TYPE_DQ, rank, bit_index);
        tx_lpde_init(&dq_path->rank[rank].dq.lpde[bit_index], base, WDDR_SLICE_TYPE_DQ, rank, bit_index);
    }
    rt_pipeline_init_reg_if(&dq_path->rank[rank].dq.rt, base, WDDR_SLICE_TYPE_DQ, rank);
    tx_pi_init(&dq_path->rank[rank].dq.pi, base, WDDR_SLICE_TYPE_DQ, rank);

    // DQS
    for (bit_index = 0; bit_index < WDDR_PHY_DQS_SLICE_NUM; bit_index++)
    {
        pipeline_bit_init(&dq_path->rank[rank].dqs.pipeline[bit_index], base, WDDR_SLICE_TYPE_DQS, rank, bit_index);
        if (bit_index < WDDR_PHY_DQS_TXRX_SLICE_NUM)
        {
            tx_lpde_init(&dq_path->rank[rank].dqs.lpde[bit_index], base, WDDR_SLICE_TYPE_DQS, rank, bit_index);
        }
    }

    rt_pipeline_init_reg_if(&dq_path->rank[rank].dqs.rt, base, WDDR_SLICE_TYPE_DQS, rank);
    tx_pi_init(&dq_path->rank[rank].dqs.pi, base, WDDR_SLICE_TYPE_DQS, rank);
    driver_cmn_init(&dq_path->rank[rank].dqs.driver, base, WDDR_SLICE_TYPE_DQS, rank);
}

static void dq_tx_path_init(dq_tx_path_t *dq_path, uint32_t base)
{
    // Bit TX Driver
    driver_init(&dq_path->dq.driver, base, WDDR_SLICE_TYPE_DQ);
    driver_init(&dq_path->dqs.driver, base, WDDR_SLICE_TYPE_DQS);
    tx_gb_init_reg_if(&dq_path->gearbox, base, WDDR_SLICE_TYPE_DQS);
}

static void dq_rx_path_init_rank(dq_rx_path_t *dq_path, uint32_t base, wddr_rank_t rank)
{
    rx_pi_init(&dq_path->rank[rank].dqs.pi, base, WDDR_SLICE_TYPE_DQS, rank);
    rx_lpde_init(&dq_path->rank[rank].dqs.sdr_lpde, base, WDDR_SLICE_TYPE_DQS, rank);
    receiver_init(&dq_path->rank[rank].dqs.receiver, base, rank);
}

static void dq_rx_path_init(dq_rx_path_t *dq_path, uint32_t base)
{
    sensamp_dqbyte_init(&dq_path->sa, base);
    rx_gb_init_reg_if(&dq_path->gearbox, base, WDDR_SLICE_TYPE_DQ);
}

static void dq_path_set_state(dq_path_t *dq_path, bool enable)
{
    uint8_t rank_index, bit_index;
    pi_state_t pi_state = enable ? PI_STATE_ENABLED : PI_STATE_DISABLED;
    lpde_state_t lpde_state = enable ? LPDE_STATE_ENABLED : LPDE_STATE_ENABLED;

    for (rank_index = 0; rank_index < WDDR_PHY_RANK; rank_index++)
    {
        for (int8_t msr = WDDR_MSR_1; msr >= WDDR_MSR_0; msr--)
        {
            // Phase Interpolators
            // DQ
            tx_pi_set_msr_reg_if(&dq_path->tx.rank[rank_index].dq.pi, msr, WDDR_SLICE_TYPE_DQ, rank_index);
            pi_set_state_reg_if(&dq_path->tx.rank[rank_index].dq.pi.odr, pi_state);
            pi_set_state_reg_if(&dq_path->tx.rank[rank_index].dq.pi.qdr, pi_state);
            pi_set_state_reg_if(&dq_path->tx.rank[rank_index].dq.pi.ddr, pi_state);
            pi_set_state_reg_if(&dq_path->tx.rank[rank_index].dq.pi.rt, pi_state);

            // DQS
            tx_pi_set_msr_reg_if(&dq_path->tx.rank[rank_index].dqs.pi, msr, WDDR_SLICE_TYPE_DQS, rank_index);
            pi_set_state_reg_if(&dq_path->tx.rank[rank_index].dqs.pi.odr, pi_state);
            pi_set_state_reg_if(&dq_path->tx.rank[rank_index].dqs.pi.qdr, pi_state);
            pi_set_state_reg_if(&dq_path->tx.rank[rank_index].dqs.pi.ddr, pi_state);
            pi_set_state_reg_if(&dq_path->tx.rank[rank_index].dqs.pi.rt, pi_state);
            pi_set_state_reg_if(&dq_path->tx.rank[rank_index].dqs.pi.sdr, pi_state);
            pi_set_state_reg_if(&dq_path->tx.rank[rank_index].dqs.pi.dfi, pi_state);

            // LPDE
            // DQ
            for (bit_index = 0; bit_index < WDDR_PHY_DQ_SLICE_NUM; bit_index++)
            {
                tx_lpde_set_msr_reg_if(&dq_path->tx.rank[rank_index].dq.lpde[bit_index], msr, WDDR_SLICE_TYPE_DQ, rank_index, bit_index);
                lpde_set_state_reg_if(&dq_path->tx.rank[rank_index].dq.lpde[bit_index], lpde_state);
            }

            for (bit_index = 0; bit_index < WDDR_PHY_DQS_TXRX_SLICE_NUM; bit_index++)
            {
                tx_lpde_set_msr_reg_if(&dq_path->tx.rank[rank_index].dqs.lpde[bit_index], msr, WDDR_SLICE_TYPE_DQS, rank_index, bit_index);
                lpde_set_state_reg_if(&dq_path->tx.rank[rank_index].dqs.lpde[bit_index], lpde_state);
            }

            // RX
            // Phase Interpolators
            rx_pi_set_msr_reg_if(&dq_path->rx.rank[rank_index].dqs.pi, msr, WDDR_SLICE_TYPE_DQS, rank_index);
            pi_set_state_reg_if(&dq_path->rx.rank[rank_index].dqs.pi.ren, pi_state);
            pi_set_state_reg_if(&dq_path->rx.rank[rank_index].dqs.pi.rcs, pi_state);
            pi_set_state_reg_if(&dq_path->rx.rank[rank_index].dqs.pi.rdqs, pi_state);

            // LPDE
            rx_lpde_set_msr_reg_if(&dq_path->rx.rank[rank_index].dqs.sdr_lpde, msr, WDDR_SLICE_TYPE_DQS, rank_index);
            lpde_set_state_reg_if(&dq_path->rx.rank[rank_index].dqs.sdr_lpde, lpde_state);
        }
    }
}
