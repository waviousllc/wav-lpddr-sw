/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _CA_TX_DRIVER_H_
#define _CA_TX_DRIVER_H_

#include <stdbool.h>
#include "ca_reg.h"
#include <driver/table.h>

/*******************************************************************************
**                            CA
*******************************************************************************/
/**
 * @brief   CA DQ Driver Set Impedance Register Interface
 *
 * @details Sets the driver impedance.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   bit     which bit register set to use.
 * @param[in]   tx      tx impedance to set.
 * @param[in]   rx      rx impedance to set.
 *
 * @return      void
 */
void ca_dq_driver_set_impedance_reg_if(ca_reg_t *ca_reg,
                                       wddr_msr_t msr,
                                       uint8_t bit,
                                       driver_impedance_t tx,
                                       driver_impedance_t rx);

/**
 * @brief   CA DQ Driver Set Output Enable Register Interface
 *
 * @details Sets the driver output to given enable state.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   bit     which bit register set to use.
 * @param[in]   enable  flag to indicate if output is enabled.
 *
 * @return      void
 */
void ca_dq_driver_set_oe_reg_if(ca_reg_t *ca_reg,
                                wddr_msr_t msr,
                                uint8_t bit,
                                bool enable);

/**
 * @brief   CA DQ Driver Set Override Register Interface
 *
 * @details Configures the driver override settings.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   bit     which bit register set to use.
 * @param[in]   sel     software override selection value.
 * @param[in]   val_t   value for True (T) side.
 *
 * @return      void
 */
void ca_dq_driver_set_override_reg_if(ca_reg_t *ca_reg,
                                      wddr_msr_t msr,
                                      uint8_t bit,
                                      uint8_t sel,
                                      uint8_t val_t);

/*******************************************************************************
**                            CK
*******************************************************************************/
/**
 * @brief   CA DQS Driver Set Mode Register Interface
 *
 * @details Sets the driver mode. This configures all DQ and DQS drivers to the
 *          same mode.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   mode    mode to set.
 *
 * @return      void.
 */
void ca_dqs_driver_set_mode_reg_if(ca_reg_t *ca_reg,
                                   wddr_msr_t msr,
                                   wddr_rank_t rank,
                                   driver_mode_t mode);

/**
 * @brief   CA DQS Driver Set Code Register Interface
 *
 * @details Sets the driver code. This configures all DQ and DQS drivers to the
 *          same code.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   code    set of PN codes to set.
 *
 * @return      void.
 */
void ca_dqs_driver_set_code_reg_if(ca_reg_t *ca_reg,
                                   wddr_msr_t msr,
                                   wddr_rank_t rank,
                                   uint8_t code[DRVR_PN_CAL_NUM]);

/**
 * @brief   CA DQS Driver Set Loopback Mode Register Interface
 *
 * @details Sets the driver mode. This configures all DQ and DQS drivers to the
 *          same loopback mode.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   mode    loopback mode to set.
 *
 * @return      void.
 */
void ca_dqs_driver_set_loopback_mode_reg_if(ca_reg_t *ca_reg,
                                            wddr_msr_t msr,
                                            wddr_rank_t rank,
                                            driver_loopback_mode_t mode);

/**
 * @brief   CA DQS Driver Set Impedance Register Interface
 *
 * @details Sets the driver impedance.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   bit     which bit register set to use.
 * @param[in]   tx      tx impedance to set.
 * @param[in]   rx      rx impedance to set.
 *
 * @return      void
 */
void ca_dqs_driver_set_impedance_reg_if(ca_reg_t *ca_reg,
                                        wddr_msr_t msr,
                                        uint8_t bit,
                                        driver_impedance_t tx,
                                        driver_impedance_t rx);

/**
 * @brief   CA DQS Driver Set Output Enable Register Interface
 *
 * @details Sets the driver output to given enable state.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   bit     which bit register set to use.
 * @param[in]   enable  flag to indicate if output is enabled.
 *
 * @return      void
 */
void ca_dqs_driver_set_oe_reg_if(ca_reg_t *ca_reg,
                                 wddr_msr_t msr,
                                 uint8_t bit,
                                 bool enable);

/**
 * @brief   CA DQS Driver Set Override Register Interface
 *
 * @details Configures the driver override settings.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   bit     which bit register set to use.
 * @param[in]   sel     software override selection value.
 * @param[in]   val_t   value for True (T) side.
 * @param[in]   val_c   value for Complementary (C) side.
 *
 * @return      void
 */
void ca_dqs_driver_set_override_reg_if(ca_reg_t *ca_reg,
                                       wddr_msr_t msr,
                                       uint8_t bit,
                                       uint8_t sel,
                                       uint8_t val_t,
                                       uint8_t val_c);

#endif /* _CA_TX_DRIVER_H_ */
