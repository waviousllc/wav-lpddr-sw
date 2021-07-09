/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _WDDR_TRAIN_H_
#define _WDDR_TRAIN_H_

#include <wddr/device.h>
#include <wddr/eye_algo.h>

// GENERAL DEFINES
#define TRAINING_RESULT_ROW_NUM     (64)

#define PI_MAX                      (64)
#define REN_PI_CODE_MAX             (0x20)

#define DQ_BYTE_0_MASK              (1 << WDDR_DQ_BYTE_0)
#define DQ_BYTE_1_MASK              (1 << WDDR_DQ_BYTE_1)
#define DQ_BYTE_BOTH_MASK           (DQ_BYTE_0_MASK | DQ_BYTE_1_MASK)

// TRAINING ID DEFINES
#define CBT_TRAINING_ID             (1)
#define READ_EYE_TRUE_TRAINING_ID   (2)
#define READ_EYE_COMP_TRAINING_ID   (3)
#define READ_EYE_BOTH_TRAINING_ID   (4)
#define REN_TRAINING_ID             (5)
#define WRLVL_TRAINING_ID           (6)
#define WRITE_EYE_TRAINING_ID       (7)

// CBT TRAINING CONFIG DEFINES
#define CBT_VREF_RANGE_START        (1)
#define CBT_VREF_RANGE_STOP         (2)
#define CBT_VREF_RANGE_STEP         (1)
#define CBT_PI_RANGE_START          (0)
#define CBT_PI_RANGE_STOP           (PI_MAX)
#define CBT_PI_RANGE_STEP           (1)

// READ DQ TRAINING CONFIG DEFINES
#define RDDQ_VREF_RANGE_START       (10)
#define RDDQ_VREF_RANGE_STOP        (11)
#define RDDQ_VREF_RANGE_STEP        (1)
#define RDDQ_RCVR_RANGE_START       (0)
#define RDDQ_RCVR_RANGE_STOP        (0xFF)
#define RDDQ_RCVR_RANGE_STEP        (32)
#define RDDQ_CYC_RANGE_START        (0)
#define RDDQ_CYC_RANGE_STOP         (4)
#define RDDQ_CYC_RANGE_STEP         (1)

// REN TRAINING CONFIG DEFINES
#define REN_CYC_RANGE_START         (0)
#define REN_CYC_RANGE_STOP          (4)
#define REN_CYC_RANGE_STEP          (1)
#define REN_PI_RANGE_START          (0)
#define REN_PI_RANGE_STOP           (REN_PI_CODE_MAX)
#define REN_PI_RANGE_STEP           (4)

// WRITE LEVEL TRAINING CONFIG DEFINES
#define WRLVL_PI_RANGE_START        (0)
#define WRLVL_PI_RANGE_STOP         (PI_MAX)
#define WRLVL_PI_RANGE_STEP         (4)

// DQ / DQS TRAINING CONFIG DEFINES
#define DQ_DQS_VREF_RANGE_START     (0)
#define DQ_DQS_VREF_RANGE_STOP      (50)
#define DQ_DQS_VREF_RANGE_STEP      (50)
#define DQ_DQS_PI_RANGE_START       (0)
#define DQ_DQS_PI_RANGE_STOP        (PI_MAX)
#define DQ_DQS_PI_RANGE_STEP        (4)
#define DQ_DQS_CYC_RANGE_START      (0)
#define DQ_DQS_CYC_RANGE_STOP       (4)
#define DQ_DQS_CYC_RANGE_STEP       (1)

/**
 * @brief   Log Header Structure
 *
 * @details Log header defining training test sweep values
 *
 * training_id  ID of training which was run
 * freq_id      Frequency ID which was trained
 * x_range      X range of sweep performed
 * y_range      Y range of sweep performed
 * x_opt        Optimal X value
 * y_opt        Optimal Y value
 */
typedef struct log_header_t
{
    uint16_t training_id;
    uint16_t freq_id;
    range_t  *x_range;
    range_t  *y_range;
    uint16_t x_opt;
    uint16_t y_opt;
} log_header_t;

/**
 * @brief   Log Entry Structure
 *
 * @details Log entry structure containing training header and results.
 *
 * log_header   log header structure for storing training params.
 * result       pointer to training result array.
 */
typedef struct log_entry_t
{
    log_header_t log_header;
    bit_array_t  *result;
} log_entry_t;

/**
 * @brief   Pad Enable Phase Extension Table
 *
 * @details Phase Extension table for each frequency for optimized signals.
 *
 * @note    This is in DDR tables but duplciated for trainings
 *          because changes to table could break training.
 *
 * ie       input enable value
 * re       read enable value
 */
static const struct
{
    uint8_t ie;
    uint8_t re;
} paden_pext_table[WDDR_PHY_FREQ_NUM] =
{
    {3, 4},     // 422.4
    {3, 4},     // 806.4
    {4, 5},     // 1612.9
    {4, 5},     // 2131.2
};

/**
 * @brief   Convert DRAM Delay To SDR Delay
 *
 * @details Converts input delay in DRAM Clock Domain to delay in SDR Clock Domain.
 *          Indicates if half cycle delay should be used.
 *
 * @note    This only works for DDR signals (DQ, DQS[0:6], CK).
 *
 * TODO: Probably want to make this more general and not SDR specific.
 *
 * @param[inout]    fc_delay    Delay in DRAM clock domain cycles.
 *                              Output as SDR Full cycle delay configuration.
 * @param[in]       ratio       Ratio of SDR Clock to DRAM Clock.
 * @param[out]      pipe_en     Pipe enable delay configuration.
 * @param[out]      x_sel       X select delay configuration.
 *
 * @retval          void.
 */
