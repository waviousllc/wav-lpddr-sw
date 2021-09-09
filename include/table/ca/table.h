/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _CA_TABLE_H_
#define _CA_TABLE_H_

#include <wddr/phy_defs.h>
#include <driver/table.h>
#include <dp/table.h>
#include <lpde/table.h>
#include <pi/rx_table.h>
#include <pi/tx_table.h>
#include <gearbox/rx_table.h>
#include <gearbox/tx_table.h>

/*************************************************************************
**                          Receive Path
*************************************************************************/
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
 * @brief   CA TX Path Frequency Configuration Structure
 *
 * @details Per-Frequency configuration structure for CA TX Path.
 *
 * rank.ca.lpde             LPDE CA bit configuration data.
 * rank.ca.pipeline         CA Pipeline bit configuration data.
 * rank.ca.pi               CA PI configuration data.
 * rank.ca.rt               CA Retimer Pipeline configuration data.
 * rank.ck.pipeline         CK Pipeline bit configuration data.
 * rank.ck.pi               CK PI configuration data.
 * rank.ck.driver           CK Driver common configuration data.
 * rank.ck.lpde             LPDE CK bit configuration data.
 * rank.ck.rt               CK Retimer Pipeline configuration data.
 * rank_cmn.ca.driver       CA Driver configuration data.
 * rank_cmn.ck.driver       CK Driver configuration data.
 * rank_cmn.ck.gearbox      CK Gearbox configuration data.
 * rank_cmn.cmn.egress      Common digital and analog egress configuration data.
 */
typedef struct ca_tx_path_freq_cfg_t
{
    struct
    {
        struct {
            lpde_cfg_t          lpde[WDDR_PHY_CA_SLICE_NUM];
            pipeline_bit_cfg_t  pipeline;
            tx_pi_cfg_t         pi;
            rt_pipeline_cfg_t   rt;
        } ca;

        struct {
            pipeline_bit_cfg_t  pipeline;
            tx_pi_cfg_t         pi;
            driver_cmn_cfg_t    driver;
            lpde_cfg_t          lpde[WDDR_PHY_CK_TXRX_SLICE_NUM];
            rt_pipeline_cfg_t   rt;
        } ck;
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
            egress_bit_cfg_t    egress;
        } cmn;
   } rank_cmn;
} ca_tx_path_freq_cfg_t;

/*************************************************************************
**                          Aggregate Path
*************************************************************************/
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

#endif /* _CA_TABLE_H_ */
