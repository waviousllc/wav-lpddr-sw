/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _FSW_DRIVER_H_
#define _FSW_DRIVER_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief   HW Frequency Switch (FSW) MSR/VCO Value Override Register Interface
 *
 * @details Sets the override value of MSR and VCO via CSR.
 *
 * @param[in]   msr     desired MSR override.
 * @param[in]   vco_id  desired VCO ID override.
 *
 * @return      void
 */
void fsw_ctrl_set_msr_vco_ovr_val_reg_if(uint8_t msr, uint8_t vco_id);

/**
 * @brief   HW Frequency Switch (FSW) MSR/VCO Override Register Interface
 *
 * @details Sets the override enable of MSR and VCO via CSR.
 *
 * @param[in]   enable  desired override state.
 *
 * @return      void
 */
void fsw_ctrl_set_msr_vco_ovr_reg_if(bool enable);

/**
 * @brief   HW Frequency Switch (FSW) Clock Disable Override Register Interface
 *
 * @details Sets the override value of FSW CSP Clock Disable via CSR.
 *
 * @param[in]   enable  desired override state.
 *
 * @return      void
 */
void fsw_csp_set_clk_disable_ovr_val_reg_if(bool enable);

/**
 * @brief   HW Frequency Switch (FSW) CSP Sync Register Interface
 *
 * @details Performs CSP Sync procedure via CSR.
 *
 * @return  void
 */
void fsw_csp_sync_reg_if(void);

#endif /* _FSW_DRIVER_H_ */
