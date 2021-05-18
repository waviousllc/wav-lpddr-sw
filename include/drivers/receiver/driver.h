/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _RECEIVER_DRIVER_H_
#define _RECEIVER_DRIVER_H_

#include <stdint.h>
#include <error.h>
#include <receiver/device.h>
#include <wddr/phy_defs.h>

/**
 * @brief   Receiver Initialize Register Interface
 *
 * @details Initializes receiver device at register interface level.
 *
 * @param[in]   receiver    pointer to the Receiver device to initialize.
 * @param[in]   base        base address of the Receiver device.
 * @param[in]   rank        rank of the Receiver device.
 *
 * @return      void
 */
void receiver_init_reg_if(receiver_dev_t *receiver,
                          uint32_t base,
                          wddr_rank_t rank);

/**
 * @brief   Receiver Set Mode Set Register (MSR) Register Interface
 *
 * @details Configures Receiver device for given mode set register value.
 *
 * @param[in]   receiver    pointer to the Receiver device.
 * @param[in]   msr         MSR value to configure.
 * @param[in]   rank        rank of the Receiver device.
 *
 * @return      void
 */
void receiver_set_msr_reg_if(receiver_dev_t *receiver,
                             wddr_msr_t msr,
                             wddr_rank_t rank);

/**
 * @brief   Receiver Set State Register Interface
 *
 * @details Sets the state of the Receiver device via CSR.
 *
 * @param[in]   receiver    pointer to Receiver device.
 * @param[in]   state       state to set.
 *
 * @return      void
 */
void receiver_set_state_reg_if(receiver_dev_t *receiver,
                               receiver_state_t state);

/**
 * @brief   Receiver Set Mode Register Interface
 *
 * @details Sets the mode of the Receiver device via CSR.
 *
 * @param[in]   receiver    pointer to Receiver device.
 * @param[in]   mode        mode to set.
 * @param[in]   path_state  state of path to set.
 *
 * @return      void
 */
void receiver_set_mode_reg_if(receiver_dev_t *receiver,
                              receiver_mode_t mode,
                              receiver_path_state_t path_state);

/**
 * @brief   Receiver Calibrate Register Interface
 *
 * @details Performs calibration sequence of Receiver device via CSR.
 *
 * @param[in]   receiver    pointer to Receiver device.
 * @param[out]  code        pointer to table to store calibrated codes.
 *
 * @return      returns whether calibration was successful.
 * @return      WDDR_SUCCESS if successful.
 * @return      WDDR_ERROR otherwise.
 */
wddr_return_t receiver_calibrate_reg_if(receiver_dev_t *receiver,
                                        uint8_t code[REC_PN_SIDE_NUM][REC_TC_SIDE_NUM]);

/**
 * @brief   Receiver Set Code Register Interface
 *
 * @details Sets the code values of the Receiver device via CSR.
 *
 * @note    These values should come from the calibration procedure.
 *
 * @param[in]   receiver    pointer to Receiver device.
 * @param[in]   code        set of codes to set.
 *
 * @return      void
 */
void receiver_set_code_reg_if(receiver_dev_t *receiver,
                              uint8_t code[REC_PN_SIDE_NUM][REC_TC_SIDE_NUM]);

/**
 * @brief   Receiver Set Feedback Resistor Register Interface
 *
 * @details Sets the feedback resistor value of the Receiver device via CSR.
 *
 * @param[in]   receiver            pointer to Receiver device.
 * @param[in]   feedback_resistor   the feedback resistor value to set.
 *
 * @return      void
 */
void receiver_set_feedback_resistor_reg_if(receiver_dev_t *receiver,
                                           uint8_t feedback_resistor);

#endif /* _RECEIVER_DRIVER_H_ */
