/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DRAM_DEV_H_
#define _DRAM_DEV_H_

#include <stdint.h>
#include <dram/table.h>
#include <dfi/buffer.h>

/**
 * @brief   DRAM Device Structure
 *
 * @details Structure representing DRAM device. This device is meant to
 *          represent the state of the DRAM when software is in control of
 *          the DFI interface. It doesn't reflect the state of the DRAM when
 *          the DFI interface is controlled via Memory Controller. Typically,
 *          this device is only used during DRAM training.
 *
 * mr13     Stores current value of MR13 in DRAM.
 * cfg      Pointer to current DRAM freq cfg structure.
 */
typedef struct dram_dev_t
{
    uint8_t         mr13;
    dram_freq_cfg_t *cfg;
} dram_dev_t;

/**
 * @brief   DRAM Initialization
 *
 * @details Initializes the DRAM device at the device level.
 *
 * @param[in]   dram        pointer to DRAM device.
 * @param[in]   dram_cfg    pointer to DRAM Frequency Configuration data.
 *
 * @return      void
 */
void dram_init(dram_dev_t *dram,
               dram_freq_cfg_t *dram_cfg);

/**
 * @brief   DRAM Frequency Set
 *
 * @details Sets frequency specific config and calibration in DRAM device
 *
 * @param[in]   dram        pointer to DRAM device.
 * @param[in]   dram_cfg    pointer to DRAM Frequency Configuration data.
 *
 * @return      void
 */
void dram_frequency_set(dram_dev_t *dram,
                        dram_freq_cfg_t *dram_cfg);

/**
 * @brief   DRAM Power Down
 *
 * @details Performs a Power Down sequence for DRAM using DFI Buffer.
 *
 * @param[in]   dram    pointer to DRAM device.
 * @param[in]   dfi     pointer to DFI device.
 *
 * @return      void
 */
void dram_power_down(dram_dev_t *dram, dfi_dev_t *dfi);

/**
 * @brief   DRAM Idle
 *
 * @details Puts DRAM into IDLE state using DFI Buffer.
 *
 * @param[in]   dram    pointer to DRAM device.
 * @param[in]   dfi     pointer to DFI device.
 *
 * @return      void
 */
void dram_idle(dram_dev_t *dram, dfi_dev_t *dfi);

/**
 * @brief   DRAM Frequency Innitialization
 *
 * @details Initializes DRAM for a given frequency by writing required Mode
 *          Register values.
 *
 *
 * @param[in]   dram        pointer to DRAM device.
 * @param[in]   dfi         pointer to DFI device.
 * @param[in]   dram_cfg    pointer to DRAM Frequency Configuration data.
 *
 * @return      void
 */
void dram_frequency_init(dram_dev_t *dram,
                         dfi_dev_t *dfi,
                         dram_freq_cfg_t *dram_cfg);

/**
 * @brief   DRAM Command Bus Training (CBT) Enter
 *
 * @details Puts DRAM into Command Bus Training mode using DFI Buffer.
 *
 * @note    cfg parameter must be initialized for current frequency prior to
 *          calling this function.
 *
 * @param[in]   dram    pointer to DRAM device.
 * @param[in]   dfi     pointer to DFI device.
 *
 * @return      void
 */
void dram_cbt_enter(dram_dev_t *dram,
                    dfi_dev_t *dfi);

/**
 * @brief   DRAM Command Bus Training (CBT) Exit
 *
 * @details Takes DRAM out of Command Bus Training mode using DFI Buffer.
 *
 * @note    cfg parameter must be initialized for current frequency prior to
 *          calling this function.
 *
 * @param[in]   dram    pointer to DRAM device.
 * @param[in]   dfi     pointer to DFI device.
 *
 * @return      void
 */
void dram_cbt_exit(dram_dev_t *dram,
                   dfi_dev_t *dfi);

/**
 * @brief   DRAM FSP WR Set
 *
 * @details Sets desired FSP-WR value in DRAM.
 *
 * @param[in]   dram    pointer to DRAM device.
 * @param[in]   dfi     pointer to DFI device.
 * @param[in]   fsp     desired FSP to set WR to.
 *
 * @return      void
 */
void dram_set_fsp_wr(dram_dev_t *dram,
                     dfi_dev_t *dfi,
                     uint8_t fsp);

/**
 * @brief   DRAM FSP OP Set
 *
 * @details Sets desired FSP-OP value in DRAM.
 *
 * @note    This will also set FSP-WR to same FSP as FSP-OP.
 *
 * @param[in]   dram    pointer to DRAM device.
 * @param[in]   dfi     pointer to DFI device.
 * @param[in]   fsp     desired FSP to set WR to
 *
 * @return      void
 */
void dram_set_fsp_op(dram_dev_t *dram,
                     dfi_dev_t *dfi,
                     uint8_t fsp);

