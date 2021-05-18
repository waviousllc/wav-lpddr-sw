/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _PIPELINE_QDR_DRIVER_H_
#define _PIPELINE_QDR_DRIVER_H_

#include <wddr/memory_map.h>
#include <pipeline/device.h>

// Register Offsets (DQ)
#define QDR_DQ_PIPE_EN__OFFSET          (DDR_DQ_DQ_TX_QDR_M0_R0_CFG_0__ADR       - DDR_DQ_DQ_TX_QDR_M0_R0_CFG_0__ADR)
#define QDR_DQ_X_SEL__OFFSET            (DDR_DQ_DQ_TX_QDR_X_SEL_M0_R0_CFG_0__ADR - DDR_DQ_DQ_TX_QDR_M0_R0_CFG_0__ADR)
// Register Offsets (DQS)
#define QDR_DQS_PIPE_EN__OFFSET         (DDR_DQ_DQS_TX_QDR_M0_R0_CFG_0__ADR       - DDR_DQ_DQS_TX_QDR_M0_R0_CFG_0__ADR)
#define QDR_DQS_X_SEL__OFFSET           (DDR_DQ_DQS_TX_QDR_X_SEL_M0_R0_CFG_0__ADR - DDR_DQ_DQS_TX_QDR_M0_R0_CFG_0__ADR)
// Regisetr Offsets (CA)
#define QDR_CA_PIPE_EN__OFFSET          (DDR_CA_DQ_TX_QDR_M0_R0_CFG_0__ADR       - DDR_CA_DQ_TX_QDR_M0_R0_CFG_0__ADR)
#define QDR_CA_X_SEL__OFFSET            (DDR_CA_DQ_TX_QDR_X_SEL_M0_R0_CFG_0__ADR - DDR_CA_DQ_TX_QDR_M0_R0_CFG_0__ADR)
// Regisetr Offsets (CK)
#define QDR_CK_PIPE_EN__OFFSET          (DDR_CA_DQS_TX_QDR_M0_R0_CFG_0__ADR       - DDR_CA_DQS_TX_QDR_M0_R0_CFG_0__ADR)
#define QDR_CK_X_SEL__OFFSET            (DDR_CA_DQS_TX_QDR_X_SEL_M0_R0_CFG_0__ADR - DDR_CA_DQS_TX_QDR_M0_R0_CFG_0__ADR)

// Mode, rank, bit offsets
// DQ
#define QDR_DQ_BIT__OFFSET              (DDR_DQ_DQ_TX_QDR_M0_R0_CFG_1__ADR - DDR_DQ_DQ_TX_QDR_M0_R0_CFG_0__ADR)
#define QDR_DQ_RANK__OFFSET             (DDR_DQ_DQ_TX_QDR_M0_R1_CFG_0__ADR - DDR_DQ_DQ_TX_QDR_M0_R0_CFG_0__ADR)
#define QDR_DQ_MODE__OFFSET             (DDR_DQ_DQ_TX_QDR_M1_R0_CFG_0__ADR - DDR_DQ_DQ_TX_QDR_M0_R0_CFG_0__ADR)
// DQS
#define QDR_DQS_BIT__OFFSET             (DDR_DQ_DQS_TX_QDR_M0_R0_CFG_1__ADR - DDR_DQ_DQS_TX_QDR_M0_R0_CFG_0__ADR)
#define QDR_DQS_RANK__OFFSET            (DDR_DQ_DQS_TX_QDR_M0_R1_CFG_0__ADR - DDR_DQ_DQS_TX_QDR_M0_R0_CFG_0__ADR)
#define QDR_DQS_MODE__OFFSET            (DDR_DQ_DQS_TX_QDR_M1_R0_CFG_0__ADR - DDR_DQ_DQS_TX_QDR_M0_R0_CFG_0__ADR)
// CA
#define QDR_CA_BIT__OFFSET              (DDR_CA_DQ_TX_QDR_M0_R0_CFG_1__ADR - DDR_CA_DQ_TX_QDR_M0_R0_CFG_0__ADR)
#define QDR_CA_RANK__OFFSET             (DDR_CA_DQ_TX_QDR_M0_R1_CFG_0__ADR - DDR_CA_DQ_TX_QDR_M0_R0_CFG_0__ADR)
#define QDR_CA_MODE__OFFSET             (DDR_CA_DQ_TX_QDR_M1_R0_CFG_0__ADR - DDR_CA_DQ_TX_QDR_M0_R0_CFG_0__ADR)
// CK
#define QDR_CK_BIT__OFFSET              (0)
#define QDR_CK_RANK__OFFSET             (DDR_CA_DQS_TX_QDR_M0_R1_CFG_0__ADR - DDR_CA_DQS_TX_QDR_M0_R0_CFG_0__ADR)
#define QDR_CK_MODE__OFFSET             (DDR_CA_DQS_TX_QDR_M1_R0_CFG_0__ADR - DDR_CA_DQS_TX_QDR_M0_R0_CFG_0__ADR)

/**
 * @brief   QDR Pipeline Initialize Register Interface
 *
 * @details Initializes QDR Pipeline device at the driver level.
 *
 * @param[in]   qdr_pipeline    pointer to QDR Pipeline device.
 * @param[in]   base            base address of the QDR Pipeline device.
 * @param[in]   slice_type      type of slice of the QDR Pipeline device.
 * @param[in]   rank            rank of the QDR Pipeline device.
 * @param[in]   bit             bit index of the QDR Pipeline device.
 *
 * @return      void
 */
void qdr_pipeline_init_reg_if(qdr_pipeline_dev_t *qdr_pipeline,
                              uint32_t base,
                              wddr_slice_type_t slice_type,
                              wddr_rank_t rank,
                              uint8_t bit);

/**
 * @brief   QDR Pipeline Set MSR Register Interface
 *
 * @details Configures QDR Pipeline device for given Mode Set Register.
 *
 * @param[in]   qdr_pipeline    pointer to QDR Pipeline device.
 * @param[in]   msr             mode set register value to configure for.
 *
 * @return      void
 */
void qdr_pipeline_set_msr_reg_if(qdr_pipeline_dev_t *qdr_pipeline,
                                 wddr_msr_t msr);

/**
 * @brief   QDR Pipeline Set Pipe Enable Register Interface
 *
 * @details Sets Pipeline Enable state for all phases for the QDR Pipeline
 *          device via CSR.
 *
 * @param[in]   qdr_pipeline    pointer to QDR Pipeline device.
 * @param[in]   slice_type      type of slice of the QDR Pipeline device.
 * @param[in]   pipe_en         pipe enable value to set.
 *
 * @return      void
 */
void qdr_pipeline_set_pipe_en_reg_if(qdr_pipeline_dev_t *qdr_pipeline,
                                     wddr_slice_type_t slice_type,
                                     uint32_t pipe_en);

/**
 * @brief   QDR Pipeline Set X SEL Register Interface
 *
 * @details Sets Phase Select (X SEL) state for the QDR Pipeline
 *          device via CSR.
 *
 * @param[in]   qdr_pipeline    pointer to QDR Pipeline device.
 * @param[in]   slice_type      type of slice of the QDR Pipeline device.
 * @param[in]   x_sel           phase select value to set.
 *
 * @return      void
 */
void qdr_pipeline_set_x_sel_reg_if(qdr_pipeline_dev_t *qdr_pipeline,
                                   wddr_slice_type_t slice_type,
                                   uint32_t x_sel);

#endif /* _PIPELINE_QDR_DRIVER_H_ */
