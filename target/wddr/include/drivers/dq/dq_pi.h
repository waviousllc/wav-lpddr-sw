/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DQ_PI_H_
#define _DQ_PI_H_

#include <stdbool.h>
#include <dq_reg.h>

/*******************************************************************************
**                            DQ
*******************************************************************************/
/**
 * @brief   DQ DQ PI ODR Set Configuration Register Interface
 *
 * @details Configures phase interpolator.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   enable  flag to indicate if PI should be enabled.
 * @param[in]   cfg     raw PI configuration (code, xcpl, and gear).
 *
 * @return      void.
 */
void dq_dq_pi_odr_set_cfg_reg_if(dq_reg_t *dq_reg,
                                 wddr_msr_t msr,
                                 wddr_rank_t rank,
                                 bool enable,
                                 uint32_t cfg);

/**
 * @brief   DQ DQ PI QDR Set Configuration Register Interface
 *
 * @details Configures phase interpolator.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   enable  flag to indicate if PI should be enabled.
 * @param[in]   cfg     raw PI configuration (code, xcpl, and gear).
 *
 * @return      void
 */
void dq_dq_pi_qdr_set_cfg_reg_if(dq_reg_t *dq_reg,
                                 wddr_msr_t msr,
                                 wddr_rank_t rank,
                                 bool enable,
                                 uint32_t cfg);

/**
 * @brief   DQ DQ PI DDR Set Configuration Register Interface
 *
 * @details Configures phase interpolator.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   enable  flag to indicate if PI should be enabled.
 * @param[in]   cfg     raw PI configuration (code, xcpl, and gear).
 *
 * @return      void.
 */
void dq_dq_pi_ddr_set_cfg_reg_if(dq_reg_t *dq_reg,
                                 wddr_msr_t msr,
                                 wddr_rank_t rank,
                                 bool enable,
                                 uint32_t cfg);

/**
 * @brief   DQ DQ PI RT Set Configuration Register Interface
 *
 * @details Configures phase interpolator.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   enable  flag to indicate if PI should be enabled.
 * @param[in]   cfg     raw PI configuration (code, xcpl, and gear).
 *
 * @return      void.
 */
void dq_dq_pi_rt_set_cfg_reg_if(dq_reg_t *dq_reg,
                                wddr_msr_t msr,
                                wddr_rank_t rank,
                                bool enable,
                                uint32_t cfg);

/*******************************************************************************
**                            DQS
*******************************************************************************/
/**
 * @brief   DQ DQS PI ODR Set Configuration Register Interface
 *
 * @details Configures phase interpolator.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   enable  flag to indicate if PI should be enabled.
 * @param[in]   cfg     raw PI configuration (code, xcpl, and gear).
 *
 * @return      void.
 */
void dq_dqs_pi_odr_set_cfg_reg_if(dq_reg_t *dq_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  bool enable,
                                  uint32_t cfg);
/**
 * @brief   DQ DQS PI QDR Set Configuration Register Interface
 *
 * @details Configures phase interpolator.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   enable  flag to indicate if PI should be enabled.
 * @param[in]   cfg     raw PI configuration (code, xcpl, and gear).
 *
 * @return      void.
 */
void dq_dqs_pi_qdr_set_cfg_reg_if(dq_reg_t *dq_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  bool enable,
                                  uint32_t cfg);

/**
 * @brief   DQ DQS PI DDR Set Configuration Register Interface
 *
 * @details Configures phase interpolator.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   enable  flag to indicate if PI should be enabled.
 * @param[in]   cfg     raw PI configuration (code, xcpl, and gear).
 *
 * @return      void.
 */
void dq_dqs_pi_ddr_set_cfg_reg_if(dq_reg_t *dq_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  bool enable,
                                  uint32_t cfg);
/**
 * @brief   DQ DQS PI RT Set Configuration Register Interface
 *
 * @details Configures phase interpolator.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   enable  flag to indicate if PI should be enabled.
 * @param[in]   cfg     raw PI configuration (code, xcpl, and gear).
 *
 * @return      void.
 */
void dq_dqs_pi_rt_set_cfg_reg_if(dq_reg_t *dq_reg,
                                 wddr_msr_t msr,
                                 wddr_rank_t rank,
                                 bool enable,
                                 uint32_t cfg);
/**
 * @brief   DQ DQS PI SDR Set Configuration Register Interface
 *
 * @details Configures phase interpolator.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   enable  flag to indicate if PI should be enabled.
 * @param[in]   cfg     raw PI configuration (code, xcpl, and gear).
 *
 * @return      void.
 */
void dq_dqs_pi_sdr_set_cfg_reg_if(dq_reg_t *dq_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  bool enable,
                                  uint32_t cfg);

/**
 * @brief   DQ DQS PI DFI Set Configuration Register Interface
 *
 * @details Configures phase interpolator.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   enable  flag to indicate if PI should be enabled.
 * @param[in]   cfg     raw PI configuration (code, xcpl, and gear).
 *
 * @return      void.
 */
void dq_dqs_pi_dfi_set_cfg_reg_if(dq_reg_t *dq_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  bool enable,
                                  uint32_t cfg);
/**
 * @brief   DQ DQS PI REN Set Configuration Register Interface
 *
 * @details Configures phase interpolator.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   enable  flag to indicate if PI should be enabled.
 * @param[in]   cfg     raw PI configuration (code, xcpl, and gear).
 *
 * @return      void.
 */
void dq_dqs_pi_ren_set_cfg_reg_if(dq_reg_t *dq_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  bool enable,
                                  uint32_t cfg);
/**
 * @brief   DQ DQS PI RCS Set Configuration Register Interface
 *
 * @details Configures phase interpolator.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   enable  flag to indicate if PI should be enabled.
 * @param[in]   cfg     raw PI configuration (code, xcpl, and gear).
 *
 * @return      void.
 */
void dq_dqs_pi_rcs_set_cfg_reg_if(dq_reg_t *dq_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  bool enable,
                                  uint32_t cfg);
/**
 * @brief   DQ DQS PI RDQS Set Configuration Register Interface
 *
 * @details Configures phase interpolator.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr     which MSR register set to use.
 * @param[in]   rank    which rank register set to use.
 * @param[in]   enable  flag to indicate if PI should be enabled.
 * @param[in]   cfg     raw PI configuration (code, xcpl, and gear).
 *
 * @return      void.
 */
void dq_dqs_pi_rdqs_set_cfg_reg_if(dq_reg_t *dq_reg,
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
 * @param[in]   dq_reg      pointer to DQ Byte register space.
 * @param[out]  ren_status  pointer to store ren status.
 * @param[out]  rcs_status  pointer to store rcs status.
 *
 * @return      void.
 */
void dq_dqs_pi_get_status(dq_reg_t *dq_reg,
                          uint8_t *ren_status,
                          uint8_t *rcs_status);

#endif /* _DQ_PI_H_ */
