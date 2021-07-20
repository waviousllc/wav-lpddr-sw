/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <receiver/device.h>
#include <receiver/driver.h>
#include <receiver/delay_driver.h>

void receiver_init(receiver_dev_t *receiver, uint32_t base, wddr_rank_t rank)
{
    receiver_init_reg_if(receiver, base, rank);
    receiver_delay_init_reg_if(&receiver->rx_delay, base, rank);
}

void receiver_enable(receiver_dev_t  *receiver)
{
    receiver_set_state_reg_if(receiver, REC_STATE_ENABLE);
}

void receiver_disable(receiver_dev_t  *receiver)
{
    receiver_set_state_reg_if(receiver, REC_STATE_DISABLE);
}

wddr_return_t receiver_calibrate(receiver_dev_t *receiver, receiver_common_cal_t *cal)
{
    return receiver_calibrate_reg_if(receiver, cal->cal_code);
}

void receiver_set_delay(receiver_dev_t *receiver,
                        uint16_t delay,
                        receiver_tc_side_mask_t side)
{
    receiver_delay_set_delay_code_reg_if(&receiver->rx_delay, delay, side);
}
