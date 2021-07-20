/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <vref/device.h>
#include <vref/driver.h>

void vref_init(vref_dev_t *vref, uint32_t base)
{
    vref_init_reg_if(vref, base);
}

void vref_set_mode_enable(vref_dev_t *vref)
{
    vref_set_state_reg_if(vref, VREF_STATE_ENABLED);
}

void vref_set_mode_disable(vref_dev_t *vref)
{
    vref_set_state_reg_if(vref, VREF_STATE_DISABLED);
}

void vref_set_mode_hiz(vref_dev_t *vref)
{
    vref_set_state_reg_if(vref, VREF_STATE_HIZ);
}

void vref_set_code(vref_dev_t *vref, uint16_t code)
{
    vref_set_code_reg_if(vref, code);
}

void vref_set_pwr_mode(vref_dev_t *vref, vref_pwr_mode_t pwr_mode)
{
    vref_set_pwr_mode_reg_if(vref, pwr_mode);
}
