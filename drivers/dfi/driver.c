/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <dfi/driver.h>
#include <wddr/memory_map.h>
#include <kernel/io.h>

#define DFI_MODE__OFFSET    (0x4)

void dfi_set_msr_reg_if(dfi_dev_t *dfi, wddr_msr_t msr)
{
    dfi->msr = msr;
}

void dfi_rdd_cfg_reg_if(dfi_dev_t *dfi, dfi_pipe_cfg_t *cfg)
{
    uint32_t reg_val;

    reg_val = reg_read(dfi->base + DDR_DFICH_RDD_M0_CFG__ADR + dfi->msr * DFI_MODE__OFFSET);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_RDD_M0_CFG_GB_MODE, cfg->gb_mode);
    reg_write(dfi->base + DDR_DFICH_RDD_M0_CFG__ADR + dfi->msr * DFI_MODE__OFFSET, reg_val);
}

void dfi_wrd_cfg_reg_if(dfi_dev_t *dfi, dfi_pipe_cfg_t *cfg)
{
    uint32_t reg_val;

    reg_val = reg_read(dfi->base + DDR_DFICH_WRD_M0_CFG__ADR + dfi->msr * DFI_MODE__OFFSET);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WRD_M0_CFG_POST_GB_FC_DLY, cfg->post_gb_fc_dly);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WRD_M0_CFG_GB_MODE, cfg->gb_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WRD_M0_CFG_PIPE_EN, cfg->pipe_en);
    reg_write(dfi->base + DDR_DFICH_WRD_M0_CFG__ADR + dfi->msr * DFI_MODE__OFFSET, reg_val);
}

void dfi_ckctrl_cfg_reg_if(dfi_dev_t *dfi, dfi_pipe_cfg_t *cfg)
{
    uint32_t reg_val;

    reg_val = reg_read(dfi->base + DDR_DFICH_CKCTRL_M0_CFG__ADR + dfi->msr * DFI_MODE__OFFSET);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_CKCTRL_M0_CFG_POST_GB_FC_DLY, cfg->post_gb_fc_dly);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_CKCTRL_M0_CFG_GB_MODE, cfg->gb_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_CKCTRL_M0_CFG_PIPE_EN, cfg->pipe_en);
    reg_write(dfi->base + DDR_DFICH_CKCTRL_M0_CFG__ADR + dfi->msr * DFI_MODE__OFFSET, reg_val);
}

void dfi_wctrl_cfg_reg_if(dfi_dev_t *dfi, dfi_pipe_cfg_t *cfg)
{
    uint32_t reg_val;

    reg_val = reg_read(dfi->base + DDR_DFICH_WCTRL_M0_CFG__ADR + dfi->msr * DFI_MODE__OFFSET);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WCTRL_M0_CFG_POST_GB_FC_DLY, cfg->post_gb_fc_dly);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WCTRL_M0_CFG_GB_MODE, cfg->gb_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WCTRL_M0_CFG_PIPE_EN, cfg->pipe_en);
    reg_write(dfi->base + DDR_DFICH_WCTRL_M0_CFG__ADR + dfi->msr * DFI_MODE__OFFSET, reg_val);
}

void dfi_rctrl_cfg_reg_if(dfi_dev_t *dfi, dfi_pipe_cfg_t *cfg)
{
    uint32_t reg_val;

    reg_val = reg_read(dfi->base + DDR_DFICH_RCTRL_M0_CFG__ADR + dfi->msr * DFI_MODE__OFFSET);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_RCTRL_M0_CFG_POST_GB_FC_DLY, cfg->post_gb_fc_dly);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_RCTRL_M0_CFG_GB_MODE, cfg->gb_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_RCTRL_M0_CFG_PIPE_EN, cfg->pipe_en);
    reg_write(dfi->base + DDR_DFICH_RCTRL_M0_CFG__ADR + dfi->msr * DFI_MODE__OFFSET, reg_val);
}

