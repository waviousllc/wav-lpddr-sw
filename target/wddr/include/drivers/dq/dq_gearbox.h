/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DQ_GEARBOX_H_
#define _DQ_GEARBOX_H_

#include <stdbool.h>
#include <dq_reg.h>
#include <wddr/phy_config.h>
#include <gearbox/rx_table.h>
#include <gearbox/tx_table.h>

/*******************************************************************************
**                            DQ
*******************************************************************************/
/**
 * @brief   DQ DQ Receive (RX) Gearbox Set Mode Register Interface
 *
 * @details Configures RX Gearbox for given mode.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   cfg     pointer to RX gearbox configuration.
 *
 * @return      void
 */
void dq_dq_rx_gb_set_mode_reg_if(dq_reg_t *dq_reg,
                                 wddr_msr_t msr,
                                 const rx_gb_cfg_t *cfg);

/*******************************************************************************
**                            DQS
*******************************************************************************/
/**
 * @brief   DQ DQS Receive (RX) Gearbox Set Mode Register Interface
 *
 * @details Configures RX Gearbox for given mode.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   cfg     pointer to RX gearbox configuration.
 *
 * @return      void
 */
void dq_dqs_rx_gb_set_mode_reg_if(dq_reg_t *dq_reg,
                                  wddr_msr_t msr,
                                  const rx_gb_cfg_t *cfg);

/**
 * @brief   DQ DQS Transmit (TX) Gearbox Set Mode Register Interface
 *
 * @details Configures TX Gearbox for given mode.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   cfg     pointer to TX gearbox configuration.
 *
 * @return      void
 */
void dq_dqs_tx_gb_set_mode_reg_if(dq_reg_t *dq_reg,
                                  wddr_msr_t msr,
                                  const tx_gb_cfg_t *cfg);
#endif /* _DQ_GEARBOX_H_ */
