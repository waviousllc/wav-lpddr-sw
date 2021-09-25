/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <dq/driver.h>

/*******************************************************************************
**                            DQ
*******************************************************************************/
void dq_dq_bscan_get_status_reg_if(dq_reg_t *dq_reg, uint8_t *status)
{
    *status = dq_reg->DDR_DQ_DQ_RX_BSCAN_STA;
}

/*******************************************************************************
**                            DQS
*******************************************************************************/
void dq_dqs_bscan_get_status_reg_if(dq_reg_t *dq_reg, uint8_t *status)
{
    *status = dq_reg->DDR_DQ_DQS_RX_BSCAN_STA;
}
