/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _PIPELINE_SDR_DRIVER_H_
#define _PIPELINE_SDR_DRIVER_H_

#include <wddr/memory_map.h>
#include <pipeline/device.h>

// Register Offsets
// DQ
#define SDR_DQ_PIPE_EN__OFFSET          (DDR_DQ_DQ_TX_SDR_M0_R0_CFG_0__ADR        - DDR_DQ_DQ_TX_SDR_M0_R0_CFG_0__ADR)
#define SDR_DQ_X_SEL__OFFSET            (DDR_DQ_DQ_TX_SDR_X_SEL_M0_R0_CFG_0__ADR  - DDR_DQ_DQ_TX_SDR_M0_R0_CFG_0__ADR)
#define SDR_DQ_FC_DLY__OFFSET           (DDR_DQ_DQ_TX_SDR_FC_DLY_M0_R0_CFG_0__ADR - DDR_DQ_DQ_TX_SDR_M0_R0_CFG_0__ADR)
// DQS
#define SDR_DQS_PIPE_EN__OFFSET         (DDR_DQ_DQS_TX_SDR_M0_R0_CFG_0__ADR        - DDR_DQ_DQS_TX_SDR_M0_R0_CFG_0__ADR)
#define SDR_DQS_X_SEL__OFFSET           (DDR_DQ_DQS_TX_SDR_X_SEL_M0_R0_CFG_0__ADR  - DDR_DQ_DQS_TX_SDR_M0_R0_CFG_0__ADR)
#define SDR_DQS_FC_DLY__OFFSET          (DDR_DQ_DQS_TX_SDR_FC_DLY_M0_R0_CFG_0__ADR - DDR_DQ_DQS_TX_SDR_M0_R0_CFG_0__ADR)
// CA
#define SDR_CA_PIPE_EN__OFFSET          (DDR_CA_DQ_TX_SDR_M0_R0_CFG_0__ADR        - DDR_CA_DQ_TX_SDR_M0_R0_CFG_0__ADR)
#define SDR_CA_X_SEL__OFFSET            (DDR_CA_DQ_TX_SDR_X_SEL_M0_R0_CFG_0__ADR  - DDR_CA_DQ_TX_SDR_M0_R0_CFG_0__ADR)
#define SDR_CA_FC_DLY__OFFSET           (DDR_CA_DQ_TX_SDR_FC_DLY_M0_R0_CFG_0__ADR - DDR_CA_DQ_TX_SDR_M0_R0_CFG_0__ADR)
// CK
#define SDR_CK_PIPE_EN__OFFSET          (DDR_CA_DQS_TX_SDR_M0_R0_CFG_0__ADR        - DDR_CA_DQS_TX_SDR_M0_R0_CFG_0__ADR)
#define SDR_CK_X_SEL__OFFSET            (DDR_CA_DQS_TX_SDR_X_SEL_M0_R0_CFG_0__ADR  - DDR_CA_DQS_TX_SDR_M0_R0_CFG_0__ADR)
#define SDR_CK_FC_DLY__OFFSET           (DDR_CA_DQS_TX_SDR_FC_DLY_M0_R0_CFG_0__ADR - DDR_CA_DQS_TX_SDR_M0_R0_CFG_0__ADR)

