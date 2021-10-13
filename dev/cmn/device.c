/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <cmn/device.h>
#include <wddr/memory_map.h>

#define ZQCAL_PCAL_CODE_MIN  (0)
#define ZQCAL_PCAL_CODE_MAX  (0x3f)
#define ZQCAL_NCAL_CODE_MIN  (0)
#define ZQCAL_NCAL_CODE_MAX  (0x1f)

void cmn_init(cmn_dev_t *cmn_dev, uint32_t base)
{
    cmn_dev->cmn_reg = (cmn_reg_t *)(base + WDDR_MEMORY_MAP_CMN);
}

void cmn_enable(cmn_dev_t *cmn_dev)
{
    cmn_vref_set_mode_enable(cmn_dev, WDDR_MSR_0);
    cmn_vref_set_mode_enable(cmn_dev, WDDR_MSR_1);
    cmn_ibias_enable(cmn_dev);
}

void cmn_ibias_enable(cmn_dev_t *cmn_dev)
{
    cmn_ibias_set_state_reg_if(cmn_dev->cmn_reg, IBIAS_STATE_ENABLE);
}

void cmn_ibias_disable(cmn_dev_t *cmn_dev)
{
    cmn_ibias_set_state_reg_if(cmn_dev->cmn_reg, IBIAS_STATE_DISABLE);
}

void cmn_pmon_run(cmn_dev_t *cmn_dev, uint32_t *count)
{
    cmn_pmon_set_state_reg_if(cmn_dev->cmn_reg, PMON_STATE_ENABLED);
    cmn_pmon_get_status_count_reg_if(cmn_dev->cmn_reg, count);
    cmn_pmon_set_state_reg_if(cmn_dev->cmn_reg, PMON_STATE_DISABLED);
}

void cmn_pmon_configure(cmn_dev_t *cmn_dev, pmon_cfg_t *cfg)
{
    cmn_pmon_configure_reg_if(cmn_dev->cmn_reg, cfg->refclk_count, cfg->init_wait);
}

void cmn_vref_set_mode_enable(cmn_dev_t *cmn_dev, wddr_msr_t msr)
{
    cmn_vref_set_state_reg_if(cmn_dev->cmn_reg, msr, VREF_STATE_ENABLED);
}

void cmn_vref_set_mode_disable(cmn_dev_t *cmn_dev, wddr_msr_t msr)
{
    cmn_vref_set_state_reg_if(cmn_dev->cmn_reg, msr, VREF_STATE_DISABLED);
}

void cmn_vref_set_mode_hiz(cmn_dev_t *cmn_dev, wddr_msr_t msr)
{
    cmn_vref_set_state_reg_if(cmn_dev->cmn_reg, msr, VREF_STATE_HIZ);
}

static void zqcal_calibrate_common(cmn_dev_t *cmn_dev,
                                   zqcal_mode_t mode,
                                   uint8_t max_code,
                                   uint8_t *code)
{
    uint8_t zqval;
    uint8_t tmp_code;

    tmp_code = *code;
    cmn_zqcal_set_mode_reg_if(cmn_dev->cmn_reg, mode);
    do
    {
        cmn_zqcal_set_code_reg_if(cmn_dev->cmn_reg, mode, tmp_code);
        // TODO: need wait for it to settle?
        cmn_zqcal_get_output_reg_if(cmn_dev->cmn_reg, &zqval);
        *code = tmp_code++;
    } while (zqval && tmp_code <= max_code);
}

static wddr_return_t zqcal_calibrate_voh(cmn_dev_t *cmn_dev,
                                         zqcal_voh_t voh,
                                         zqcal_cfg_t *cfg)
{
    uint8_t n_code = ZQCAL_NCAL_CODE_MIN;
    uint8_t p_code = ZQCAL_PCAL_CODE_MIN;

    cmn_zqcal_set_voh_reg_if(cmn_dev->cmn_reg, voh);

    zqcal_calibrate_common(cmn_dev,
                           ZQCAL_MODE_PULL_DOWN,
                           ZQCAL_NCAL_CODE_MAX,
                           &n_code);

    if (n_code == ZQCAL_NCAL_CODE_MIN)
    {
        return WDDR_ERROR;
    }

    if (n_code == ZQCAL_NCAL_CODE_MAX)
    {
        return WDDR_ERROR;
    }

    zqcal_calibrate_common(cmn_dev,
                           ZQCAL_MODE_PULL_UP,
                           ZQCAL_PCAL_CODE_MAX,
                           &p_code);

    if (p_code == ZQCAL_PCAL_CODE_MIN)
    {
        return WDDR_ERROR;
    }

    if (p_code == ZQCAL_PCAL_CODE_MAX)
    {
        return WDDR_ERROR;
    }

    cfg->code[voh][ZQCAL_N_CAL] = n_code;
    cfg->code[voh][ZQCAL_P_CAL] = p_code;
    return WDDR_SUCCESS;
}

wddr_return_t cmn_zqcal_calibrate(cmn_dev_t *cmn_dev, zqcal_cfg_t *cfg)
{
    wddr_return_t ret;
    cmn_zqcal_set_state_reg_if(cmn_dev->cmn_reg, ZQCAL_STATE_ENABLED);
    for (uint8_t voh = ZQCAL_VOH_0P5; voh < ZQCAL_VOH_NUM; voh++)
    {
        ret = zqcal_calibrate_voh(cmn_dev, (zqcal_voh_t) voh, cfg);
        if (ret)
        {
            break;
        }
    }
    cmn_zqcal_set_state_reg_if(cmn_dev->cmn_reg, ZQCAL_STATE_DISABLED);
    return ret;
}
