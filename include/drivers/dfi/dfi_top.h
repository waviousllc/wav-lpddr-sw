/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DFI_TOP_H_
#define _DFI_TOP_H_

#include "dfi_reg.h"

/**
 * @brief   DFI Set CA Loopback Select Register Interface
 *
 * @details Selects which channel is used for CA loopback.
 *
 *
 * @param[in]   dfi_reg     pointer to DFI register space.
 * @param[in]   channel_sel which channel to select.
 *
 * @return      void.
 */
void dfi_set_ca_loopback_sel_reg_if(dfi_reg_t *dfi_reg,
                                    uint8_t channel_sel);

#endif /* _DFI_TOP_H_ */
