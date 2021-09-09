/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _CA_LPDE_H_
#define _CA_LPDE_H_

#include <stdbool.h>
#include "ca_reg.h"

/*******************************************************************************
**                            CA
*******************************************************************************/
/**
 * @brief   CA DQ LPDE Set Configuration Register Interface
 *
 * @details Configures TX LPDE.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   bit     which bit register set to use.
 * @param[in]   enable  flag to indicate if LPDE should be enabled.
 * @param[in]   cfg     raw LPDE configuration (code and gear).
 */
void ca_dq_lpde_set_cfg_reg_if(ca_reg_t *ca_reg,
                               wddr_msr_t msr,
                               wddr_rank_t rank,
                               uint8_t bit,
                               bool enable,
                               uint32_t cfg);

/*******************************************************************************
**                            CK
*******************************************************************************/
/**
 * @brief   CA DQS LPDE Set Configuration Register Interface
 *
 * @details Configures TX LPDE.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   bit     which bit register set to use.
 * @param[in]   enable  flag to indicate if LPDE should be enabled.
 * @param[in]   cfg     raw LPDE configuration (code and gear).
 */
void ca_dqs_lpde_set_cfg_reg_if(ca_reg_t *ca_reg,
                                wddr_msr_t msr,
                                wddr_rank_t rank,
                                uint8_t bit,
                                bool enable,
                                uint32_t cfg);

/**
 * @brief   CA DQS LPDE RX SDR Set Configuration Register Interface
 *
 * @details Configures RX SDR LPDE.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   bit     which bit register set to use.
 * @param[in]   enable  flag to indicate if LPDE should be enabled.
 * @param[in]   cfg     raw LPDE configuration (code and gear).
 */
void ca_dqs_lpde_rx_sdr_set_cfg_reg_if(ca_reg_t *ca_reg,
                                      wddr_msr_t msr,
                                      wddr_rank_t rank,
                                      bool enable,
                                      uint32_t cfg);

#endif /* _CA_LPDE_H_ */
