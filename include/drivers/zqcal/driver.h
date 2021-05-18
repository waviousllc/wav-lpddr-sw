/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _ZQCAL_DRIVER_H_
#define _ZQCAL_DRIVER_H_

#include <zqcal/device.h>

/**
 * @brief   ZQCAL Initialization Register Interface
 *
 * @details Initailizes ZQCAL Device at driver level.
 *
 * @param[in]   zqcal   pointer to ZQCAL device.
 * @param[in]   base    base address of the ZQCAL device.
 *
 * @return      void
 */
void zqcal_init_reg_if(zqcal_dev_t *zqcal, uint32_t base);

/**
 * @brief   ZQCAL Set State Register Interface
 *
 * @details Sets the state of the ZQCAL device via CSR.
 *
 * @param[in]   zqcal   pointer to ZQCAL device.
 * @param[in]   state   state to set.
 *
 * @return      returns whether state was set successfully.
 * @retval      WDDR_SUCCESS if state was set.
 * @retval      WDDR_ERROR otherwise.
 */
wddr_return_t zqcal_set_state_reg_if(zqcal_dev_t *zqcal, zqcal_state_t state);

/**
 * @brief   ZQCAL Set Mode Register Interface
 *
 * @details Sets the mode of the ZQCAL device via CSR.
 *
 * @param[in]   zqcal   pointer to ZQCAL device.
 * @param[in]   mode    mode to set.
 *
 * @return      returns whether mode was set successfully.
 * @retval      WDDR_SUCCESS if mode was set.
 * @retval      WDDR_ERROR otherwise.
 */
wddr_return_t zqcal_set_mode_reg_if(zqcal_dev_t *zqcal, zqcal_mode_t mode);

/**
 * @brief   ZQCAL Set Code Register Interface
 *
 * @details Sets the code of the ZQCAL device via CSR.
 *
 * @param[in]   zqcal   pointer to ZQCAL device.
 * @param[in]   mode    mode of device. Determines if P or N code is set.
 * @param[in]   code    code to set.
 *
 * @return      returns whether code was set successfully.
 * @retval      WDDR_SUCCESS if valid mode given and code is set.
 * @retval      WDDR_ERROR otherwise.
 */
wddr_return_t zqcal_set_code_reg_if(zqcal_dev_t *zqcal, zqcal_mode_t mode, uint8_t code);

/**
 * @brief   ZQCAL Set VOH Register Interface
 *
 * @details Sets the VOH of the ZQCAL device via CSR.
 *
 * @param[in]   zqcal   pointer to ZQCAL device.
 * @param[in]   voh     voh to set.
 *
 * @return      returns whether voh was set successfully.
 * @retval      WDDR_SUCCESS if voh was set.
 * @retval      WDDR_ERROR otherwise.
 */
wddr_return_t zqcal_set_voh_reg_if(zqcal_dev_t *zqcal, zqcal_voh_t voh);

/**
 * @brief   ZQCAL Get Comparator Ouptut Register Interface
 *
 * @details Gets the compartor output result of the ZQCAL device via CSR.
 *
 * @param[in]   zqcal   pointer to ZQCAL device.
 * @param[out]  val     pointer of where to store comparator value.
 *
 * @return      void
 */
void zqcal_get_output_reg_if(zqcal_dev_t *zqcal, uint8_t *val);

#endif /* ZQCAL_DRIVER_H_ */
