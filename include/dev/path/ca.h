/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _CA_PATH_H_
#define _CA_PATH_H_

#include <stdint.h>
#include <wddr/phy_config.h>
#include <pipeline/device.h>
#include <sensamp/device.h>
#include <receiver/device.h>
#include <lpde/device.h>
#include <pi/rx_device.h>
#include <pi/tx_device.h>
#include <gearbox/rx_device.h>
#include <gearbox/tx_device.h>
#include <driver/device.h>

/**
 * @brief   CA RX Path Structure
 *
 * rank.ck.sdr_ldpe     CK SDR LPDE for each rank.
 * gearbox              RX Gearbox  (has CA and CK within structure).
 */
typedef struct ca_rx_path_t
{
    struct {
        struct
        {
            lpde_dev_t      sdr_lpde;
        } ck;
    } rank[WDDR_PHY_RANK];
    rx_gearbox_dev_t        gearbox;
} ca_rx_path_t;

/**
 * @brief   CA TX Path Block Structure
 *
 * @details Structure representing all devices in the CA transmit path.
 *
 * rank.ca.pipeline     CA Pipeline bit.
 * rank.ca.lpde         LPDE for each CA bit.
 * rank.ca.rt           CA Retimer Pipeline (common to all bits).
 * rank.ca.pi           CA Transmit PI device (common to all bits).
 * rank.ck.pipeline     DQS Pipeline bit.
 * rank.ck.lpde         LPDE for each DQS bit.
 * rank.ck.rt           DQS Retimer Pipeline (common to all bits).
 * rank.ck.pi           DQS Transmit PI device (common to all bits).
 * rank.ck.driver       Driver Common device for DQS.
 * ca.driver            Driver device for all CA bits.
 * ck.driver            Driver device for all CK bits.
 * gearbox              TX Gearbox device.
 */
typedef struct ca_tx_path_t
{
    struct {
        struct
        {
            pipeline_bit_dev_t  pipeline[WDDR_PHY_CA_SLICE_NUM];
            lpde_dev_t          lpde[WDDR_PHY_CA_SLICE_NUM];
            rt_pipeline_dev_t   rt;
            tx_pi_dev_t         pi;
        } ca;
        struct
        {
            pipeline_bit_dev_t  pipeline[WDDR_PHY_CK_SLICE_NUM];
            lpde_dev_t          lpde[WDDR_PHY_CK_TXRX_SLICE_NUM];
            rt_pipeline_dev_t   rt;
            tx_pi_dev_t         pi;
            driver_cmn_dev_t    driver;
        } ck;
    } rank[WDDR_PHY_RANK];

    struct
    {
        driver_dev_t            driver;
    } ca;

    struct
    {
        driver_dev_t            driver;
    } ck;
    tx_gearbox_dev_t            gearbox;
} ca_tx_path_t;

/**
 * @brief   CA Path Structure
 *
 * @details Structure reprsenting all devices in the entire CA path.
 *
 * tx       TX path devices.
 * rx       RX path devices.
 */
typedef struct ca_path_t
{
    ca_tx_path_t    tx;
    ca_rx_path_t    rx;
} ca_path_t;

/**
 * @brief   CA Path Intalization
 *
 * @details Initializes all devices in the CA Path.
 *
 * @param[in]   ca_path     pointer to CA Path structure.
 * @param[in]   base        base address of CA Path.
 *
 * @return      void
 */
void ca_path_init(ca_path_t *ca_path, uint32_t base);

/**
 * @brief   CA Path Enable
 *
 * @details Enables all devices in the CA Path. This is for devices that
 *          need to be enabled.
 *
 * @param[in]   ca_path     pointer to CA Path structure.
 *
 * @return      void
 */
void ca_path_enable(ca_path_t *ca_path);


#endif /* _CA_PATH_H_ */
