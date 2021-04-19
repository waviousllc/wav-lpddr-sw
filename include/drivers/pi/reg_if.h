/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _PI_REG_IF_H_
#define _PI_REG_IF_H_

#include <pi/pi.h>
#include <error.h>

/**
 * @brief   Phase Interpolator Set State Register Interface
 *
 * @details Handles setting state of PI device via CSR.
 *
 * @param[in]   pi          pointer to Phase Interpolator device.
 * @param[in]   state       state requested to change to.
 *
 * @return      returns whether pi state was set successfully.
 * @retval      WAV_LPDDR_SUCCESS if state was set.
 * @retval      WAV_LPDDR_ERROR otherwise.
 */
wlpddr_return_t pi_set_state_reg_if(pi_dev_t *pi, pi_state_t state);

/**
 * @brief   Phase Interpolator Set Code Register Interface
 *
 * @details Handles setting code of PI device via CSR.
 *
 * @param[in]   pi      pointer to Phase Interpolator device.
 * @param[in]   code    value of code to set.
 *
 * @return      void
 */
void pi_set_code_reg_if(pi_dev_t *pi, uint8_t code);

/**
 * @brief   Phase Interpolator Set Gear Register Interface
 *
 * @details Handles setting gear of PI device via CSR.
 *
 * @param[in]   pi      pointer to Phase Interpolator device.
 * @param[in]   gear    value of gear to set.
 *
 * @return      void
 */
void pi_set_gear_reg_if(pi_dev_t *pi, uint8_t gear);

/**
 * @brief Phase Interpolator Set XCPL Register Interface
 *
 * @details Handles setting XCPL of PI device via CSR.
 *
 * @param[in]   pi      pointer to Phase Interpolator device.
 * @param[in]   xcpl    value of gear to set.
 *
 * @return      void
 */
void pi_set_xcpl_reg_if(pi_dev_t *pi, uint8_t xcpl);

/**
 * @brief   Phase Interpolator Set Register Interface
 *
 * @details Handles setting the entire configuration register of the phase
 *          interpolator device via CSR.
 *
 * @param[in]   pi      pointer to Phase Interpolator device.
 * @param[in]   code    the code of PI to set.
 * @param[in]   gear    the gear of PI to set.
 * @param[in]   xcpl    the xcpl of PI to set.
 *
 * @return      void
 */
void pi_set_reg_if(pi_dev_t *pi, uint8_t code, uint8_t gear, uint8_t xcpl);

#endif /* PI_REG_IF_H_ */
