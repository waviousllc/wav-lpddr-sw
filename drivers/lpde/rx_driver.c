/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <lpde/rx_driver.h>
#include <wddr/memory_map.h>

// BASE
#define RX_LPDE_DQS_BASE__ADDR      (DDR_DQ_DQS_RX_SDR_LPDE_M0_R0_CFG__ADR)
#define RX_LPDE_CK_BASE__ADDR       (DDR_CA_DQS_RX_SDR_LPDE_M0_R0_CFG__ADR)
// Offsets
// DQS
#define RX_LPDE_DQS_RANK__OFFSET    (DDR_DQ_DQS_RX_SDR_LPDE_M0_R1_CFG__ADR - DDR_DQ_DQS_RX_SDR_LPDE_M0_R0_CFG__ADR)
#define RX_LPDE_DQS_MODE__OFFSET    (DDR_DQ_DQS_RX_SDR_LPDE_M1_R0_CFG__ADR - DDR_DQ_DQS_RX_SDR_LPDE_M0_R0_CFG__ADR)
// CK
#define RX_LPDE_CK_RANK__OFFSET    (DDR_CA_DQS_RX_SDR_LPDE_M0_R1_CFG__ADR - DDR_CA_DQS_RX_SDR_LPDE_M0_R0_CFG__ADR)
#define RX_LPDE_CK_MODE__OFFSET    (DDR_CA_DQS_RX_SDR_LPDE_M1_R0_CFG__ADR - DDR_CA_DQS_RX_SDR_LPDE_M0_R0_CFG__ADR)

void rx_lpde_set_msr_reg_if(lpde_dev_t *rx_lpde,
                     wddr_msr_t msr,
                     wddr_slice_type_t slice_type,
                     wddr_rank_t rank)
{
    uint32_t base = rx_lpde->base & ~WDDR_MEMORY_MAP_REL_MASK;
    switch(slice_type)
    {
        case WDDR_SLICE_TYPE_DQS:
            rx_lpde->base = base + RX_LPDE_DQS_BASE__ADDR + rank * RX_LPDE_DQS_RANK__OFFSET + msr * RX_LPDE_DQS_MODE__OFFSET;
            break;
        case WDDR_SLICE_TYPE_CK:
            rx_lpde->base = base + RX_LPDE_CK_BASE__ADDR + rank * RX_LPDE_CK_RANK__OFFSET + msr * RX_LPDE_CK_MODE__OFFSET;
            break;
        default:
            return;
    }
}

void rx_lpde_init_reg_if(lpde_dev_t *rx_lpde,
                  uint32_t base,
                  wddr_slice_type_t slice_type,
                  wddr_rank_t rank)
{
    rx_lpde->base = base;
    rx_lpde_set_msr_reg_if(rx_lpde, WDDR_MSR_0, slice_type, rank);
}