void convert_dram_delay_to_sdr_delay(uint32_t *fc_delay, wddr_freq_ratio_t ratio, uint32_t *pipe_en, uint32_t *x_sel);

/**
 * @brief   Validate DFI Receive Data
 *
 * @details Verifies if all data received in rx_buffer matches expected result.
 *
 * @param[in]   rx_buffer       pointer to rx_buffer containing received data.
 * @param[in]   dq_byte_mask    DQ Bytes to validate.
 * @param[in]   data_mask       data mask identifying even or odd data to validate.
 * @param[in]   ratio           ratio of DFI to DRAM clock frequency.
 * @param[in]   expected        pointer to array of expected data values.
 * @param[out]  cmp_result      pointer to return result of compare.
 *
 * @return      void.
 */
void validate_dfi_receive_data(dfi_rx_packet_buffer_t *rx_buffer,
                               uint8_t dq_byte_mask,
                               packet_data_mask_t data_mask,
                               uint8_t ratio,
                               command_data_t *expected,
                               bit_array_t *cmp_result);

/**
 * @brief   Reset RX FIFO
 *
 * @details Used to clear and reset WDDR RX FIFO.
 *
 * @param[in]   wddr    pointer to wddr device.
 * @param[in]   channel channel to reset FIFO for.
 *
 * @return      void.
 */
void reset_rx_fifo(wddr_dev_t *wddr, wddr_channel_t channel);

/**
 * @brief   Command Bus Training
 *
 * @details Performs command bus training.
 *
 * @param[in]   wddr    pointer to WDDR device.
 * @param[in]   channel channel to perform training on.
 * @param[in]   freq_id frequency ID to train.
 * @param[out]  result  pointer to array for storing results of training.
 *
 * @return      void.
 */
void command_bus_training(wddr_dev_t *wddr, wddr_channel_t channel, uint8_t freq_id, bit_array_t *result);

/**
 * @brief   Write Level Training
 *
 * @details Performs per DQ byte write level training.
 *
 * @param[in]   wddr    pointer to WDDR device.
 * @param[in]   channel channel to perform training on.
 * @param[in]   dq_byte DQ byte to perform training on.
 * @param[in]   freq_id frequency ID to train.
 * @param[out]  result  pointer to array for storing results of training.
 *
 * @return      void.
 */
void write_level_training(wddr_dev_t *wddr, wddr_channel_t channel, wddr_dq_byte_t dq_byte, uint8_t freq_id, bit_array_t *result);

/**
 * @brief   Read DQ Calibration Training
 *
 * @details Performs per DQ byte read DQ calibration training.
 *
 * @param[in]   wddr                pointer to WDDR device.
 * @param[in]   rec_tc_side_mask    Reciever T/C side mask to train.
 * @param[in]   channel             channel to perform training on.
 * @param[in]   dq_byte             DQ byte to perform training on.
 * @param[in]   freq_id             frequency ID to train.
 * @param[out]  result              pointer to array for storing results of training.
 *
 * @return      void.
 */
void read_dq_training(wddr_dev_t *wddr,
                      receiver_tc_side_mask_t rec_tc_side_mask,
                      wddr_channel_t channel,
                      wddr_dq_byte_t dq_byte,
                      uint8_t freq_id,
                      bit_array_t *result);

/**
 * @brief   REN Training
 *
 * @details Performs per DQ byte REN window training.
 *
 * @param[in]   wddr    pointer to WDDR device.
 * @param[in]   channel channel to perform training on.
 * @param[in]   dq_byte             DQ byte to perform training on.
 * @param[in]   freq_id frequency ID to train
 * @param[out]  result  pointer to array for storing results of training.
 *
 * @return      void.
 */
void ren_training(wddr_dev_t *wddr, wddr_channel_t channel, wddr_dq_byte_t dq_byte, uint8_t freq_id, bit_array_t *result);

/**
 * @brief   DQ-DQS Training
 *
 * @details Performs per DQ byte DQ-DQS training.
 *
 * @param[in]   wddr    pointer to WDDR device.
 * @param[in]   channel channel to perform training on.
 * @param[in]   dq_byte             DQ byte to perform training on.
 * @param[in]   freq_id frequency ID to train
 * @param[out]  result  pointer to array for storing results of training.
 *
 * @return      void.
 */
void dq_dqs_training(wddr_dev_t *wddr, wddr_channel_t channel, wddr_dq_byte_t dq_byte, uint8_t freq_id, bit_array_t *result);

/**
 * @brief   Read Window Training
 *
 * @details Performs entire channel read window training.
 *
 * @param[in]   wddr    pointer to WDDR device.
 * @param[in]   channel channel to perform training on.
 * @param[in]   freq_id frequency ID to train
 * @param[out]  result  pointer to array for storing results of training.
 *
 * @return      void.
 */
void read_window_training(wddr_dev_t *wddr, wddr_channel_t channel, uint8_t freq_id, bit_array_t *result);

/**
 * @brief   Baseline Write/Read Test
 *
 * @details Performs baseline write/read test to ensure training was successful.
 *
 * @param[in]   wddr    pointer to WDDR device.
 * @param[in]   freq_id frequency ID to train
 *
 * @return      Returns if baseline test was successful or not.
 * @retval      WDDR_SUCCESS    If test was successful.
 * @retval      WDDR_ERROR      If test failed.
 */
wddr_return_t baseline_write_read(wddr_dev_t *wddr, uint8_t freq_id);

#endif /* _WDDR_TRAIN_H_ */
