/**
* Copyright (c) 2021 Wavious LLC.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef _DQ_REG_H_
#define _DQ_REG_H_

/* Standard includes. */
#include <stdint.h>

/**
 * @brief   DQ Register Layout
 *
 * @details Layout of register address space of DQ CSRs. This structure should
 *          be "overlayed" by pointing to the base address of a given DQ byte.
 *
 * @note    See hardware documentation for information about these registers.
 *
 * @todo    Update MSR, RANK and bits with phy_config constants
 */
typedef struct dq_reg
{
    volatile uint32_t DDR_DQ_TOP_CFG;
    volatile uint32_t DDR_DQ_TOP_STA;
    volatile uint32_t DDR_DQ_DQ_RX_BSCAN_STA;
    volatile uint32_t DDR_DQ_DQ_RX_CFG[2];
    volatile uint32_t DDR_DQ_DQ_RX_IO_CFG[2][2][9];
    volatile uint32_t DDR_DQ_DQ_RX_IO_STA;
    volatile uint32_t DDR_DQ_DQ_RX_SA_CFG[2][2][9];
    volatile uint32_t DDR_DQ_DQ_RX_SA_DLY_CFG[2][2][9];
    volatile uint32_t DDR_DQ_DQ_RX_SA_STA[9];
    volatile uint32_t DDR_DQ_DQ_TX_BSCAN_CFG;
    volatile uint32_t DDR_DQ_DQ_TX_EGRESS_ANA_CFG[2][9];
    volatile uint32_t DDR_DQ_DQ_TX_EGRESS_DIG_CFG[2][9];
    volatile uint32_t DDR_DQ_DQ_TX_ODR_PI_CFG[2][2];
    volatile uint32_t DDR_DQ_DQ_TX_QDR_PI_0_CFG[2][2];
    volatile uint32_t DDR_DQ_DQ_TX_QDR_PI_1_CFG[2][2];
    volatile uint32_t DDR_DQ_DQ_TX_DDR_PI_0_CFG[2][2];
    volatile uint32_t DDR_DQ_DQ_TX_DDR_PI_1_CFG[2][2];
    volatile uint32_t DDR_DQ_DQ_TX_PI_RT_CFG[2][2];
    volatile uint32_t DDR_DQ_DQ_TX_RT_CFG[2][2];
    volatile uint32_t DDR_DQ_DQ_TX_SDR_CFG[2][2][9];
    volatile uint32_t DDR_DQ_DQ_TX_SDR_X_SEL_CFG[2][2][9];
    volatile uint32_t DDR_DQ_DQ_TX_SDR_FC_DLY_CFG[2][2][9];
    volatile uint32_t DDR_DQ_DQ_TX_DDR_CFG[2][2][9];
    volatile uint32_t DDR_DQ_DQ_TX_DDR_X_SEL_CFG[2][2][9];
    volatile uint32_t DDR_DQ_DQ_TX_QDR_CFG[2][2][9];
    volatile uint32_t DDR_DQ_DQ_TX_QDR_X_SEL_CFG[2][2][9];
    volatile uint32_t DDR_DQ_DQ_TX_LPDE_CFG[2][2][9];
    volatile uint32_t DDR_DQ_DQ_TX_IO_CFG[2][9];
    volatile uint32_t DDR_DQ_DQS_RX_CFG[2];
    volatile uint32_t DDR_DQ_DQS_RX_BSCAN_STA;
    volatile uint32_t DDR_DQ_DQS_RX_SDR_LPDE_CFG[2][2];
    volatile uint32_t DDR_DQ_DQS_RX_REN_PI_CFG[2][2];
    volatile uint32_t DDR_DQ_DQS_RX_RCS_PI_CFG[2][2];
    volatile uint32_t DDR_DQ_DQS_RX_RDQS_PI_0_CFG[2][2];
    volatile uint32_t DDR_DQ_DQS_RX_RDQS_PI_1_CFG[2][2];
    volatile uint32_t DDR_DQ_DQS_RX_PI_STA;
    volatile uint32_t DDR_DQ_DQS_RX_IO_CFG[2][2][2];
    volatile uint32_t DDR_DQ_DQS_RX_IO_CMN_CFG[2][2];
    volatile uint32_t DDR_DQ_DQS_RX_IO_STA;
    volatile uint32_t DDR_DQ_DQS_RX_SA_CFG[2][2][2];
    volatile uint32_t DDR_DQ_DQS_RX_SA_CMN_CFG;
    volatile uint32_t DDR_DQ_DQS_TX_CFG[2];
    volatile uint32_t DDR_DQ_DQS_TX_BSCAN_CTRL_CFG;
    volatile uint32_t DDR_DQ_DQS_TX_BSCAN_CFG;
    volatile uint32_t DDR_DQ_DQS_TX_EGRESS_ANA_CFG[2][9];
    volatile uint32_t DDR_DQ_DQS_TX_EGRESS_DIG_CFG[2][9];
    volatile uint32_t DDR_DQ_DQS_TX_ODR_PI_CFG[2][2];
    volatile uint32_t DDR_DQ_DQS_TX_QDR_PI_0_CFG[2][2];
    volatile uint32_t DDR_DQ_DQS_TX_QDR_PI_1_CFG[2][2];
    volatile uint32_t DDR_DQ_DQS_TX_DDR_PI_0_CFG[2][2];
    volatile uint32_t DDR_DQ_DQS_TX_DDR_PI_1_CFG[2][2];
    volatile uint32_t DDR_DQ_DQS_TX_PI_RT_CFG[2][2];
    volatile uint32_t DDR_DQ_DQS_TX_SDR_PI_CFG[2][2];
    volatile uint32_t DDR_DQ_DQS_TX_DFI_PI_CFG[2][2];
    volatile uint32_t DDR_DQ_DQS_TX_RT_CFG[2][2];
    volatile uint32_t DDR_DQ_DQS_TX_SDR_CFG[2][2][9];
    volatile uint32_t DDR_DQ_DQS_TX_SDR_X_SEL_CFG[2][2][9];
    volatile uint32_t DDR_DQ_DQS_TX_SDR_FC_DLY_CFG[2][2][9];
    volatile uint32_t DDR_DQ_DQS_TX_DDR_CFG[2][2][9];
    volatile uint32_t DDR_DQ_DQS_TX_DDR_X_SEL_CFG[2][2][9];
    volatile uint32_t DDR_DQ_DQS_TX_QDR_CFG[2][2][9];
    volatile uint32_t DDR_DQ_DQS_TX_QDR_X_SEL_CFG[2][2][9];
    volatile uint32_t DDR_DQ_DQS_TX_LPDE_CFG[2][2][2];
    volatile uint32_t DDR_DQ_DQS_TX_IO_CFG[2][2];
    volatile uint32_t DDR_DQ_DQS_TX_IO_CMN_CFG[2][2];
} dq_reg_t;

#endif /* _DQ_REG_H_ */
