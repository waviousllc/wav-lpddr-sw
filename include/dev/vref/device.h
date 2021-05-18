/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _VREF_DEV_H_
#define _VREF_DEV_H_

#include <stdint.h>
#include <vref/table.h>
#include <wddr/phy_defs.h>

/**
 * @brief   Voltage Reference State Enumeration
 *
 * DISABLED    Disabled state.
 * HIZ         Hi-Z state.
 * ENABLED     Enabled state.
 */
typedef enum vref_state_t
{
    VREF_STATE_DISABLED,
    VREF_STATE_HIZ,
    VREF_STATE_ENABLED
} vref_state_t;

/**
 * @brief   Voltage Reference Structure
 *
 * base    structure for storing base address.
 */
typedef struct voltage_ref_dev_t
{
    uint32_t        base;
} vref_dev_t;

/**
 * @brief   Voltage Reference Initilization
 *
 * @details Initailizes Voltage Reference Device at device level.
 *
 * @param[in]   vref    pointer to Voltage Reference device.
 * @param[in]   base    base address of the Voltage Reference device.
 *
 * @return      void
 */
void vref_init(vref_dev_t *vref, uint32_t base);

/**
 * @brief   Voltage Reference Set Mode Enable
 *
 * @details Places the Voltage Reference device into Enabled mode.
 *
 * @param[in]   vref    pointer to Voltage Reference device.
 *
 * @return      void
 */
void vref_set_mode_enable(vref_dev_t *vref);

/**
 * @brief   Voltage Reference Set Mode Disable
 *
 * @details Places the Voltage Reference device into Disabled mode.
 *
 * @param[in]   vref    pointer to Voltage Reference device.
 *
 * @return      void
 */
void vref_set_mode_disable(vref_dev_t *vref);

/**
 * @brief   Voltage Reference Set Mode HIZ
 *
 * @details Places the Voltage Reference device into HI-Z mode.
 *
 * @param[in]   vref    pointer to Voltage Reference device.
 *
 * @return      void
 */
void vref_set_mode_hiz(vref_dev_t *vref);

/**
 * @brief   Voltage Reference Set Code
 *
 * @details Sets the code (level) of the Voltage Reference device.
 *
 * @param[in]   vref    pointer to Voltage Reference device.
 * @param[in]   code    code (level) to set.
 *
 * @return      void
 */
void vref_set_code(vref_dev_t *vref, uint16_t code);

/**
 * @brief   Voltage Reference Set Power Mode
 *
 * @details Sets the power mode of the Voltage Reference device.
 *
 * @param[in]   vref        pointer to Voltage Reference device.
 * @param[in]   pwr_mode    power mode to set.
 *
 * @return      void
 */
void vref_set_pwr_mode(vref_dev_t *vref, vref_pwr_mode_t pwr_mode);

#endif /* _VREF_DEV_H_ */
