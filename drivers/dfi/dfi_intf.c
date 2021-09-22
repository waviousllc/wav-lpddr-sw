/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <dfi/driver.h>
#include <kernel/io.h>

void dfi_set_init_complete_ovr_reg_if(dfi_reg_t *dfi_reg,
                                      bool override,
                                      uint8_t val)
{
    uint32_t reg_val = dfi_reg->DDR_DFI_STATUS_IF_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_STATUS_IF_CFG_SW_ACK_VAL, val);
    dfi_reg->DDR_DFI_STATUS_IF_CFG = reg_val;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_STATUS_IF_CFG_SW_ACK_OVR, override);
    dfi_reg->DDR_DFI_STATUS_IF_CFG = reg_val;
}

void dfi_set_init_start_ovr_reg_if(dfi_reg_t *dfi_reg,
                                   bool override,
                                   uint8_t val)
{
    uint32_t reg_val = dfi_reg->DDR_DFI_STATUS_IF_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_STATUS_IF_CFG_SW_REQ_VAL, val);
    dfi_reg->DDR_DFI_STATUS_IF_CFG = reg_val;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_STATUS_IF_CFG_SW_REQ_OVR, override);
    dfi_reg->DDR_DFI_STATUS_IF_CFG = reg_val;
}

uint8_t dfi_get_init_start_status_reg_if(dfi_reg_t *dfi_reg)
{
    return GET_REG_FIELD(dfi_reg->DDR_DFI_STATUS_IF_STA, DDR_DFI_STATUS_IF_STA_REQ);
}

uint8_t dfi_get_init_complete_status_reg_if(dfi_reg_t *dfi_reg)
{
    return GET_REG_FIELD(dfi_reg->DDR_DFI_STATUS_IF_STA, DDR_DFI_STATUS_IF_STA_ACK);
}

void dfi_phymstr_req_assert_reg_if(dfi_reg_t *dfi_reg,
                                   dfi_phymstr_req_t *req)
{
    uint32_t reg_val = dfi_reg->DDR_DFI_PHYMSTR_IF_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYMSTR_IF_CFG_SW_TYPE, req->type);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYMSTR_IF_CFG_SW_CS_STATE, req->cs_state);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYMSTR_IF_CFG_SW_STATE_SEL, req->state_sel);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYMSTR_IF_CFG_SW_REQ_OVR, 0x1);
    dfi_reg->DDR_DFI_PHYMSTR_IF_CFG = reg_val;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYMSTR_IF_CFG_SW_REQ_VAL, 0x1);
    dfi_reg->DDR_DFI_PHYMSTR_IF_CFG = reg_val;
}

void dfi_phymstr_req_deassert_reg_if(dfi_reg_t *dfi_reg)
{
    uint32_t reg_val = dfi_reg->DDR_DFI_PHYMSTR_IF_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYMSTR_IF_CFG_SW_REQ_VAL, 0x0);
    dfi_reg->DDR_DFI_PHYMSTR_IF_CFG = reg_val;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYMSTR_IF_CFG_SW_EVENT_OVR, 0x1);
    dfi_reg->DDR_DFI_PHYMSTR_IF_CFG = reg_val;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYMSTR_IF_CFG_SW_EVENT_VAL, 0x1);
    dfi_reg->DDR_DFI_PHYMSTR_IF_CFG = reg_val;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYMSTR_IF_CFG_SW_REQ_OVR, 0x0);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYMSTR_IF_CFG_SW_TYPE, 0x0);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYMSTR_IF_CFG_SW_CS_STATE, 0x0);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYMSTR_IF_CFG_SW_STATE_SEL, 0x0);
    dfi_reg->DDR_DFI_PHYMSTR_IF_CFG = reg_val;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYMSTR_IF_CFG_SW_EVENT_VAL, 0x0);
    dfi_reg->DDR_DFI_PHYMSTR_IF_CFG = reg_val;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYMSTR_IF_CFG_SW_EVENT_OVR, 0x0);
    dfi_reg->DDR_DFI_PHYMSTR_IF_CFG = reg_val;

    // Ensure PHYMSTR ACK from MC is low before proceeding
    do
    {
        reg_val = dfi_reg->DDR_DFI_PHYMSTR_IF_STA;
    } while (GET_REG_FIELD(reg_val, DDR_DFI_PHYMSTR_IF_STA_ACK) != 0x0);
}

