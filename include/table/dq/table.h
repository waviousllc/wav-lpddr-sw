/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _PATH_DQ_TABLE_H_
#define _PATH_DQ_TABLE_H_

#include <wddr/phy_defs.h>
#include <driver/table.h>
#include <dp/table.h>
#include <lpde/table.h>
#include <pi/rx_table.h>
#include <pi/tx_table.h>
#include <gearbox/rx_table.h>
#include <gearbox/tx_table.h>
#include <sensamp/table.h>
#include <receiver/table.h>

/*************************************************************************
**                          Receive Path
*************************************************************************/
/**
 * @brief   DQ RX Path Frequency Configuration Structure
 *
 * @details Per-Frequency configuration data for all devices in Receive DQ
 *          path.
 *
 * rank.dqs.receiver        DQS Receiver data for each rank.
 * rank.dqs.pi              DQS RX PI data for each rank.
 * rank.dqs.sdr_lpde        DQS SDR LPDE data for each rank.
 * rank_cmn.cmn.gearbox     DQ/DQS Gearbox data.
 */
typedef struct dq_rx_path_freq_cfg_t
{
    struct
    {
        struct
        {
            receiver_freq_cfg_t receiver;
            rx_pi_cfg_t         pi; // Commmon for all PIs
            lpde_cfg_t          sdr_lpde;
        } dqs;
    } rank[WDDR_PHY_RANK];
    struct
    {
        struct
        {
            const rx_gb_cfg_t gearbox;
        } cmn;
    } rank_cmn;
} dq_rx_path_freq_cfg_t;

/**
 * @brief   DQ RX Path Common Configuration Structure
 *
 * @details Frequency independent configuration data for all devices in Receive DQ
 *          path.
 *
 * sa                   Sensamp configuration data.
 * rank.dqs.receiver    DQS Receiver configuration data.
 */
typedef struct dq_rx_path_common_cfg_t
{
    // TODO: move SA rank into here
    sensamp_dqbyte_common_cfg_t sa;
    struct
    {
        struct
        {
            receiver_common_cfg_t receiver;
        } dqs;
    } rank[WDDR_PHY_RANK];
} dq_rx_path_common_cfg_t;

/*************************************************************************
**                          Transmit Path
*************************************************************************/
/**
 * @brief   DQ TX Path Frequency Configuration Structure
 *
 * @details Per-Frequency configuration structure for DQ TX Path.
 *
 * rank.dqs.pipeline        DQS Pipeline bit configuration data.
 * rank.dqs.pi              DQS PI configuration data.
 * rank.dqs.driver          DQS Driver common configuration data.
 * rank.dqs.lpde            LPDE DQS bit configuration data.
 * rank.dqs.rt              DQS Retimer Pipeline configuration data.
 * rank.dq.lpde             LPDE DQ bit configuration data.
 * rank.dq.pi               DQ PI configuration data.
 * rank.dq.pipeline         DQ Pipeline bit configuration data.
 * rank.dq.rt               DQ Retimer Pipeline configuration data.
 * rank_cmn.dq.driver       DQ Driver configuration data.
 * rank_cmn.dqs.driver      DQS Driver configuration data.
 * rank_cmn.dqs.gearbox     DQS Gearbox configuration data.
 * rank_cmn.cmn.egress      Common digital and analog egress configuration data.
 */
typedef struct dq_tx_path_freq_cfg_t
{
    struct
    {
        struct {
            pipeline_bit_cfg_t      pipeline[WDDR_PHY_DQS_SLICE_NUM];
            tx_pi_cfg_t             pi;
            driver_cmn_cfg_t        driver;
            lpde_cfg_t              lpde[WDDR_PHY_DQS_TXRX_SLICE_NUM];
            rt_pipeline_cfg_t       rt;
        } dqs;

        struct {
            lpde_cfg_t              lpde[WDDR_PHY_DQ_SLICE_NUM];
            tx_pi_cfg_t             pi;
            pipeline_bit_cfg_t      pipeline;
            rt_pipeline_cfg_t       rt;
        } dq;
    } rank[WDDR_PHY_RANK];
    struct
    {
        struct {
            const driver_cfg_t      driver;
        } dq;
        struct {
            const driver_cfg_t      driver;
            const tx_gb_cfg_t       gearbox;
        } dqs;
        struct {
            const egress_bit_cfg_t  egress;
        } cmn;
   } rank_cmn;
} dq_tx_path_freq_cfg_t;

/*************************************************************************
**                          Aggregate Path
*************************************************************************/
/**
 * @brief   DQ Path Frequency Configuration Structure
 *
 * @details Per-Frequency Configuration data for entire DQ Path.
 *
 * tx       TX path.
 * rx       RX path.
 */
typedef struct dq_path_freq_cfg_t
{
    dq_tx_path_freq_cfg_t tx;
    dq_rx_path_freq_cfg_t rx;
} dq_path_freq_cfg_t;

/**
 * @brief   DQ Path Frequency Common Calibration Structure
 *
 * @details Frequency Independent Calibration data for entire DQ Path.
 *
 * rx       RX path.
 */
typedef struct dq_path_common_cfg_t
{
    dq_rx_path_common_cfg_t rx;
} dq_path_common_cfg_t;

#endif /* _PATH_DQ_TABLE_H_ */
