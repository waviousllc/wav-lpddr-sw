/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DQ_RECEIVER_H_
#define _DQ_RECEIVER_H_

#include <stdbool.h>
#include <dq_reg.h>
#include <receiver/table.h>

/**
 * @brief Receiver State Enumerations
 *
 * @details Supported Receiver States.
 *
 * DISABLE  receiver not operational.
 * CAL      receiver in calibration mode.
 * ENABLE   receiver is enabled.
 */
typedef enum receiver_state_t
{
    REC_STATE_DISABLE,
    REC_STATE_CAL,
    REC_STATE_ENABLE
} receiver_state_t;

/*******************************************************************************
**                            DQS
*******************************************************************************/
/**
 * @brief   DQ DQS Receiver Set State Register Interface
 *
 * @details Sets the state of the receiver.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   rank        which rank register set to use.
 * @param[in]   state       state to set.
 *
 * @return      void
 */
void dq_dqs_receiver_set_state_reg_if(dq_reg_t *dq_reg,
                                      wddr_msr_t msr,
                                      wddr_rank_t rank,
                                      receiver_state_t state);

/**
 * @brief   DQ DQS Receiver Set Mode Register Interface
 *
 * @details Sets the mode of the receiver.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   rank        which rank register set to use.
 * @param[in]   mode        mode to set.
 * @param[in]   path_state  path state to set.
 *
 * @return      void
 */
void dq_dqs_receiver_set_mode_reg_if(dq_reg_t *dq_reg,
                                     wddr_msr_t msr,
                                     wddr_rank_t rank,
                                     receiver_mode_t mode,
                                     receiver_path_state_t path_state);

/**
 * @brief   DQ DQS Receiver Set Code Register Interface
 *
 * @details Sets the calibration codes of the receiver.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   rank        which rank register set to use.
 * @param[in]   code        codes to set for PN and TC side.
 *
 * @return      void
 */
void dq_dqs_receiver_set_code_reg_if(dq_reg_t *dq_reg,
                                     wddr_msr_t msr,
                                     wddr_rank_t rank,
                                     uint8_t code[REC_PN_SIDE_NUM][REC_TC_SIDE_NUM]);

/**
 * @brief   DQ DQS Receiver Set Feedback Resistor Register Interface
 *
 * @details Sets the feedback resistor of the receiver.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr                 which MSR register set to use.
 * @param[in]   rank                which rank register set to use.
 * @param[in]   feedback_resistor   feedback resistor value to set.
 *
 * @return      void
 */
void dq_dqs_receiver_set_feedback_resistor_reg_if(dq_reg_t *dq_reg,
                                                  wddr_msr_t msr,
                                                  wddr_rank_t rank,
                                                  uint8_t feedback_resistor);

/**
 * @brief   DQ DQS Receiver Set Delay Register Interface
 *
 * @details Sets the delay of the receiver.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   rank        which rank register set to use.
 * @param[in]   rank        which bit register set to use.
 * @param[in]   delay       delay value to set.
 * @param[in]   side        which side of receiver to delay.
 *
 * @return      void
 */
void dq_dqs_receiver_set_delay_reg_if(dq_reg_t *dq_reg,
                                      wddr_msr_t msr,
                                      wddr_rank_t rank,
                                      uint8_t bit,
                                      uint16_t delay,
                                      receiver_tc_side_mask_t side);

#endif /* _DQ_RECEIVER_H_ */