/**
 * @brief   DRAM DQ VREF Set
 *
 * @details Sets desired DQ VREF value in DRAM.
 *
 * @param[in]   dram            pointer to DRAM device.
 * @param[in]   dfi             pointer to DFI device.
 * @param[in]   vref_setting    desired DQ VREF value to set.
 *
 * @return      void
 */
void dram_set_dq_vref(dram_dev_t *dram,
                      dfi_dev_t *dfi,
                      uint8_t vref_setting);

/**
 * @brief   DRAM Write Level Training Enable
 *
 * @details Enables Write Level Training in DRAM using DFI Buffer.
 *
 *
 * @param[in]   dram    pointer to DRAM device.
 * @param[in]   dfi     pointer to DFI device.
 *
 * @return      void
 */
void dram_wrlvl_enable(dram_dev_t *dram,
                       dfi_dev_t *dfi);

/**
 * @brief   DRAM Write Level Training Disable
 *
 * @details Disables Write Level Training in DRAM using DFI Buffer.
 *
 *
 * @param[in]   dram    pointer to DRAM device.
 * @param[in]   dfi     pointer to DFI device.
 *
 * @return      void
 */
void dram_wrlvl_disable(dram_dev_t *dram,
                        dfi_dev_t *dfi);

/**
 * @brief   DRAM Vref Current Generator(VRCG) Enable
 *
 * @details Enables VRCG in DRAM using DFI Buffer.
 *
 *
 * @param[in]   dram    pointer to DRAM device.
 * @param[in]   dfi     pointer to DFI device.
 *
 * @return      void
 */
void dram_vrcg_enable(dram_dev_t *dram,
                      dfi_dev_t *dfi);

/**
 * @brief   DRAM Vref Current Generator(VRCG) Disable
 *
 * @details Disables VRCG in DRAM using DFI Buffer.
 *
 *
 * @param[in]   dram    pointer to DRAM device.
 * @param[in]   dfi     pointer to DFI device.
 *
 * @return      void
 */
void dram_vrcg_disable(dram_dev_t *dram,
                       dfi_dev_t *dfi);

/**
 * @brief   DRAM Write Mode Register 13
 *
 * @details Sets the value of Mode Register 13 in DRAM using DFI Buffer.
 *
 * @param[in]   dram    pointer to DRAM device.
 * @param[in]   dfi     pointer to DFI device.
 * @param[in]   mr13    the value of MR13 to set.
 *
 * @return      void
 */
void dram_write_mode_register_13(dram_dev_t *dram,
                                 dfi_dev_t *dfi,
                                 uint8_t mr13);

/**
 * @brief   DRAM Write Mode Register 14
 *
 * @details Sets the value of Mode Register 14 in DRAM using DFI Buffer.
 *
 * @param[in]   dram    pointer to DRAM device.
 * @param[in]   dfi     pointer to DFI device.
 * @param[in]   mr14    the value of MR13 to set.
 *
 * @return      void
 */
void dram_write_mode_register_14(dram_dev_t *dram,
                                 dfi_dev_t *dfi,
                                 uint8_t mr14);

/**
 * @brief   DRAM Write Mode Register 2
 *
 * @details Sets the value of Mode Register 2 in DRAM using DFI Buffer.
 *
 * @param[in]   dram    pointer to DRAM device.
 * @param[in]   dfi     pointer to DFI device.
 * @param[in]   mr2     the value of MR2 to set.
 *
 * @return      void
 */
