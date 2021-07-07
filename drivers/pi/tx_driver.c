/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#include <pi/tx_driver.h>
#include <wddr/memory_map.h>

// BASE
// DQ
#define DQ_ODR_PI_BASE__ADDR        (DDR_DQ_DQ_TX_ODR_PI_M0_R0_CFG__ADR)
#define DQ_QDR_PI_BASE__ADDR        (DDR_DQ_DQ_TX_QDR_PI_0_M0_R0_CFG__ADR)
#define DQ_DDR_PI_BASE__ADDR        (DDR_DQ_DQ_TX_DDR_PI_0_M0_R0_CFG__ADR)
#define DQ_RT_PI_BASE__ADDR         (DDR_DQ_DQ_TX_PI_RT_M0_R0_CFG__ADR)
// DQS
#define DQS_ODR_PI_BASE__ADDR       (DDR_DQ_DQS_TX_ODR_PI_M0_R0_CFG__ADR)
#define DQS_QDR_PI_BASE__ADDR       (DDR_DQ_DQS_TX_QDR_PI_0_M0_R0_CFG__ADR)
#define DQS_DDR_PI_BASE__ADDR       (DDR_DQ_DQS_TX_DDR_PI_0_M0_R0_CFG__ADR)
#define DQS_RT_PI_BASE__ADDR        (DDR_DQ_DQS_TX_PI_RT_M0_R0_CFG__ADR)
#define DQS_SDR_PI_BASE__ADDR       (DDR_DQ_DQS_TX_SDR_PI_M0_R0_CFG__ADR)
#define DQS_DFI_PI_BASE__ADDR       (DDR_DQ_DQS_TX_DFI_PI_M0_R0_CFG__ADR)
// CA
#define CA_ODR_PI_BASE__ADDR        (DDR_CA_DQ_TX_ODR_PI_M0_R0_CFG__ADR)
#define CA_QDR_PI_BASE__ADDR        (DDR_CA_DQ_TX_QDR_PI_0_M0_R0_CFG__ADR)
#define CA_DDR_PI_BASE__ADDR        (DDR_CA_DQ_TX_DDR_PI_0_M0_R0_CFG__ADR)
#define CA_RT_PI_BASE__ADDR         (DDR_CA_DQ_TX_PI_RT_M0_R0_CFG__ADR)
// CK
#define CK_ODR_PI_BASE__ADDR        (DDR_CA_DQS_TX_ODR_PI_M0_R0_CFG__ADR)
#define CK_QDR_PI_BASE__ADDR        (DDR_CA_DQS_TX_QDR_PI_0_M0_R0_CFG__ADR)
#define CK_DDR_PI_BASE__ADDR        (DDR_CA_DQS_TX_DDR_PI_0_M0_R0_CFG__ADR)
#define CK_RT_PI_BASE__ADDR         (DDR_CA_DQS_TX_PI_RT_M0_R0_CFG__ADR)
#define CK_SDR_PI_BASE__ADDR        (DDR_CA_DQS_TX_SDR_PI_M0_R0_CFG__ADR)
#define CK_DFI_PI_BASE__ADDR        (DDR_CA_DQS_TX_DFI_PI_M0_R0_CFG__ADR)