// Mode, rank, bit offsets
// DQ
#define SDR_DQ_BIT__OFFSET              (DDR_DQ_DQ_TX_SDR_M0_R0_CFG_1__ADR - DDR_DQ_DQ_TX_SDR_M0_R0_CFG_0__ADR)
#define SDR_DQ_RANK__OFFSET             (DDR_DQ_DQ_TX_SDR_M0_R1_CFG_0__ADR - DDR_DQ_DQ_TX_SDR_M0_R0_CFG_0__ADR)
#define SDR_DQ_MODE__OFFSET             (DDR_DQ_DQ_TX_SDR_M1_R0_CFG_0__ADR - DDR_DQ_DQ_TX_SDR_M0_R0_CFG_0__ADR)
// DQS
#define SDR_DQS_BIT__OFFSET             (DDR_DQ_DQS_TX_SDR_M0_R0_CFG_1__ADR - DDR_DQ_DQS_TX_SDR_M0_R0_CFG_0__ADR)
#define SDR_DQS_RANK__OFFSET            (DDR_DQ_DQS_TX_SDR_M0_R1_CFG_0__ADR - DDR_DQ_DQS_TX_SDR_M0_R0_CFG_0__ADR)
#define SDR_DQS_MODE__OFFSET            (DDR_DQ_DQS_TX_SDR_M1_R0_CFG_0__ADR - DDR_DQ_DQS_TX_SDR_M0_R0_CFG_0__ADR)
// CA
#define SDR_CA_BIT__OFFSET              (DDR_CA_DQ_TX_SDR_M0_R0_CFG_1__ADR - DDR_CA_DQ_TX_SDR_M0_R0_CFG_0__ADR)
#define SDR_CA_RANK__OFFSET             (DDR_CA_DQ_TX_SDR_M0_R1_CFG_0__ADR - DDR_CA_DQ_TX_SDR_M0_R0_CFG_0__ADR)
#define SDR_CA_MODE__OFFSET             (DDR_CA_DQ_TX_SDR_M1_R0_CFG_0__ADR - DDR_CA_DQ_TX_SDR_M0_R0_CFG_0__ADR)
// CK
#define SDR_CK_BIT__OFFSET              (0)
#define SDR_CK_RANK__OFFSET             (DDR_CA_DQS_TX_SDR_M0_R1_CFG_0__ADR - DDR_CA_DQS_TX_SDR_M0_R0_CFG_0__ADR)
#define SDR_CK_MODE__OFFSET             (DDR_CA_DQS_TX_SDR_M1_R0_CFG_0__ADR - DDR_CA_DQS_TX_SDR_M0_R0_CFG_0__ADR)

/**
 * @brief   SDR Pipeline Initialize Register Interface
 *
 * @details Initializes SDR Pipeline device at the driver level.
 *
 * @param[in]   sdr_pipeline    pointer to SDR Pipeline device.
 * @param[in]   base            base address of the SDR Pipeline device.
 * @param[in]   slice_type      type of slice of the SDR Pipeline device.
 * @param[in]   rank            rank of the SDR Pipeline device.
 * @param[in]   bit             bit index of the SDR Pipeline device.
 *
 * @return      void
 */
void sdr_pipeline_init_reg_if(sdr_pipeline_dev_t *sdr_pipeline,
                              uint32_t base,
                              wddr_slice_type_t slice_type,
                              wddr_rank_t rank,
                              uint8_t bit);

/**
 * @brief   SDR Pipeline Set MSR Register Interface
 *
 * @details Configures SDR Pipeline device for given Mode Set Register.
 *
 * @param[in]   sdr_pipeline    pointer to SDR Pipeline device.
 * @param[in]   msr             mode set register value to configure for.
 *
 * @return      void
 */
void sdr_pipeline_set_msr_reg_if(sdr_pipeline_dev_t *sdr_pipeline,
                                 wddr_msr_t msr);

/**
 * @brief   SDR Pipeline Set Pipe Enable Register Interface
 *
 * @details Sets Pipeline Enable state for all phases for the SDR Pipeline
 *          device via CSR.
 *
 * @param[in]   sdr_pipeline    pointer to SDR Pipeline device.
 * @param[in]   slice_type      type of slice of the SDR Pipeline device.
 * @param[in]   pipe_en         pipe enable value to set.
 *
 * @return      void
 */
void sdr_pipeline_set_pipe_en_reg_if(sdr_pipeline_dev_t *sdr_pipeline,
                                     wddr_slice_type_t slice_type,
                                     uint32_t pipe_en);

/**
 * @brief   SDR Pipeline Set X SEL Register Interface
 *
 * @details Sets Phase Select (X SEL) state for the SDR Pipeline
 *          device via CSR.
 *
 * @param[in]   sdr_pipeline    pointer to SDR Pipeline device.
 * @param[in]   slice_type      type of slice of the SDR Pipeline device,
 * @param[in]   x_sel           phase select value to set.
 *
 * @return      void
 */
void sdr_pipeline_set_x_sel_reg_if(sdr_pipeline_dev_t *sdr_pipeline,
                                   wddr_slice_type_t slice_type,
                                   uint32_t x_sel);


/**
 * @brief   SDR Pipeline Set Full Cycle Delay Register Interface
 *
 * @details Sets Full Cycle Delay for SDR Pipeline device via CSR.
 *
 * @param[in]   sdr_pipeline    pointer to SDR Pipeline device.
 * @param[in]   slice_type      type of slice of the SDR Pipeline device.
 * @param[in]   fc_delay        full cycle delay to set.
 *
 * @return      void
 */
void sdr_pipeline_set_fc_delay_reg_if(sdr_pipeline_dev_t *sdr_pipeline,
                                      wddr_slice_type_t slice_type,
                                      uint32_t fc_delay);

#endif /* _PIPELINE_SDR_DRIVER_H_ */
