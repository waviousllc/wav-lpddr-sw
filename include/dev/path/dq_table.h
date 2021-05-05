/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _PATH_DQ_TABLE_H_
#define _PATH_DQ_TABLE_H_
#include <wddr/phy_defs.h>
#include <driver/table.h>
#include <lpde/table.h>
#include <pi/rx_table.h>
#include <pi/tx_table.h>
#include <pipeline/table.h>
#include <gearbox/rx_table.h>
#include <gearbox/tx_table.h>
#include <sensamp/table.h>
#include <receiver/table.h>

/*************************************************************************
**                          Receive Path
*************************************************************************/
/**
 * @brief   DQ RX Path Frequency Calibration Structure
 *
 * @details Per-Frequency calibration data for all devices in the Receive DQ
 *          path.
 *
 * rank.dqs.receiver    DQS Receiver data for each rank.
 * rank.dqs.sdr_lpde    DQS SDR LPDE data for each rank.
 * rank.dqs.pi          DQS RX PI data for each rank.
 */
typedef struct dq_rx_path_freq_cal_t
{
    struct
    {
        struct
        {
            receiver_freq_cal_t receiver;
            lpde_cal_t          sdr_lpde;
            rx_pi_cal_t         pi;
        } dqs;
    } rank[WDDR_PHY_RANK];
} dq_rx_path_freq_cal_t;

/**
 * @brief   DQ RX Path Frequency Configuration Structure
 *
 * @details Per-Frequency configuration data for all devices in Receive DQ
 *          path.
 *
 * rank.dqs.receiver        DQS Receiver data for each rank.
 * rank.dqs.sdr_lpde        DQS SDR LPDE data for each rank.
 * rank.dqs.pi              DQS RX PI data for each rank.
 * rank_cmn.cmn.gearbox     DQ/DQS Gearbox data.
 */
typedef struct dq_rx_path_freq_cfg_t
{
    struct
    {
        struct
        {
            receiver_freq_cfg_t receiver;
            lpde_cfg_t          sdr_lpde;
            pi_cfg_t            pi; // Commmon for all PIs
        } dqs;
    } rank[WDDR_PHY_RANK];
    struct
    {
        struct
        {
            rx_gb_cfg_t gearbox;
        } cmn;
    } rank_cmn;
} dq_rx_path_freq_cfg_t;

/**
 * @brief   DQ RX Path Common Calibration Structure
 *
 * @details Frequency independent calibration data for all devices in Receive DQ
 *          path.
 *
 * sa                   Sensamp calibration data.
 * rank.dqs.receiver    DQS Receiver calibration data.
 */
typedef struct dq_rx_path_common_cal_t
{
    // TODO: move SA rank into here
    sensamp_dqbyte_common_cal_t sa;
    struct
    {
        struct
        {
            receiver_common_cal_t receiver;
        } dqs;
    } rank[WDDR_PHY_RANK];
} dq_rx_path_common_cal_t;

/*************************************************************************
**                          Transmit Path
*************************************************************************/
/**
 * @brief   DQ TX Path Frequency Calibration Structure
 *
 * @brief   Per-Frequency calibration structure for DQ TX Path.
 *
 * rank.dq.pipeline     DQ Pipeline bit calibration data.
 * rank.dq.lpde         LPDE DQ bit calibration data.
 * rank.dq.rt           DQ Retimer Pipeline calibration data.
 * rank.dq.pi           DQ Transmit PI calibration data.
 * rank.dqs.pipeline    DQS Pipeline bit calibration data.
 * rank.dqs.lpde        LPDE DQS bit calibration data.
 * rank.dqs.rt          DQS Retimer Pipeline calibration data.
 * rank.dqs.pi          DQS Transmit PI calibration data.
 * rank.dqs.driver      DQS Driver Common calibration data.
 */
typedef struct dq_tx_path_freq_cal_t
{
    struct
    {
        struct {
            pipeline_bit_cal_t  pipeline;
            lpde_cal_t          lpde[WDDR_PHY_DQ_SLICE_NUM];
            rt_pipeline_cal_t   rt;
            tx_pi_cal_t         pi;
        } dq;

        struct {
            pipeline_bit_cal_t  pipeline[WDDR_PHY_DQS_SLICE_NUM];
            lpde_cal_t          lpde[WDDR_PHY_DQS_TXRX_SLICE_NUM];
            rt_pipeline_cal_t   rt;
            tx_pi_cal_t         pi;
            driver_cmn_cal_t    driver;
        } dqs;
    } rank[WDDR_PHY_RANK];
} dq_tx_path_freq_cal_t;

/**
 * @brief   DQ TX Path Frequency Configuration Structure
 *
 * @details Per-Frequency configuration structure for DQ TX Path.
 *
 * rank.dq.pi               DQ PI configuration data.
 * rank.dqs.pi              DQS PI configuration data.
 * rank.dqs.driver          DQS Driver common configuration data.
 * rank.cmn.lpde            LPDE configuration data common to DQ and DQS.
 * rank_cmn.dq.driver       DQ Driver configuration data.
 * rank_cmn.dqs.driver      DQS Driver configuration data.
 * rank_cmn.dqs.gearbox     DQS Gearbox configuration data.
 * rank_cmn.cmn.pipeline    Pipeline Bit configuration data common to all
 *                          DQ / DQS bits.
 */
typedef struct dq_tx_path_freq_cfg_t
{
    struct
    {
        struct {
            tx_pi_cfg_t         pi;
        } dq;

        struct {
            tx_pi_cfg_t         pi;
            driver_cmn_cfg_t    driver;
        } dqs;
        struct {
            lpde_cfg_t          lpde;
        } cmn;
    } rank[WDDR_PHY_RANK];
    struct
    {
        struct {
            driver_cfg_t        driver;
        } dq;
        struct {
            driver_cfg_t        driver;
            tx_gb_cfg_t         gearbox;
        } dqs;
        struct {
            pipeline_bit_cfg_t  pipeline;
        } cmn;
   } rank_cmn;
} dq_tx_path_freq_cfg_t;

/*************************************************************************
**                          Aggregate Path
*************************************************************************/
/**
 * @brief   DQ Path Frequency Calibration Structure
 *
 * @details Per-Frequency Calibration data for entire DQ Path.
 *
 * tx       TX path.
 * rx       RX path.
 */
typedef struct dq_path_freq_cal_t
{
    dq_tx_path_freq_cal_t tx;
    dq_rx_path_freq_cal_t rx;
} dq_path_freq_cal_t;

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
typedef struct dq_path_common_cal_t
{
    dq_rx_path_common_cal_t rx;
} dq_path_common_cal_t;

#endif /* _PATH_DQ_TABLE_H_ */
