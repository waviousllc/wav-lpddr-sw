/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _DRIVER_DEV_H_
#define _DRIVER_DEV_H_
#include <stdint.h>
#include <stdbool.h>
#include <error.h>
#include <wddr/phy_defs.h>
#include <driver/table.h>

/**
 * @brief   Driver Common Device Structure
 *
 * @details Driver that is shared between DQ and DQS slices in the TX path.
 *
 * @note    Changes made using the Driver device affects both DQ and DQS.
 *
 * base     Base address of the Common Driver device.
 * mode     current mode of the Common Driver device.
 */
typedef struct driver_cmn_dev_t
{
    uint32_t base;
    driver_mode_t mode;
} driver_cmn_dev_t;

/**
 * @brief   Driver Device Structure
 *
 * @details Driver that controls per-bit transmission settings for a given slice.
 *
 * base     Base address of the Driver Device.
 */
typedef struct driver_dev_t
{
    uint32_t base;
} driver_dev_t;

/**
 * @brief   Driver Common Device Initalization
 *
 * @details Initializes Driver Common device at device level.
 *
 * @note    Only DQS or CK Slice Types are valid.
 *
 * @param[in]   driver      pointer to Driver Common device.
 * @param[in]   base        base address of the Driver Common device.
 * @param[in]   slice_type  type of slice associated with the Driver Common
 *                          device. (DQS or CK).
 * @param[in]   rank        rank of Driver Common device.
 *
 * @return      void
 */
void driver_cmn_init(driver_cmn_dev_t *driver,
                     uint32_t base,
                     wddr_slice_type_t slice_type,
                     wddr_rank_t rank);

/**
 * @brief   Driver Device Initalization
 *
 * @details Initializes Driver device at device level.
 *
 * @param[in]   driver      pointer to Driver device.
 * @param[in]   base        base address of the Driver device.
 * @param[in]   slice_type  type of slice associated with the Driver device.
 *
 * @return      returns whether Driver device was initialized successfully.
 * @retval      WDDR_SUCCESS if initialized successfully.
 * @retval      WDDR_ERROR otherwise.
 */
void driver_init(driver_dev_t *driver,
                 uint32_t base,
                 wddr_slice_type_t slice_type);

/**
 * @brief   Driver Set Impedance All Bits
 *
 * @details Sets the Driver device impedance for all bits for the given slice.
 *
 * @param[in]   driver      pointer to Driver device.
 * @param[in]   slice_type  slice type of Driver device.
 * @param[in]   tx          tx impedance to set.
 * @param[in]   rx          rx impedance to set.
 *
 * @return      void
 */
void driver_set_impedance_all_bits(driver_dev_t *driver,
                                   wddr_slice_type_t slice_type,
                                   driver_impedance_t tx,
                                   driver_impedance_t rx);

/**
 * @brief   Driver Override
 *
 * @details Overrides the Driver device output for a given bit within a given
 *          slice.
 *
 * @param[in]   driver      pointer to Driver device.
 * @param[in]   cfg         pointer to Driver device configuration.
 * @param[in]   slice_type  type of slice to configure.
 * @param[in]   bit_index   which bit to configure.
 * @param[in]   override    flag to indicate if sw override is enabled.
 *
 * @return      void
 */
void driver_override(driver_dev_t *driver,
                     driver_cfg_t *cfg,
                     wddr_slice_type_t slice_type,
                     uint8_t bit_index,
                     bool override);

#endif /* _DRIVER_DEV_H_ */
