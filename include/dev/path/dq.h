/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _PATH_DQ_H_
#define _PATH_DQ_H_

#include <stdint.h>
#include <wddr/phy_config.h>
#include <pipeline/device.h>
#include <driver/device.h>
#include <sensamp/device.h>
#include <receiver/device.h>
#include <lpde/device.h>
#include <pi/rx_device.h>
#include <pi/tx_device.h>
#include <pipeline/device.h>
#include <gearbox/rx_device.h>
#include <gearbox/tx_device.h>

/**
 * @brief   DQ RX Path Structure
 *
 * @details Structure representing all devices in the DQ receive path.
 *
 * rank.dqs.receiver    DQS Receiver for each rank.
 * rank.dqs.sdr_lpde    DQS SDR LPDE for each rank.
 * rank.dqs.pi          DQS RX PI for each rank.
 * sa                   DQ Sensamp receiver.
 * gearbox              RX Gearbox (has DQ and DQS within structure).
 */
typedef struct dq_rx_path_t
{
    struct {
        struct
        {
            receiver_dev_t  receiver;
            lpde_dev_t      sdr_lpde;
            rx_pi_dev_t     pi;
        } dqs;
    } rank[WDDR_PHY_RANK];
    sensamp_dqbyte_dev_t    sa;
    rx_gearbox_dev_t        gearbox;
} dq_rx_path_t;

/**
 * @brief   DQ TX Path Block Structure
 *
 * @details Structure representing all devices in the DQ transmit path.
 *
 * rank.dq.pipeline         DQ Pipeline bit.
 * rank.dq.lpde             LPDE for each DQ bit.
 * rank.dq.rt               DQ Retimer Pipeline (common to all bits).
 * rank.dq.pi               DQ Transmit PI device (common to all bits).
 * rank.dqs.pipeline        DQS Pipeline bit.
 * rank.dqs.lpde            LPDE for each DQS bit.
 * rank.dqs.rt              DQS Retimer Pipeline (common to all bits).
 * rank.dqs.pi              DQS Transmit PI device (common to all bits).
 * rank.dqs.driver          Driver Common device for DQS.
 * dq.driver                Driver device for all DQ bits.
 * dq.pipeline.egress_dig   Ditigal Egress device for each bit in DQ pipeline.
 * dq.pipeline.egress_ana   Analog Egress device for each bit in DQ pipeline.
 * dqs.driver               Driver device for all DQS bits.
 * dqs.pipeline.egress_dig  Ditigal Egress device for each bit in DQS pipeline.
 * dqs.pipeline.egress_ana  Analog Egress device for each bit in DQS pipeline.
 * gearbox                  TX Gearbox device.
 */
typedef struct dq_tx_path_t
{
    struct {
        struct
        {
            pipeline_bit_dev_t  pipeline[WDDR_PHY_DQ_SLICE_NUM];
            lpde_dev_t          lpde[WDDR_PHY_DQ_SLICE_NUM];
            rt_pipeline_dev_t   rt;
            tx_pi_dev_t         pi;
        } dq;
        struct
        {
            pipeline_bit_dev_t  pipeline[WDDR_PHY_DQS_SLICE_NUM];
            lpde_dev_t          lpde[WDDR_PHY_DQS_TXRX_SLICE_NUM];
            rt_pipeline_dev_t   rt;
            tx_pi_dev_t         pi;
            driver_cmn_dev_t    driver;
        } dqs;
    } rank[WDDR_PHY_RANK];

    struct
    {
        driver_dev_t            driver;
        struct
        {
            bit_egress_dev_t    egress_dig;
            bit_egress_dev_t    egress_ana;
        } pipeline[WDDR_PHY_DQ_SLICE_NUM];
    } dq;

    struct
    {
        driver_dev_t            driver;
        struct
        {
            bit_egress_dev_t    egress_dig;
            bit_egress_dev_t    egress_ana;
        } pipeline[WDDR_PHY_DQS_SLICE_NUM];
    } dqs;
    tx_gearbox_dev_t            gearbox;
} dq_tx_path_t;

/**
 * @brief   DQ Path Structure
 *
 * @details Structure reprsenting all devices in the entire DQ path.
 *
 * tx       TX path devices.
 * rx       RX path devices.
 */
typedef struct dq_path_t
{
    dq_tx_path_t    tx;
    dq_rx_path_t    rx;
} dq_path_t;

/**
 * @brief   DQ Path Initalization
 *
 * @details Initializes all devices in the DQ Path.
 *
 * @param[in]   dq_path     pointer to DQ Path structure.
 * @param[in]   base        base address of DQ Path.
 *
 * @return      void
 */
void dq_path_init(dq_path_t *dq_path, uint32_t base);

/**
 * @brief   DQ Path Enable
 *
 * @details Enables all devices in the DQ Path. This is for devices that
 *          need to be enabled.
 *
 * @param[in]   dq_path     pointer to DQ Path structure.
 *
 * @return      void
 */
void dq_path_enable(dq_path_t *dq_path);

#endif /* _PATH_DQ_H_ */
