/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#ifndef _PIPELINE_RT_DRIVER_H_
#define _PIPELINE_RT_DRIVER_H_

#include <wddr/memory_map.h>
#include <pipeline/device.h>

// Register Offsets (DQ)
#define RT_PIPE_EN__OFFSET      (DDR_DQ_DQ_TX_RT_M0_R0_CFG__ADR  - DDR_DQ_DQ_TX_RT_M0_R0_CFG__ADR)
// CA
#define RT_CA_PIPE_EN__OFFSET   (DDR_CA_DQ_TX_RT_M0_R0_CFG__ADR  - DDR_CA_DQ_TX_RT_M0_R0_CFG__ADR)

// Mode, rank, bit offsets
// DQ
#define RT_DQ_RANK__OFFSET      (DDR_DQ_DQ_TX_RT_M0_R1_CFG__ADR  - DDR_DQ_DQ_TX_RT_M0_R0_CFG__ADR)
#define RT_DQ_MODE__OFFSET      (DDR_DQ_DQ_TX_RT_M1_R0_CFG__ADR  - DDR_DQ_DQ_TX_RT_M0_R0_CFG__ADR)
// DQS
#define RT_DQS_RANK__OFFSET     (DDR_DQ_DQS_TX_RT_M0_R1_CFG__ADR - DDR_DQ_DQS_TX_RT_M0_R0_CFG__ADR)
#define RT_DQS_MODE__OFFSET     (DDR_DQ_DQS_TX_RT_M1_R0_CFG__ADR - DDR_DQ_DQS_TX_RT_M0_R0_CFG__ADR)
// CA
#define RT_CA_RANK__OFFSET      (DDR_CA_DQ_TX_RT_M0_R1_CFG__ADR  - DDR_CA_DQ_TX_RT_M0_R0_CFG__ADR)
#define RT_CA_MODE__OFFSET      (DDR_CA_DQ_TX_RT_M1_R0_CFG__ADR  - DDR_CA_DQ_TX_RT_M0_R0_CFG__ADR)
// CK
#define RT_CK_RANK__OFFSET      (DDR_CA_DQS_TX_RT_M0_R1_CFG__ADR - DDR_CA_DQS_TX_RT_M0_R0_CFG__ADR)
#define RT_CK_MODE__OFFSET      (DDR_CA_DQS_TX_RT_M1_R0_CFG__ADR - DDR_CA_DQS_TX_RT_M0_R0_CFG__ADR)

/**
 * @brief   RT Pipeline Initialize Register Interface
 *
 * @details Initializes RT Pipeline device at the driver level.
 *
 * @param[in]   rt_pipeline    pointer to RT Pipeline device.
 * @param[in]   base            base address of the RT Pipeline device.
 * @param[in]   slice_type      type of slice of the RT Pipeline device.
 * @param[in]   rank            rank of the RT Pipeline device.
 *
 * @return      void
 */
void rt_pipeline_init_reg_if(rt_pipeline_dev_t *rt_pipeline,
                             uint32_t base,
                             wddr_slice_type_t slice_type,
                             wddr_rank_t rank);

/**
 * @brief   RT Pipeline Set MSR Register Interface
 *
 * @details Configures RT Pipeline device for given Mode Set Register.
 *
 * @param[in]   rt_pipeline     pointer to RT Pipeline device.
 * @param[in]   msr             mode set register value to configure for.
 *
 * @return      void
 */
void rt_pipeline_set_msr_reg_if(rt_pipeline_dev_t *rt_pipeline,
                                wddr_msr_t msr);

/**
 * @brief   RT Pipeline Set Pipe Enable Register Interface
 *
 * @details Sets Pipeline Enable state for all phases for the RT Pipeline
 *          device via CSR.
 *
 * @param[in]   rt_pipeline     pointer to RT Pipeline device.
 * @param[in]   slice_type      type of slice of the RT Pipeline device.
 * @param[in]   pipe_en         pipe enable value to set.
 *
 * @return      void
 */
void rt_pipeline_set_pipe_en_reg_if(rt_pipeline_dev_t *rt_pipeline,
                                    wddr_slice_type_t slice_type,
                                    uint32_t pipe_en);

#endif /* _PIPELINE_RT_DRIVER_H_ */
