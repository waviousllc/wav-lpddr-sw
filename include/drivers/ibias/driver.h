/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _IBIAS_DRIVER_H_
#define _IBIAS_DRIVER_H_

#include <error.h>
#include <ibias/device.h>

/**
 * @brief   Bias Current Initialization Register Interface
 *
 * @details Initializes the Bias Current Device at the register interface level.
 *
 * @param[in]   ibias   pointer to Bias Current device to initialize.
 * @param[in]   base    base address of the Bias Current device.
 *
 * @return      void
 */
void ibias_init_reg_if(ibias_dev_t *ibias, uint32_t base);

/**
 * @brief   Bias Current Set State Register Interface
 *
 * @details Sets the Bias Current device state via CSR.
 *
 * @param[in]   ibias     pointer to Bias Current Device.
 * @param[in]   state     state requested to change to.
 *
 * @return      returns whether state was set successfully.
 * @retval      WDDR_SUCCESS if state was set.
 * @retval      WDDR_ERROR otherwise.
 */
wddr_return_t ibias_set_state_reg_if(ibias_dev_t *ibias, ibias_state_t state);

#endif /* _IBIAS_DRIVER_H_ */
