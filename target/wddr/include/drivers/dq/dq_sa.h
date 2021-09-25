/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DQ_SA_H_
#define _DQ_SA_H_

#include <stdbool.h>
#include <dq_reg.h>
#include <sensamp/table.h>

/**
 * @brief   Sense Amp (Sensamp) State Enumerations
 *
 * @details Enumerations for valid states that Sensamp device an be in.
 *
 * DYNAMIC          sensamp device dynamically controlled by PHY logic.
 * ENABLED          sensamp device forced enable.
 * CAL_ENABELED     sensamp device forced enable and in calibration mode.
 */
typedef enum sensamp_state_t
{
    SA_STATE_DYNAMIC,
    SA_STATE_ENABLED,
    SA_STATE_CAL_ENABLED
} sensamp_state_t;

/*******************************************************************************
**                            DQ
*******************************************************************************/
/**
 * @brief   DQ DQ Sensamp Get Status Register Interface
 *
 * @details Retrieves current status of the sensamp associated with a given bit.
 *
 * @param[in]   dq_reg      pointer to DQ Byte register space.
 * @param[in]   bit         which bit register set to use.
 * @param[in]   sa_index    which sa index to get status from.
 * @param[out]  status      pointer to where to store status.
 *
 * @return      void
 */
 void dq_dq_sa_get_status_reg_if(dq_reg_t *dq_reg,
                                uint8_t bit,
                                sensamp_index_t sa_index,
                                uint8_t *status);

/**
 * @brief   DQ DQ Sensamp Clear Cal Code Register Interface
 *
 * @details Clears stored calibration code for all SA indices.
 *
 * @param[in]   dq_reg      pointer to DQ Byte register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   rank        which rank register set to use.
 * @param[in]   bit         which bit register set to use.
 *
 * @return      void.
 */
void dq_dq_sa_clear_cal_code_reg_if(dq_reg_t *dq_reg,
                                    wddr_msr_t msr,
                                    wddr_rank_t rank,
                                    uint8_t bit);

/**
 * @brief   DQ DQ Sensamp Set Cal Code Register Interface
 *
 * @details Sets stored calibration code for given SA index.
 *
 * @param[in]   dq_reg      pointer to DQ Byte register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   rank        which rank register set to use.
 * @param[in]   bit         which bit register set to use.
 * @param[in]   sa_index    which SA index to set.
 * @param[in]   code        calibration code to set.
 *
 * @return      void.
 */
void dq_dq_sa_set_cal_code_reg_if(dq_reg_t *dq_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  uint8_t bit,
                                  sensamp_index_t sa_index,
                                  uint8_t code);

/*******************************************************************************
**                            DQS
*******************************************************************************/
/**
 * @brief   DQ DQS Sensamp Set State Register Interface
 *
 * @details Sets state for all Sensamps for the given byte.
 *
 * @param[in]   dq_reg      pointer to DQ Byte register space.
 * @param[in]   state       state to set.
 *
 * @return      void.
 */
void dq_dqs_sa_cmn_set_state_reg_if(dq_reg_t *dq_reg,
                                    sensamp_state_t state);

#endif /* _DQ_SA_H_ */
