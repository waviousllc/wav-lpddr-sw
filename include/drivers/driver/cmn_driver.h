/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _DRIVER_CMN_DRIVER_H_
#define _DRIVER_CMN_DRIVER_H_

#include <driver/device.h>

/**
 * @brief   Driver Common Device Initalization Register Interface
 *
 * @details Initializes Driver Common device at register interface level.
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
void driver_cmn_init_reg_if(driver_cmn_dev_t *driver,
                            uint32_t base,
                            wddr_slice_type_t slice_type,
                            wddr_rank_t rank);
/**
 * @brief   Driver Common Device Set MSR Register Interface
 *
 * @details Configures Driver Common device for given Mode Set Register.
 *
 * @param[in]   driver      pointer to Driver Common device.
 * @param[in]   msr         mode set register value to configure for.
 * @param[in]   slice_type  type of slice associated with the Driver Common
 *                          device. (DQS or CK).
 * @param[in]   rank        rank of Driver Common device.
 *
 * @return      void
 */
void driver_cmn_set_msr_reg_if(driver_cmn_dev_t *driver,
                               wddr_msr_t msr,
                               wddr_slice_type_t slice_type,
                               wddr_rank_t rank);

/**
 * @brief   Driver Common Device Set Mode Register Interface
 *
 * @details Sets the mode of the Driver Common device via CSR.
 *
 * @param[in]   driver  pointer to Driver Common device.
 * @param[in]   mode    mode to set.
 *
 * @return      returns whether mode was set successfully.
 * @retval      WDDR_SUCCESS if mode was set.
 * @retval      WDDR_ERROR otherwise.
 */
wddr_return_t driver_cmn_set_mode_reg_if(driver_cmn_dev_t *driver,
                                         driver_mode_t mode);

/**
 * @brief   Diver Common Device Set Code Register Interface
 *
 * @details Sets the codes of the Driver Common device via CSR.
 *
 * @param[in]   driver  pointer to Driver Common device.
 * @param[in]   code    pointer to Driver Common codes.
 *
 * @return      void
 */
void driver_cmn_set_code_reg_if(driver_cmn_dev_t *driver,
                                uint8_t code[DRVR_PN_CAL_NUM]);

/**
 * @brief   Driver Common Device Set Loopback Mode Register Interface
 *
 * @details Sets the loopback mode of the Driver Common device via CSR.
 *
 * @param[in]   driver  pointer to Driver Common device.
 * @param[in]   mode    loopback mode to set.
 *
 * @return      void
 */
void driver_cmn_set_loopback_reg_if(driver_cmn_dev_t *driver,
                                    driver_loopback_mode_t mode);

#endif /* _DRIVER_CMN_DRIVER_H_ */
