/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#ifndef _PIPELINE_EGRESS_ANA_DRIVER_H_
#define _PIPELINE_EGRESS_ANA_DRIVER_H_

#include <wddr/memory_map.h>
#include <pipeline/device.h>

// Register Offset
#define EGRESS_ANA_DQ_CFG__OFFSET       (DDR_DQ_DQ_TX_EGRESS_ANA_M0_CFG_0__ADR  - DDR_DQ_DQ_TX_EGRESS_ANA_M0_CFG_0__ADR)
#define EGRESS_ANA_DQS_CFG__OFFSET      (DDR_DQ_DQS_TX_EGRESS_ANA_M0_CFG_0__ADR - DDR_DQ_DQS_TX_EGRESS_ANA_M0_CFG_0__ADR)

#define EGRESS_ANA_CA_CFG__OFFSET       (DDR_CA_DQ_TX_EGRESS_ANA_M0_CFG_0__ADR  - DDR_CA_DQ_TX_EGRESS_ANA_M0_CFG_0__ADR)
#define EGRESS_ANA_CK_CFG__OFFSET       (DDR_CA_DQS_TX_EGRESS_ANA_M0_CFG_0__ADR - DDR_CA_DQS_TX_EGRESS_ANA_M0_CFG_0__ADR)

//Mode, rank, bit offsets
// DQ
#define EGRESS_ANA_DQ_BIT__OFFSET       (DDR_DQ_DQ_TX_EGRESS_ANA_M0_CFG_1__ADR  - DDR_DQ_DQ_TX_EGRESS_ANA_M0_CFG_0__ADR)
#define EGRESS_ANA_DQ_RANK__OFFSET      (0)
#define EGRESS_ANA_DQ_MODE__OFFSET      (DDR_DQ_DQ_TX_EGRESS_ANA_M1_CFG_0__ADR  - DDR_DQ_DQ_TX_EGRESS_ANA_M0_CFG_0__ADR)
// DQS
#define EGRESS_ANA_DQS_BIT__OFFSET      (DDR_DQ_DQS_TX_EGRESS_ANA_M0_CFG_1__ADR - DDR_DQ_DQS_TX_EGRESS_ANA_M0_CFG_0__ADR)
#define EGRESS_ANA_DQS_RANK__OFFSET     (0)
#define EGRESS_ANA_DQS_MODE__OFFSET     (DDR_DQ_DQS_TX_EGRESS_ANA_M1_CFG_0__ADR - DDR_DQ_DQS_TX_EGRESS_ANA_M0_CFG_0__ADR)
// CA
#define EGRESS_ANA_CA_BIT__OFFSET       (DDR_CA_DQ_TX_EGRESS_ANA_M0_CFG_1__ADR  - DDR_CA_DQ_TX_EGRESS_ANA_M0_CFG_0__ADR)
#define EGRESS_ANA_CA_RANK__OFFSET      (0)
#define EGRESS_ANA_CA_MODE__OFFSET      (DDR_CA_DQ_TX_EGRESS_ANA_M1_CFG_0__ADR  - DDR_CA_DQ_TX_EGRESS_ANA_M0_CFG_0__ADR)
// CK
#define EGRESS_ANA_CK_BIT__OFFSET       (0)
#define EGRESS_ANA_CK_RANK__OFFSET      (0)
#define EGRESS_ANA_CK_MODE__OFFSET      (DDR_CA_DQS_TX_EGRESS_ANA_M1_CFG_0__ADR - DDR_CA_DQS_TX_EGRESS_ANA_M0_CFG_0__ADR)

/**
 * @brief   Egress Analog Initialize Register Interface
 *
 * @details Initializes Egress Analog device at the driver level.
 *
 * @param[in]   egress_ana      pointer to Egress Analog device.
 * @param[in]   base            base address of the Egress Analog device.
 * @param[in]   slice_type      type of slice of the Egress Analog device.
 * @param[in]   bit             bit index of the Egress Analog device.
 *
 * @return      void
 */
void egress_ana_init_reg_if(bit_egress_dev_t *egress_ana,
                            uint32_t base,
                            wddr_slice_type_t slice_type,
                            uint8_t bit);

/**
 * @brief   Egress Analog Set MSR Register Interface
 *
 * @details Configures Egress Analog device for given Mode Set Register.
 *
 * @param[in]   egress_ana      pointer to Egress Analog device.
 * @param[in]   msr             mode set register value to configure for.
 *
 * @return      void
 */
void egress_ana_set_msr_reg_if(bit_egress_dev_t *egress_ana,
                               wddr_msr_t msr);

/**
 * @brief   Egress Analog Set Mode Register Interface
 *
 * @details Sets Egress Analog device mode via CSR.
 *
 * @param[in]   egress_ana      pointer to Egress Analog device.
 * @param[in]   slice_type      type of slice of the Egress Analog device.
 * @param[in]   mode            egress mode to set.
 *
 * @return      void
 */
void egress_ana_set_mode_reg_if(bit_egress_dev_t *egress_ana,
                                wddr_slice_type_t slice_type,
                                egress_ana_mode_t mode);

#endif /* _PIPELINE_EGRESS_ANA_DRIVER_H_ */
