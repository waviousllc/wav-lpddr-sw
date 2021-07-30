/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <driver/device.h>
#include <driver/driver.h>
#include <driver/cmn_driver.h>
#include <wddr/phy_config.h>

void driver_cmn_init(driver_cmn_dev_t *driver,
                        uint32_t base,
                        wddr_slice_type_t slice_type,
                        wddr_rank_t rank)
{
    driver_cmn_init_reg_if(driver, base, slice_type, rank);
}

void driver_init(driver_dev_t *driver,
                 uint32_t base,
                 wddr_slice_type_t slice_type)
{
    driver_init_reg_if(driver, base, slice_type);
}

void driver_set_impedance_all_bits(driver_dev_t *driver,
                                   wddr_slice_type_t slice_type,
                                   driver_impedance_t tx,
                                   driver_impedance_t rx)
{
    uint8_t num_bits, bit_index;

    // Loop parameter
    switch(slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            num_bits = WDDR_PHY_DQ_SLICE_NUM;
            break;
        case WDDR_SLICE_TYPE_DQS:
            num_bits = WDDR_PHY_DQS_TXRX_SLICE_NUM;
            break;
        case WDDR_SLICE_TYPE_CA:
            num_bits = WDDR_PHY_CA_SLICE_NUM;
            break;
        case WDDR_SLICE_TYPE_CK:
            num_bits = WDDR_PHY_CK_TXRX_SLICE_NUM;
            break;
        default:
            return;
    }

        // Set all bits
    for (bit_index = 0; bit_index < num_bits; bit_index++)
    {
        driver_set_impedance_reg_if(driver, slice_type, bit_index, tx, rx);
    }
}

void driver_override(driver_dev_t *driver,
                     driver_cfg_t *cfg,
                     wddr_slice_type_t slice_type,
                     uint8_t bit_index,
                     bool output_enable)
{
    driver_set_impedance_reg_if(driver, slice_type, bit_index, cfg->tx_impd, cfg->rx_impd);
    driver_set_override_reg_if(driver, slice_type, bit_index, cfg->override.sel, cfg->override.val_t, cfg->override.val_c);
    driver_set_oe_reg_if(driver, slice_type, bit_index, output_enable);
}

void driver_override_all_bits(driver_dev_t *driver,
                              wddr_slice_type_t slice_type,
                              driver_cfg_t *cfg,
                              bool output_enable)
{
    uint8_t num_bits, bit_index;

    // Loop parameter
    switch(slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            num_bits = WDDR_PHY_DQ_SLICE_NUM;
            break;
        case WDDR_SLICE_TYPE_DQS:
            num_bits = WDDR_PHY_DQS_TXRX_SLICE_NUM;
            break;
        case WDDR_SLICE_TYPE_CA:
            num_bits = WDDR_PHY_CA_SLICE_NUM;
            break;
        case WDDR_SLICE_TYPE_CK:
            num_bits = WDDR_PHY_CK_TXRX_SLICE_NUM;
            break;
        default:
            return;
    }

    // Set all bits
    for (bit_index = 0; bit_index < num_bits; bit_index++)
    {
        driver_override(driver, cfg, slice_type, bit_index, output_enable);
    }
}
