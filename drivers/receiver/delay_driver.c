/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#include <receiver/delay_driver.h>
#include <wddr/memory_map.h>
#include <kernel/io.h>

#define RECEIVER_DISABLE    (0x0)
#define RECEIVER_ENABLE     (0x1)

#define RX_DQS_DQ_BASE__ADDR       (DDR_DQ_DQS_RX_IO_M0_R0_CFG_0__ADR)
// Bit "1" used for LPDDR4
// TODO: Make this support LPDDR5 in future
#define RX_DQS_DQ_BIT              (0x1)

// Bit / Rank / Mode Offsets
#define RX_DQS_DQ_BIT__OFFSET      (DDR_DQ_DQS_RX_IO_M0_R0_CFG_1__ADR - DDR_DQ_DQS_RX_IO_M0_R0_CFG_0__ADR)
#define RX_DQS_DQ_RANK__OFFSET     (DDR_DQ_DQS_RX_IO_M0_R1_CFG_0__ADR - DDR_DQ_DQS_RX_IO_M0_R0_CFG_0__ADR)
#define RX_DQS_DQ_MODE__OFFSET     (DDR_DQ_DQS_RX_IO_M1_R0_CFG_0__ADR - DDR_DQ_DQS_RX_IO_M0_R0_CFG_0__ADR)


void receiver_delay_set_msr_reg_if(receiver_delay_dev_t *rx_delay,
                                   wddr_msr_t msr,
                                   wddr_rank_t rank)
{
    // TODO: Update required for LPDDR5
    uint32_t base = rx_delay->base & ~WDDR_MEMORY_MAP_REL_MASK;
    rx_delay->base = base + RX_DQS_DQ_BASE__ADDR +
                          rank * RX_DQS_DQ_RANK__OFFSET +
                          msr * RX_DQS_DQ_MODE__OFFSET +
                          RX_DQS_DQ_BIT * RX_DQS_DQ_BIT__OFFSET;
}

void receiver_delay_init_reg_if(receiver_delay_dev_t *rx_delay,
                                uint32_t base,
                                wddr_rank_t rank)
{
    rx_delay->base = base;
    receiver_delay_set_msr_reg_if(rx_delay, WDDR_MSR_0, rank);
}

void receiver_delay_set_delay_code_reg_if(receiver_delay_dev_t *rx_delay,
                                          uint16_t delay,
                                          receiver_tc_side_mask_t side)
{
    uint32_t reg_val;

    reg_val = reg_read(rx_delay->base);

    if (side & REC_T_SIDE_MASK)
    {
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_M0_R0_CFG_0_DLY_CTRL_T, delay);
    }

    if (side & REC_C_SIDE_MASK)
    {
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_IO_M0_R0_CFG_0_DLY_CTRL_C, delay);
    }

    reg_write(rx_delay->base, reg_val);
}