void dfi_wckctrl_cfg_reg_if(dfi_dev_t *dfi, dfi_pipe_cfg_t *cfg)
{
    uint32_t reg_val;

    reg_val = reg_read(dfi->base + DDR_DFICH_WCKCTRL_M0_CFG__ADR + dfi->msr * DFI_MODE__OFFSET);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WCKCTRL_M0_CFG_POST_GB_FC_DLY, cfg->post_gb_fc_dly);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WCKCTRL_M0_CFG_GB_MODE, cfg->gb_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WCKCTRL_M0_CFG_PIPE_EN, cfg->pipe_en);
    reg_write(dfi->base + DDR_DFICH_WCKCTRL_M0_CFG__ADR + dfi->msr * DFI_MODE__OFFSET, reg_val);
}

void dfi_wrcctrl_cfg_reg_if(dfi_dev_t *dfi, dfi_pipe_cfg_t *cfg)
{
    uint32_t reg_val;

    reg_val = reg_read(dfi->base + DDR_DFICH_WRCCTRL_M0_CFG__ADR + dfi->msr * DFI_MODE__OFFSET);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WRCCTRL_M0_CFG_POST_GB_FC_DLY, cfg->post_gb_fc_dly);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WRCCTRL_M0_CFG_GB_MODE, cfg->gb_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WRCCTRL_M0_CFG_PIPE_EN, cfg->pipe_en);
    reg_write(dfi->base + DDR_DFICH_WRCCTRL_M0_CFG__ADR + dfi->msr * DFI_MODE__OFFSET, reg_val);
}

void dfi_wrc_cfg_reg_if(dfi_dev_t *dfi, dfi_pipe_cfg_t *cfg)
{
    uint32_t reg_val;

    reg_val = reg_read(dfi->base + DDR_DFICH_WRC_M0_CFG__ADR + dfi->msr * DFI_MODE__OFFSET);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WRC_M0_CFG_POST_GB_FC_DLY, cfg->post_gb_fc_dly);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WRC_M0_CFG_GB_MODE, cfg->gb_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WRC_M0_CFG_PIPE_EN, cfg->pipe_en);
    reg_write(dfi->base + DDR_DFICH_WRC_M0_CFG__ADR + dfi->msr * DFI_MODE__OFFSET, reg_val);
}

void dfi_wenctrl_cfg_reg_if(dfi_dev_t *dfi, dfi_pipe_cfg_t *cfg)
{
    uint32_t reg_val;

    reg_val = reg_read(dfi->base + DDR_DFICH_WENCTRL_M0_CFG__ADR + dfi->msr * DFI_MODE__OFFSET);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WENCTRL_M0_CFG_POST_GB_FC_DLY, cfg->post_gb_fc_dly);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WENCTRL_M0_CFG_GB_MODE, cfg->gb_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFICH_WENCTRL_M0_CFG_PIPE_EN, cfg->pipe_en);
    reg_write(dfi->base + DDR_DFICH_WENCTRL_M0_CFG__ADR + dfi->msr * DFI_MODE__OFFSET, reg_val);
}

void dfi_paden_pext_cfg_reg_if(dfi_dev_t *dfi, dfi_paden_pext_cfg_t *cfg)
{
    reg_write(dfi->base + DDR_DFICH_CTRL3_M0_CFG__ADR + dfi->msr * DFI_MODE__OFFSET, cfg->wrd.val);
    reg_write(dfi->base + DDR_DFICH_CTRL4_M0_CFG__ADR + dfi->msr * DFI_MODE__OFFSET, cfg->wck.val);
    reg_write(dfi->base + DDR_DFICH_CTRL5_M0_CFG__ADR + dfi->msr * DFI_MODE__OFFSET, cfg->rd.val);
}

void dfi_clken_pext_cfg_reg_if(dfi_dev_t *dfi, dfi_clken_pext_cfg_t *cfg)
{
    reg_write(dfi->base + DDR_DFICH_CTRL2_M0_CFG__ADR + dfi->msr * DFI_MODE__OFFSET, cfg->val);
}

void dfi_ovr_traffic_cfg_reg_if(dfi_dev_t *dfi, dfi_ovr_traffic_cfg_t *cfg)
{
    reg_write(dfi->base + DDR_DFICH_CTRL1_M0_CFG__ADR + dfi->msr * DFI_MODE__OFFSET, cfg->val);
}