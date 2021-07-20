/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <driver/driver.h>
#include <wddr/phy_defs.h>
#include <wddr/memory_map.h>
#include <kernel/io.h>

// Base Address
#define DRVR_DQ_BASE__ADDR      (DDR_DQ_DQ_TX_IO_M0_CFG_0__ADR)
#define DRVR_DQS_BASE__ADDR     (DDR_DQ_DQS_TX_IO_M0_CFG_0__ADR)
#define DRVR_CA_BASE__ADDR      (DDR_CA_DQ_TX_IO_M0_CFG_0__ADR)
#define DRVR_CK_BASE__ADDR      (DDR_CA_DQS_TX_IO_M0_CFG_0__ADR)

// Rank, Mode Offsets
// DQ
#define DRVR_DQ_BIT__OFFSET     (DDR_DQ_DQ_TX_IO_M0_CFG_1__ADR - DDR_DQ_DQ_TX_IO_M0_CFG_0__ADR)
#define DRVR_DQ_MODE__OFFSET    (DDR_DQ_DQ_TX_IO_M1_CFG_0__ADR - DDR_DQ_DQ_TX_IO_M0_CFG_0__ADR)
// DQS
#define DRVR_DQS_BIT__OFFSET    (DDR_DQ_DQS_TX_IO_M0_CFG_1__ADR - DDR_DQ_DQS_TX_IO_M0_CFG_0__ADR)
#define DRVR_DQS_MODE__OFFSET   (DDR_DQ_DQS_TX_IO_M1_CFG_0__ADR - DDR_DQ_DQS_TX_IO_M0_CFG_0__ADR)
// CA
#define DRVR_CA_BIT__OFFSET     (DDR_CA_DQ_TX_IO_M0_CFG_1__ADR - DDR_CA_DQ_TX_IO_M0_CFG_0__ADR)
#define DRVR_CA_MODE__OFFSET    (DDR_CA_DQ_TX_IO_M1_CFG_0__ADR - DDR_CA_DQ_TX_IO_M0_CFG_0__ADR)
// CK
#define DRVR_CK_BIT__OFFSET     (0)
#define DRVR_CK_MODE__OFFSET    (DDR_CA_DQS_TX_IO_M1_CFG_0__ADR - DDR_CA_DQS_TX_IO_M0_CFG_0__ADR)

void driver_set_msr_reg_if(driver_dev_t *driver,
                           wddr_msr_t msr,
                           wddr_slice_type_t slice_type)
{
    uint32_t base, mode_offset;
    switch(slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            base = DRVR_DQ_BASE__ADDR;
            mode_offset = DRVR_DQ_MODE__OFFSET;
            break;
        case WDDR_SLICE_TYPE_DQS:
            base = DRVR_DQS_BASE__ADDR;
            mode_offset = DRVR_DQS_MODE__OFFSET;
            break;
        case WDDR_SLICE_TYPE_CA:
            base = DRVR_CA_BASE__ADDR;
            mode_offset = DRVR_CA_MODE__OFFSET;
            break;
        case WDDR_SLICE_TYPE_CK:
            base = DRVR_CK_BASE__ADDR;
            mode_offset = DRVR_CK_MODE__OFFSET;
            break;
        default:
            return;
    }

    driver->base = (driver->base & ~WDDR_MEMORY_MAP_REL_MASK) +
                        base + msr * mode_offset;
}

void driver_init_reg_if(driver_dev_t *driver,
                                 uint32_t base,
                                 wddr_slice_type_t slice_type)
{
    driver->base = base;
    driver_set_msr_reg_if(driver, WDDR_MSR_0, slice_type);
}

