/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <ca/driver.h>

/*******************************************************************************
**                            CA
*******************************************************************************/
void ca_dq_bscan_get_status_reg_if(ca_reg_t *ca_reg, uint8_t *status)
{
    *status = ca_reg->DDR_CA_DQ_RX_BSCAN_STA;
}

/*******************************************************************************
**                            CK
*******************************************************************************/
void ca_dqs_bscan_get_status_reg_if(ca_reg_t *ca_reg, uint8_t *status)
{
    *status = ca_reg->DDR_CA_DQS_RX_BSCAN_STA;
}
