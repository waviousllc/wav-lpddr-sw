/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DQ_EGRESS_H_
#define _DQ_EGRESS_H_

#include <stdbool.h>
#include <dq_reg.h>
#include <dp/table.h>

/*******************************************************************************
**                            DQ
*******************************************************************************/
/**
 * @brief   DQ DQ Egress Analog Set Mode Register Interface
 *
 * @details Sets Egress analog mode.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   bit     which bit register set to use.
 * @param[in]   mode    egress mode to set.
 *
 * @return      void
 */
void dq_dq_egress_ana_set_mode_reg_if(dq_reg_t *dq_reg,
                                      wddr_msr_t msr,
                                      uint8_t bit,
                                      egress_ana_mode_t mode);

/**
 * @brief   DQ DQ Egress Digital Set Mode Register Interface
 *
 * @details Sets Egress digital mode.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   bit     which bit register set to use.
 * @param[in]   mode    egress mode to set.
 *
 * @return      void
 */
void dq_dq_egress_dig_set_mode_reg_if(dq_reg_t *dq_reg,
                                      wddr_msr_t msr,
                                      uint8_t bit,
                                      egress_dig_mode_t mode);

/*******************************************************************************
**                            DQS
*******************************************************************************/
/**
 * @brief   DQ DQS Egress Analog Set Mode Register Interface
 *
 * @details Sets Egress analog mode.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   bit     which bit register set to use.
 * @param[in]   mode    egress mode to set.
 *
 * @return      void
 */
void dq_dqs_egress_ana_set_mode_reg_if(dq_reg_t *dq_reg,
                                       wddr_msr_t msr,
                                       uint8_t bit,
                                       egress_ana_mode_t mode);

/**
 * @brief   DQ DQS Egress Digital Set Mode Register Interface
 *
 * @details Sets Egress digital mode.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   bit     which bit register set to use.
 * @param[in]   mode    egress mode to set.
 *
 * @return      void
 */
void dq_dqs_egress_dig_set_mode_reg_if(dq_reg_t *dq_reg,
                                       wddr_msr_t msr,
                                       uint8_t bit,
                                       egress_dig_mode_t mode);

#endif /* _DQ_EGRESS_H_ */