void dram_write_mode_register_2(dram_dev_t *dram,
                                dfi_dev_t *dfi,
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

/**
 * @brief   DRAM Prepare Command Bus Training Packet Sequence
 *
 * @details Creates a set of packets in order to send the necessary DRAM signals
 *          specifying VREF setting to use and command address and clock signals
 *          during command bus training.
 *
 * @note    This will always create the packets at time offset of 1 from
 *          timestamp of last packet in the buffer.
 *
 * @note    This command is expected to be used with an empty buffer.
 *
 * @param[in]   dram            pointer to DRAM device.
 * @param[in]   buffer          pointer to packet buffer to save packet to.
 * @param[in]   cs              chipselect value.
 * @param[in]   vref_setting    VREF value to send via DQ bits during CBT.
 * @param[in]   command_address command address value used to train.
 *
 * @return      returns whether packet added to the packet buffer.
 * @retval      WDDR_SUCCESS if added.
 * @retval      WDDR_ERROR otherwise.
 */
wddr_return_t dram_prepare_cbt_sequence(dram_dev_t *dram,
                                        dfi_tx_packet_buffer_t *buffer,
                                        chipselect_t cs,
                                        uint8_t vref_setting,
                                        uint8_t command_address);

/**
 * @brief   DRAM Prepare Write FIFO Packet Sequence
 *
 * @details Creates a set of packets in order to write data into the DRAM
 *          FIFO for reading without disrupting array contents.
 *
 * @note    This will always create the packets at time offset of 1 from
 *          timestamp of last packet in the buffer.
 *
 * @note    This command is expected to be used with an empty buffer.
 *
 * @param[in]   dram                pointer to DRAM device.
 * @param[in]   buffer              pointer to packet buffer to save packet to.
 * @param[in]   burst_length        data burst length of the DRAM.
 * @param[in]   cs                  chipselect value.
 * @param[in]   wrdata_en_offset    write enable delay value to be used.
 * @param[in]   data                pointer to command data structure to send.
 *
 * @return      returns whether packet added to the packet buffer.
 * @retval      WDDR_SUCCESS if added.
 * @retval      WDDR_ERROR otherwise.
 */
wddr_return_t dram_prepare_wrfifo_sequence(dram_dev_t *dram,
                                           dfi_tx_packet_buffer_t *buffer,
                                           burst_length_t burst_length,
                                           chipselect_t cs,
                                           uint16_t wrdata_en_offset,
                                           command_data_t *data);

/**
 * @brief   DRAM Prepare Read DQ Calibration Training Packet Sequence
 *
 * @details Creates a set of packets in order to send the Read DQ Calibration
 *          Training sequence to the DRAM.
 *
 * @note    This will always create the packets at time offset of 1 from
 *          timestamp of last packet in the buffer.
 *
 * @note    This command is expected to be used with an empty buffer.
 *
 * @param[in]   dram                pointer to DRAM device.
 * @param[in]   buffer              pointer to packet buffer to save packet to.
 * @param[in]   burst_length        data burst length of the DRAM.
 * @param[in]   cs                  chipselect value.
 * @param[in]   rddata_en_offset    read enable delay value to be used.
 *
 * @return      returns whether packet added to the packet buffer.
 * @retval      WDDR_SUCCESS if added.
 * @retval      WDDR_ERROR otherwise.
 */
wddr_return_t dram_prepare_rddq_sequence(dram_dev_t *dram,
                                         dfi_tx_packet_buffer_t *buffer,
                                         burst_length_t burst_length,
                                         chipselect_t cs,
                                         uint16_t rddata_en_offset);

/**
 * @brief   DRAM Prepare Read FIFO Packet Sequence
 *
 * @details Creates a set of packets in order to read data from the DRAM
 *          FIFO.
 *
 * @note    This will always create the packets at time offset of WRITE_LATENCY
 *          + BL + TWTR + 1 from timestamp of last packet in the buffer.
 *
 * @param[in]   dram                pointer to DRAM device.
 * @param[in]   buffer              pointer to packet buffer to save packet to.
 * @param[in]   burst_length        data burst length of the DRAM.
 * @param[in]   cs                  chipselect value.
 * @param[in]   rddata_en_offset    read enable delay value to be used.
 *
 * @return      returns whether packet added to the packet buffer.
 * @retval      WDDR_SUCCESS if added.
 * @retval      WDDR_ERROR otherwise.
 */
wddr_return_t dram_prepare_rdfifo_sequence(dram_dev_t *dram,
                                           dfi_tx_packet_buffer_t *buffer,
                                           burst_length_t burst_length,
                                           chipselect_t cs,
                                           uint16_t rddata_en_offset);

/**
 * @brief   DRAM Prepare Write Level Training Packet Sequence
 *
 * @details Creates a set of packets in order to send the specified Write
 *          Level Training sequence to the DRAM.
 *
 * @note    This will always create the packets at time offset of 1 from
 *          timestamp of last packet in the buffer.
 *
 * @note    This command is expected to be used with an empty buffer.
 *
 * @param[in]   dram            pointer to DRAM device.
 * @param[in]   buffer          pointer to packet buffer to save packet to.
 *
 * @return      returns whether packet added to the packet buffer.
 * @retval      WDDR_SUCCESS if added.
 * @retval      WDDR_ERROR otherwise.
 */
wddr_return_t dram_prepare_wrlvl_sequence(dram_dev_t *dram,
                                          dfi_tx_packet_buffer_t *buffer);

/**
 * @brief   DRAM Prepare MRW Update Packet Sequence
 *
 * @details Creates a set of packets in order to send the specified MRW
 *          update sequence to the DRAM.
 *
 * @note    This will always create the packets at time offset of 1 from
 *          timestamp of last packet in the buffer.
 *
 * @note    This command is expected to be used with an empty buffer.
 *
 * @param[in]   dram        pointer to DRAM device.
 * @param[in]   buffer      pointer to packet buffer to save packet to.
 * @param[in]   dram_cfg    pointer to DRAM Frequency Configuration data.
 *
 * @return      void.
 */
void dram_prepare_mrw_update(dram_dev_t *dram,
                             dfi_tx_packet_buffer_t *packet_buffer,
                             dram_freq_cfg_t *dram_cfg);

#endif /* _DRAM_DEV_H_ */
