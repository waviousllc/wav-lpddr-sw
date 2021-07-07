/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#include <pmon/device.h>
#include <pmon/driver.h>

void pmon_init(pmon_dev_t *pmon, uint32_t base)
{
    pmon_init_reg_if(pmon, base);
}

void pmon_run(pmon_dev_t *pmon, uint32_t *count)
{
    pmon_set_state_reg_if(pmon, PMON_STATE_ENABLED);
    pmon_get_status_count_reg_if(pmon, count);
    pmon_set_state_reg_if(pmon, PMON_STATE_DISABLED);
}

void pmon_configure(pmon_dev_t *pmon, pmon_cfg_t *cfg)
{
    pmon_configure_reg_if(pmon, cfg->refclk_count, cfg->init_wait);
}