// Offsets
// DQ
#define DQ_ODR_PI_RANK__OFFSET      (DDR_DQ_DQ_TX_ODR_PI_M0_R1_CFG__ADR - DDR_DQ_DQ_TX_ODR_PI_M0_R0_CFG__ADR)
#define DQ_ODR_PI_MODE__OFFSET      (DDR_DQ_DQ_TX_ODR_PI_M1_R0_CFG__ADR - DDR_DQ_DQ_TX_ODR_PI_M0_R0_CFG__ADR)
#define DQ_QDR_PI_RANK__OFFSET      (DDR_DQ_DQ_TX_QDR_PI_0_M0_R1_CFG__ADR - DDR_DQ_DQ_TX_QDR_PI_0_M0_R0_CFG__ADR)
#define DQ_QDR_PI_MODE__OFFSET      (DDR_DQ_DQ_TX_QDR_PI_0_M1_R0_CFG__ADR - DDR_DQ_DQ_TX_QDR_PI_0_M0_R0_CFG__ADR)
#define DQ_DDR_PI_RANK__OFFSET      (DDR_DQ_DQ_TX_DDR_PI_0_M0_R1_CFG__ADR - DDR_DQ_DQ_TX_DDR_PI_0_M0_R0_CFG__ADR)
#define DQ_DDR_PI_MODE__OFFSET      (DDR_DQ_DQ_TX_DDR_PI_0_M1_R0_CFG__ADR - DDR_DQ_DQ_TX_DDR_PI_0_M0_R0_CFG__ADR)
#define DQ_RT_PI_RANK__OFFSET       (DDR_DQ_DQ_TX_PI_RT_M0_R1_CFG__ADR - DDR_DQ_DQ_TX_PI_RT_M0_R0_CFG__ADR)
#define DQ_RT_PI_MODE__OFFSET       (DDR_DQ_DQ_TX_PI_RT_M1_R0_CFG__ADR - DDR_DQ_DQ_TX_PI_RT_M0_R0_CFG__ADR)

