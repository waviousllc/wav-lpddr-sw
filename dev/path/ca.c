/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <path/ca.h>
#include <gearbox/tx_driver.h>
#include <gearbox/rx_driver.h>
#include <lpde/driver.h>
#include <lpde/rx_driver.h>
#include <lpde/tx_driver.h>
#include <pipeline/driver.h>
#include <pi/driver.h>
#include <pi/tx_driver.h>

/** @brief  Initializes CA TX Path for the given rank */
static void ca_tx_path_init_rank(ca_tx_path_t *ca_path, uint32_t base, wddr_rank_t rank);

/** @brief  Initializes CA TX Path for rank independent devices */
static void ca_tx_path_init(ca_tx_path_t *ca_path, uint32_t base);

/** @brief  Initializes CA RX Path for the given rank */
static void ca_rx_path_init_rank(ca_rx_path_t *ca_path, uint32_t base, wddr_rank_t rank);

/** @brief  Initializes CA RX Path for rank independent devices */
static void ca_rx_path_init(ca_rx_path_t *ca_path, uint32_t base);

/** @brief  Sets state of all devices in the CA Path */
static void ca_path_set_state(ca_path_t *ca_path, bool enable);

void ca_path_init(ca_path_t *ca_path, uint32_t base)
{
    // Rank dependent
    for (uint8_t rank = 0; rank < WDDR_PHY_RANK; rank++)
    {
        ca_tx_path_init_rank(&ca_path->tx, base, rank);
        ca_rx_path_init_rank(&ca_path->rx, base, rank);
    }

    // Rank independent
    ca_tx_path_init(&ca_path->tx, base);
    ca_rx_path_init(&ca_path->rx, base);
}

void ca_path_enable(ca_path_t *ca_path)
{
    ca_path_set_state(ca_path, true);
}

static void ca_tx_path_init_rank(ca_tx_path_t *ca_path, uint32_t base, wddr_rank_t rank)
{
    uint8_t bit_index;

    // CA
    for (bit_index = 0; bit_index < WDDR_PHY_CA_SLICE_NUM; bit_index++)
    {
        pipeline_bit_init(&ca_path->rank[rank].ca.pipeline[bit_index], base, WDDR_SLICE_TYPE_CA, rank, bit_index);
        tx_lpde_init_reg_if(&ca_path->rank[rank].ca.lpde[bit_index], base, WDDR_SLICE_TYPE_CA, rank, bit_index);
    }
    rt_pipeline_init_reg_if(&ca_path->rank[rank].ca.rt, base, WDDR_SLICE_TYPE_CA, rank);
    tx_pi_init_reg_if(&ca_path->rank[rank].ca.pi, base, WDDR_SLICE_TYPE_CA, rank);

    // CK
    for (bit_index = 0; bit_index < WDDR_PHY_CK_SLICE_NUM; bit_index++)
    {
        pipeline_bit_init(&ca_path->rank[rank].ck.pipeline[bit_index], base, WDDR_SLICE_TYPE_CK, rank, bit_index);
        tx_lpde_init_reg_if(&ca_path->rank[rank].ck.lpde[bit_index], base, WDDR_SLICE_TYPE_CK, rank, bit_index);
    }
    rt_pipeline_init_reg_if(&ca_path->rank[rank].ck.rt, base, WDDR_SLICE_TYPE_CK, rank);
    tx_pi_init_reg_if(&ca_path->rank[rank].ck.pi, base, WDDR_SLICE_TYPE_CK, rank);

    // CMN
    driver_cmn_init(&ca_path->rank[rank].ck.driver, base, WDDR_SLICE_TYPE_CK, rank);
}

static void ca_tx_path_init(ca_tx_path_t *ca_path, uint32_t base)
{
    // CA
    for (uint8_t bit_index = 0; bit_index < WDDR_PHY_CA_SLICE_NUM; bit_index++)
    {
        egress_dig_init_reg_if(&ca_path->ca.pipeline[bit_index].egress_dig,
                              base,
                              WDDR_SLICE_TYPE_CA,
                              bit_index);
        egress_ana_init_reg_if(&ca_path->ca.pipeline[bit_index].egress_ana,
                              base,
                              WDDR_SLICE_TYPE_CA,
                              bit_index);
    }

    // Bit TX Driver
    driver_init(&ca_path->ca.driver, base, WDDR_SLICE_TYPE_CA);

    // CK
    for (uint8_t bit_index = 0; bit_index < WDDR_PHY_CK_SLICE_NUM; bit_index++)
    {
        egress_dig_init_reg_if(&ca_path->ck.pipeline[bit_index].egress_dig,
                              base,
                              WDDR_SLICE_TYPE_CK,
                              bit_index);
        egress_ana_init_reg_if(&ca_path->ck.pipeline[bit_index].egress_ana,
                               base,
                               WDDR_SLICE_TYPE_CK,
                               bit_index);
    }

    driver_init(&ca_path->ck.driver, base, WDDR_SLICE_TYPE_CK);
    tx_gb_init_reg_if(&ca_path->gearbox, base, WDDR_SLICE_TYPE_CK);
}

