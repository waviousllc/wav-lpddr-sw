/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#ifndef _PATH_CA_TABLE_H_
#define _PATH_CA_TABLE_H_

#include <wddr/phy_defs.h>
#include <lpde/table.h>
#include <gearbox/rx_table.h>
#include <gearbox/tx_table.h>

/*************************************************************************
**                          Receive Path
*************************************************************************/
/**
 * @brief   CA RX Path Frequency Calibration Structure
 *
 * @details Per-Frequency calibration data for all devices in the Receive CA
 *          path.
 *
 * rank.ck.sdr_lpde    CK SDR LPDE data for each rank.
 */
typedef struct ca_rx_path_freq_cal_t
{
    struct
    {
        struct
        {
            lpde_cal_t          sdr_lpde;
        } ck;
    } rank[WDDR_PHY_RANK];
} ca_rx_path_freq_cal_t;

/**
 * @brief   CA RX Path Frequency Configuration Structure
 *
 * @details Per-Frequency configuration data for all devices in Receive CA
 *          path.
 *
 * rank.ck.sdr_lpde         CK SDR LPDE data for each rank.
 * rank_cmn.cmn.gearbox     CA/CK Gearbox data.
 */
typedef struct ca_rx_path_freq_cfg_t
{
    struct
    {
        struct
        {
            lpde_cfg_t          sdr_lpde;
        } ck;
    } rank[WDDR_PHY_RANK];
    struct
    {
        struct
        {
            rx_gb_cfg_t gearbox;
        } cmn;
    } rank_cmn;
} ca_rx_path_freq_cfg_t;

/*************************************************************************
**                          Transmit Path
*************************************************************************/
/**
 * @brief   CA TX Path Frequency Calibration Structure
 *
 * @brief   Per-Frequency calibration structure for CA TX Path.
 *
 * rank.ca.pipeline     CA Pipeline bit calibration data.
 * rank.ca.lpde         LPDE CA bit calibration data.
 * rank.ca.rt           CA Retimer Pipeline calibration data.
 * rank.ca.pi           CA Transmit PI calibration data.
 * rank.ck.pipeline     CK Pipeline bit calibration data.
 * rank.ck.lpde         LPDE CK bit calibration data.
 * rank.ck.rt           CK Retimer Pipeline calibration data.
 * rank.ck.pi           CK Transmit PI calibration data.
 * rank.ck.driver       CK Driver Common calibration data.
 */
typedef struct ca_tx_path_freq_cal_t
{
    struct
    {
        struct {
            pipeline_bit_cal_t  pipeline;
            lpde_cal_t          lpde[WDDR_PHY_CA_SLICE_NUM];
            rt_pipeline_cal_t   rt;
            tx_pi_cal_t         pi;
        } ca;

        struct {
            pipeline_bit_cal_t  pipeline;
            lpde_cal_t          lpde[WDDR_PHY_CK_TXRX_SLICE_NUM];
            rt_pipeline_cal_t   rt;
            tx_pi_cal_t         pi;
            driver_cmn_cal_t    driver;
        } ck;
    } rank[WDDR_PHY_RANK];
} ca_tx_path_freq_cal_t;

/**
 * @brief   CA TX Path Frequency Configuration Structure
 *
 * @details Per-Frequency configuration structure for CA TX Path.
 *
 * rank.ca.pi               CA PI configuration data.
 * rank.ck.pi               CK PI configuration data.
 * rank.ck.driver           CK Driver common configuration data.
 * rank.cmn.lpde            LPDE configuration data common to CA and CK.
 * rank_cmn.ca.driver       CA Driver configuration data.
 * rank_cmn.ck.driver       CK Driver configuration data.
 * rank_cmn.ck.gearbox      CK Gearbox configuration data.
 * rank_cmn.cmn.pipeline    Pipeline Bit configuration data common to all
 *                          CA / CK bits.
 */
typedef struct ca_tx_path_freq_cfg_t
{
    struct
    {
        struct {
            tx_pi_cfg_t         pi;
        } ca;

        struct {
            tx_pi_cfg_t         pi;
            driver_cmn_cfg_t    driver;
        } ck;
        struct {
            lpde_cfg_t          lpde;
        } cmn;
    } rank[WDDR_PHY_RANK];
    struct
    {
        struct {
            driver_cfg_t        driver;
        } ca;
        struct {
            driver_cfg_t        driver;
            tx_gb_cfg_t         gearbox;
        } ck;
        struct {
            pipeline_bit_cfg_t  pipeline;
        } cmn;
   } rank_cmn;
} ca_tx_path_freq_cfg_t;

/*************************************************************************
**                          Aggregate Path
*************************************************************************/
/**
 * @brief   CA Path Frequency Calibration Structure
 *
 * @details Per-Frequency Calibration data for entire CA Path.
 *
 * tx       TX path.
 * rx       RX path.
 */
typedef struct ca_path_freq_cal_t
{
    ca_tx_path_freq_cal_t tx;
    ca_rx_path_freq_cal_t rx;
} ca_path_freq_cal_t;

/**
 * @brief   CA Path Frequency Configuration Structure
 *
 * @details Per-Frequency Configuration data for entire CA Path.
 *
 * tx       TX path.
 * rx       RX path.
 */
typedef struct ca_path_freq_cfg_t
{
    ca_tx_path_freq_cfg_t tx;
    ca_rx_path_freq_cfg_t rx;
} ca_path_freq_cfg_t;

#endif /* _PATH_CA_TABLE_H_ */
