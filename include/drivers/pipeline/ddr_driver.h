/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _PIPELINE_DDR_DRIVER_H_
#define _PIPELINE_DDR_DRIVER_H_

#include <wddr/memory_map.h>
#include <pipeline/device.h>

// Register Offsets (DQ)
#define DDR_DQ_PIPE_EN__OFFSET          (DDR_DQ_DQ_TX_DDR_M0_R0_CFG_0__ADR       - DDR_DQ_DQ_TX_DDR_M0_R0_CFG_0__ADR)
#define DDR_DQ_X_SEL__OFFSET            (DDR_DQ_DQ_TX_DDR_X_SEL_M0_R0_CFG_0__ADR - DDR_DQ_DQ_TX_DDR_M0_R0_CFG_0__ADR)
// Register Offsets (DQS)
#define DDR_DQS_PIPE_EN__OFFSET         (DDR_DQ_DQS_TX_DDR_M0_R0_CFG_0__ADR       - DDR_DQ_DQS_TX_DDR_M0_R0_CFG_0__ADR)
#define DDR_DQS_X_SEL__OFFSET           (DDR_DQ_DQS_TX_DDR_X_SEL_M0_R0_CFG_0__ADR - DDR_DQ_DQS_TX_DDR_M0_R0_CFG_0__ADR)
// Register Offsets (CA)
#define DDR_CA_PIPE_EN__OFFSET          (DDR_CA_DQ_TX_DDR_M0_R0_CFG_0__ADR      - DDR_CA_DQ_TX_DDR_M0_R0_CFG_0__ADR)
#define DDR_CA_X_SEL__OFFSET            (DDR_CA_DQ_TX_DDR_X_SEL_M0_R0_CFG_0__ADR - DDR_CA_DQ_TX_DDR_M0_R0_CFG_0__ADR)
// Register Offsets (CK)
#define DDR_CK_PIPE_EN__OFFSET          (DDR_CA_DQS_TX_DDR_M0_R0_CFG_0__ADR       - DDR_CA_DQS_TX_DDR_M0_R0_CFG_0__ADR)
#define DDR_CK_X_SEL__OFFSET            (DDR_CA_DQS_TX_DDR_X_SEL_M0_R0_CFG_0__ADR - DDR_CA_DQS_TX_DDR_M0_R0_CFG_0__ADR)

// Mode, rank, bit offsets
// DQ
#define DDR_DQ_BIT__OFFSET              (DDR_DQ_DQ_TX_DDR_M0_R0_CFG_1__ADR - DDR_DQ_DQ_TX_DDR_M0_R0_CFG_0__ADR)
#define DDR_DQ_RANK__OFFSET             (DDR_DQ_DQ_TX_DDR_M0_R1_CFG_0__ADR - DDR_DQ_DQ_TX_DDR_M0_R0_CFG_0__ADR)
#define DDR_DQ_MODE__OFFSET             (DDR_DQ_DQ_TX_DDR_M1_R0_CFG_0__ADR - DDR_DQ_DQ_TX_DDR_M0_R0_CFG_0__ADR)
// DQS
#define DDR_DQS_BIT__OFFSET             (DDR_DQ_DQS_TX_DDR_M0_R0_CFG_1__ADR - DDR_DQ_DQS_TX_DDR_M0_R0_CFG_0__ADR)
#define DDR_DQS_RANK__OFFSET            (DDR_DQ_DQS_TX_DDR_M0_R1_CFG_0__ADR - DDR_DQ_DQS_TX_DDR_M0_R0_CFG_0__ADR)
#define DDR_DQS_MODE__OFFSET            (DDR_DQ_DQS_TX_DDR_M1_R0_CFG_0__ADR - DDR_DQ_DQS_TX_DDR_M0_R0_CFG_0__ADR)
// CA
#define DDR_CA_BIT__OFFSET              (DDR_CA_DQ_TX_DDR_M0_R0_CFG_1__ADR - DDR_CA_DQ_TX_DDR_M0_R0_CFG_0__ADR)
#define DDR_CA_RANK__OFFSET             (DDR_CA_DQ_TX_DDR_M0_R1_CFG_0__ADR - DDR_CA_DQ_TX_DDR_M0_R0_CFG_0__ADR)
#define DDR_CA_MODE__OFFSET             (DDR_CA_DQ_TX_DDR_M1_R0_CFG_0__ADR - DDR_CA_DQ_TX_DDR_M0_R0_CFG_0__ADR)
// CK
#define DDR_CK_BIT__OFFSET              (0)
#define DDR_CK_RANK__OFFSET             (DDR_CA_DQS_TX_DDR_M0_R1_CFG_0__ADR - DDR_CA_DQS_TX_DDR_M0_R0_CFG_0__ADR)
#define DDR_CK_MODE__OFFSET             (DDR_CA_DQS_TX_DDR_M1_R0_CFG_0__ADR - DDR_CA_DQS_TX_DDR_M0_R0_CFG_0__ADR)

/**
 * @brief   DDR Pipeline Initialize Register Interface
 *
 * @details Initializes DDR Pipeline device at the driver level.
 *
 * @param[in]   ddr_pipeline    pointer to DDR Pipeline device.
 * @param[in]   base            base address of the DDR Pipeline device.
 * @param[in]   slice_type      type of slice of the DDR Pipeline device.
 * @param[in]   rank            rank of the DDR Pipeline device.
 * @param[in]   bit             bit index of the DDR Pipeline device.
 *
 * @return      void
 */
void ddr_pipeline_init_reg_if(ddr_pipeline_dev_t *ddr_pipeline,
                              uint32_t base,
                              wddr_slice_type_t slice_type,
                              wddr_rank_t rank,
                              uint8_t bit);

/**
 * @brief   DDR Pipeline Set MSR Register Interface
 *
 * @details Configures DDR Pipeline device for given Mode Set Register.
 *
 * @param[in]   ddr_pipeline    pointer to DDR Pipeline device.
 * @param[in]   msr             mode set register value to configure for.
 *
 * @return      void
 */
void ddr_pipeline_set_msr_reg_if(ddr_pipeline_dev_t *ddr_pipeline,
                                 wddr_msr_t msr);

/**
 * @brief   DDR Pipeline Set Pipe Enable Register Interface
 *
 * @details Sets Pipeline Enable state for all phases for the DDR Pipeline
 *          device via CSR.
 *
 * @param[in]   ddr_pipeline    pointer to DDR Pipeline device.
 * @param[in]   slice_type      type of slice of the DDR Pipeline device.
 * @param[in]   pipe_en         pipe enable value to set.
 *
 * @return      void
 */
void ddr_pipeline_set_pipe_en_reg_if(ddr_pipeline_dev_t *ddr_pipeline,
                                     wddr_slice_type_t slice_type,
                                     uint32_t pipe_en);

/**
 * @brief   DDR Pipeline Set X SEL Register Interface
 *
 * @details Sets Phase Select (X SEL) state for the DDR Pipeline
 *          device via CSR.
 *
 * @param[in]   ddr_pipeline    pointer to DDR Pipeline device.
 * @param[in]   slice_type      type of slice of the DDR Pipeline device.
 * @param[in]   x_sel           phase select value to set.
 *
 * @return      void
 */
void ddr_pipeline_set_x_sel_reg_if(ddr_pipeline_dev_t *ddr_pipeline,
                                   wddr_slice_type_t slice_type,
                                   uint32_t x_sel);

#endif /* _PIPELINE_DDR_DRIVER_H_ */
