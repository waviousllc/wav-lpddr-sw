/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _FSW_CSP_H_
#define _FSW_CSP_H_

#include "fsw_reg.h"

/**
 * @brief   HW Frequency Switch (FSW) Clock Disable Override Register Interface
 *
 * @details Sets the override value of FSW CSP Clock Disable via CSR.
 *
 * @param[in]   fsw_reg pointer to frequency switch register space.
 * @param[in]   enable  desired override state.
 *
 * @return      void
 */
void fsw_csp_set_clk_disable_over_val_reg_if(fsw_reg_t *fsw_reg,
                                             bool enable);

/**
 * @brief   HW Frequency Switch (FSW) CSP Sync Register Interface
 *
 * @details Performs CSP Sync procedure via CSR.
 *
 * @param[in]   fsw_reg pointer to frequency switch register space.
 *
 * @return  void
 */
void fsw_csp_sync_reg_if(fsw_reg_t *fsw_reg);

#endif /* _FSW_CSP_H_ */
