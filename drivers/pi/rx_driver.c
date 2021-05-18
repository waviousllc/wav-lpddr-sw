/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#include <pi/rx_driver.h>
#include <wddr/memory_map.h>

// BASE
// DQS
#define DQS_REN_PI_BASE__ADDR       (DDR_DQ_DQS_RX_REN_PI_M0_R0_CFG__ADR)
#define DQS_RCS_PI_BASE__ADDR       (DDR_DQ_DQS_RX_RCS_PI_M0_R0_CFG__ADR)
#define DQS_RDQS_PI_BASE__ADDR      (DDR_DQ_DQS_RX_RDQS_PI_0_M0_R0_CFG__ADR)
// CK
#define CK_REN_PI_BASE__ADDR        (DDR_CA_DQS_RX_REN_PI_M0_R0_CFG__ADR)
#define CK_RCS_PI_BASE__ADDR        (DDR_CA_DQS_RX_RCS_PI_M0_R0_CFG__ADR)
#define CK_RDQS_PI_BASE__ADDR       (DDR_CA_DQS_RX_RDQS_PI_0_M0_R0_CFG__ADR)

// Offsets
// DQS
#define DQS_REN_PI_RANK__OFFSET     (DDR_DQ_DQS_RX_REN_PI_M0_R1_CFG__ADR - DDR_DQ_DQS_RX_REN_PI_M0_R0_CFG__ADR)
#define DQS_REN_PI_MODE__OFFSET     (DDR_DQ_DQS_RX_REN_PI_M1_R0_CFG__ADR - DDR_DQ_DQS_RX_REN_PI_M0_R0_CFG__ADR)
#define DQS_RCS_PI_RANK__OFFSET     (DDR_DQ_DQS_RX_RCS_PI_M0_R1_CFG__ADR - DDR_DQ_DQS_RX_RCS_PI_M0_R0_CFG__ADR)
#define DQS_RCS_PI_MODE__OFFSET     (DDR_DQ_DQS_RX_RCS_PI_M1_R0_CFG__ADR - DDR_DQ_DQS_RX_RCS_PI_M0_R0_CFG__ADR)
#define DQS_RDQS_PI_RANK__OFFSET    (DDR_DQ_DQS_RX_RDQS_PI_0_M0_R1_CFG__ADR - DDR_DQ_DQS_RX_RDQS_PI_0_M0_R0_CFG__ADR)
#define DQS_RDQS_PI_MODE__OFFSET    (DDR_DQ_DQS_RX_RDQS_PI_0_M1_R0_CFG__ADR - DDR_DQ_DQS_RX_RDQS_PI_0_M0_R0_CFG__ADR)
// CK
#define CK_REN_PI_RANK__OFFSET      (DDR_CA_DQS_RX_REN_PI_M0_R1_CFG__ADR - DDR_CA_DQS_RX_REN_PI_M0_R0_CFG__ADR)
#define CK_REN_PI_MODE__OFFSET      (DDR_CA_DQS_RX_REN_PI_M1_R0_CFG__ADR - DDR_CA_DQS_RX_REN_PI_M0_R0_CFG__ADR)
#define CK_RCS_PI_RANK__OFFSET      (DDR_CA_DQS_RX_RCS_PI_M0_R1_CFG__ADR - DDR_CA_DQS_RX_RCS_PI_M0_R0_CFG__ADR)
#define CK_RCS_PI_MODE__OFFSET      (DDR_CA_DQS_RX_RCS_PI_M1_R0_CFG__ADR - DDR_CA_DQS_RX_RCS_PI_M0_R0_CFG__ADR)
#define CK_RDQS_PI_RANK__OFFSET     (DDR_CA_DQS_RX_RDQS_PI_0_M0_R1_CFG__ADR - DDR_CA_DQS_RX_RDQS_PI_0_M0_R0_CFG__ADR)
#define CK_RDQS_PI_MODE__OFFSET     (DDR_CA_DQS_RX_RDQS_PI_0_M1_R0_CFG__ADR - DDR_CA_DQS_RX_RDQS_PI_0_M0_R0_CFG__ADR)


void rx_pi_set_msr_reg_if(rx_pi_dev_t *rx_pi,
                          wddr_msr_t msr,
                          wddr_slice_type_t slice_type,
                          wddr_rank_t rank)
{
    uint32_t ren_base, ren_rank_offset, ren_mode_offset;
    uint32_t rcs_base, rcs_rank_offset, rcs_mode_offset;
    uint32_t rdqs_base, rdqs_rank_offset, rdqs_mode_offset;

    switch(slice_type)
    {
        case WDDR_SLICE_TYPE_DQS:
            ren_base = DQS_REN_PI_BASE__ADDR;
            ren_rank_offset = DQS_REN_PI_RANK__OFFSET;
            ren_mode_offset = DQS_REN_PI_MODE__OFFSET;
            rcs_base = DQS_RCS_PI_BASE__ADDR;
            rcs_rank_offset = DQS_RCS_PI_RANK__OFFSET;
            rcs_mode_offset = DQS_RCS_PI_MODE__OFFSET;
            rdqs_base = DQS_RDQS_PI_BASE__ADDR;
            rdqs_rank_offset = DQS_RDQS_PI_RANK__OFFSET;
            rdqs_mode_offset = DQS_RDQS_PI_MODE__OFFSET;
            break;
        case WDDR_SLICE_TYPE_CK:
            ren_base = CK_REN_PI_BASE__ADDR;
            ren_rank_offset = CK_REN_PI_RANK__OFFSET;
            ren_mode_offset = CK_REN_PI_MODE__OFFSET;
            rcs_base = CK_RCS_PI_BASE__ADDR;
            rcs_rank_offset = CK_RCS_PI_RANK__OFFSET;
            rcs_mode_offset = CK_RCS_PI_MODE__OFFSET;
            rdqs_base = CK_RDQS_PI_BASE__ADDR;
            rdqs_rank_offset = CK_RDQS_PI_RANK__OFFSET;
            rdqs_mode_offset = CK_RDQS_PI_MODE__OFFSET;
            break;
        default:
            return;
    }

    rx_pi->ren.base = (rx_pi->ren.base & ~WDDR_MEMORY_MAP_REL_MASK) +
                           ren_base + rank * ren_rank_offset + msr * ren_mode_offset;
    rx_pi->rcs.base = (rx_pi->rcs.base & ~WDDR_MEMORY_MAP_REL_MASK) +
                           rcs_base + rank * rcs_rank_offset + msr * rcs_mode_offset;
    rx_pi->rdqs.base = (rx_pi->rdqs.base & ~WDDR_MEMORY_MAP_REL_MASK) +
                           rdqs_base + rank * rdqs_rank_offset + msr * rdqs_mode_offset;
}


void rx_pi_init_reg_if(rx_pi_dev_t *rx_pi,
                       uint32_t base,
                       wddr_slice_type_t slice_type,
                       wddr_rank_t rank)
{
    rx_pi->rcs.base = base;
    rx_pi->ren.base = base;
    rx_pi->rdqs.base = base;
    rx_pi_set_msr_reg_if(rx_pi, WDDR_MSR_0, slice_type, rank);
}
