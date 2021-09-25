/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _CA_EGRESS_H_
#define _CA_EGRESS_H_

#include <stdbool.h>
#include <ca_reg.h>
#include <dp/table.h>

/*******************************************************************************
**                            CA
*******************************************************************************/
/**
 * @brief   CA DQ Egress Analog Set Mode Register Interface
 *
 * @details Sets Egress analog mode.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   bit     which bit register set to use.
 * @param[in]   mode    egress mode to set.
 *
 * @return      void
 */
void ca_dq_egress_ana_set_mode_reg_if(ca_reg_t *ca_reg,
                                      wddr_msr_t msr,
                                      uint8_t bit,
                                      egress_ana_mode_t mode);

/**
 * @brief   CA DQ Egress Digital Set Mode Register Interface
 *
 * @details Sets Egress digital mode.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   bit     which bit register set to use.
 * @param[in]   mode    egress mode to set.
 *
 * @return      void
 */
void ca_dq_egress_dig_set_mode_reg_if(ca_reg_t *ca_reg,
                                      wddr_msr_t msr,
                                      uint8_t bit,
                                      egress_dig_mode_t mode);

/*******************************************************************************
**                            CK
*******************************************************************************/
/**
 * @brief   CA DQS Egress Analog Set Mode Register Interface
 *
 * @details Sets Egress analog mode.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   bit     which bit register set to use.
 * @param[in]   mode    egress mode to set.
 *
 * @return      void
 */
void ca_dqs_egress_ana_set_mode_reg_if(ca_reg_t *ca_reg,
                                       wddr_msr_t msr,
                                       uint8_t bit,
                                       egress_ana_mode_t mode);

/**
 * @brief   CA DQS Egress Digital Set Mode Register Interface
 *
 * @details Sets Egress digital mode.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   bit     which bit register set to use.
 * @param[in]   mode    egress mode to set.
 *
 * @return      void
 */
void ca_dqs_egress_dig_set_mode_reg_if(ca_reg_t *ca_reg,
                                       wddr_msr_t msr,
                                       uint8_t bit,
                                       egress_dig_mode_t mode);

#endif /* _CA_EGRESS_H_ */
