/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#ifndef _PIPELINE_EGRESS_DIG_DRIVER_H_
#define _PIPELINE_EGRESS_DIG_DRIVER_H_

#include <wddr/memory_map.h>
#include <pipeline/device.h>

// Register Offset
#define EGRESS_DIG_DQ_CFG__OFFSET       (DDR_DQ_DQ_TX_EGRESS_DIG_M0_CFG_0__ADR  - DDR_DQ_DQ_TX_EGRESS_DIG_M0_CFG_0__ADR)
#define EGRESS_DIG_DQS_CFG__OFFSET      (DDR_DQ_DQS_TX_EGRESS_DIG_M0_CFG_0__ADR - DDR_DQ_DQS_TX_EGRESS_DIG_M0_CFG_0__ADR)

#define EGRESS_DIG_CA_CFG__OFFSET       (DDR_CA_DQ_TX_EGRESS_DIG_M0_CFG_0__ADR  - DDR_CA_DQ_TX_EGRESS_DIG_M0_CFG_0__ADR)
#define EGRESS_DIG_CK_CFG__OFFSET       (DDR_CA_DQS_TX_EGRESS_DIG_M0_CFG_0__ADR - DDR_CA_DQS_TX_EGRESS_DIG_M0_CFG_0__ADR)

//Mode, rank, bit offsets
// DQ
#define EGRESS_DIG_DQ_BIT__OFFSET       (DDR_DQ_DQ_TX_EGRESS_DIG_M0_CFG_1__ADR  - DDR_DQ_DQ_TX_EGRESS_DIG_M0_CFG_0__ADR)
#define EGRESS_DIG_DQ_MODE__OFFSET      (DDR_DQ_DQ_TX_EGRESS_DIG_M1_CFG_0__ADR  - DDR_DQ_DQ_TX_EGRESS_DIG_M0_CFG_0__ADR)
// DQS
#define EGRESS_DIG_DQS_BIT__OFFSET      (DDR_DQ_DQS_TX_EGRESS_DIG_M0_CFG_1__ADR - DDR_DQ_DQS_TX_EGRESS_DIG_M0_CFG_0__ADR)
#define EGRESS_DIG_DQS_MODE__OFFSET     (DDR_DQ_DQS_TX_EGRESS_DIG_M1_CFG_0__ADR - DDR_DQ_DQS_TX_EGRESS_DIG_M0_CFG_0__ADR)
// CA
#define EGRESS_DIG_CA_BIT__OFFSET       (DDR_CA_DQ_TX_EGRESS_DIG_M0_CFG_1__ADR  - DDR_CA_DQ_TX_EGRESS_DIG_M0_CFG_0__ADR)
#define EGRESS_DIG_CA_MODE__OFFSET      (DDR_CA_DQ_TX_EGRESS_DIG_M1_CFG_0__ADR  - DDR_CA_DQ_TX_EGRESS_DIG_M0_CFG_0__ADR)
// CK
#define EGRESS_DIG_CK_BIT__OFFSET       (0)
#define EGRESS_DIG_CK_MODE__OFFSET      (DDR_CA_DQS_TX_EGRESS_DIG_M1_CFG_0__ADR - DDR_CA_DQS_TX_EGRESS_DIG_M0_CFG_0__ADR)

/**
 * @brief   Egress Digital Initialize Register Interface
 *
 * @details Initializes Egress Digital device at the driver level.
 *
 * @param[in]   egress_dig      pointer to Egress Digital device.
 * @param[in]   base            base address of the Egress Digital device.
 * @param[in]   slice_type      type of slice of the Egress Digital device.
 * @param[in]   bit             bit index of the Egress Digital device.
 *
 * @return      void
 */
void egress_dig_init_reg_if(bit_egress_dev_t *egress_dig,
                            uint32_t base,
                            wddr_slice_type_t slice_type,
                            uint8_t bit);

/**
 * @brief   Egress Digital Set MSR Register Interface
 *
 * @details Configures Egress Digital device for given Mode Set Register.
 *
 * @param[in]   egress_dig      pointer to Egress Digital device.
 * @param[in]   msr             mode set register value to configure for.
 *
 * @return      void
 */
void egress_dig_set_msr_reg_if(bit_egress_dev_t *egress_dig,
                               wddr_msr_t msr);

/**
 * @brief   Egress Digital Set Mode Register Interface
 *
 * @details Sets Egress Digital device mode via CSR.
 *
 * @param[in]   egress_dig      pointer to Egress Digital device.
 * @param[in]   slice_type      type of slice of the Egress Digital device.
 * @param[in]   mode            egress mode to set.
 *
 * @return      void
 */
void egress_dig_set_mode_reg_if(bit_egress_dev_t *egress_dig,
                                wddr_slice_type_t slice_type,
                                egress_dig_mode_t mode);

#endif /* _PIPELINE_EGRESS_DIG_DRIVER_H_ */
