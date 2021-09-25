/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DQ_LPDE_H_
#define _DQ_LPDE_H_

#include <stdbool.h>
#include <dq_reg.h>

/*******************************************************************************
**                            DQ
*******************************************************************************/
/**
 * @brief   DQ DQ LPDE Set Configuration Register Interface
 *
 * @details Configures TX LPDE.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   bit     which bit register set to use.
 * @param[in]   enable  flag to indicate if LPDE should be enabled.
 * @param[in]   cfg     raw LPDE configuration (code and gear).
 */
void dq_dq_lpde_set_cfg_reg_if(dq_reg_t *dq_reg,
                               wddr_msr_t msr,
                               wddr_rank_t rank,
                               uint8_t bit,
                               bool enable,
                               uint32_t cfg);

/*******************************************************************************
**                            DQS
*******************************************************************************/
/**
 * @brief   DQ DQS LPDE Set Configuration Register Interface
 *
 * @details Configures TX LPDE.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   bit     which bit register set to use.
 * @param[in]   enable  flag to indicate if LPDE should be enabled.
 * @param[in]   cfg     raw LPDE configuration (code and gear).
 */
void dq_dqs_lpde_set_cfg_reg_if(dq_reg_t *dq_reg,
                                wddr_msr_t msr,
                                wddr_rank_t rank,
                                uint8_t bit,
                                bool enable,
                                uint32_t cfg);

/**
 * @brief   DQ DQS LPDE RX SDR Set Configuration Register Interface
 *
 * @details Configures RX SDR LPDE.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   enable  flag to indicate if LPDE should be enabled.
 * @param[in]   cfg     raw LPDE configuration (code and gear).
 */
void dq_dqs_lpde_rx_sdr_set_cfg_reg_if(dq_reg_t *dq_reg,
                                       wddr_msr_t msr,
                                       wddr_rank_t rank,
                                       bool enable,
                                       uint32_t cfg);

#endif /* _DQ_LPDE_H_ */
