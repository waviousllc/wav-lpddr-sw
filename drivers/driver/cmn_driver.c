/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <driver/cmn_driver.h>
#include <wddr/memory_map.h>
#include <kernel/io.h>

// Base Address
#define DRVR_DQS_BASE__ADDR     (DDR_DQ_DQS_TX_IO_CMN_M0_R0_CFG__ADR)
#define DRVR_CK_BASE__ADDR      (DDR_CA_DQS_TX_IO_CMN_M0_R0_CFG__ADR)

// Rank, Mode Offsets
// DQS
#define DRVR_DQS_RANK__OFFSET   (DDR_DQ_DQS_TX_IO_CMN_M0_R1_CFG__ADR - DDR_DQ_DQS_TX_IO_CMN_M0_R0_CFG__ADR)
#define DRVR_DQS_MODE__OFFSET   (DDR_DQ_DQS_TX_IO_CMN_M1_R0_CFG__ADR - DDR_DQ_DQS_TX_IO_CMN_M0_R0_CFG__ADR)
// CK
#define DRVR_CK_RANK__OFFSET    (DDR_CA_DQS_TX_IO_CMN_M0_R1_CFG__ADR - DDR_CA_DQS_TX_IO_CMN_M0_R0_CFG__ADR)
#define DRVR_CK_MODE__OFFSET    (DDR_CA_DQS_TX_IO_CMN_M1_R0_CFG__ADR - DDR_CA_DQS_TX_IO_CMN_M0_R0_CFG__ADR)

void driver_cmn_set_msr_reg_if(driver_cmn_dev_t *driver,
                               wddr_msr_t msr,
                               wddr_slice_type_t slice_type,
                               wddr_rank_t rank)
{
    uint32_t base, mode_offset, rank_offset;

    switch(slice_type)
    {
        case WDDR_SLICE_TYPE_DQS:
            base = DRVR_DQS_BASE__ADDR;
            rank_offset = DRVR_DQS_RANK__OFFSET;
            mode_offset = DRVR_DQS_MODE__OFFSET;
            break;
        case WDDR_SLICE_TYPE_CK:
            base = DRVR_CK_BASE__ADDR;
            rank_offset = DRVR_CK_RANK__OFFSET;
            mode_offset = DRVR_CK_MODE__OFFSET;
            break;
        default:
            return;
    }

    driver->base = (driver->base & ~WDDR_MEMORY_MAP_REL_MASK) + base +
                        rank * rank_offset + msr * mode_offset;

}

void driver_cmn_init_reg_if(driver_cmn_dev_t *driver,
                            uint32_t base,
                            wddr_slice_type_t slice_type,
                            wddr_rank_t rank)
{
    driver->base = base;
    driver_cmn_set_msr_reg_if(driver, WDDR_MSR_0, slice_type, rank);
}

wddr_return_t driver_cmn_set_mode_reg_if(driver_cmn_dev_t *driver,
                                        driver_mode_t mode)
{
    uint32_t reg_val;
    reg_val = reg_read(driver->base);
    switch(mode)
    {
        case DRIVER_MODE_SE:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_IO_CMN_M0_R0_CFG_SE_MODE, 0x1);
            break;
        case DRIVER_MODE_DIFF:
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_IO_CMN_M0_R0_CFG_SE_MODE, 0x0);
            break;
        default:
            return WDDR_ERROR;
    }

    reg_write(driver->base, reg_val);
    return WDDR_SUCCESS;
}

void driver_cmn_set_code_reg_if(driver_cmn_dev_t *driver,
                                uint8_t code[DRVR_PN_CAL_NUM])
{
    uint32_t reg_val;
    reg_val = reg_read(driver->base);

    // Update cal codes
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_IO_CMN_M0_R0_CFG_NCAL, code[DRVR_N_CAL]);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_IO_CMN_M0_R0_CFG_PCAL, code[DRVR_P_CAL]);

    // Update HW
    reg_write(driver->base, reg_val);
}

void driver_cmn_set_loopback_reg_if(driver_cmn_dev_t *driver,
                                    driver_loopback_mode_t mode)
{
    uint32_t reg_val;
    reg_val = reg_read(driver->base);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_IO_CMN_M0_R0_CFG_LPBK_EN, mode);
    reg_write(driver->base, reg_val);
}
