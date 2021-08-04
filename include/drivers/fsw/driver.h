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
 * @return      void.
 */
void fsw_ctrl_set_msr_vco_ovr_val_reg_if(uint8_t msr, uint8_t vco_id);

/**
 * @brief   HW Frequency Switch (FSW) MSR/VCO Override Register Interface
 *
 * @details Sets the override enable of MSR and VCO via CSR.
 *
 * @param[in]   enable  desired override state.
 *
 * @return      void.
 */
void fsw_ctrl_set_msr_vco_ovr_reg_if(bool enable);

/**
 * @brief   HW Frequency Switch (FSW) MSR Toggle Enable Register Interface
 *
 * @details Sets if MSR toggle should be enabled during frequency switch.
 *
 * @param[in]   enable  desired toggle state.
 *
 * @return      void.
 */
void fsw_ctrl_set_msr_toggle_en_reg_if(bool enable);

/**
 * @brief   HW Frequency Switch (FSW) VCO Toggle Enable Register Interface
 *
 * @details Sets if VCO toggle should be enabled during frequency switch.
 *
 * @param[in]   enable  desired toggle state.
 *
 * @return      void.
 */
void fsw_ctrl_set_vco_toggle_en_reg_if(bool enable);

/**
 * @brief   HW Frequency Switch (FSW) Set Prep Done Register Interface
 *
 * @details Sets PREP_DONE state via CSR.
 *
 * @param[in]   done    state of prep.
 *
 * @return      void.
 */
void fsw_ctrl_set_prep_done_reg_if(bool done);

/**
 * @brief   HW Frequency Switch (FSW) Set Post Work Done Register Interface
 *
 * @details Sets the Post Work Done status and override state via CSR.
 *
 * @param[in]   override    state of override.
 * @param[in]   done        state of post work done.
 *
 * @return      void.
 */
void fsw_ctrl_set_post_work_done(bool override, bool done);

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
