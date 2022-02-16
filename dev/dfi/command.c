/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <dfi/command.h>
#include <string.h>

#define CS_LOW          (0)
#define CS_HIGH         (1)

#define WR_1_CA_PINS        (0b000100)
#define RD_1_CA_PINS        (0b000010)
#define MPC_1_CA_PINS       (0b100000)
#define WRFIFO_2_CA_PINS    (0b000111)
#define RDFIFO_2_CA_PINS    (0b000001)
#define RDDQ_2_CA_PINS      (0b000011)
#define CAS_2_CA_PINS       (0b010010)
#define MRW_1_CA_PINS       (0b000110)
#define MRW_2_CA_PINS       (0b010110)
#define MRR_1_CA_PINS       (0b001110)
#define SRE_1_CA_PINS       (0b011000)
#define SRX_1_CA_PINS       (0b010100)
#define RFAB_1_CA_PINS      (0b101000)

// Per Section 3.4.1 of JEDEC 209-4C
#define MODE_REGISTER_NUM_MAX   (0x28)

// This is common because all commands that we support use same CS pattern
static void set_command_chipselect(command_frame_t frame[MAX_COMMAND_FRAMES],
                                   chipselect_t cs)
{
    // Chipselect
    frame[0].cs = CS_HIGH << cs;
    frame[1].cs = 0;
    frame[2].cs = CS_HIGH << cs;
    frame[3].cs = 0;
}

static void create_self_refresh_entry_frame(command_frame_t frame[MAX_COMMAND_FRAMES],
                                            chipselect_t cs)
{
    frame[0].ca_pins = SRE_1_CA_PINS;
    frame[1].ca_pins = 0x0;
    frame[2].ca_pins = 0x0;
    frame[3].ca_pins = 0x0;

    frame[0].cs = CS_HIGH << cs;
    frame[1].cs = 0x0;
    frame[2].cs = 0x0;
    frame[3].cs = 0x0;
}

static void create_self_refresh_exit_frame(command_frame_t frame[MAX_COMMAND_FRAMES],
                                           chipselect_t cs)
{
    frame[0].ca_pins = SRX_1_CA_PINS;
    frame[1].ca_pins = 0x0;
    frame[2].ca_pins = 0x0;
    frame[3].ca_pins = 0x0;

    frame[0].cs = CS_HIGH << cs;
    frame[1].cs = 0x0;
    frame[2].cs = 0x0;
    frame[3].cs = 0x0;
}

static void create_refresh_all_banks_frame(command_frame_t frame[MAX_COMMAND_FRAMES],
                                           chipselect_t cs)
{
    frame[0].ca_pins = RFAB_1_CA_PINS;
    frame[1].ca_pins = 0x0;
    frame[2].ca_pins = 0x0;
    frame[3].ca_pins = 0x0;

    frame[0].cs = CS_HIGH << cs;
    frame[1].cs = 0x0;
    frame[2].cs = 0x0;
    frame[3].cs = 0x0;
}

static void create_mode_register_write_frame(command_frame_t frame[MAX_COMMAND_FRAMES],
                                             chipselect_t cs,
                                             uint8_t mode_address,
                                             uint8_t op)
{
    // Update CA Pins for this command
    frame[0].ca_pins = MRW_1_CA_PINS | ((op  & 0x80) >> 2);  // Bit 5 of CA Pins is OP[7]
    frame[1].ca_pins = mode_address & 0x3F;  // MA[5:0] are CA pins
    frame[2].ca_pins = MRW_2_CA_PINS | ((op & 0x40) >> 1);  // Bit 5 of CA Pins is OP[6]
    frame[3].ca_pins = op & 0x3F;  // OP[0:5]

    // Chipselect
    set_command_chipselect(frame, cs);
}

static void create_mode_register_read_frame(command_frame_t frame[MAX_COMMAND_FRAMES],
                                            chipselect_t cs,
                                            uint8_t mode_address)
{
    // NOTE: Per Section 4.46.1 of JEDEC 209-4C, NOTE #8 states C[8:2] assumed 0
    // Update CA Pins for this command
    frame[0].ca_pins = MRR_1_CA_PINS;
    frame[1].ca_pins = mode_address & 0x3F;  // MA[5:0] are CA Pins
    frame[2].ca_pins = CAS_2_CA_PINS; // C8 is assumed zero for MRR
    frame[3].ca_pins = 0;  // C[7:2] assumed zero for MRR

    // Chipselect
    set_command_chipselect(frame, cs);
}

static void create_cbt_write_frame(command_frame_t frame[MAX_COMMAND_FRAMES],
                                   chipselect_t cs,
                                   uint8_t ca_val)
{
    // Update CA Pins for this command
    frame[0].ca_pins = 0x0;
    frame[1].ca_pins = ca_val;
    frame[2].ca_pins = 0x0;
    frame[3].ca_pins = 0x0;

    frame[0].cs = CS_LOW;
    frame[1].cs = CS_HIGH << cs;
    frame[2].cs = CS_LOW;
    frame[3].cs = CS_LOW;
}

static void create_write_frame(command_frame_t frame[MAX_COMMAND_FRAMES],
                               chipselect_t cs,
                               uint8_t bank_address,
                               uint8_t column_address,
                               uint8_t ap,
                               uint8_t bl)
{
    bl = bl == BL_32 ? 1 : 0;
    // Update CA Pins for this command
    frame[0].ca_pins = WR_1_CA_PINS | bl << 5;
    frame[1].ca_pins = (0x7 & bank_address) | ((column_address & 0x80) >> 3) | (ap << 5); //  BA2:0 | V | C9 | AP
    frame[2].ca_pins = CAS_2_CA_PINS | (column_address & 0x40) >> 1;  // C8
    frame[3].ca_pins = column_address & 0x3F;  // C[7:2]

    // Chipselect
    set_command_chipselect(frame, cs);
}