static void ca_rx_path_init_rank(ca_rx_path_t *ca_path, uint32_t base, wddr_rank_t rank)
{
    rx_lpde_init_reg_if(&ca_path->rank[rank].ck.sdr_lpde, base, WDDR_SLICE_TYPE_CK, rank);
}

static void ca_rx_path_init(ca_rx_path_t *ca_path, uint32_t base)
{
    rx_gb_init_reg_if(&ca_path->gearbox, base, WDDR_SLICE_TYPE_CA);
}

static void ca_path_set_state(ca_path_t *ca_path, bool enable)
{
    uint8_t rank_index, bit_index;
    pi_state_t pi_state = enable ? PI_STATE_ENABLED : PI_STATE_DISABLED;
    lpde_state_t lpde_state = enable ? LPDE_STATE_ENABLED : LPDE_STATE_ENABLED;

    for (rank_index = 0; rank_index < WDDR_PHY_RANK; rank_index++)
    {
        for (int8_t msr = WDDR_MSR_1; msr >= WDDR_MSR_0; msr--)
        {
            // Phase Interpolators
            // CA
            tx_pi_set_msr_reg_if(&ca_path->tx.rank[rank_index].ca.pi, msr, WDDR_SLICE_TYPE_CA, rank_index);
            pi_set_state_reg_if(&ca_path->tx.rank[rank_index].ca.pi.odr, pi_state);
            pi_set_state_reg_if(&ca_path->tx.rank[rank_index].ca.pi.qdr, pi_state);
            pi_set_state_reg_if(&ca_path->tx.rank[rank_index].ca.pi.ddr, pi_state);
            pi_set_state_reg_if(&ca_path->tx.rank[rank_index].ca.pi.rt, pi_state);

            // CK
            tx_pi_set_msr_reg_if(&ca_path->tx.rank[rank_index].ck.pi, msr, WDDR_SLICE_TYPE_CK, rank_index);
            pi_set_state_reg_if(&ca_path->tx.rank[rank_index].ck.pi.odr, pi_state);
            pi_set_state_reg_if(&ca_path->tx.rank[rank_index].ck.pi.qdr, pi_state);
            pi_set_state_reg_if(&ca_path->tx.rank[rank_index].ck.pi.ddr, pi_state);
            pi_set_state_reg_if(&ca_path->tx.rank[rank_index].ck.pi.rt, pi_state);
            pi_set_state_reg_if(&ca_path->tx.rank[rank_index].ck.pi.sdr, pi_state);
            pi_set_state_reg_if(&ca_path->tx.rank[rank_index].ck.pi.dfi, pi_state);

            // LPDE
            // CA
            for (bit_index = 0; bit_index < WDDR_PHY_CA_SLICE_NUM; bit_index++)
            {
                tx_lpde_set_msr_reg_if(&ca_path->tx.rank[rank_index].ca.lpde[bit_index], msr, WDDR_SLICE_TYPE_CA, rank_index, bit_index);
                lpde_set_state_reg_if(&ca_path->tx.rank[rank_index].ca.lpde[bit_index], lpde_state);
            }

            for (bit_index = 0; bit_index < WDDR_PHY_CK_TXRX_SLICE_NUM; bit_index++)
            {
                tx_lpde_set_msr_reg_if(&ca_path->tx.rank[rank_index].ck.lpde[bit_index], msr, WDDR_SLICE_TYPE_CK, rank_index, bit_index);
                lpde_set_state_reg_if(&ca_path->tx.rank[rank_index].ck.lpde[bit_index], lpde_state);
            }

            rx_lpde_set_msr_reg_if(&ca_path->rx.rank[rank_index].ck.sdr_lpde, msr, WDDR_SLICE_TYPE_CK, rank_index);
            lpde_set_state_reg_if(&ca_path->rx.rank[rank_index].ck.sdr_lpde, lpde_state);
        }
    }
}
