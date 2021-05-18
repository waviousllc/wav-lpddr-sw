/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _VREF_DRIVER_H_
#define _VREF_DRIVER_H_

#include <vref/device.h>
#include <error.h>

/**
 * @brief   Voltage Reference Initilization Register Interface
 *
 * @details Initailizes Voltage Reference Device at driver level.
 *
 * @param[in]   vref    pointer to Voltage Reference device.
 * @param[in]   base    base address of the Voltage Reference device.
 *
 * @return      void
 */
void vref_init_reg_if(vref_dev_t *vref, uint32_t base);

/**
 * @brief   Voltage Reference Set MSR Register Interface
 *
 * @details Configures Voltage Reference device for given Mode Set Register.
 *
 * @note    This function should only be called when preparing for a frequency
 *          switch.
 *
 * @param[in]   vref    pointer to Voltage Reference device.
 * @param[in]   msr     mode set register value to configure for.
 *
 * @return      void
 */
void vref_set_msr_reg_if(vref_dev_t *vref, wddr_msr_t msr);

/**
 * @brief   Voltage Reference Set State Register Interface
 *
 * @details Sets the state of the Voltage Reference device via CSR.
 *
 * @param[in]   vref    pointer to Voltage Reference device.
 * @param[in]   state   state to set.
 *
 * @return      returns whether state was set successfully.
 * @retval      WDDR_SUCCESS if state was set.
 * @retval      WDDR_ERROR otherwise.
 */
wddr_return_t vref_set_state_reg_if(vref_dev_t *vref, vref_state_t state);

/**
 * @brief   Voltage Reference Set Code Register Interface
 *
 * @details Sets the code (level) of the Voltage Reference device via CSR.
 *
 * @param[in]   vref    pointer to Voltage Reference device.
 * @param[in]   code    code (level) to set.
 *
 * @return      void
 */
void vref_set_code_reg_if(vref_dev_t *vref, uint32_t code);

/**
 * @brief   Voltage Reference Set Power Mode Register Interface
 *
 * @details Sets the power mode of the Voltage Reference device via CSR.
 *
 * @param[in]   vref        pointer to Voltage Reference device.
 * @param[in]   pwr_mode    power mode to set.
 *
 * @return      void
 */
void vref_set_pwr_mode_reg_if(vref_dev_t *vref, vref_pwr_mode_t pwr_mode);

#endif /* VREF_DRIVER_H_ */
