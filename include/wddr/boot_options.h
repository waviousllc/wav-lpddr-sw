/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _WDDR_BOOT_OPTIONS_H_
#define _WDDR_BOOT_OPTIONS_H_

#include <stdint.h>
#include <kernel/io.h>

#define SET_BOOT_OPTION(option_name, option_val)            \
    (SET_REG_FIELD(option_name, option_val))

#define GET_BOOT_OPTION(option_name, option_val)            \
    (GET_REG_FIELD(option_name, option_val))

#define UPDATE_BOOT_OPTION(cfg, option_name, option_val)    \
    (UPDATE_REG_FIELD(cfg, option_name, option_val))

#define WDDR_BOOT_OPTION_PLL_CAL__MSK           (0x00000001)
#define WDDR_BOOT_OPTION_PLL_CAL__SHFT          (0x00000000)
#define WDDR_BOOT_OPTION_ZQCAL_CAL__MSK         (0x00000002)
#define WDDR_BOOT_OPTION_ZQCAL_CAL__SHFT        (0x00000001)
#define WDDR_BOOT_OPTION_SA_CAL__MSK            (0x00000004)
#define WDDR_BOOT_OPTION_SA_CAL__SHFT           (0x00000002)
#define WDDR_BOOT_OPTION_TRAIN_DRAM__MSK        (0x00000008)
#define WDDR_BOOT_OPTION_TRAIN_DRAM__SHFT       (0x00000003)

#define WDDR_BOOT_CONFIG_NONE                   (0x00000000)

/**
 * @brief   WDDR Boot Configuration Type
 *
 * @details Type used to represent boot options mask. The following macros
 *          should be used:
 *
 *              SET_BOOT_OPTION
 *              GET_BOOT_OPTION
 *              UPDATE_BOOT_OPTION
 *
 *          The option definitions above are used
 *
 *          // Enable PLL calibration
 *          cfg = SET_REG_FIELD(WDDR_BOOT_OPTION_PLL_CAL, 0x1);
 *
 *          .....
 *
 *          if (GET_REG_FIELD(WDDR_BOOT_OPTION_PLL_CAL))
 *          {
 *              // perform calibration
 *          }
 */
typedef uint32_t wddr_boot_cfg_t;

#endif /* _WDDR_BOOT_OPTIONS_H_ */
