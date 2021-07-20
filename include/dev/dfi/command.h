/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DFI_COMMAND_H_
#define _DFI_COMMAND_H_

#include <stdint.h>
#include <stddef.h>
#include <error.h>
#include <wddr/phy_config.h>

// Can send at most 32 bytes worth of data in single DFI transaction
#define MAX_DATA_SIZE       (32U)
#define MAX_COMMAND_FRAMES  (4)
/**
 * @brief   Command Frame
 *
 * @details Frame for DFI Commands Per Section 4.46 of JEDEC 209-4C
 *
 * cs        chip select values.
 * ca_pins   command address values.
 */
typedef struct command_frame_t
{
    uint8_t cs;
    uint8_t ca_pins;
} command_frame_t;

/**
 * @brief   Command Types
 *
 * @details Supported High Level command types.
 *
 * WRITE_LVL    Write Level Command.
 * READ         Read Command.
 * MRR          Mode Register Read Command.
 * MRW          Mode Register Write Command.
 * CK           Clock Command. (Toggles CK only).
 * CKE          Clock Enable Command. (Holds CKE High and toggles CK).
 * CBT          Command Bus Training Command.
 * SR           Self-Refresh Command.
 */
typedef enum command_type_t
{
    COMMAND_TYPE_WRITE_LVL,
    COMMAND_TYPE_READ,
    COMMAND_TYPE_WRITE,
    COMMAND_TYPE_MRR,
    COMMAND_TYPE_MRW,
    COMMAND_TYPE_CK,
    COMMAND_TYPE_CKE,
    COMMAND_TYPE_CBT,
    COMMAND_TYPE_SR,
    COMMAND_TYPE_TOTAL_NUM,
} command_type_t;

/**
 * @brief   Chip Select Enumerations
 *
 * @details Chip select to specify which DRAM Rank will be selected.
 *
 * CS_0   Chip Select 0.
 * CS_1   Chip Select 1.
 */
typedef enum chipselect_t
{
    CS_0,
    CS_1
} chipselect_t;

/**
 * @brief   Burst Length Enumerations
 *
 * @details Supported Burth Lengths.
 *
 * BL_16   Burst Length of 16.
 * BL_32   Burst Lenght of 32.
 */
typedef enum burst_length_t
{
    BL_16,
    BL_32
} burst_length_t;

/**
 * @brief   Command Data Structure
 *
 * @details Holds Command Data for a single Write Transaction.
 *
 * dq       DQ Byte array.
 * dm       DQ Data Mask array.
 */
typedef struct command_data_t
{
    uint8_t dq[WDDR_PHY_DQ_BYTE_NUM][MAX_DATA_SIZE];
    uint8_t dm[WDDR_PHY_DQ_BYTE_NUM][MAX_DATA_SIZE];
} command_data_t;

/**
 * @brief   Commmand Structure
 *
 * @details High-Level command structure.
 *
 * command_type    command type.
 * cs              chip select.
 * address         command frames.
 * data            pointer to command data associated with command.
 */
typedef struct command_t
{
    command_type_t  command_type;
    command_frame_t address[MAX_COMMAND_FRAMES];
    command_data_t  *data;
} command_t;

/**
 * @brief   Create Write Command
 *
 * @details Creates a Write Command
 *
 * @param[out]  command         pointer to command to fill in.
 * @param[in]   data            pointer to data to write.
 * @param[in]   bl              burst length.
 * @param[in]   cs              chip select.
 * @param[in]   bank_address    bank address.
 * @param[in]   column_address  column address.
 * @param[in]   ap              auto precharge flag.
 *
 * @return      void
 */
void create_write_command(command_t *command,
                          command_data_t *data,
                          burst_length_t bl,
                          chipselect_t cs,
                          uint8_t bank_address,
                          uint8_t column_address,
                          uint8_t ap);

/**
 * @brief   Create MRR Command
 *
 * @details Creates a Mode Register Read Command.
 *
 * @param[out]  command         pointer to command to fill in.
 * @param[in]   cs              chip select.
 * @param[in]   mode_register   which mode register to read.
 *
 * @return      void
 */
