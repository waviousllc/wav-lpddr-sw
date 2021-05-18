/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _LPDE_DRIVER_H_
#define _LPDE_DRIVER_H_

#include <stdint.h>
#include <lpde/device.h>

/**
 * @brief   LPDE Set State Register Interface
 *
 * @details Handles setting the state of the LPDE device via CSR.
 *
 * @param[in]   lpde    pointer to LPDE Device.
 * @param[in]   state   the state that LPDE should move to.
 *
 * @return      void
 */
void lpde_set_state_reg_if(lpde_dev_t *lpde, lpde_state_t state);

/**
 * @brief   LPDE Set Delay Register Interface
 *
 * @details Handles setting the delay of the LPDE device via CSR.
 *
 * @param[in]   lpde    pointer to LPDE Device.
 * @param[in]   delay   the delay to set.
 *
 * @return      void
 */
void lpde_set_delay_reg_if(lpde_dev_t *lpde, uint8_t delay);

/**
 * @brief   LPDE Set Gear Register Interface
 *
 * @details Handles setting the gear of the LPDE device via CSR.
 *
 * @param[in]   lpde    pointer to LPDE Device.
 * @param[in]   gear    the gear to set.
 *
 * @return      void
 */
void lpde_set_gear_reg_if(lpde_dev_t *lpde, uint8_t gear);

/**
 * @brief   LPDE Set Register Interface
 *
 * @details Handles setting the entire configuration register of the LPDE device
 *          via CSR.
 *
 * @param[in]   lpde    pointer to LPDE Device.
 * @param[in]   gear    the gear of LPDE to set.
 * @param[in]   delay   the delay of LPDE to set.
 * @param[in]   state   the state that LPDE should move to.
 *
 * @return      void
 */
void lpde_set_reg_if(lpde_dev_t *lpde,
                     uint8_t gear,
                     uint8_t delay,
                     lpde_state_t state);

#endif /* _LPDE_DRIVER_H_ */
