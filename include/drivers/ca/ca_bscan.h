/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _CA_BSCAN_H_
#define _CA_BSCAN_H_

#include <stdbool.h>
#include "ca_reg.h"

/*******************************************************************************
**                            CA
*******************************************************************************/
/**
 * @brief   CA DQ BSCAN Get Status Register Interface
 *
 * @details Gets the CA slice BSCAN status.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[out]  status  pointer to where to store bscan status.
 *
 * @return  void.
 */
void ca_dq_bscan_get_status_reg_if(ca_reg_t *ca_reg, uint8_t *status);

/*******************************************************************************
**                            CK
*******************************************************************************/
/**
 * @brief   CA DQS BSCAN Get Status Register Interface
 *
 * @details Gets the CK slice BSCAN status.
 *
 * @param[in]   ca_reg  pointer to CA register space.
 * @param[out]  status  pointer to where to store bscan status.
 *
 * @return  void.
 */
void ca_dqs_bscan_get_status_reg_if(ca_reg_t *ca_reg, uint8_t *status);

#endif /* _CA_BSCAN_H_ */