void create_read_mode_register_command(command_t *command,
                                       chipselect_t cs,
                                       uint8_t mode_register);

/**
 * @brief   Create MRW Command
 *
 * @details Creates a Mode Register Write Command.
 *
 * @param[out]  command         pointer to command to fill in.
 * @param[in]   cs              chip select.
 * @param[in]   mode_register   which mode register to write.
 * @param[in]   op              op code (write data).
 *
 * @return      void
 */
void create_write_mode_register_command(command_t *command,
                                        chipselect_t cs,
                                        uint8_t mode_register,
                                        uint8_t op);

/**
 * @brief   Create CBT Command
 *
 * @details Creates a CBT Command.
 *
 * @note    Command is not from JEDEC Truth Table. Used for training only.
 *
 * @param[out]  command     pointer to command to fill in.
 * @param[out]   data       pointer to data to store CA VREF value.
 * @param[in]   cs          chip select.
 * @param[in]   ca_vref     vref value for CA.
 * @param[in]   ca_val      value to be sent over CA bus for training.
 *
 * @return      void
 */
void create_cbt_command(command_t *command,
                        command_data_t *data,
                        chipselect_t cs,
                        uint8_t vref_ca,
                        uint8_t ca_val);

/**
 * @brief   Create Write FIFO Command
 *
 * @details Creates a Write FIFO MPC command.
 *
 * @param[out]  command     pointer to command to fill in.
 * @param[in]   data        pointer to data to write.
 * @param[in]   cs          chip select.
 *
 * @return      void
 */
void create_wrfifo_command(command_t *command,
                           command_data_t *data,
                           chipselect_t cs);

/**
 * @brief   Create Read FIFO Command
 *
 * @details Creates a Read FIFO MPC command.
 *
 * @param[out]  command     pointer to command to fill in.
 * @param[in]   cs          chip select.
 *
 * @return      void
 */
void create_rdfifo_command(command_t *command, chipselect_t cs);

/**
 * @brief   Create Read FIFO Command
 *
 * @details Creates a Read DQ calibration MPC command.
 *
 * @param[out]  command     pointer to command to fill in.
 * @param[in]   cs          chip select.
 *
 * @return      void
 */
void create_rddq_command(command_t *command, chipselect_t cs);

/**
 * @brief   Create Self Refresh Entry Command
 *
 * @details Creates a self refresh entry MPC command.
 *
 * @param[out]  command     pointer to command to fill in.
 * @param[in]   cs          chip select.
 *
 * @return      void
 */
void create_self_refresh_entry_command(command_t *command, chipselect_t cs);

/**
 * @brief   Create Self Refresh Exit Command
 *
 * @details Creates a self refresh exit MPC command.
 *
 * @param[out]  command     pointer to command to fill in.
 * @param[in]   cs          chip select.
 *
 * @return      void
 */
void create_self_refresh_exit_command(command_t *command, chipselect_t cs);

/**
 * @brief   Create Refresh All Banks (RAB) Command
 *
 * @details Creates a Refresh All Banks command.
 *
 * @param[out]  command     pointer to command to fill in.
 * @param[in]   cs          chip select.
 *
 * @return      void
 */
void create_refresh_all_banks_command(command_t *command, chipselect_t cs);

/**
 * @brief   Create Data Frame
 *
 * @details Creates a data frame.
 *
 * @param[out]  data        pointer to data to fill in.
 * @param[in]   dq_byte     which DQ byte to fill in.
 * @param[in]   src         source data to copy.
 * @param[in]   len         length of data to copy.
 * @param[in]   offset      offset from source data to start copy.
 *
 * @return      returns whether data was filled in.
 * @retval      WDDR_SUCCESS if successfully filled in.
 * @retval      WDDR_ERROR otherwise.
 */
wddr_return_t create_data_frame(command_data_t *data,
                                wddr_dq_byte_t dq_byte,
                                void *src,
                                size_t len,
                                uint8_t offset);

#endif /* _DFI_COMMAND_H_ */