void driver_set_impedance_reg_if(driver_dev_t *driver,
                                 wddr_slice_type_t slice_type,
                                 uint8_t bit_index,
                                 driver_impedance_t tx,
                                 driver_impedance_t rx)
{
    uint32_t reg_val;
    switch(slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            reg_val = reg_read(driver->base + bit_index * DRVR_DQ_BIT__OFFSET);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_IO_M0_CFG_0_TX_IMPD, tx);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_IO_M0_CFG_0_RX_IMPD, rx);
            reg_write(driver->base + bit_index * DRVR_DQ_BIT__OFFSET, reg_val);
            break;
        case WDDR_SLICE_TYPE_DQS:
            reg_val = reg_read(driver->base + bit_index * DRVR_DQS_BIT__OFFSET);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_IO_M0_CFG_0_TX_IMPD, tx);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_IO_M0_CFG_0_RX_IMPD, rx);
            reg_write(driver->base + bit_index * DRVR_DQS_BIT__OFFSET, reg_val);
            break;
        case WDDR_SLICE_TYPE_CA:
            reg_val = reg_read(driver->base + bit_index * DRVR_CA_BIT__OFFSET);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_TX_IO_M0_CFG_0_TX_IMPD, tx);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_TX_IO_M0_CFG_0_RX_IMPD, rx);
            reg_write(driver->base + bit_index * DRVR_CA_BIT__OFFSET, reg_val);
            break;
        case WDDR_SLICE_TYPE_CK:
            reg_val = reg_read(driver->base + bit_index * DRVR_CK_BIT__OFFSET);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_IO_M0_CFG_0_TX_IMPD, tx);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_IO_M0_CFG_0_RX_IMPD, rx);
            reg_write(driver->base + bit_index * DRVR_CK_BIT__OFFSET, reg_val);
            break;
    }
}

void driver_set_oe_reg_if(driver_dev_t *driver,
                          wddr_slice_type_t slice_type,
                          uint8_t bit_index,
                          bool enable)
{
    uint32_t reg_val;
    switch(slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            reg_val = reg_read(driver->base + bit_index * DRVR_DQ_BIT__OFFSET);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_IO_M0_CFG_0_SW_OVR, enable);
            reg_write(driver->base + bit_index * DRVR_DQ_BIT__OFFSET, reg_val);
            break;
        case WDDR_SLICE_TYPE_DQS:
            reg_val = reg_read(driver->base + bit_index * DRVR_DQS_BIT__OFFSET);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_IO_M0_CFG_0_SW_OVR, enable);
            reg_write(driver->base + bit_index * DRVR_DQS_BIT__OFFSET, reg_val);
            break;
        case WDDR_SLICE_TYPE_CA:
            reg_val = reg_read(driver->base + bit_index * DRVR_CA_BIT__OFFSET);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_TX_IO_M0_CFG_0_SW_OVR, enable);
            reg_write(driver->base + bit_index * DRVR_CA_BIT__OFFSET, reg_val);
            break;
        case WDDR_SLICE_TYPE_CK:
            reg_val = reg_read(driver->base + bit_index * DRVR_CK_BIT__OFFSET);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_IO_M0_CFG_0_SW_OVR, enable);
            reg_write(driver->base + bit_index * DRVR_CK_BIT__OFFSET, reg_val);
            break;
    }
}


void driver_set_override_reg_if(driver_dev_t *driver,
                                wddr_slice_type_t slice_type,
                                uint8_t bit_index,
                                uint8_t sel,
                                uint8_t val_t,
                                uint8_t val_c)
{
    uint32_t reg_val;
    switch(slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            reg_val = reg_read(driver->base + bit_index * DRVR_DQ_BIT__OFFSET);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_IO_M0_CFG_0_OVRD_SEL, sel);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_IO_M0_CFG_0_OVRD_VAL, val_t);
            reg_write(driver->base + bit_index * DRVR_DQ_BIT__OFFSET, reg_val);
            break;
        case WDDR_SLICE_TYPE_DQS:
            reg_val = reg_read(driver->base + bit_index * DRVR_DQS_BIT__OFFSET);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_IO_M0_CFG_0_OVRD_SEL, sel);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_IO_M0_CFG_0_OVRD_VAL_T, val_t);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_IO_M0_CFG_0_OVRD_VAL_C, val_c);
            reg_write(driver->base + bit_index * DRVR_DQS_BIT__OFFSET, reg_val);
            break;
        case WDDR_SLICE_TYPE_CA:
            reg_val = reg_read(driver->base + bit_index * DRVR_CA_BIT__OFFSET);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_TX_IO_M0_CFG_0_OVRD_SEL, sel);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_TX_IO_M0_CFG_0_OVRD_VAL, val_t);
            reg_write(driver->base + bit_index * DRVR_CA_BIT__OFFSET, reg_val);
            break;
        case WDDR_SLICE_TYPE_CK:
            reg_val = reg_read(driver->base + bit_index * DRVR_CK_BIT__OFFSET);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_IO_M0_CFG_0_OVRD_SEL, sel);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_IO_M0_CFG_0_OVRD_VAL_T, val_t);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_IO_M0_CFG_0_OVRD_VAL_C, val_c);
            reg_write(driver->base + bit_index * DRVR_CK_BIT__OFFSET, reg_val);
            break;
    }
}
