/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _CA_PI_H_
#define _CA_PI_H_

#include <stdbool.h>
#include <ca_reg.h>

/*******************************************************************************
**                            CA
*******************************************************************************/
/**
 * @brief   CA DQ PI ODR Set Configuration Register Interface
 *
 * @details Configures phase interpolator.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   enable  flag to indicate if PI should be enabled.
 * @param[in]   cfg     raw PI configuration (code, xcpl, and gear).
 *
 * @return      void.
 */
void ca_dq_pi_odr_set_cfg_reg_if(ca_reg_t *ca_reg,
                                 wddr_msr_t msr,
                                 wddr_rank_t rank,
                                 bool enable,
                                 uint32_t cfg);

/**
 * @brief   CA DQ PI QDR Set Configuration Register Interface
 *
 * @details Configures phase interpolator.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   enable  flag to indicate if PI should be enabled.
 * @param[in]   cfg     raw PI configuration (code, xcpl, and gear).
 *
 * @return      void
 */
void ca_dq_pi_qdr_set_cfg_reg_if(ca_reg_t *ca_reg,
                                 wddr_msr_t msr,
                                 wddr_rank_t rank,
                                 bool enable,
                                 uint32_t cfg);

/**
 * @brief   CA DQ PI DDR Set Configuration Register Interface
 *
 * @details Configures phase interpolator.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   enable  flag to indicate if PI should be enabled.
 * @param[in]   cfg     raw PI configuration (code, xcpl, and gear).
 *
 * @return      void.
 */
void ca_dq_pi_ddr_set_cfg_reg_if(ca_reg_t *ca_reg,
                                 wddr_msr_t msr,
                                 wddr_rank_t rank,
                                 bool enable,
                                 uint32_t cfg);

/**
 * @brief   CA DQ PI RT Set Configuration Register Interface
 *
 * @details Configures phase interpolator.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   enable  flag to indicate if PI should be enabled.
 * @param[in]   cfg     raw PI configuration (code, xcpl, and gear).
 *
 * @return      void.
 */
void ca_dq_pi_rt_set_cfg_reg_if(ca_reg_t *ca_reg,
                                wddr_msr_t msr,
                                wddr_rank_t rank,
                                bool enable,
                                uint32_t cfg);

/*******************************************************************************
**                            CK
*******************************************************************************/
/**
 * @brief   CA DQS PI ODR Set Configuration Register Interface
 *
 * @details Configures phase interpolator.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   enable  flag to indicate if PI should be enabled.
 * @param[in]   cfg     raw PI configuration (code, xcpl, and gear).
 *
 * @return      void.
 */
void ca_dqs_pi_odr_set_cfg_reg_if(ca_reg_t *ca_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  bool enable,
                                  uint32_t cfg);

/**
 * @brief   CA DQS PI QDR Set Configuration Register Interface
 *
 * @details Configures phase interpolator.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   enable  flag to indicate if PI should be enabled.
 * @param[in]   cfg     raw PI configuration (code, xcpl, and gear).
 *
 * @return      void.
 */
void ca_dqs_pi_qdr_set_cfg_reg_if(ca_reg_t *ca_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  bool enable,
                                  uint32_t cfg);

/**
 * @brief   CA DQS PI DDR Set Configuration Register Interface
 *
 * @details Configures phase interpolator.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   enable  flag to indicate if PI should be enabled.
 * @param[in]   cfg     raw PI configuration (code, xcpl, and gear).
 *
 * @return      void.
 */
void ca_dqs_pi_ddr_set_cfg_reg_if(ca_reg_t *ca_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  bool enable,
                                  uint32_t cfg);

/**
 * @brief   CA DQS PI RT Set Configuration Register Interface
 *
 * @details Configures phase interpolator.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   enable  flag to indicate if PI should be enabled.
 * @param[in]   cfg     raw PI configuration (code, xcpl, and gear).
 *
 * @return      void.
 */
void ca_dqs_pi_rt_set_cfg_reg_if(ca_reg_t *ca_reg,
                                 wddr_msr_t msr,
                                 wddr_rank_t rank,
                                 bool enable,
                                 uint32_t cfg);

/**
 * @brief   CA DQS PI SDR Set Configuration Register Interface
 *
 * @details Configures phase interpolator.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   enable  flag to indicate if PI should be enabled.
 * @param[in]   cfg     raw PI configuration (code, xcpl, and gear).
 *
 * @return      void.
 */
void ca_dqs_pi_sdr_set_cfg_reg_if(ca_reg_t *ca_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  bool enable,
                                  uint32_t cfg);

/**
 * @brief   CA DQS PI DFI Set Configuration Register Interface
 *
 * @details Configures phase interpolator.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   enable  flag to indicate if PI should be enabled.
 * @param[in]   cfg     raw PI configuration (code, xcpl, and gear).
 *
 * @return      void.
 */
void ca_dqs_pi_dfi_set_cfg_reg_if(ca_reg_t *ca_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  bool enable,
                                  uint32_t cfg);

/**
 * @brief   CA DQS PI REN Set Configuration Register Interface
 *
 * @details Configures phase interpolator.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   enable  flag to indicate if PI should be enabled.
 * @param[in]   cfg     raw PI configuration (code, xcpl, and gear).
 *
 * @return      void.
 */
void ca_dqs_pi_ren_set_cfg_reg_if(ca_reg_t *ca_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  bool enable,
                                  uint32_t cfg);

/**
 * @brief   CA DQS PI RCS Set Configuration Register Interface
 *
 * @details Configures phase interpolator.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   enable  flag to indicate if PI should be enabled.
 * @param[in]   cfg     raw PI configuration (code, xcpl, and gear).
 *
 * @return      void.
 */
void ca_dqs_pi_rcs_set_cfg_reg_if(ca_reg_t *ca_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  bool enable,
                                  uint32_t cfg);

/**
 * @brief   CA DQS PI RDQS Set Configuration Register Interface
 *
 * @details Configures phase interpolator.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   enable  flag to indicate if PI should be enabled.
 * @param[in]   cfg     raw PI configuration (code, xcpl, and gear).
 *
 * @return      void.
 */
void ca_dqs_pi_rdqs_set_cfg_reg_if(ca_reg_t *ca_reg,
                                   wddr_msr_t msr,
                                   wddr_rank_t rank,
                                   bool enable,
                                   uint32_t cfg);

/**
 * @brief   DQ DQS PI Get Status Register Interface
 *
 * @details Gets the current phase alignment status for REN and RCS PIs.
 *
 * @note    Passing NULL for either parameter will indicate that status for
 *          that parameter should not be read.
 *
 * @param[in]   ca_reg      pointer to CA register space.
 * @param[out]  ren_status  pointer to store ren status.
 * @param[out]  rcs_status  pointer to store rcs status.
 *
 * @return      void.
 */
void ca_dqs_pi_get_status(ca_reg_t *ca_reg,
                          uint8_t *ren_status,
                          uint8_t *rcs_status);

#endif /* _CA_PI_H_ */
