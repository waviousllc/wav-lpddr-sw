/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <dfi/driver.h>
#include <kernel/io.h>

void dfi_set_ca_loopback_sel_reg_if(dfi_reg_t *dfi_reg,
                                    uint8_t channel_sel)
{
    uint32_t reg_val = dfi_reg->DDR_DFI_TOP_0_CFG;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_TOP_0_CFG_CA_LPBK_SEL, channel_sel);
    dfi_reg->DDR_DFI_TOP_0_CFG = reg_val;
}
