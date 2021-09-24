/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _FSW_CTRL_H_
#define _FSW_CTRL_H_

#include <stdbool.h>
#include "fsw_reg.h"

/**
 * @brief   HW Frequency Switch (FSW) MSR/VCO Value Override Register Interface
 *
 * @details Sets the override value of MSR and VCO via CSR.
 *
 * @param[in]   fsw_reg pointer to frequency switch register space.
 * @param[in]   msr     desired MSR override.
 * @param[in]   vco_id  desired VCO ID override.
 *
 * @return      void.
 */
void fsw_ctrl_set_msr_vco_ovr_val_reg_if(fsw_reg_t *fsw_reg,
                                         uint8_t msr,
                                         uint8_t vco_id);

/**
 * @brief   HW Frequency Switch (FSW) MSR/VCO Override Register Interface
 *
 * @details Sets the override enable of MSR and VCO via CSR.
 *
 * @param[in]   fsw_reg pointer to frequency switch register space.
 * @param[in]   enable  desired override state.
 *
 * @return      void.
 */
void fsw_ctrl_set_msr_vco_ovr_reg_if(fsw_reg_t *fsw_reg, bool enable);

/**
 * @brief   HW Frequency Switch (FSW) MSR Toggle Enable Register Interface
 *
 * @details Sets if MSR toggle should be enabled during frequency switch.
 *
 * @param[in]   fsw_reg pointer to frequency switch register space.
 * @param[in]   enable  desired toggle state.
 *
 * @return      void.
 */
void fsw_ctrl_set_msr_toggle_en_reg_if(fsw_reg_t *fsw_reg, bool enable);

/**
 * @brief   HW Frequency Switch (FSW) VCO Toggle Enable Register Interface
 *
 * @details Sets if VCO toggle should be enabled during frequency switch.
 *
 * @param[in]   fsw_reg pointer to frequency switch register space.
 * @param[in]   enable  desired toggle state.
 *
 * @return      void.
 */
void fsw_ctrl_set_vco_toggle_en_reg_if(fsw_reg_t *fsw_reg, bool enable);

/**
 * @brief   HW Frequency Switch (FSW) Set Prep Done Register Interface
 *
 * @details Sets PREP_DONE state via CSR.
 *
 * @param[in]   fsw_reg pointer to frequency switch register space.
 * @param[in]   done    state of prep.
 *
 * @return      void.
 */
void fsw_ctrl_set_prep_done_reg_if(fsw_reg_t *fsw_reg, bool done);

/**
 * @brief   HW Frequency Switch (FSW) Set Post Work Done Register Interface
 *
 * @details Sets the Post Work Done status and override state via CSR.
 *
 * @param[in]   fsw_reg     pointer to frequency switch register space.
 * @param[in]   override    state of override.
 * @param[in]   done        state of post work done.
 *
 * @return      void.
 */
void fsw_ctrl_set_post_work_done_reg_if(fsw_reg_t *fsw_reg,
                                        bool override,
                                        bool done);

/**
 * @brief   HW Frequency Switch Get Current MSR Register Interface
 *
 * @details Gets the current Mode Switch Register (MSR) in the PHY.
 *
 * @param[in]   fsw_reg     pointer to frequency switch register space.
 *
 * @return  returns current Mode Switch Register value.
 * @retval  Possible MSRs values are 0 and 1.
 */
uint8_t fsw_ctrl_get_current_msr_reg_if(fsw_reg_t *fsw_reg);

#endif /* _FSW_CTRL_H_ */