// DQS
#define DQS_ODR_PI_RANK__OFFSET     (DDR_DQ_DQS_TX_ODR_PI_M0_R1_CFG__ADR - DDR_DQ_DQS_TX_ODR_PI_M0_R0_CFG__ADR)
#define DQS_ODR_PI_MODE__OFFSET     (DDR_DQ_DQS_TX_ODR_PI_M1_R0_CFG__ADR - DDR_DQ_DQS_TX_ODR_PI_M0_R0_CFG__ADR)
#define DQS_QDR_PI_RANK__OFFSET     (DDR_DQ_DQS_TX_QDR_PI_0_M0_R1_CFG__ADR - DDR_DQ_DQS_TX_QDR_PI_0_M0_R0_CFG__ADR)
#define DQS_QDR_PI_MODE__OFFSET     (DDR_DQ_DQS_TX_QDR_PI_0_M1_R0_CFG__ADR - DDR_DQ_DQS_TX_QDR_PI_0_M0_R0_CFG__ADR)
#define DQS_DDR_PI_RANK__OFFSET     (DDR_DQ_DQS_TX_DDR_PI_0_M0_R1_CFG__ADR - DDR_DQ_DQS_TX_DDR_PI_0_M0_R0_CFG__ADR)
#define DQS_DDR_PI_MODE__OFFSET     (DDR_DQ_DQS_TX_DDR_PI_0_M1_R0_CFG__ADR - DDR_DQ_DQS_TX_DDR_PI_0_M0_R0_CFG__ADR)
#define DQS_RT_PI_RANK__OFFSET      (DDR_DQ_DQS_TX_PI_RT_M0_R1_CFG__ADR - DDR_DQ_DQS_TX_PI_RT_M0_R0_CFG__ADR)
#define DQS_RT_PI_MODE__OFFSET      (DDR_DQ_DQS_TX_PI_RT_M1_R0_CFG__ADR - DDR_DQ_DQS_TX_PI_RT_M0_R0_CFG__ADR)
#define DQS_SDR_PI_RANK__OFFSET     (DDR_DQ_DQS_TX_SDR_PI_M0_R1_CFG__ADR - DDR_DQ_DQS_TX_SDR_PI_M0_R0_CFG__ADR)
#define DQS_SDR_PI_MODE__OFFSET     (DDR_DQ_DQS_TX_SDR_PI_M1_R0_CFG__ADR - DDR_DQ_DQS_TX_SDR_PI_M0_R0_CFG__ADR)
#define DQS_DFI_PI_RANK__OFFSET     (DDR_DQ_DQS_TX_DFI_PI_M0_R1_CFG__ADR - DDR_DQ_DQS_TX_DFI_PI_M0_R0_CFG__ADR)
#define DQS_DFI_PI_MODE__OFFSET     (DDR_DQ_DQS_TX_DFI_PI_M1_R0_CFG__ADR - DDR_DQ_DQS_TX_DFI_PI_M0_R0_CFG__ADR)
// CA
#define CA_ODR_PI_RANK__OFFSET      (DDR_CA_DQ_TX_ODR_PI_M0_R1_CFG__ADR - DDR_CA_DQ_TX_ODR_PI_M0_R0_CFG__ADR)
#define CA_ODR_PI_MODE__OFFSET      (DDR_CA_DQ_TX_ODR_PI_M1_R0_CFG__ADR - DDR_CA_DQ_TX_ODR_PI_M0_R0_CFG__ADR)
#define CA_QDR_PI_RANK__OFFSET      (DDR_CA_DQ_TX_QDR_PI_0_M0_R1_CFG__ADR - DDR_CA_DQ_TX_QDR_PI_0_M0_R0_CFG__ADR)
#define CA_QDR_PI_MODE__OFFSET      (DDR_CA_DQ_TX_QDR_PI_0_M1_R0_CFG__ADR - DDR_CA_DQ_TX_QDR_PI_0_M0_R0_CFG__ADR)
#define CA_DDR_PI_RANK__OFFSET      (DDR_CA_DQ_TX_DDR_PI_0_M0_R1_CFG__ADR - DDR_CA_DQ_TX_DDR_PI_0_M0_R0_CFG__ADR)
#define CA_DDR_PI_MODE__OFFSET      (DDR_CA_DQ_TX_DDR_PI_0_M1_R0_CFG__ADR - DDR_CA_DQ_TX_DDR_PI_0_M0_R0_CFG__ADR)
#define CA_RT_PI_RANK__OFFSET       (DDR_CA_DQ_TX_PI_RT_M0_R1_CFG__ADR - DDR_CA_DQ_TX_PI_RT_M0_R0_CFG__ADR)
#define CA_RT_PI_MODE__OFFSET       (DDR_CA_DQ_TX_PI_RT_M1_R0_CFG__ADR - DDR_CA_DQ_TX_PI_RT_M0_R0_CFG__ADR)
// CK
#define CK_ODR_PI_RANK__OFFSET      (DDR_CA_DQS_TX_ODR_PI_M0_R1_CFG__ADR - DDR_CA_DQS_TX_ODR_PI_M0_R0_CFG__ADR)
#define CK_ODR_PI_MODE__OFFSET      (DDR_CA_DQS_TX_ODR_PI_M1_R0_CFG__ADR - DDR_CA_DQS_TX_ODR_PI_M0_R0_CFG__ADR)
#define CK_QDR_PI_RANK__OFFSET      (DDR_CA_DQS_TX_QDR_PI_0_M0_R1_CFG__ADR - DDR_CA_DQS_TX_QDR_PI_0_M0_R0_CFG__ADR)
#define CK_QDR_PI_MODE__OFFSET      (DDR_CA_DQS_TX_QDR_PI_0_M1_R0_CFG__ADR - DDR_CA_DQS_TX_QDR_PI_0_M0_R0_CFG__ADR)
#define CK_DDR_PI_RANK__OFFSET      (DDR_CA_DQS_TX_DDR_PI_0_M0_R1_CFG__ADR - DDR_CA_DQS_TX_DDR_PI_0_M0_R0_CFG__ADR)
#define CK_DDR_PI_MODE__OFFSET      (DDR_CA_DQS_TX_DDR_PI_0_M1_R0_CFG__ADR - DDR_CA_DQS_TX_DDR_PI_0_M0_R0_CFG__ADR)
#define CK_RT_PI_RANK__OFFSET       (DDR_CA_DQS_TX_PI_RT_M0_R1_CFG__ADR - DDR_CA_DQS_TX_PI_RT_M0_R0_CFG__ADR)
#define CK_RT_PI_MODE__OFFSET       (DDR_CA_DQS_TX_PI_RT_M1_R0_CFG__ADR - DDR_CA_DQS_TX_PI_RT_M0_R0_CFG__ADR)
#define CK_SDR_PI_RANK__OFFSET     (DDR_CA_DQS_TX_SDR_PI_M0_R1_CFG__ADR - DDR_CA_DQS_TX_SDR_PI_M0_R0_CFG__ADR)
#define CK_SDR_PI_MODE__OFFSET     (DDR_CA_DQS_TX_SDR_PI_M1_R0_CFG__ADR - DDR_CA_DQS_TX_SDR_PI_M0_R0_CFG__ADR)
#define CK_DFI_PI_RANK__OFFSET     (DDR_CA_DQS_TX_DFI_PI_M0_R1_CFG__ADR - DDR_CA_DQS_TX_DFI_PI_M0_R0_CFG__ADR)
#define CK_DFI_PI_MODE__OFFSET     (DDR_CA_DQS_TX_DFI_PI_M1_R0_CFG__ADR - DDR_CA_DQS_TX_DFI_PI_M0_R0_CFG__ADR)

