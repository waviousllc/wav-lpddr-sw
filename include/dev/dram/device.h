/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _DRAM_DEV_H_
#define _DRAM_DEV_H_

#include <stdint.h>
#include <dram/table.h>
#include <dfi/buffer_device.h>

/**
 * @brief   DRAM Device Structure
 *
 * @details Structure representing DRAM device. This device is meant to
 *          represent the state of the DRAM when software is in control of
 *          the DFI interface. It doesn't reflect the state of the DRAM when
 *          the DFI interface is controlled via Memory Controller. Typically,
 *          this device is only used during DRAM training.
 *
 * mr2      Stores current value of MR2 in DRAM.
 * mr13     Stores current value of MR13 in DRAM.
 */
typedef struct dram_dev_t
{
    uint8_t mr2;
    uint8_t mr13;
} dram_dev_t;

/**
 * @brief   DRAM Initialization
 *
 * @details Initializes the DRAM device at the device level.
 *
 * @param[in]   dram    pointer to DRAM device.
 *
 * @return      void
 */
void dram_init(dram_dev_t *dram);

/**
 * @brief   DRAM Power Down
 *
 * @details Performs a Power Down sequence for DRAM using DFI Buffer.
 *
 * @param[in]   dram            pointer to DRAM device.
 * @param[in]   dfi_buffer      pointer to DFI Buffer device.
 *
 * @return      void
 */
void dram_power_down(dram_dev_t *dram, dfi_buffer_dev_t *dfi_buffer);

/**
 * @brief   DRAM Idle
 *
 * @details Puts DRAM into IDLE state using DFI Buffer.
 *
 * @param[in]   dram            pointer to DRAM device.
 * @param[in]   dfi_buffer      pointer to DFI Buffer device.
 *
 * @return      void
 */
void dram_idle(dram_dev_t *dram, dfi_buffer_dev_t *dfi_buffer);

/**
 * @brief   DRAM Frequency Innitialization
 *
 * @details Initializes DRAM for a given frequency by writing required Mode
 *          Register values.
 *
 *
 * @param[in]   dram            pointer to DRAM device.
 * @param[in]   dfi_buffer      pointer to DFI Buffer device.
 * @param[in]   dram_cfg        pointer to DRAM Frequency Configuration data.
 * @param[in]   dram_cal        pointer to DRAM Frequency Calibration data.
 *
 * @return      void
 */
void dram_frequency_init(dram_dev_t *dram,
                         dfi_buffer_dev_t *dfi_buffer,
                         dram_freq_cfg_t *dram_cfg,
                         dram_freq_cal_t *dram_cal);

/**
 * @brief   DRAM Command Bus Training (CBT) Enter
 *
 * @details Puts DRAM into Command Bus Training mode using DFI Buffer.
 *
 * @note    cfg parameter must be initialized for current frequency prior to
 *          calling this function.
 *
 * @param[in]   dram            pointer to DRAM device.
 * @param[in]   dfi_buffer      pointer to DFI Buffer device.
 * @param[in]   cfg             pointer to DRAM Frequency Configuration data.
 *
 * @return      void
 */
void dram_cbt_enter(dram_dev_t *dram,
                    dfi_buffer_dev_t *dfi_buffer,
                    dram_freq_cfg_t *cfg);

/**
 * @brief   DRAM Command Bus Training (CBT) Exit
 *
 * @details Takes DRAM out of Command Bus Training mode using DFI Buffer.
 *
 * @note    cfg parameter must be initialized for current frequency prior to
 *          calling this function.
 *
 * @param[in]   dram            pointer to DRAM device.
 * @param[in]   dfi_buffer      pointer to DFI Buffer device.
 * @param[in]   cfg             pointer to DRAM Frequency Configuration data.
 *
 * @return      void
 */
void dram_cbt_exit(dram_dev_t *dram,
                   dfi_buffer_dev_t *dfi_buffer,
                   dram_freq_cfg_t *cfg);

