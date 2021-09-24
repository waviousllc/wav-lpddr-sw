/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _CMN_RSTN_H_
#define _CMN_RSTN_H_

#include "cmn_reg.h"

/**
 * @brief   Common RESETN Set Pin Register Interface
 *
 * @details Sets the state of the RESETN pin to the DRAM. Driving RESETN high
 *          releases the RESET of the DRAM chip.
 *
 * @param[in]   cmn_reg     pointer to CTRL register space.
 * @param[in]   override    flag to indicate if SW should override RESETN poin
 *                          state.
 * @param[in]   high        flag to indicate if RESETN pin should be driven
 *                          high.
 *
 * @return      void.
 */
void cmn_rstn_set_pin_reg_if(cmn_reg_t *cmn_reg, bool override, bool high);

/**
 * @brief   Common RESETN Get Pin Register Interface
 *
 * @details Gets the value of RESETN loopack.
 *
 * @param[in]   cmn_reg     pointer to CTRL register space.
 * @param[in]   rstn_lb     RESETN loopback value.
 *
 * @return      void.
 */
void cmn_rstn_get_loopback_reg_if(cmn_reg_t *cmn_reg, uint8_t *rstn_lb);

#endif /* _CMN_RSTN_H_ */