static void create_read_frame(command_frame_t frame[MAX_COMMAND_FRAMES],
                              chipselect_t cs,
                              uint8_t bank_address,
                              uint8_t column_address,
                              uint8_t ap,
                              uint8_t bl)
{
    bl = bl == BL_32 ? 1 : 0;
    // Update CA Pins for this command
    frame[0].ca_pins = RD_1_CA_PINS | bl << 5;
    frame[1].ca_pins = (0x7 & bank_address) | ((column_address & 0x80) >> 3) | (ap << 5); //  BA2:0 | V | C9 | AP
    frame[2].ca_pins = CAS_2_CA_PINS | (column_address & 0x40) >> 1;  // C8
    frame[3].ca_pins = column_address & 0x3F;  // C[7:2]

    // Chipselect
    set_command_chipselect(frame, cs);
}

static void create_mpc_frame(command_frame_t frame[MAX_COMMAND_FRAMES],
                             chipselect_t cs,
                             uint8_t op)
{
    // Update CA Pins for this command
    frame[0].ca_pins = MPC_1_CA_PINS;
    frame[1].ca_pins = op & 0x3F;
    frame[2].ca_pins = CAS_2_CA_PINS;
    frame[3].ca_pins = 0;

    // Chipselect
    set_command_chipselect(frame, cs);
}

void create_write_command(command_t *command,
                          command_data_t *data,
                          burst_length_t bl,
                          chipselect_t cs,
                          uint8_t bank_address,
                          uint8_t column_address,
                          uint8_t ap)
{
    command->command_type = COMMAND_TYPE_WRITE;
    create_write_frame(command->address, cs, bank_address, column_address, ap, bl);
    command->data = data;
}

void create_read_command(command_t *command,
                         burst_length_t bl,
                         chipselect_t cs,
                         uint8_t bank_address,
                         uint8_t column_address,
                         uint8_t ap)
{
    command->command_type = COMMAND_TYPE_READ;
    create_read_frame(command->address, cs, bank_address, column_address, ap, bl);
}

void create_write_mode_register_command(command_t *command,
                                        chipselect_t cs,
                                        uint8_t mode_register,
                                        uint8_t op)
{
    if (mode_register > MODE_REGISTER_NUM_MAX)
    {
        return;
    }

    command->command_type = COMMAND_TYPE_MRW;
    create_mode_register_write_frame(command->address, cs, mode_register, op);
}

void create_read_mode_register_command(command_t *command,
                                       chipselect_t cs,
                                       uint8_t mode_register)
{
    if (mode_register > MODE_REGISTER_NUM_MAX)
    {
        return;
    }

    command->command_type = COMMAND_TYPE_MRR;
    create_mode_register_read_frame(command->address, cs, mode_register);
}

void create_cbt_command(command_t *command,
                        command_data_t *data,
                        chipselect_t cs,
                        uint8_t vref_ca,
                        uint8_t ca_val)
{
    command->command_type = COMMAND_TYPE_CBT;
    create_cbt_write_frame(command->address, cs, ca_val);

    for (uint8_t nn = 0; nn < MAX_DATA_SIZE; nn++)
    {
        data->dq[0][nn] = vref_ca;
    }
    command->data = data;
}

void create_wrfifo_command(command_t *command,
                           command_data_t *data,
                           chipselect_t cs)
{
    command->command_type = COMMAND_TYPE_WRITE;
    create_mpc_frame(command->address, cs, WRFIFO_2_CA_PINS);
    command->data = data;
}

void create_rdfifo_command(command_t *command, chipselect_t cs)
{
    command->command_type = COMMAND_TYPE_READ;
    create_mpc_frame(command->address, cs, RDFIFO_2_CA_PINS);
}

void create_rddq_command(command_t *command, chipselect_t cs)
{
    command->command_type = COMMAND_TYPE_READ;
    create_mpc_frame(command->address, cs, RDDQ_2_CA_PINS);
}

void create_self_refresh_entry_command(command_t *command, chipselect_t cs)
{
    command->command_type = COMMAND_TYPE_SR;
    create_self_refresh_entry_frame(command->address, cs);
}

void create_self_refresh_exit_command(command_t *command, chipselect_t cs)
{
    command->command_type = COMMAND_TYPE_SR;
    create_self_refresh_exit_frame(command->address, cs);
}

void create_refresh_all_banks_command(command_t *command, chipselect_t cs)
{
    command->command_type = COMMAND_TYPE_SR;
    create_refresh_all_banks_frame(command->address, cs);
}

wddr_return_t create_data_frame(command_data_t *data,
                                wddr_dq_byte_t dq_byte,
                                void *src,
                                size_t len,
                                uint8_t offset)
{
    if (data == NULL)
    {
        return WDDR_ERROR;
    }

    offset = offset < MAX_DATA_SIZE ? offset : MAX_DATA_SIZE;
    len = len < MAX_DATA_SIZE - offset ? len : MAX_DATA_SIZE - offset;
    memcpy(&data->dq[dq_byte][offset], src, len);
    return WDDR_SUCCESS;
}
