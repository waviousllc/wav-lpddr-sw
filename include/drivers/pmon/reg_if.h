/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _PMON_REG_IF_H_
#define _PMON_REG_IF_H_

#include <pmon/pmon.h>

/**
 * @brief   Process Monitor Register Interface Initialization
 *
 * @details Initializes Process Monitor device at the register interface level.
 *
 * @param[in]   pmon    pointer to Processs Monitor device to initialize.
 * @param[in]   base    base adders of Process Monitor device.
 *
 * @return      void
 */
void pmon_init_reg_if(pmon_dev_t *pmon, uint32_t base);

/**
 * @brief   Process Monitor Set State Register Interface
 *
 * @details Sets the Process Monitor Device state via CSR.
 *
 * @param[in]   pmon    pointer to Processs Monitor device to initialize.
 * @param[in]   state   state requested to change to.
 *
 * @return      returns whether state was set successfully.
 * @retval      WAV_LPDDR_SUCCESS if state was set.
 * @retval      WAV_LPDDR_ERROR otherwise.
 */
wlpddr_return_t pmon_set_state_reg_if(pmon_dev_t *pmon, pmon_state_t state_req);

/**
 * @brief   Process Monitor Configuration Register Interface
 *
 * @details Configures Process Monitor Device via CSR.
 *
 * @param[in]   pmon        pointer to Processs Monitor device to initialize.
 * @param[in]   refcount    number of refclks to sample for.
 * @param[in]   init_wait   number of refclks to wait before starting.
 *
 * @return      void
 */
void pmon_configure_reg_if(pmon_dev_t *pmon,
                           uint16_t refcount,
                           uint8_t init_wait);

/**
 * @brief   Process Monitor Get Status Count Register Interface
 *
 * @details Gets status count value via CSR.
 *
 * @param[in]   pmon    pointer to Processs Monitor device to initialize.
 * @param[out]  count   pointer to store current status count.
 *
 * @return  void
 */
void pmon_get_status_count_reg_if(pmon_dev_t *pmon, uint32_t *count);

#endif /* _PMON_REG_IF_H_ */
