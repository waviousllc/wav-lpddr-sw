/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#ifndef _SENSAMP_DRIVER_H_
#define _SENSAMP_DRIVER_H_

#include <stdint.h>
#include <stdbool.h>
#include <sensamp/device.h>

/**
 * @brief   Sense Amp (Sensamp) DQ Byte Initialize Register Interface
 *
 * @details Initializes Sensamp device at the register interface level.
 *
 * @param[in]   sa_dqbyte   pointer to Sensamp device.
 * @param[in]   base        base address of Sensamp device.
 *
 * @return      void
 */
void sensamp_dqbyte_init_reg_if(sensamp_dqbyte_dev_t *sa_dqbyte,
                                uint32_t base);

/**
 * @brief   Sense Amp (Sensamp) DQ Byte Set MSR Register Interface
 *
 * @details Configures Sensamp DQ Byte device for give mode set register value.
 *
 * @param[in]   sa_dqbyte   pointer to Sensamp device.
 * @param[in]   msr         MSR value to configure.
 *
 * @return      void
 */
void sensamp_dqbyte_set_msr_reg_if(sensamp_dqbyte_dev_t *sa_dqbyte,
                                   wddr_msr_t msr);

/**
 * @brief   Sense Amp (Sensamp) DQ Bit Get Status Register Interface
 *
 * @details Returns Sensamp device status via CSR for a given bit and SA index.
 *
 * @param[in]   sa_dqbyte   pointer to Sensamp device.
 * @param[in]   bit         which bit to get status from.
 * @param[in]   sa_index    which sa index to get status from.
 * @param[out]  status      pointer to where to store status.
 *
 * @return      void
 */
void sensamp_dqbyte_get_status_reg_if(sensamp_dqbyte_dev_t *sa_dqbyte,
                                      uint8_t bit,
                                      sensamp_index_t sa_index,
                                      uint8_t *status);

/**
 * @brief  Sense Amp (Sensamp) DQ Byte Set State Register Interface
 *
 * @details Sets the state of the Sensamp device via CSR.
 *
 * @param[in]   sa_dqbyte   pointer to Sensamp device.
 * @param[in]   state       state to set.
 *
 * @return      returns whether state was set successfully.
 * @retval      WDDR_SUCCESS if state was set.
 * @retval      WDDR_ERROR otherwise.
 */
wddr_return_t sensamp_dqbyte_set_state_reg_if(sensamp_dqbyte_dev_t *sa_dqbyte,
                                              sensamp_state_t state);
/**
 * @brief   Sense Amp (Sensamp) DQ Bit Set Code Register Interface
 *
 * @details Sets the given code via CSR for the given sensamp index.
 *
  * @param[in]  sa_dqbit    pointer to Sensamp DQ Bit device.
  * @param[in]  sa_index    sensamp index to update.
  * @param[in]  code        code to set.
  * @param[in]  clear       flag to indicate if other codes should be cleared.
  *
  * @return     void
  */
void sensamp_dqbit_set_cal_code_reg_if(sensamp_dqbit_dev_t *sa_dqbit,
                                       sensamp_index_t sa_index,
                                       uint8_t code,
                                       bool clear);

#endif /* _SENSAMP_DRIVER_H_ */
