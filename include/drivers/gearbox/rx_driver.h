/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _GEARBOX_RX_DRIVER_H_
#define _GEARBOX_RX_DRIVER_H_

#include <gearbox/rx_table.h>
#include <gearbox/rx_device.h>
#include <wddr/phy_defs.h>

/**
 * @brief   Receive (RX) Gearbox Initialization Register Interface
 *
 * @details Initializes RX Gearbox device at register interface level.
 *
 * @note    Slice type should be given as CA or DQ, which implies CK and
 *          DQS, respectively.
 *
 * @param[in]   gb          pointer to RX Gearbox device to initialize.
 * @param[in]   base        base address of RX Gearbox device.
 * @param[in]   slice_type  slice type of RX Gearbox to initialize.
 *
 * @return      void
 */
void rx_gb_init_reg_if(rx_gearbox_dev_t *gb,
                       uint32_t base,
                       wddr_slice_type_t slice_type);

/**
 * @brief   Receive (RX) Gearbox Set Mode Set Register (MSR) Register Interface
 *
 * @details Configures RX Gearbox device for given mode set register value.
 *
 * @note    Slice type should be given as CA or DQ, which implies CK and
 *          DQS, respectively.
 *
 * @param[in]   gb          pointer to RX Gearbox device.
 * @param[in]   msr         MSR value to configure.
 * @param[in]   slice_type  slice type of RX Gearbox.
 *
 * @return      void
 */
void rx_gb_set_msr_reg_if(rx_gearbox_dev_t *gb,
                          wddr_msr_t msr,
                          wddr_slice_type_t slice_type);

/**
 * @brief   Receive (RX) Gearbox Set Mode Register Interface
 *
 * @details Configures RX Gearbox device for given mode via CSR.
 *
 * @note    Slice type should be given as CA or DQ, which implies CK and
 *          DQS, respectively.
 *
 * @param[in]   gb          pointer to RX Gearbox device.
 * @param[in]   slice_type  slice type of RX Gearbox.
 * @param[in]   cfg         pointer to RX gearbox configuration.
 *
 * @return      void
 */
void rx_gb_set_mode_reg_if(rx_gearbox_dev_t *gb,
                           wddr_slice_type_t slice_type,
                           rx_gb_cfg_t *cfg);

#endif /* _GEARBOX_RX_DRIVER_H_ */
