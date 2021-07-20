/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <lpde/tx_driver.h>
#include <wddr/memory_map.h>

// BASE
#define TX_LPDE_DQ_BASE__ADDR       (DDR_DQ_DQ_TX_LPDE_M0_R0_CFG_0__ADR)
#define TX_LPDE_DQS_BASE__ADDR      (DDR_DQ_DQS_TX_LPDE_M0_R0_CFG_0__ADR)
#define TX_LPDE_CA_BASE__ADDR       (DDR_CA_DQ_TX_LPDE_M0_R0_CFG_0__ADR)
#define TX_LPDE_CK_BASE__ADDR       (DDR_CA_DQS_TX_LPDE_M0_R0_CFG_0__ADR)

// Mode, rank, bit offsets
// DQ
#define TX_LPDE_DQ_BIT__OFFSET      (DDR_DQ_DQ_TX_LPDE_M0_R0_CFG_1__ADR - DDR_DQ_DQ_TX_LPDE_M0_R0_CFG_0__ADR)
#define TX_LPDE_DQ_RANK__OFFSET     (DDR_DQ_DQ_TX_LPDE_M0_R1_CFG_0__ADR - DDR_DQ_DQ_TX_LPDE_M0_R0_CFG_0__ADR)
#define TX_LPDE_DQ_MODE__OFFSET     (DDR_DQ_DQ_TX_LPDE_M1_R0_CFG_0__ADR - DDR_DQ_DQ_TX_LPDE_M0_R0_CFG_0__ADR)
// DQS
#define TX_LPDE_DQS_BIT__OFFSET     (DDR_DQ_DQS_TX_LPDE_M0_R0_CFG_1__ADR - DDR_DQ_DQS_TX_LPDE_M0_R0_CFG_0__ADR)
#define TX_LPDE_DQS_RANK__OFFSET    (DDR_DQ_DQS_TX_LPDE_M0_R1_CFG_0__ADR - DDR_DQ_DQS_TX_LPDE_M0_R0_CFG_0__ADR)
#define TX_LPDE_DQS_MODE__OFFSET    (DDR_DQ_DQS_TX_LPDE_M1_R0_CFG_0__ADR - DDR_DQ_DQS_TX_LPDE_M0_R0_CFG_0__ADR)
// CA
#define TX_LPDE_CA_BIT__OFFSET      (DDR_CA_DQ_TX_LPDE_M0_R0_CFG_1__ADR - DDR_CA_DQ_TX_LPDE_M0_R0_CFG_0__ADR)
#define TX_LPDE_CA_RANK__OFFSET     (DDR_CA_DQ_TX_LPDE_M0_R1_CFG_0__ADR - DDR_CA_DQ_TX_LPDE_M0_R0_CFG_0__ADR)
#define TX_LPDE_CA_MODE__OFFSET     (DDR_CA_DQ_TX_LPDE_M1_R0_CFG_0__ADR - DDR_CA_DQ_TX_LPDE_M0_R0_CFG_0__ADR)
// CK
#define TX_LPDE_CK_BIT__OFFSET      (0)
#define TX_LPDE_CK_RANK__OFFSET     (DDR_CA_DQS_TX_LPDE_M0_R1_CFG_0__ADR - DDR_CA_DQS_TX_LPDE_M0_R0_CFG_0__ADR)
#define TX_LPDE_CK_MODE__OFFSET     (DDR_CA_DQS_TX_LPDE_M1_R0_CFG_0__ADR - DDR_CA_DQS_TX_LPDE_M0_R0_CFG_0__ADR)

void tx_lpde_set_msr_reg_if(lpde_dev_t *tx_lpde,
                            wddr_msr_t msr,
                            wddr_slice_type_t slice_type,
                            wddr_rank_t rank,
                            uint8_t bit)

{
    uint32_t base, bit_offset, rank_offset, mode_offset;

    switch(slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            base = TX_LPDE_DQ_BASE__ADDR;
            bit_offset = TX_LPDE_DQ_BIT__OFFSET;
            rank_offset = TX_LPDE_DQ_RANK__OFFSET;
            mode_offset = TX_LPDE_DQ_MODE__OFFSET;
            break;
        case WDDR_SLICE_TYPE_DQS:
            base = TX_LPDE_DQS_BASE__ADDR;
            bit_offset = TX_LPDE_DQS_BIT__OFFSET;
            rank_offset = TX_LPDE_DQS_RANK__OFFSET;
            mode_offset = TX_LPDE_DQS_MODE__OFFSET;
            break;
        case WDDR_SLICE_TYPE_CA:
            base = TX_LPDE_CA_BASE__ADDR;
            bit_offset = TX_LPDE_CA_BIT__OFFSET;
            rank_offset = TX_LPDE_CA_RANK__OFFSET;
            mode_offset = TX_LPDE_CA_MODE__OFFSET;
            break;
        case WDDR_SLICE_TYPE_CK:
            base = TX_LPDE_CK_BASE__ADDR;
            bit_offset = TX_LPDE_CK_BIT__OFFSET;
            rank_offset = TX_LPDE_CK_RANK__OFFSET;
            mode_offset = TX_LPDE_CK_MODE__OFFSET;
            break;
        default:
            return;
    }

    tx_lpde->base = (tx_lpde->base & ~WDDR_MEMORY_MAP_REL_MASK) +
                         base + rank * rank_offset + bit * bit_offset + msr * mode_offset;
}

void tx_lpde_init_reg_if(lpde_dev_t *tx_lpde,
                         uint32_t base,
                         wddr_slice_type_t slice_type,
                         wddr_rank_t rank,
                         uint8_t bit)
{
    tx_lpde->base = base;
    tx_lpde_set_msr_reg_if(tx_lpde, WDDR_MSR_0, slice_type, rank, bit);
}