/**
 * @brief   DRAM Vref Current Generator(VRCG) Enable
 *
 * @details Enables VRCG in DRAM using DFI Buffer.
 *
 *
 * @param[in]   dram            pointer to DRAM device.
 * @param[in]   dfi_buffer      pointer to DFI Buffer device.
 * @param[in]   cfg             pointer to DRAM Frequency Configuration data.
 *
 * @return      void
 */
void dram_vrcg_enable(dram_dev_t *dram,
                      dfi_buffer_dev_t *dfi_buffer,
                      dram_freq_cfg_t *cfg);

/**
 * @brief   DRAM Vref Current Generator(VRCG) Disable
 *
 * @details Disables VRCG in DRAM using DFI Buffer.
 *
 *
 * @param[in]   dram            pointer to DRAM device.
 * @param[in]   dfi_buffer      pointer to DFI Buffer device.
 * @param[in]   cfg             pointer to DRAM Frequency Configuration data.
 *
 * @return      void
 */
void dram_vrcg_disable(dram_dev_t *dram,
                       dfi_buffer_dev_t *dfi_buffer,
                       dram_freq_cfg_t *cfg);

/**
 * @brief   DRAM Write Mode Register 13
 *
 * @details Sets the value of Mode Register 13 in DRAM using DFI Buffer.
 *
 * @param[in]   dram            pointer to DRAM device.
 * @param[in]   dfi_buffer      pointer to DFI Buffer device.
 * @param[in]   cfg             pointer to DRAM Frequency Configuration data.
 * @param[in]   mr13            the value of MR13 to set.
 *
 * @return      void
 */
void dram_write_mode_register_13(dram_dev_t *dram,
                                 dfi_buffer_dev_t *dfi_buffer,
                                 dram_freq_cfg_t *cfg,
                                 uint8_t mr13);

/**
 * @brief   DRAM Write Mode Register 2
 *
 * @details Sets the value of Mode Register 2 in DRAM using DFI Buffer.
 *
 * @param[in]   dram            pointer to DRAM device.
 * @param[in]   dfi_buffer      pointer to DFI Buffer device.
 * @param[in]   cfg             pointer to DRAM Frequency Configuration data.
 * @param[in]   mr13            the value of MR2 to set.
 *
 * @return      void
 */
void dram_write_mode_register_2(dram_dev_t *dram,
                                dfi_buffer_dev_t *dfi_buffer,
                                dram_freq_cfg_t *cfg,
                                uint8_t mr2);

/**
 * @brief   DRAM Get Read Enable Offset Timing
 *
 * @details Helper function for extracting Read Enable Offset Timing for a
 *          given Frequency Configuration.
 *
 * @param[in]   dram_cfg    pointer to DRAM Frequency Configuration.
 * @param[out]  offset      pointer to where to store timing value.
 *
 * @return      void
 */
void dram_get_read_enable_offset_timing(dram_freq_cfg_t *dram_cfg,
                                        uint8_t *offset);

/**
 * @brief   DRAM Get Write Enable Offset Timing
 *
 * @details Helper function for extracting Write Enable Offset Timing for a
 *          given Frequency Configuration.
 *
 * @param[in]   dram_cfg    pointer to DRAM Frequency Configuration.
 * @param[out]  offset      pointer to where to store timing value.
 *
 * @return      void
 */
void dram_get_write_enable_offset_timing(dram_freq_cfg_t *dram_cfg,
                                         uint8_t *offset);

/**
 * @brief   DRAM Get Write Data Offset Timing
 *
 * @details Helper function for extracting Write Data Offset Timing for a
 *          given Frequency Configuration.
 *
 * @param[in]   dram_cfg    pointer to DRAM Frequency Configuration.
 * @param[out]  offset      pointer to where to store timing value.
 *
 * @return      void
 */
void dram_get_write_data_offset_timing(dram_freq_cfg_t *dram_cfg,
                                       uint8_t *offset);

#endif /* _DRAM_DEV_H_ */
