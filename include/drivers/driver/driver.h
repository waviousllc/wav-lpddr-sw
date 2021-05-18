/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _DRIVER_DRIVER_H_
#define _DRIVER_DRIVER_H_

#include <driver/device.h>

/**
 * @brief   Driver Device Initalization Register Interface
 *
 * @details Initializes Driver device at register interface level.
 *
 * @param[in]   driver      pointer to Driver device.
 * @param[in]   base        base address of the Driver device.
 * @param[in]   slice_type  type of slice associated with the Driver device.
 *
 * @return      void
 */
void driver_init_reg_if(driver_dev_t *driver,
                        uint32_t base,
                        wddr_slice_type_t slice_type);

/**
 * @brief   Driver Device Set MSR Register Interface
 *
 * @details Configures Driver device for given Mode Set Register.
 *
 * @param[in]   driver      pointer to Driver device.
 * @param[in]   msr         mode set register value to configure for.
 * @param[in]   slice_type  type of slice associated with the Driver device.
 *
 * @return      void
 */
void driver_set_msr_reg_if(driver_dev_t *driver,
                           wddr_msr_t msr,
                           wddr_slice_type_t slice_type);

/**
 * @brief   Driver Set Impedance Register Interface
 *
 * @details Sets the Driver device impedance via CSR for the given bit and
 *          slice.
 *
 * @param[in]   driver      pointer to Driver device.
 * @param[in]   slice_type  slice type of Driver device.
 * @param[in]   bit_index   bit to set impedance for.
 * @param[in]   tx          tx impedance to set.
 * @param[in]   rx          rx impedance to set.
 *
 * @return      void
 */
void driver_set_impedance_reg_if(driver_dev_t *driver,
                                 wddr_slice_type_t slice_type,
                                 uint8_t bit_index,
                                 driver_impedance_t tx,
                                 driver_impedance_t rx);

/**
 * @brief   Diver Set Output Enable Register Interface
 *
 * @details Sets the Driver device Output Enable via CSR for the given bit and
 *          slice.
 *
 * @param[in]   driver      pointer to Driver device.
 * @param[in]   slice_type  slice type of Driver device.
 * @param[in]   bit_index   bit to set OE for.
 * @param[in]   enable      flag to indicate if output is enabled.
 *
 * @return      void
 */
void driver_set_oe_reg_if(driver_dev_t *driver,
                          wddr_slice_type_t slice_type,
                          uint8_t bit_index,
                          bool enable);

/**
 * @brief   Driver Set Override Register Interface
 *
 * @details Sets the Override of the Driver via CSR for given bit and
 *          slice.

 * @param[in]   driver      pointer to Driver device.
 * @param[in]   slice_type  slice type of Driver device.
 * @param[in]   bit_index   bit to set OE for.
 * @param[in]   sel         software override selection value.
 * @param[in]   val_t       value for True (T) side.
 * @param[in]   val_c       value for Complementary (C) side.
 *
 * @return      void
 */
void driver_set_override_reg_if(driver_dev_t *driver,
                                wddr_slice_type_t slice_type,
                                uint8_t bit_index,
                                uint8_t sel,
                                uint8_t val_t,
                                uint8_t val_c);

#endif /* _DRIVER_DRIVER_H_ */
