/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DQ_BSCAN_H_
#define _DQ_BSCAN_H_

#include <stdbool.h>
#include <dq_reg.h>

/*******************************************************************************
**                            DQ
*******************************************************************************/
/**
 * @brief   DQ DQ BSCAN Get Status Register Interface
 *
 * @details Gets the DQ slice BSCAN status.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[out]  status  pointer to where to store bscan status.
 *
 * @return  void.
 */
void dq_dq_bscan_get_status_reg_if(dq_reg_t *dq_reg, uint8_t *status);

/*******************************************************************************
**                            DQS
*******************************************************************************/
/**
 * @brief   DQ DQS BSCAN Get Status Register Interface
 *
 * @details Gets the DQS slice BSCAN status.
 *
 * @param[in]   dq_reg  pointer to DQ Byte register space.
 * @param[out]  status  pointer to where to store bscan status.
 *
 * @return  void.
 */
void dq_dqs_bscan_get_status_reg_if(dq_reg_t *dq_reg, uint8_t *status);

#endif /* _DQ_BSCAN_H_ */
