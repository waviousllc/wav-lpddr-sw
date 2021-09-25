/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _WDDR_PHY_CONFIG_H_
#define _WDDR_PHY_CONFIG_H_
#include "phy_defs.h"

// TODO: This should be generated by external script
#define WDDR_PHY_BOOT_FREQ                  (0)
#define WDDR_PHY_CFG                        (WDDR_PHY_CFG_DDR)
#define WDDR_PHY_RANK                       (2)
#define WDDR_PHY_DQ_SLICE_NUM               (9)
#define WDDR_PHY_DQS_SLICE_NUM              (DQS_SLICE_NUM)
#define WDDR_PHY_DQS_TXRX_SLICE_NUM         (2)
#define WDDR_PHY_CA_SLICE_NUM               (CA_SLICE_NUM)
#define WDDR_PHY_CK_SLICE_NUM               (1)
#define WDDR_PHY_CK_TXRX_SLICE_NUM          (1)
#define WDDR_PHY_DQ_BYTE_NUM                (WDDR_DQ_BYTE_TOTAL)
#define WDDR_PHY_CHANNEL_NUM                (WDDR_CHANNEL_TOTAL)
#define WDDR_PHY_MAX_FREQ_RATIO             (WDDR_PHY_FREQ_RATIO_1TO2)

#endif /* _WDDR_PHY_CONFIG_H_ */