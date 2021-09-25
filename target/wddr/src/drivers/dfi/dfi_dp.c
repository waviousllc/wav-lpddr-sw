/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <dfi/driver.h>
#include <kernel/io.h>

void dfi_set_rdd_cfg_reg_if(dfich_reg_t *dfich_reg,
                            wddr_msr_t msr,
                            dfi_pipe_cfg_t *cfg)
{
    uint32_t reg_val = dfich_reg->DDR_DFICH_RDD_CFG[msr];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_RDD_M0_CFG_GB_MODE, cfg->gb_mode);
    dfich_reg->DDR_DFICH_RDD_CFG[msr] = reg_val;
}

void dfi_set_wrd_cfg_reg_if(dfich_reg_t *dfich_reg,
                            wddr_msr_t msr,
                            dfi_pipe_cfg_t *cfg)
{
    uint32_t reg_val = dfich_reg->DDR_DFICH_WRD_CFG[msr];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WRD_M0_CFG_POST_GB_FC_DLY, cfg->post_gb_fc_dly);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WRD_M0_CFG_GB_MODE, cfg->gb_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WRD_M0_CFG_PIPE_EN, cfg->pipe_en);
    dfich_reg->DDR_DFICH_WRD_CFG[msr] = reg_val;
}

void dfi_set_ckctrl_cfg_reg_if(dfich_reg_t *dfich_reg,
                               wddr_msr_t msr,
                               dfi_pipe_cfg_t *cfg)
{
    uint32_t reg_val = dfich_reg->DDR_DFICH_CKCTRL_CFG[msr];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_CKCTRL_M0_CFG_POST_GB_FC_DLY, cfg->post_gb_fc_dly);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_CKCTRL_M0_CFG_GB_MODE, cfg->gb_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_CKCTRL_M0_CFG_PIPE_EN, cfg->pipe_en);;
    dfich_reg->DDR_DFICH_CKCTRL_CFG[msr] = reg_val;
}

void dfi_set_wctrl_cfg_reg_if(dfich_reg_t *dfich_reg,
                              wddr_msr_t msr,
                              dfi_pipe_cfg_t *cfg)
{
    uint32_t reg_val = dfich_reg->DDR_DFICH_WCTRL_CFG[msr];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WCTRL_M0_CFG_POST_GB_FC_DLY, cfg->post_gb_fc_dly);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WCTRL_M0_CFG_GB_MODE, cfg->gb_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WCTRL_M0_CFG_PIPE_EN, cfg->pipe_en);;
    dfich_reg->DDR_DFICH_WCTRL_CFG[msr] = reg_val;
}

void dfi_set_rctrl_cfg_reg_if(dfich_reg_t *dfich_reg,
                              wddr_msr_t msr,
                              dfi_pipe_cfg_t *cfg)
{
    uint32_t reg_val = dfich_reg->DDR_DFICH_RCTRL_CFG[msr];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_RCTRL_M0_CFG_POST_GB_FC_DLY, cfg->post_gb_fc_dly);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_RCTRL_M0_CFG_GB_MODE, cfg->gb_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_RCTRL_M0_CFG_PIPE_EN, cfg->pipe_en);;
    dfich_reg->DDR_DFICH_RCTRL_CFG[msr] = reg_val;
}

void dfi_set_wckctrl_cfg_reg_if(dfich_reg_t *dfich_reg,
                                wddr_msr_t msr,
                                dfi_pipe_cfg_t *cfg)
{
    uint32_t reg_val = dfich_reg->DDR_DFICH_WCKCTRL_CFG[msr];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WCKCTRL_M0_CFG_POST_GB_FC_DLY, cfg->post_gb_fc_dly);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WCKCTRL_M0_CFG_GB_MODE, cfg->gb_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WCKCTRL_M0_CFG_PIPE_EN, cfg->pipe_en);;
    dfich_reg->DDR_DFICH_WCKCTRL_CFG[msr] = reg_val;
}

void dfi_set_wrcctrl_cfg_reg_if(dfich_reg_t *dfich_reg,
                                wddr_msr_t msr,
                                dfi_pipe_cfg_t *cfg)
{
    uint32_t reg_val = dfich_reg->DDR_DFICH_WRCCTRL_CFG[msr];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WRCCTRL_M0_CFG_POST_GB_FC_DLY, cfg->post_gb_fc_dly);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WRCCTRL_M0_CFG_GB_MODE, cfg->gb_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WRCCTRL_M0_CFG_PIPE_EN, cfg->pipe_en);;
    dfich_reg->DDR_DFICH_WRCCTRL_CFG[msr] = reg_val;
}

void dfi_set_wrc_cfg_reg_if(dfich_reg_t *dfich_reg,
                            wddr_msr_t msr,
                            dfi_pipe_cfg_t *cfg)
{
    uint32_t reg_val = dfich_reg->DDR_DFICH_WRC_CFG[msr];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WRC_M0_CFG_POST_GB_FC_DLY, cfg->post_gb_fc_dly);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WRC_M0_CFG_GB_MODE, cfg->gb_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WRC_M0_CFG_PIPE_EN, cfg->pipe_en);;
    dfich_reg->DDR_DFICH_WRC_CFG[msr] = reg_val;
}

void dfi_set_wenctrl_cfg_reg_if(dfich_reg_t *dfich_reg,
                                wddr_msr_t msr,
                                dfi_pipe_cfg_t *cfg)
{
    uint32_t reg_val = dfich_reg->DDR_DFICH_WENCTRL_CFG[msr];
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WENCTRL_M0_CFG_POST_GB_FC_DLY, cfg->post_gb_fc_dly);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WENCTRL_M0_CFG_GB_MODE, cfg->gb_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WENCTRL_M0_CFG_PIPE_EN, cfg->pipe_en);;
    dfich_reg->DDR_DFICH_WENCTRL_CFG[msr] = reg_val;
}

void dfi_set_paden_pext_cfg_reg_if(dfich_reg_t *dfich_reg,
                                   wddr_msr_t msr,
                                   dfi_paden_pext_cfg_t *cfg)
{
    dfich_reg->DDR_DFICH_CTRL3_CFG[msr] = cfg->wrd.val;
    dfich_reg->DDR_DFICH_CTRL4_CFG[msr] = cfg->wck.val;
    dfich_reg->DDR_DFICH_CTRL5_CFG[msr] = cfg->rd.val;
}

void dfi_set_clken_pext_cfg_reg_if(dfich_reg_t *dfich_reg,
                                   wddr_msr_t msr,
                                   dfi_clken_pext_cfg_t *cfg)
{
    dfich_reg->DDR_DFICH_CTRL2_CFG[msr] = cfg->val;
}

void dfi_set_ovr_traffic_cfg_reg_if(dfich_reg_t *dfich_reg,
                                    wddr_msr_t msr,
                                    dfi_ovr_traffic_cfg_t *cfg)
{
    dfich_reg->DDR_DFICH_CTRL1_CFG[msr] = cfg->val;
}
