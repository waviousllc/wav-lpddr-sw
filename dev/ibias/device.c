/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#include <ibias/device.h>
#include <ibias/driver.h>

void ibias_init(ibias_dev_t *ibias, uint32_t base)
{
    ibias_init_reg_if(ibias, base);
}

wddr_return_t ibias_enable(ibias_dev_t *ibias)
{
    return ibias_set_state_reg_if(ibias, IBIAS_STATE_ENABLE);
}

wddr_return_t ibias_disable(ibias_dev_t *ibias)
{
    return ibias_set_state_reg_if(ibias, IBIAS_STATE_DISABLE);
}
