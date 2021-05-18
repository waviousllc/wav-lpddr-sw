/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _GEARBOX_TX_DRIVER_H_
#define _GEARBOX_TX_DRIVER_H_

#include <gearbox/tx_table.h>
#include <gearbox/tx_device.h>
#include <wddr/phy_defs.h>

/**
 * @brief   Transmit (TX) Gearbox Initialization Register Interface
 *
 * @details Initializes TX Gearbox device at register interface level.
 *
 * @param[in]   gb          pointer to TX Gearbox device to initialize.
 * @param[in]   base        base address of TX Gearbox device.
 * @param[in]   slice_type  slice type of TX Gearbox to initialize.
 *
 * @return      void
 */
void tx_gb_init_reg_if(tx_gearbox_dev_t *gb,
                       uint32_t base,
                       wddr_slice_type_t slice_type);

/**
 * @brief   Transmit (TX) Gearbox Set Mode Set Register (MSR) Register Interface
 *
 * @details Configures TX Gearbox device for given mode set register value.
 *
 * @param[in]   gb          pointer to TX Gearbox device.
 * @param[in]   msr         MSR value to configure.
 * @param[in]   slice_type  slice type of TX Gearbox.
 *
 * @return      void
 */
void tx_gb_set_msr_reg_if(tx_gearbox_dev_t *gb,
                          wddr_msr_t msr,
                          wddr_slice_type_t slice_type);

/**
 * @brief   Transmit (TX) Gearbox Set Mode Register Interface
 *
 * @details Configures TX Gearbox device for given mode via CSR.
 *
 * @param[in]   gb          pointer to TX Gearbox device.
 * @param[in]   slice_type  slice type of TX Gearbox.
 * @param[in]   cfg         pointer to TX gearbox configuration.
 *
 * @return      void
 */
void tx_gb_set_mode_reg_if(tx_gearbox_dev_t *gb,
                           wddr_slice_type_t slice_type,
                           tx_gb_cfg_t *cfg);

#endif /* _GEARBOX_TX_DRIVER_H_ */
