/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DQ_TX_DRIVER_H_
#define _DQ_TX_DRIVER_H_

#include <stdbool.h>
#include <dq_reg.h>
#include <driver/table.h>

/*******************************************************************************
**                            DQ
*******************************************************************************/
/**
 * @brief   DQ DQ Driver Set Impedance Register Interface
 *
 * @details Sets the driver impedance.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   bit     which bit register set to use.
 * @param[in]   tx      tx impedance to set.
 * @param[in]   rx      rx impedance to set.
 *
 * @return      void
 */
void dq_dq_driver_set_impedance_reg_if(dq_reg_t *dq_reg,
                                       wddr_msr_t msr,
                                       uint8_t bit,
                                       driver_impedance_t tx,
                                       driver_impedance_t rx);

/**
 * @brief   DQ DQ Driver Set Output Enable Register Interface
 *
 * @details Sets the driver output to given enable state.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   bit     which bit register set to use.
 * @param[in]   enable  flag to indicate if output is enabled.
 *
 * @return      void
 */
void dq_dq_driver_set_oe_reg_if(dq_reg_t *dq_reg,
                                wddr_msr_t msr,
                                uint8_t bit,
                                bool enable);

/**
 * @brief   DQ DQ Driver Set Override Register Interface
 *
 * @details Configures the driver override settings.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   bit     which bit register set to use.
 * @param[in]   sel     software override selection value.
 * @param[in]   val_t   value for True (T) side.
 *
 * @return      void
 */
void dq_dq_driver_set_override_reg_if(dq_reg_t *dq_reg,
                                      wddr_msr_t msr,
                                      uint8_t bit,
                                      uint8_t sel,
                                      uint8_t val_t);

/*******************************************************************************
**                            DQS
*******************************************************************************/
/**
 * @brief   DQ DQS Driver Set Mode Register Interface
 *
 * @details Sets the driver mode. This configures all DQ and DQS drivers to the
 *          same mode.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   mode    mode to set.
 *
 * @return      void.
 */
void dq_dqs_driver_set_mode_reg_if(dq_reg_t *dq_reg,
                                   wddr_msr_t msr,
                                   wddr_rank_t rank,
                                   driver_mode_t mode);

/**
 * @brief   DQ DQS Driver Set Code Register Interface
 *
 * @details Sets the driver code. This configures all DQ and DQS drivers to the
 *          same code.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   code    set of PN codes to set.
 *
 * @return      void.
 */
void dq_dqs_driver_set_code_reg_if(dq_reg_t *dq_reg,
                                   wddr_msr_t msr,
                                   wddr_rank_t rank,
                                   uint8_t code[DRVR_PN_CAL_NUM]);

/**
 * @brief   DQ DQS Driver Set Loopback Mode Register Interface
 *
 * @details Sets the driver mode. This configures all DQ and DQS drivers to the
 *          same loopback mode.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   mode    loopback mode to set.
 *
 * @return      void.
 */
void dq_dqs_driver_set_loopback_mode_reg_if(dq_reg_t *dq_reg,
                                            wddr_msr_t msr,
                                            wddr_rank_t rank,
                                            driver_loopback_mode_t mode);

/**
 * @brief   DQ DQS Driver Set Impedance Register Interface
 *
 * @details Sets the driver impedance.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   bit     which bit register set to use.
 * @param[in]   tx      tx impedance to set.
 * @param[in]   rx      rx impedance to set.
 *
 * @return      void
 */
void dq_dqs_driver_set_impedance_reg_if(dq_reg_t *dq_reg,
                                        wddr_msr_t msr,
                                        uint8_t bit,
                                        driver_impedance_t tx,
                                        driver_impedance_t rx);

/**
 * @brief   DQ DQS Driver Set Output Enable Register Interface
 *
 * @details Sets the driver output to given enable state.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   bit     which bit register set to use.
 * @param[in]   enable  flag to indicate if output is enabled.
 *
 * @return      void
 */
void dq_dqs_driver_set_oe_reg_if(dq_reg_t *dq_reg,
                                 wddr_msr_t msr,
                                 uint8_t bit,
                                 bool enable);

/**
 * @brief   DQ DQS Driver Set Override Register Interface
 *
 * @details Configures the driver override settings.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   bit     which bit register set to use.
 * @param[in]   sel     software override selection value.
 * @param[in]   val_t   value for True (T) side.
 * @param[in]   val_c   value for Complementary (C) side.
 *
 * @return      void
 */
void dq_dqs_driver_set_override_reg_if(dq_reg_t *dq_reg,
                                       wddr_msr_t msr,
                                       uint8_t bit,
                                       uint8_t sel,
                                       uint8_t val_t,
                                       uint8_t val_c);

#endif /* _DQ_TX_DRIVER_H_ */
