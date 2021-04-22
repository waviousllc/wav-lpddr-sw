/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#include <path/common.h>
#include <ibias/device.h>
#include <pmon/device.h>
#include <zqcal/device.h>
#include <vref/device.h>
#include <vref/driver.h>

void common_path_init(common_path_t *cmn_path, uint32_t base)
{
    ibias_init(&cmn_path->ibias, base);
    pmon_init(&cmn_path->pmon, base);
    vref_init(&cmn_path->vref, base);
    zqcal_init(&cmn_path->zqcal, base);
}

void common_path_enable(common_path_t *cmn_path)
{
    for (int8_t msr = WDDR_MSR_1; msr >= WDDR_MSR_0; msr--)
    {
        vref_set_msr_reg_if(&cmn_path->vref, msr);
        vref_set_mode_enable(&cmn_path->vref);
    }

    ibias_enable(&cmn_path->ibias);
}