void tx_pi_set_msr_reg_if(tx_pi_dev_t *tx_pi,
                          wddr_msr_t msr,
                          wddr_slice_type_t slice_type,
                          wddr_rank_t rank)
{
    uint32_t odr_base, odr_rank_offset, odr_mode_offset;
    uint32_t qdr_base, qdr_rank_offset, qdr_mode_offset;
    uint32_t ddr_base, ddr_rank_offset, ddr_mode_offset;
    uint32_t rt_base, rt_rank_offset, rt_mode_offset;
    uint32_t sdr_base = 0, sdr_rank_offset, sdr_mode_offset;
    uint32_t dfi_base = 0, dfi_rank_offset, dfi_mode_offset;

    switch(slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            odr_base = DQ_ODR_PI_BASE__ADDR;
            odr_rank_offset = DQ_ODR_PI_RANK__OFFSET;
            odr_mode_offset = DQ_ODR_PI_MODE__OFFSET;
            qdr_base = DQ_QDR_PI_BASE__ADDR;
            qdr_rank_offset = DQ_QDR_PI_RANK__OFFSET;
            qdr_mode_offset = DQ_QDR_PI_MODE__OFFSET;
            ddr_base = DQ_DDR_PI_BASE__ADDR;
            ddr_rank_offset = DQ_DDR_PI_RANK__OFFSET;
            ddr_mode_offset = DQ_DDR_PI_MODE__OFFSET;
            rt_base = DQ_RT_PI_BASE__ADDR;
            rt_rank_offset = DQ_RT_PI_RANK__OFFSET;
            rt_mode_offset = DQ_RT_PI_MODE__OFFSET;
            break;
        case WDDR_SLICE_TYPE_DQS:
            odr_base = DQS_ODR_PI_BASE__ADDR;
            odr_rank_offset = DQS_ODR_PI_RANK__OFFSET;
            odr_mode_offset = DQS_ODR_PI_MODE__OFFSET;
            qdr_base = DQS_QDR_PI_BASE__ADDR;
            qdr_rank_offset = DQS_QDR_PI_RANK__OFFSET;
            qdr_mode_offset = DQS_QDR_PI_MODE__OFFSET;
            ddr_base = DQS_DDR_PI_BASE__ADDR;
            ddr_rank_offset = DQS_DDR_PI_RANK__OFFSET;
            ddr_mode_offset = DQS_DDR_PI_MODE__OFFSET;
            rt_base = DQS_RT_PI_BASE__ADDR;
            rt_rank_offset = DQS_RT_PI_RANK__OFFSET;
            rt_mode_offset = DQS_RT_PI_MODE__OFFSET;
            sdr_base = DQS_SDR_PI_BASE__ADDR;
            sdr_rank_offset = DQS_SDR_PI_RANK__OFFSET;
            sdr_mode_offset = DQS_SDR_PI_MODE__OFFSET;
            dfi_base = DQS_DFI_PI_BASE__ADDR;
            dfi_rank_offset = DQS_DFI_PI_RANK__OFFSET;
            dfi_mode_offset = DQS_DFI_PI_MODE__OFFSET;
            break;
        case WDDR_SLICE_TYPE_CA:
            odr_base = CA_ODR_PI_BASE__ADDR;
            odr_rank_offset = CA_ODR_PI_RANK__OFFSET;
            odr_mode_offset = CA_ODR_PI_MODE__OFFSET;
            qdr_base = CA_QDR_PI_BASE__ADDR;
            qdr_rank_offset = CA_QDR_PI_RANK__OFFSET;
            qdr_mode_offset = CA_QDR_PI_MODE__OFFSET;
            ddr_base = CA_DDR_PI_BASE__ADDR;
            ddr_rank_offset = CA_DDR_PI_RANK__OFFSET;
            ddr_mode_offset = CA_DDR_PI_MODE__OFFSET;
            rt_base = CA_RT_PI_BASE__ADDR;
            rt_rank_offset = CA_RT_PI_RANK__OFFSET;
            rt_mode_offset = CA_RT_PI_MODE__OFFSET;
            break;
        case WDDR_SLICE_TYPE_CK:
            odr_base = CK_ODR_PI_BASE__ADDR;
            odr_rank_offset = CK_ODR_PI_RANK__OFFSET;
            odr_mode_offset = CK_ODR_PI_MODE__OFFSET;
            qdr_base = CK_QDR_PI_BASE__ADDR;
            qdr_rank_offset = CK_QDR_PI_RANK__OFFSET;
            qdr_mode_offset = CK_QDR_PI_MODE__OFFSET;
            ddr_base = CK_DDR_PI_BASE__ADDR;
            ddr_rank_offset = CK_DDR_PI_RANK__OFFSET;
            ddr_mode_offset = CK_DDR_PI_MODE__OFFSET;
            rt_base = CK_RT_PI_BASE__ADDR;
            rt_rank_offset = CK_RT_PI_RANK__OFFSET;
            rt_mode_offset = CK_RT_PI_MODE__OFFSET;
            sdr_base = CK_SDR_PI_BASE__ADDR;
            sdr_rank_offset = CK_SDR_PI_RANK__OFFSET;
            sdr_mode_offset = CK_SDR_PI_MODE__OFFSET;
            dfi_base = CK_DFI_PI_BASE__ADDR;
            dfi_rank_offset = CK_DFI_PI_RANK__OFFSET;
            dfi_mode_offset = CK_DFI_PI_MODE__OFFSET;
            break;
        default:
            return;
    }

    tx_pi->odr.base = (tx_pi->odr.base & ~WDDR_MEMORY_MAP_REL_MASK) +
                           odr_base + rank * odr_rank_offset + msr * odr_mode_offset;
    tx_pi->qdr.base = (tx_pi->qdr.base & ~WDDR_MEMORY_MAP_REL_MASK) +
                           qdr_base + rank * qdr_rank_offset + msr * qdr_mode_offset;
    tx_pi->ddr.base = (tx_pi->ddr.base & ~WDDR_MEMORY_MAP_REL_MASK) +
                           ddr_base + rank * ddr_rank_offset + msr * ddr_mode_offset;
    tx_pi->rt.base = (tx_pi->rt.base & ~WDDR_MEMORY_MAP_REL_MASK) +
                           rt_base + rank * rt_rank_offset + msr * rt_mode_offset;
    tx_pi->sdr.base = (tx_pi->sdr.base & ~WDDR_MEMORY_MAP_REL_MASK) +
                           sdr_base + rank * sdr_rank_offset + msr * sdr_mode_offset;
    tx_pi->dfi.base = (tx_pi->dfi.base & ~WDDR_MEMORY_MAP_REL_MASK) +
                           dfi_base + rank * dfi_rank_offset + msr * dfi_mode_offset;
}

void tx_pi_init_reg_if(tx_pi_dev_t *tx_pi,
                       uint32_t base,
                       wddr_slice_type_t slice_type,
                       wddr_rank_t rank)
{

    tx_pi->odr.base = base;
    tx_pi->qdr.base = base;
    tx_pi->ddr.base = base;
    tx_pi->rt.base =  base;
    tx_pi->sdr.base = base;
    tx_pi->dfi.base = base;

    // Determine offset
    tx_pi_set_msr_reg_if(tx_pi, WDDR_MSR_0, slice_type, rank);

}
