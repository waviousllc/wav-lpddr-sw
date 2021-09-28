/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _CA_GEARBOX_H_
#define _CA_GEARBOX_H_

#include <stdbool.h>
#include "ca_reg.h"
#include <wddr/phy_config.h>
#include <gearbox/rx_table.h>
#include <gearbox/tx_table.h>

/*******************************************************************************
**                            CA
*******************************************************************************/
/**
 * @brief   CA DQ Receive (RX) Gearbox Set Mode Register Interface
 *
 * @details Configures RX Gearbox for given mode.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   cfg     pointer to RX gearbox configuration.
 *
 * @return      void
 */
void ca_dq_rx_gb_set_mode_reg_if(ca_reg_t *ca_reg,
                                 wddr_msr_t msr,
                                 const rx_gb_cfg_t *cfg);

/*******************************************************************************
**                            CK
*******************************************************************************/
/**
 * @brief   CA DQ Receive (RX) Gearbox Set Mode Register Interface
 *
 * @details Configures RX Gearbox for given mode.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   cfg     pointer to RX gearbox configuration.
 *
 * @return      void
 */
void ca_dqs_rx_gb_set_mode_reg_if(ca_reg_t *ca_reg,
                                 wddr_msr_t msr,
                                 const rx_gb_cfg_t *cfg);

/**
 * @brief   CA DQ Transmit (TX) Gearbox Set Mode Register Interface
 *
 * @details Configures TX Gearbox for given mode.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   cfg     pointer to TX gearbox configuration.
 *
 * @return      void
 */
void ca_dqs_tx_gb_set_mode_reg_if(ca_reg_t *ca_reg,
                                 wddr_msr_t msr,
                                 const tx_gb_cfg_t *cfg);

#endif /* _CA_GEARBOX_H_ */