void dfi_ctrlupd_deassert_ack_reg_if(dfi_reg_t *dfi_reg)
{
    // Deassert CTRLUPD ACK
    uint32_t reg_val = dfi_reg->DDR_DFI_CTRLUPD_IF_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_CTRLUPD_IF_CFG_SW_ACK_VAL, 0x0);
    dfi_reg->DDR_DFI_CTRLUPD_IF_CFG = reg_val;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_CTRLUPD_IF_CFG_SW_EVENT_1_OVR, 0x1);
    dfi_reg->DDR_DFI_CTRLUPD_IF_CFG = reg_val;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_CTRLUPD_IF_CFG_SW_EVENT_1_VAL, 0x1);
    dfi_reg->DDR_DFI_CTRLUPD_IF_CFG = reg_val;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_CTRLUPD_IF_CFG_SW_ACK_OVR, 0x1);
    dfi_reg->DDR_DFI_CTRLUPD_IF_CFG = reg_val;

    do
    {
        reg_val = dfi_reg->DDR_DFI_CTRLUPD_IF_STA;
    } while (GET_REG_FIELD(reg_val, DDR_DFI_CTRLUPD_IF_STA_REQ) != 0x0);

    reg_val = dfi_reg->DDR_DFI_CTRLUPD_IF_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_CTRLUPD_IF_CFG_SW_EVENT_1_VAL, 0x0);
    dfi_reg->DDR_DFI_CTRLUPD_IF_CFG = reg_val;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_CTRLUPD_IF_CFG_SW_EVENT_1_OVR, 0x0);
    dfi_reg->DDR_DFI_CTRLUPD_IF_CFG = reg_val;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_CTRLUPD_IF_CFG_SW_ACK_OVR, 0x0);
    dfi_reg->DDR_DFI_CTRLUPD_IF_CFG = reg_val;
}

void dfi_ctrlupd_ack_override_reg_if(dfi_reg_t *dfi_reg,
                                     bool override,
                                     uint8_t val)
{
    uint32_t reg_val = dfi_reg->DDR_DFI_CTRLUPD_IF_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_CTRLUPD_IF_CFG_SW_ACK_VAL, val);
    dfi_reg->DDR_DFI_CTRLUPD_IF_CFG = reg_val;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_CTRLUPD_IF_CFG_SW_ACK_OVR, override);
    dfi_reg->DDR_DFI_CTRLUPD_IF_CFG = reg_val;
}

void dfi_phyupd_req_assert_reg_if(dfi_reg_t *dfi_reg,
                                  dfi_phyupd_type_t type)
{
    // Assert PHYUPD REQ and set type
    uint32_t reg_val = dfi_reg->DDR_DFI_PHYUPD_IF_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYUPD_IF_CFG_SW_TYPE, type);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYUPD_IF_CFG_SW_REQ_OVR, 0x1);
    dfi_reg->DDR_DFI_PHYUPD_IF_CFG = reg_val;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYUPD_IF_CFG_SW_REQ_VAL, 0x1);
    dfi_reg->DDR_DFI_PHYUPD_IF_CFG = reg_val;
}

void dfi_phyupd_req_deassert_reg_if(dfi_reg_t *dfi_reg)
{
    // Deassert PHYUPD REQ
    uint32_t reg_val = dfi_reg->DDR_DFI_PHYUPD_IF_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYUPD_IF_CFG_SW_REQ_VAL, 0x0);
    dfi_reg->DDR_DFI_PHYUPD_IF_CFG = reg_val;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYUPD_IF_CFG_SW_EVENT_OVR, 0x1);
    dfi_reg->DDR_DFI_PHYUPD_IF_CFG = reg_val;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYUPD_IF_CFG_SW_EVENT_VAL, 0x1);
    dfi_reg->DDR_DFI_PHYUPD_IF_CFG = reg_val;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYUPD_IF_CFG_SW_REQ_OVR, 0x0);
    dfi_reg->DDR_DFI_PHYUPD_IF_CFG = reg_val;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYUPD_IF_CFG_SW_EVENT_VAL, 0x0);
    dfi_reg->DDR_DFI_PHYUPD_IF_CFG = reg_val;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYUPD_IF_CFG_SW_EVENT_OVR, 0x0);
    dfi_reg->DDR_DFI_PHYUPD_IF_CFG = reg_val;

    // Ensure PHYUPD ACK from MC is low before proceeding
    do
    {
        reg_val = dfi_reg->DDR_DFI_PHYUPD_IF_STA;
    } while (GET_REG_FIELD(reg_val, DDR_DFI_PHYUPD_IF_STA_ACK) != 0x0);
}
