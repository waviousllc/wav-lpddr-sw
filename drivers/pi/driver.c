/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdbool.h>
#include <error.h>
#include <pi/device.h>
#include <kernel/io.h>
#include <wddr/memory_map.h>

void pi_set_state_reg_if(pi_dev_t *pi, pi_state_t state)
{
    uint32_t reg_val;
    reg_val = reg_read(pi->base);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_ODR_PI_M0_R0_CFG_EN, state);
    reg_write(pi->base, reg_val);
}

void pi_set_code_reg_if(pi_dev_t *pi, uint8_t code)
{
    uint32_t reg_val;
    reg_val = reg_read(pi->base);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_ODR_PI_M0_R0_CFG_CODE, code);
    reg_write(pi->base, reg_val);
}

void pi_set_gear_reg_if(pi_dev_t *pi, uint8_t gear)
{
    uint32_t reg_val;
    reg_val = reg_read(pi->base);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_ODR_PI_M0_R0_CFG_GEAR, gear);
    reg_write(pi->base, reg_val);
}

void pi_set_xcpl_reg_if(pi_dev_t *pi, uint8_t xcpl)
{
    uint32_t reg_val;
    reg_val = reg_read(pi->base);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_ODR_PI_M0_R0_CFG_XCPL, xcpl);
    reg_write(pi->base, reg_val);
}

void pi_set_reg_if(pi_dev_t *pi, uint8_t code, uint8_t gear, uint8_t xcpl, pi_state_t state)
{
    uint32_t reg_val = 0;
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_ODR_PI_M0_R0_CFG_CODE, code);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_ODR_PI_M0_R0_CFG_GEAR, gear);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_ODR_PI_M0_R0_CFG_XCPL, xcpl);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_ODR_PI_M0_R0_CFG_EN, state);
    reg_write(pi->base, reg_val);
}
