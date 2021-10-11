/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* Standard includes. */
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* Kernel includes. */
#include <kernel/io.h>

/* LPDDR includes. */
#include <wddr/device.h>
#include <wddr/driver.h>
#include <wddr/memory_map.h>
#include <wddr/irq_map.h>
#include <channel/device.h>
#include <cmn/device.h>
#include <ctrl/device.h>
#include <dfi/buffer.h>
#include <dram/device.h>
#include <fsw/device.h>

/*******************************************************************************
**                           VARIABLE DECLARATIONS
*******************************************************************************/
packet_item_t packets[56] __attribute__ ((section (".data"))) = {0};

/*******************************************************************************
**                            FUNCTION DECLARATIONS
*******************************************************************************/
/** @brief  Internal Function to enable all PHY LPDEs and Phase Interpolators */
static void wddr_enable(wddr_dev_t *wddr);

/** @brief  Internal Function to flush DFI interfaces using DFI Buffer */
static void wddr_dfi_buffer_flush(dfi_dev_t *dfi);

/**
 * @brief   WDDR DFI Buffer Prime
 *
 * @details Internal Function to prime DFI interface for smooth hand-offs
 *          during frequency switch.
 */
static void wddr_dfi_buffer_prime(dfi_dev_t *dfi);

/** @brief  Internal Function to prepare MRW updates for frequency switch */
static void wddr_prep_freq_switch_mrw_update(wddr_dev_t *wddr,
                                             dfi_dev_t *dfi,
                                             dram_freq_cfg_t *dram_cfg);

/** @brief  Internal Function to clear FIFO for all channels */
static void wddr_clear_fifo_all_channels(wddr_dev_t *wddr);

/** @brief  Internal Function for preparing WDDR PHY for frequency switch */
static void wddr_configure_phy(wddr_dev_t *wddr, uint8_t freq_id, wddr_msr_t msr);

/** @brief  Internal weak declaration of WDDR Training Function */
__attribute__(( weak ))
wddr_return_t wddr_train(wddr_dev_t *wddr);
/*******************************************************************************
**                              IMPLEMENTATIONS
*******************************************************************************/

void wddr_init(wddr_dev_t *wddr, uint32_t base, wddr_table_t *table)
{
    // Early init to get MCU to clk speed
    fsw_init(&wddr->fsw, base);
    cmn_init(&wddr->cmn, base);
    ctrl_init(&wddr->ctrl, base);
    pll_init(&wddr->pll, WDDR_MEMORY_MAP_PLL);

    // Enable Common Block
    cmn_enable(&wddr->cmn);

    // Boot PLL to set MCU clk @ 384 MHz
    pll_boot(&wddr->pll);

    // Enable Clocks
    ctrl_clk_set_pll_clk_en_reg_if(wddr->ctrl.ctrl_reg, true);
    ctrl_clk_set_mcu_gfm_sel_reg_if(wddr->ctrl.ctrl_reg, CLK_MCU_GFM_SEL_PLL_VCO0);
    cmn_clk_ctrl_set_pll0_div_clk_rst_reg_if(wddr->cmn.cmn_reg, false);
    cmn_clk_ctrl_set_gfcm_en_reg_if(wddr->cmn.cmn_reg, true);

    // Turn off PHY CLK gating
    fsw_csp_set_clk_disable_over_val_reg_if(wddr->fsw.fsw_reg, false);
    cmn_clk_ctrl_set_pll0_div_clk_en_reg_if(wddr->cmn.cmn_reg, true);

    // Initialize entire WDDR device
    wddr->is_booted = false;
    wddr->table = table;

    // Channel Configuration
    for (uint8_t channel = 0; channel < WDDR_PHY_CHANNEL_NUM; channel++)
    {

        uint32_t channel_base = base +
                                WDDR_MEMORY_MAP_PHY_CH_START +
                                WDDR_MEMORY_MAP_PHY_CH_OFFSET * channel;

        channel_init(&wddr->channel[channel], channel_base);
    }

    // DFI
    dfi_init(&wddr->dfi, base);

    // DRAM
    dram_init(&wddr->dram,
              &wddr->table->cfg.freq[WDDR_PHY_BOOT_FREQ].dram);


}

wddr_return_t wddr_boot(wddr_dev_t *wddr, wddr_boot_cfg_t cfg)
{
    uint8_t current_vco_id;

    // Calibrate all frequencies
    if (GET_BOOT_OPTION(cfg, WDDR_BOOT_OPTION_PLL_CAL))
    {
        for (uint8_t freq_id = 0; freq_id < WDDR_PHY_VALID_FREQ_NUM; freq_id++)
        {
            pll_calibrate_vco(&wddr->pll,
                              &wddr->table->cfg.freq[freq_id].pll);
        }
    }

    /**
     * @note Override CS / CKE as not sure of state of memory controller
     *       at this point.
     */
    for (uint8_t channel = WDDR_CHANNEL_0; channel < WDDR_PHY_CHANNEL_NUM; channel++)
    {
        driver_cfg_t cfg =
        {
            .tx_impd = DRIVER_IMPEDANCE_240,
            .rx_impd = DRIVER_IMPEDANCE_HIZ,
            .override.sel = DRIVER_IMPEDANCE_240,
            .override.val_t = 0,
            .override.val_c = 0,
        };

        ca_dq_driver_set_impedance_reg_if(wddr->channel[channel].ca_reg, WDDR_MSR_0, CA_SLICE_CS_0, cfg.tx_impd, cfg.rx_impd);
        ca_dq_driver_set_impedance_reg_if(wddr->channel[channel].ca_reg, WDDR_MSR_0, CA_SLICE_CS_1, cfg.tx_impd, cfg.rx_impd);
        ca_dq_driver_set_impedance_reg_if(wddr->channel[channel].ca_reg, WDDR_MSR_0, CA_SLICE_CKE_0, cfg.tx_impd, cfg.rx_impd);
        ca_dq_driver_set_impedance_reg_if(wddr->channel[channel].ca_reg, WDDR_MSR_0, CA_SLICE_CKE_1, cfg.tx_impd, cfg.rx_impd);

        ca_dq_driver_set_override_reg_if(wddr->channel[channel].ca_reg, WDDR_MSR_0, CA_SLICE_CS_0, cfg.override.sel, cfg.override.val_t);
        ca_dq_driver_set_override_reg_if(wddr->channel[channel].ca_reg, WDDR_MSR_0, CA_SLICE_CS_1, cfg.override.sel, cfg.override.val_t);
        ca_dq_driver_set_override_reg_if(wddr->channel[channel].ca_reg, WDDR_MSR_0, CA_SLICE_CKE_0, cfg.override.sel, cfg.override.val_t);
        ca_dq_driver_set_override_reg_if(wddr->channel[channel].ca_reg, WDDR_MSR_0, CA_SLICE_CKE_1, cfg.override.sel, cfg.override.val_t);

        ca_dq_driver_set_oe_reg_if(wddr->channel[channel].ca_reg, WDDR_MSR_0, CA_SLICE_CS_0, true);
        ca_dq_driver_set_oe_reg_if(wddr->channel[channel].ca_reg, WDDR_MSR_0, CA_SLICE_CS_1, true);
        ca_dq_driver_set_oe_reg_if(wddr->channel[channel].ca_reg, WDDR_MSR_0, CA_SLICE_CKE_0, true);
        ca_dq_driver_set_oe_reg_if(wddr->channel[channel].ca_reg, WDDR_MSR_0, CA_SLICE_CKE_1, true);
    }

    // Switch to PHY_BOOT Frequency
    wddr_sw_freq_switch(wddr, WDDR_PHY_BOOT_FREQ, WDDR_MSR_0);

    // Turn on LPDE / Phase Interpolators in PHY
    wddr_enable(wddr);

    // Flush PHY through DFI buffer
    wddr_clear_fifo_all_channels(wddr);
    fsw_csp_sync_reg_if(wddr->fsw.fsw_reg);

    wddr_configure_phy(wddr, WDDR_PHY_BOOT_FREQ, WDDR_MSR_0);

    wddr_dfi_buffer_flush(&wddr->dfi);

    wddr_clear_fifo_all_channels(wddr);

    // Perfrom ZQCAL Calibartion
    if (GET_BOOT_OPTION(cfg, WDDR_BOOT_OPTION_ZQCAL_CAL))
    {
        wddr_iocal_calibrate(wddr);
        wddr_iocal_update_phy(wddr);
    }

    // Set VREF code
    cmn_vref_set_code_reg_if(wddr->cmn.cmn_reg, WDDR_MSR_0, wddr->table->cfg.freq[WDDR_PHY_BOOT_FREQ].common.vref.code);

    // Either calibrate Sense Amps and configure or just configure Sense Amps
    for (uint8_t channel = WDDR_CHANNEL_0; channel < WDDR_PHY_CHANNEL_NUM; channel++)
    {
        // Force chipselect since calibration assumes CS=0
        wddr_set_chip_select_reg_if(wddr, channel, WDDR_RANK_0, true);

        for (uint8_t byte = 0; byte < WDDR_PHY_DQ_BYTE_NUM; byte++)
        {
            rx_gb_cfg_t gb_cfg = {
                .data_mode = DGB_2TO1_IR,
                .fifo_mode = FGB_2TO2,
                .wck_mode = GB_WCK_MODE_0,
            };

            dq_dq_rx_gb_set_mode_reg_if(wddr->channel[channel].dq_reg[byte], WDDR_MSR_0, &gb_cfg);
            dq_dqs_rx_gb_set_mode_reg_if(wddr->channel[channel].dq_reg[byte], WDDR_MSR_0, &gb_cfg);

            // Perform Sense Amp calibration
            PROPAGATE_ERROR(channel_rx_sa_dqbyte_configure(&wddr->channel[channel],
                                                           byte,
                                                           GET_BOOT_OPTION(cfg, WDDR_BOOT_OPTION_SA_CAL) == 1,
                                                           &wddr->table->cfg.common.channel[channel].dq[byte].rx.sa));

            // Reset Gearbox settings
            dq_dq_rx_gb_set_mode_reg_if(wddr->channel[channel].dq_reg[byte],
                                        WDDR_MSR_0,
                                        &wddr->table->cfg.freq[WDDR_PHY_BOOT_FREQ].channel[channel].dq[byte].rx.rank_cmn.cmn.gearbox);

            dq_dqs_rx_gb_set_mode_reg_if(wddr->channel[channel].dq_reg[byte],
                                         WDDR_MSR_0,
                                         &wddr->table->cfg.freq[WDDR_PHY_BOOT_FREQ].channel[channel].dq[byte].rx.rank_cmn.cmn.gearbox);
        } // DQ Byte loop

        // Remove Chip Select Override
        wddr_set_chip_select_reg_if(wddr, channel, WDDR_RANK_0, false);

    } // Channel loop

    if (GET_BOOT_OPTION(cfg, WDDR_BOOT_OPTION_TRAIN_DRAM))
    {
        PROPAGATE_ERROR(wddr_train(wddr));
    }

    // Prime DFI buffer
    wddr_dfi_buffer_prime(&wddr->dfi);
    wddr_clear_fifo_all_channels(wddr);

    // Switch VCOs until on VCO_INDEX_PHY_1
    pll_get_current_vco(&wddr->pll, &current_vco_id);
    while (current_vco_id != VCO_INDEX_PHY_1)
    {
        wddr_sw_freq_switch(wddr, WDDR_PHY_BOOT_FREQ, WDDR_MSR_0);
        pll_get_current_vco(&wddr->pll, &current_vco_id);
    }

    fsw_switch_to_dfi_mode(&wddr->fsw, &wddr->dfi);
    wddr->is_booted = true;
    return WDDR_SUCCESS;
}

wddr_return_t wddr_prep_switch(wddr_dev_t *wddr, uint8_t freq_id, wddr_msr_t msr)
{
    if (freq_id >= WDDR_PHY_FREQ_NUM ||
        !wddr->table->valid[freq_id])
    {
        return WDDR_ERROR;
    }

    // Configure PHY
    wddr_configure_phy(wddr, freq_id, msr);

    // Prepare MRW sequence in DFI Buffer
    wddr_prep_freq_switch_mrw_update(wddr,
                                     &wddr->dfi,
                                     &wddr->table->cfg.freq[freq_id].dram);

    // Prepare PLL
    pll_prepare_vco_switch(&wddr->pll,
                           freq_id,
                           &wddr->table->cfg.freq[freq_id].pll);

    return WDDR_SUCCESS;
}

void wddr_iocal_update_phy(wddr_dev_t *wddr)
{
    uint8_t freq_id;

    // Get current pll frequency id
    pll_get_current_freq(&wddr->pll, &freq_id);

    // Get current msr
    wddr_msr_t msr = fsw_get_current_msr(&wddr->fsw);

    // Update IOCAL
    for (uint8_t channel = 0; channel < WDDR_PHY_CHANNEL_NUM; channel++)
    {
        for (uint8_t rank = 0; rank < WDDR_PHY_RANK; rank++)
        {
            // Update CK
            ca_dqs_driver_set_code_reg_if(wddr->channel[channel].ca_reg,
                msr,
                rank,
                wddr->table->cfg.freq[freq_id].channel[channel].ca.tx.rank[rank].ck.driver.code);

            // Update DQS
            for (uint8_t dq_byte = 0; dq_byte < WDDR_PHY_DQ_BYTE_NUM; dq_byte++)
            {
                dq_dqs_driver_set_code_reg_if(wddr->channel[channel].dq_reg[dq_byte],
                    msr,
                    rank,
                    wddr->table->cfg.freq[freq_id].channel[channel].dq[dq_byte].tx.rank[rank].dqs.driver.code);
            }
        }
    }
}

void wddr_iocal_calibrate(wddr_dev_t *wddr)
{
    __UNUSED__ wddr_return_t ret;
    ret = cmn_zqcal_calibrate(&wddr->cmn, &wddr->table->cfg.common.common.zqcal);
    configASSERT(ret == WDDR_SUCCESS);
}

wddr_return_t wddr_sw_freq_switch(wddr_dev_t *wddr, uint8_t freq_id, wddr_msr_t msr)
{
    uint32_t reg_val;
    uint8_t next_vco;
    pll_dev_t *pll = &wddr->pll;

    // Can only call if wddr hasn't booted
    if (wddr->is_booted)
    {
        return WDDR_ERROR;
    }

    // Don't toggle MSR
    fsw_ctrl_set_msr_toggle_en_reg_if(wddr->fsw.fsw_reg, false);

    // Prepare PLL
    pll_prepare_vco_switch(pll,
                           freq_id,
                           &wddr->table->cfg.freq[freq_id].pll);

    // Override MSR / PLL VCO
    pll_get_next_vco(pll, &next_vco);
    configASSERT(next_vco != UNDEFINED_VCO_ID);
    fsw_ctrl_set_msr_vco_ovr_val_reg_if(wddr->fsw.fsw_reg, msr, next_vco);
    fsw_ctrl_set_msr_vco_ovr_reg_if(wddr->fsw.fsw_reg, true);

    // Switch PLL to new Frequency
    pll_switch_vco(pll, true);

    // Block until PLL is ready
    do
    {
        reg_val = reg_read(pll->base + DDR_MVP_PLL_CORE_STATUS__ADR);
    } while (!GET_REG_FIELD(reg_val, DDR_MVP_PLL_CORE_STATUS_CORE_READY));

    // Put back to normal state
    fsw_ctrl_set_msr_toggle_en_reg_if(wddr->fsw.fsw_reg, true);

    // Disable previous VCO
    pll_disable_vco(pll);

    return WDDR_SUCCESS;
}

static void wddr_enable(wddr_dev_t *wddr)
{
    for (uint8_t channel = WDDR_CHANNEL_0; channel < WDDR_PHY_CHANNEL_NUM; channel++)
    {
        channel_enable(&wddr->channel[channel], true, &wddr->table->cfg.freq[WDDR_PHY_BOOT_FREQ].channel[channel]);
    }
}

static void wddr_dfi_buffer_flush(dfi_dev_t *dfi)
{
    /**
     * @note    For boot procedure, all DFI signals need to be flushed. To do
     *          this, every bit in the packet needs to be enabled. In reality,
     *          such a packet makes no sense. Thus, software abstraction doesn't
     *          allow for this case. Instead, fill in packet buffer manually.
     */
    List_t packet_list;
    packet_item_t packets[2] = {0};

    vListInitialise(&packet_list);

    // Initialize Packets
    memset(&packets[0].packet, 0xFF, sizeof(dfi_tx_packet_t));
    packets[0].packet.packet.time = 0x1;
    packets[1].packet.packet.time = 0x10;

    // WCS disable
    packets[0].packet.packet.wrdata_cs_p0 = 0;
    packets[0].packet.packet.wrdata_cs_p1 = 0;
    packets[0].packet.packet.wrdata_cs_p2 = 0;
    packets[0].packet.packet.wrdata_cs_p3 = 0;
    packets[0].packet.packet.wrdata_cs_p4 = 0;
    packets[0].packet.packet.wrdata_cs_p5 = 0;
    packets[0].packet.packet.wrdata_cs_p6 = 0;
    packets[0].packet.packet.wrdata_cs_p7 = 0;

    // RCS disable
    packets[0].packet.packet.rddata_cs_p0 = 0;
    packets[0].packet.packet.rddata_cs_p1 = 0;
    packets[0].packet.packet.rddata_cs_p2 = 0;
    packets[0].packet.packet.rddata_cs_p3 = 0;
    packets[0].packet.packet.rddata_cs_p4 = 0;
    packets[0].packet.packet.rddata_cs_p5 = 0;
    packets[0].packet.packet.rddata_cs_p6 = 0;
    packets[0].packet.packet.rddata_cs_p7 = 0;

    // Add to list
    vListInitialiseItem(&packets[0].list_item);
    listSET_LIST_ITEM_OWNER(&packets[0].list_item, &packets[0]);
    vListInsertEnd(&packet_list, &packets[0].list_item);
    vListInitialiseItem(&packets[1].list_item);
    listSET_LIST_ITEM_OWNER(&packets[1].list_item, &packets[1]);
    vListInsertEnd(&packet_list, &packets[1].list_item);

    // Send packets to initialize buffer for first time
    dfi_buffer_fill_and_send_packets(dfi, &packet_list);

    // Must disable buffer when done
    dfi_buffer_disable(dfi);
}
static void wddr_dfi_buffer_prime(dfi_dev_t *dfi)
{
    dfi_tx_packet_buffer_t buffer;

    // Prime DFI buffer with CKE sequence
    dfi_tx_packet_buffer_init(&buffer);
    create_cke_packet_sequence(&buffer, 1);
    create_cke_packet_sequence(&buffer, 10);
    dfi_buffer_fill_and_send_packets(dfi, &buffer.list);

    // Free packets
    dfi_tx_packet_buffer_free(&buffer);

    // Must disable buffer when done
    dfi_buffer_disable(dfi);
}

static void wddr_prep_freq_switch_mrw_update(wddr_dev_t *wddr,
                                             dfi_dev_t *dfi,
                                             dram_freq_cfg_t *dram_cfg)
{
    dfi_tx_packet_buffer_t packet_buffer;
    packet_storage_t storage = {
        .packets = packets,
        .len = 56,
        .index = 0,
    };

    // Initialize TX Packet Buffer
    dfi_tx_packet_buffer_init(&packet_buffer);
    packet_buffer.storage = &storage;

    // Allocate and Initialize
    for (uint8_t j = 0; j < storage.len; j++)
    {
        volatile uint32_t *data = storage.packets[j].packet.raw_data;
        uint8_t i = TX_PACKET_SIZE_WORDS;
        while (i--)
        {
            *data++ = 0;
        }
    }

    dram_prepare_mrw_update(&wddr->dram, &packet_buffer, dram_cfg);

    // Prefill packets
    dfi_buffer_fill_packets(dfi, &packet_buffer.list);

    /**
     * @note: No need to call dfi_tx_packet_buffer_free since packet_buffer
     *        is created on the stack and won't be reused.
     */
}

static void wddr_clear_fifo_all_channels(wddr_dev_t *wddr)
{
    for (uint8_t channel = WDDR_CHANNEL_0; channel <= WDDR_CHANNEL_1; channel++)
    {
        wddr_clear_fifo_reg_if(wddr, channel);
    }
}

static void ca_enable_loopback(channel_dev_t *dev, wddr_msr_t msr, driver_cfg_t *cfg)
{
    channel_ca_tx_driver_override_all_bits(dev, WDDR_SLICE_TYPE_CA, msr, cfg, false);
    channel_ca_tx_driver_override_all_bits(dev, WDDR_SLICE_TYPE_CK, msr, cfg, false);

    for (uint8_t rank = 0; rank < WDDR_PHY_RANK; rank++)
    {
        ca_dqs_driver_set_loopback_mode_reg_if(dev->ca_reg, msr, rank, DRIVER_LOOPBACK_MODE_ENABLE);
        ca_dqs_driver_set_mode_reg_if(dev->ca_reg, msr, rank, DRIVER_MODE_DIFF);
    }
}

static void dq_enable_loopback(channel_dev_t *dev, wddr_msr_t msr, wddr_dq_byte_t byte, driver_cfg_t *cfg)
{

    channel_dq_tx_driver_override_all_bits(dev, WDDR_SLICE_TYPE_DQ, msr, byte, cfg, false);
    channel_dq_tx_driver_override_all_bits(dev, WDDR_SLICE_TYPE_DQS, msr, byte, cfg, false);

    for (uint8_t rank = 0; rank < WDDR_PHY_RANK; rank++)
    {
        dq_dqs_driver_set_loopback_mode_reg_if(dev->dq_reg[byte], msr, rank, DRIVER_LOOPBACK_MODE_ENABLE);
        dq_dqs_driver_set_mode_reg_if(dev->dq_reg[byte], msr, rank, DRIVER_MODE_DIFF);
        dq_dqs_receiver_set_mode_reg_if(dev->dq_reg[byte], msr, rank, REC_MODE_DIFF, REC_PATH_AC);
    }
}

void wddr_enable_loopback(wddr_dev_t *wddr)
{
    driver_cfg_t cfg =
    {
        .override.sel = 0x3,
        .override.val_c = 0x0,
        .override.val_t = 0x0,
        .rx_impd = DRIVER_IMPEDANCE_40,
        .tx_impd = DRIVER_IMPEDANCE_40,
    };

    wddr_msr_t msr = fsw_get_current_msr(&wddr->fsw);

    // Turn on DFI Loopback (CH0)
    dfi_set_ca_loopback_sel_reg_if(wddr->dfi.dfi_reg, 0x0);

    dfi_fifo_enable_ca_rdata_loopback_reg_if(wddr->dfi.dfich_reg, true);

    for (uint8_t channel = 0; channel < WDDR_PHY_CHANNEL_NUM; channel++)
    {
        ca_enable_loopback(&wddr->channel[channel], msr, &cfg);
        for (uint8_t dq_byte = 0; dq_byte < WDDR_PHY_DQ_BYTE_NUM; dq_byte++)
        {
            dq_enable_loopback(&wddr->channel[channel], msr, dq_byte, &cfg);
        }
    }
}

/*******************************************************************************
**                     WAVIOUS DDR PHY PREP FUNCTIONS
*******************************************************************************/

/**
 * @brief   DQ RX Path Frequency Switch Prep
 *
 * @details Prepares DQ RX path for the given DQ Byte for a frequency switch.
 *
 * @param[in]   dq_reg      pointer to dq register space.
 * @param[in]   msr         Mode Set Register value to prepare.
 * @param[in]   cfg         pointer to the frequency configuration parameters for
 *                          rx path.
 * @param[in]   common_cfg  pointer to frequency independent configuration
 *                          parameters for the rx path.
 *
 * @return      void
 */
static inline void rx_path_dq_block_freq_switch_prep(dq_reg_t *dq_reg,
                                                     wddr_msr_t msr,
                                                     dq_rx_path_freq_cfg_t *cfg,
                                                     dq_rx_path_common_cfg_t *common_cfg)
{
    uint32_t reg_val;

    for (uint8_t rank = 0; rank < WDDR_PHY_RANK; rank++)
    {
        // REN PI
        reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQS_RX_REN_PI_M1_R0_CFG_CODE, cfg->rank[rank].dqs.pi.ren.code);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_REN_PI_M1_R0_CFG_GEAR, cfg->rank[rank].dqs.pi.ren.gear);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_REN_PI_M1_R0_CFG_XCPL, cfg->rank[rank].dqs.pi.ren.xcpl);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_REN_PI_M1_R0_CFG_EN, 0x1);
        dq_reg->DDR_DQ_DQS_RX_REN_PI_CFG[msr][rank] = reg_val;

        // RCS PI
        reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQS_RX_RCS_PI_M1_R0_CFG_CODE, cfg->rank[rank].dqs.pi.rcs.code);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_RCS_PI_M1_R0_CFG_GEAR, cfg->rank[rank].dqs.pi.rcs.gear);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_RCS_PI_M1_R0_CFG_XCPL, cfg->rank[rank].dqs.pi.rcs.xcpl);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_RCS_PI_M1_R0_CFG_EN, 0x1);
        dq_reg->DDR_DQ_DQS_RX_RCS_PI_CFG[msr][rank] = reg_val;

        // RDQS PI
        reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQS_RX_RDQS_PI_0_M1_R0_CFG_CODE, cfg->rank[rank].dqs.pi.rdqs.code);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_RDQS_PI_0_M1_R0_CFG_GEAR, cfg->rank[rank].dqs.pi.rdqs.gear);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_RDQS_PI_0_M1_R0_CFG_XCPL, cfg->rank[rank].dqs.pi.rdqs.xcpl);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_RDQS_PI_0_M1_R0_CFG_EN, 0x1);
        dq_reg->DDR_DQ_DQS_RX_RDQS_PI_0_CFG[msr][rank] = reg_val;

        // SDR LPDE
        reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQS_RX_SDR_LPDE_M1_R0_CFG_GEAR, cfg->rank[rank].dqs.sdr_lpde.gear);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_SDR_LPDE_M1_R0_CFG_CTRL_BIN, cfg->rank[rank].dqs.sdr_lpde.delay);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_SDR_LPDE_M1_R0_CFG_EN, 0x1);
        dq_reg->DDR_DQ_DQS_RX_SDR_LPDE_CFG[msr][rank] = reg_val;

        // Receiver
        reg_val = UPDATE_REG_FIELD(dq_reg->DDR_DQ_DQS_RX_IO_CMN_CFG[msr][rank],
                                   DDR_DQ_DQS_RX_IO_CMN_M1_R0_CFG_CAL_P_C,
                                   common_cfg->rank[rank].dqs.receiver.code[REC_P_SIDE][REC_C_SIDE]);

        reg_val = UPDATE_REG_FIELD(reg_val,
                                   DDR_DQ_DQS_RX_IO_CMN_M1_R0_CFG_CAL_P_T,
                                   common_cfg->rank[rank].dqs.receiver.code[REC_P_SIDE][REC_T_SIDE]);

        reg_val = UPDATE_REG_FIELD(reg_val,
                                   DDR_DQ_DQS_RX_IO_CMN_M1_R0_CFG_CAL_N_C,
                                   common_cfg->rank[rank].dqs.receiver.code[REC_N_SIDE][REC_C_SIDE]);

        reg_val = UPDATE_REG_FIELD(reg_val,
                                   DDR_DQ_DQS_RX_IO_CMN_M1_R0_CFG_CAL_N_T,
                                   common_cfg->rank[rank].dqs.receiver.code[REC_N_SIDE][REC_T_SIDE]);

        reg_val = UPDATE_REG_FIELD(reg_val,
                                   DDR_DQ_DQS_RX_IO_CMN_M1_R0_CFG_FB_EN,
                                   cfg->rank[rank].dqs.receiver.feedback_resistor);

        reg_val = UPDATE_REG_FIELD(reg_val,
                                   DDR_DQ_DQS_RX_IO_CMN_M1_R0_CFG_SE_MODE,
                                   cfg->rank[rank].dqs.receiver.mode);

        reg_val = UPDATE_REG_FIELD(reg_val,
                                   DDR_DQ_DQS_RX_IO_CMN_M1_R0_CFG_DCPATH_EN,
                                   cfg->rank[rank].dqs.receiver.path_state);

        dq_reg->DDR_DQ_DQS_RX_IO_CMN_CFG[msr][rank] = reg_val;


        reg_val = UPDATE_REG_FIELD(0,
                                   DDR_DQ_DQS_RX_IO_M1_R0_CFG_0_DLY_CTRL_T,
                                   cfg->rank[rank].dqs.receiver.rx_delay[REC_T_SIDE]);

        reg_val = UPDATE_REG_FIELD(reg_val,
                                   DDR_DQ_DQS_RX_IO_M1_R0_CFG_0_DLY_CTRL_C,
                                   cfg->rank[rank].dqs.receiver.rx_delay[REC_C_SIDE]);

        dq_reg->DDR_DQ_DQS_RX_IO_CFG[msr][rank][1] = reg_val;

        // Gearbox
        reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQS_RX_M1_CFG_RGB_MODE, cfg->rank_cmn.cmn.gearbox.data_mode);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_M1_CFG_FGB_MODE, cfg->rank_cmn.cmn.gearbox.fifo_mode);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_M1_CFG_WCK_MODE, cfg->rank_cmn.cmn.gearbox.wck_mode);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_RX_M1_CFG_PRE_FILTER_SEL, cfg->rank_cmn.cmn.gearbox.pre_filter_sel);
        dq_reg->DDR_DQ_DQS_RX_CFG[msr] = reg_val;

        reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQ_RX_M1_CFG_RGB_MODE, cfg->rank_cmn.cmn.gearbox.data_mode);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_RX_M1_CFG_FGB_MODE, cfg->rank_cmn.cmn.gearbox.fifo_mode);
        dq_reg->DDR_DQ_DQ_RX_CFG[msr] = reg_val;
    }
}

/**
 * @brief   CA RX Path Frequency Switch Prep
 *
 * @details Prepares CA RX Path for a frequency switch.
 *
 * @param[in]   ca_reg      pointer to ca register space.
 * @param[in]   msr         which MSR register set to prepare.
 * @param[in]   cfg         pointer to the frequency configuration parameters
 *                          for rx path.
 *
 * @return      void
 */
static inline void rx_path_ca_block_freq_switch_prep(ca_reg_t *ca_reg,
                                                     wddr_msr_t msr,
                                                     ca_rx_path_freq_cfg_t *cfg)
{
    uint32_t reg_val;

    for (uint8_t rank = 0; rank < WDDR_PHY_RANK; rank++)
    {

        reg_val = UPDATE_REG_FIELD(0x0, DDR_CA_DQS_RX_SDR_LPDE_M1_R0_CFG_GEAR, cfg->rank[rank].ck.sdr_lpde.gear);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_RX_SDR_LPDE_M1_R0_CFG_CTRL_BIN, cfg->rank[rank].ck.sdr_lpde.delay);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_RX_SDR_LPDE_M1_R0_CFG_EN, 0x1);
        ca_reg->DDR_CA_DQS_RX_SDR_LPDE_CFG[msr][rank] = reg_val;

        reg_val = UPDATE_REG_FIELD(0x0, DDR_CA_DQS_RX_M1_CFG_RGB_MODE, cfg->rank_cmn.cmn.gearbox.data_mode);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_RX_M1_CFG_FGB_MODE, cfg->rank_cmn.cmn.gearbox.fifo_mode);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_RX_M1_CFG_WCK_MODE, cfg->rank_cmn.cmn.gearbox.wck_mode);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_RX_M1_CFG_PRE_FILTER_SEL, cfg->rank_cmn.cmn.gearbox.pre_filter_sel);
        ca_reg->DDR_CA_DQS_RX_CFG[msr] = reg_val;

        reg_val = UPDATE_REG_FIELD(0x0, DDR_CA_DQ_RX_M1_CFG_RGB_MODE, cfg->rank_cmn.cmn.gearbox.data_mode);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_RX_M1_CFG_FGB_MODE, cfg->rank_cmn.cmn.gearbox.fifo_mode);
        ca_reg->DDR_CA_DQ_RX_CFG[msr] = reg_val;
    }
}

/**
 * @brief   TX Bit Pipeline Prep CA
 *
 * @details Prepares the CA TX pipeline for a frequency switch.
 *
 * @param[in]   ca_reg      pointer to ca register space.
 * @param[in]   msr         which MSR register set to prepare.
 * @param[in]   bit_index   which bit register set to prepare.
 * @param[in]   cfg         pointer to configuration structure for ca path.
 *
 * @return      void
 */
static inline void tx_bit_pipeline_prep_ca(ca_reg_t *ca_reg,
                                            wddr_msr_t msr,
                                            uint8_t bit_index,
                                            ca_tx_path_freq_cfg_t *cfg)
{
    uint32_t reg_val;

    for (uint8_t rank = 0; rank < WDDR_PHY_RANK; rank++)
    {

        // SDR
        // FC Delay
        ca_reg->DDR_CA_DQ_TX_SDR_FC_DLY_CFG[msr][rank][bit_index] = cfg->rank[rank].ca.pipeline.sdr.fc_delay;

        // PIPE EN
        ca_reg->DDR_CA_DQ_TX_SDR_CFG[msr][rank][bit_index] = cfg->rank[rank].ca.pipeline.sdr.pipe_en;

        // X SEL
        ca_reg->DDR_CA_DQ_TX_SDR_X_SEL_CFG[msr][rank][bit_index] =cfg->rank[rank].ca.pipeline.sdr.x_sel;

        // DDR
        // PIPE EN
        ca_reg->DDR_CA_DQ_TX_DDR_CFG[msr][rank][bit_index] = cfg->rank[rank].ca.pipeline.ddr.pipe_en;

        // X SEL
        ca_reg->DDR_CA_DQ_TX_DDR_X_SEL_CFG[msr][rank][bit_index] =cfg->rank[rank].ca.pipeline.ddr.x_sel;

        // QDR
        // PIPE EN
        ca_reg->DDR_CA_DQ_TX_QDR_CFG[msr][rank][bit_index] = cfg->rank[rank].ca.pipeline.qdr.pipe_en;

        // X SEL
        ca_reg->DDR_CA_DQ_TX_QDR_X_SEL_CFG[msr][rank][bit_index] =cfg->rank[rank].ca.pipeline.qdr.x_sel;

        reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQ_TX_LPDE_M0_R0_CFG_0_GEAR, cfg->rank[rank].ca.lpde[bit_index].gear);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_LPDE_M0_R0_CFG_0_CTRL_BIN, cfg->rank[rank].ca.lpde[bit_index].delay);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_LPDE_M0_R0_CFG_0_EN, 0x1);
        ca_reg->DDR_CA_DQ_TX_LPDE_CFG[msr][rank][bit_index] = reg_val;
    }
}

/**
 * @brief   TX Bit Pipeline Prep CK
 *
 * @details Prepares the CK TX pipeline for a frequency switch.
 *
 * @param[in]   ca_reg      pointer to ca register space.
 * @param[in]   msr         which MSR register set to prepare.
 * @param[in]   bit_index   which bit register set to prepare.
 * @param[in]   cfg         pointer to configuration structure for ca path.
 *
 * @return      void
 */
static inline void tx_bit_pipeline_prep_ck(ca_reg_t *ca_reg,
                                           wddr_msr_t msr,
                                           uint8_t bit_index,
                                           ca_tx_path_freq_cfg_t *cfg)
{
    uint32_t reg_val;
    for (uint8_t rank = 0; rank < WDDR_PHY_RANK; rank++)
    {

        // SDR
        // FC Delay
        ca_reg->DDR_CA_DQS_TX_SDR_FC_DLY_CFG[msr][rank][bit_index] = cfg->rank[rank].ck.pipeline.sdr.fc_delay;

        // PIPE EN
        ca_reg->DDR_CA_DQS_TX_SDR_CFG[msr][rank][bit_index] = cfg->rank[rank].ck.pipeline.sdr.pipe_en;

        // X SEL
        ca_reg->DDR_CA_DQS_TX_SDR_X_SEL_CFG[msr][rank][bit_index] =cfg->rank[rank].ck.pipeline.sdr.x_sel;

        // DDR
        // PIPE EN
        ca_reg->DDR_CA_DQS_TX_DDR_CFG[msr][rank][bit_index] = cfg->rank[rank].ck.pipeline.ddr.pipe_en;

        // X SEL
        ca_reg->DDR_CA_DQS_TX_DDR_X_SEL_CFG[msr][rank][bit_index] =cfg->rank[rank].ck.pipeline.ddr.x_sel;

        // QDR
        // PIPE EN
        ca_reg->DDR_CA_DQS_TX_QDR_CFG[msr][rank][bit_index] = cfg->rank[rank].ck.pipeline.qdr.pipe_en;

        // X SEL
        ca_reg->DDR_CA_DQS_TX_QDR_X_SEL_CFG[msr][rank][bit_index] = cfg->rank[rank].ck.pipeline.qdr.x_sel;

        reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQ_TX_LPDE_M1_R0_CFG_0_GEAR, cfg->rank[rank].ck.lpde[bit_index].gear);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_LPDE_M1_R0_CFG_0_CTRL_BIN, cfg->rank[rank].ck.lpde[bit_index].delay);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_LPDE_M1_R0_CFG_0_EN, 0x1);
        ca_reg->DDR_CA_DQS_TX_LPDE_CFG[msr][rank][bit_index] = reg_val;
    }
}

/**
 * @brief   CA TX Path Frequency Switch Prep
 *
 * @details Prepares CA TX Path for a frequency switch.
 *
 * @param[in]   ca_reg      pointer to ca register space.
 * @param[in]   msr         which MSR register set to prepare.
 * @param[in]   cfg         pointer to configuration structure for ca path.
 *
 * @return      void
 */
static inline void tx_path_ca_block_freq_switch_prep(ca_reg_t *ca_reg,
                                                     wddr_msr_t msr,
                                                     ca_tx_path_freq_cfg_t *cfg)
{
    uint32_t reg_val = 0;

    for (uint8_t rank = 0; rank < WDDR_PHY_RANK; rank++)
    {
        /************************************************
        **                     CA
        ************************************************/

        ca_reg->DDR_CA_DQ_TX_RT_CFG[msr][rank] = cfg->rank[rank].ca.rt.pipe_en;

        // ODR
        reg_val = UPDATE_REG_FIELD(0x0, DDR_CA_DQ_TX_ODR_PI_M1_R0_CFG_CODE, cfg->rank[rank].ca.pi.odr.code);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_TX_ODR_PI_M1_R0_CFG_GEAR, cfg->rank[rank].ca.pi.odr.gear);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_TX_ODR_PI_M1_R0_CFG_XCPL, cfg->rank[rank].ca.pi.odr.xcpl);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_TX_ODR_PI_M1_R0_CFG_EN, 0x1);
        ca_reg->DDR_CA_DQ_TX_ODR_PI_CFG[msr][rank] = reg_val;

        // QDR
        reg_val = UPDATE_REG_FIELD(0x0, DDR_CA_DQ_TX_QDR_PI_0_M1_R0_CFG_CODE, cfg->rank[rank].ca.pi.qdr.code);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_TX_QDR_PI_0_M1_R0_CFG_GEAR, cfg->rank[rank].ca.pi.qdr.gear);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_TX_QDR_PI_0_M1_R0_CFG_XCPL, cfg->rank[rank].ca.pi.qdr.xcpl);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_TX_QDR_PI_0_M1_R0_CFG_EN, 0x1);
        ca_reg->DDR_CA_DQ_TX_QDR_PI_0_CFG[msr][rank] = reg_val;

        // DDR
        reg_val = UPDATE_REG_FIELD(0x0, DDR_CA_DQ_TX_DDR_PI_0_M1_R0_CFG_CODE, cfg->rank[rank].ca.pi.ddr.code);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_TX_DDR_PI_0_M1_R0_CFG_GEAR, cfg->rank[rank].ca.pi.ddr.gear);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_TX_DDR_PI_0_M1_R0_CFG_XCPL, cfg->rank[rank].ca.pi.ddr.xcpl);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_TX_DDR_PI_0_M1_R0_CFG_EN, 0x1);
        ca_reg->DDR_CA_DQ_TX_DDR_PI_0_CFG[msr][rank] = reg_val;

        // RT
        reg_val = UPDATE_REG_FIELD(0x0, DDR_CA_DQ_TX_PI_RT_M1_R0_CFG_CODE, cfg->rank[rank].ca.pi.rt.code);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_TX_PI_RT_M1_R0_CFG_GEAR, cfg->rank[rank].ca.pi.rt.gear);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_TX_PI_RT_M1_R0_CFG_XCPL, cfg->rank[rank].ca.pi.rt.xcpl);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_TX_PI_RT_M1_R0_CFG_EN, 0x1);
        ca_reg->DDR_CA_DQ_TX_PI_RT_CFG[msr][rank] = reg_val;

        /************************************************
        **                     CK
        ************************************************/
        ca_reg->DDR_CA_DQS_TX_RT_CFG[msr][rank] = cfg->rank[rank].ck.rt.pipe_en;

        // ODR
        reg_val = UPDATE_REG_FIELD(0x0, DDR_CA_DQS_TX_ODR_PI_M1_R0_CFG_CODE, cfg->rank[rank].ck.pi.odr.code);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_ODR_PI_M1_R0_CFG_GEAR, cfg->rank[rank].ck.pi.odr.gear);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_ODR_PI_M1_R0_CFG_XCPL, cfg->rank[rank].ck.pi.odr.xcpl);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_ODR_PI_M1_R0_CFG_EN, 0x1);
        ca_reg->DDR_CA_DQS_TX_ODR_PI_CFG[msr][rank] = reg_val;

        // QDR
        reg_val = UPDATE_REG_FIELD(0x0, DDR_CA_DQS_TX_QDR_PI_0_M1_R0_CFG_CODE, cfg->rank[rank].ck.pi.qdr.code);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_QDR_PI_0_M1_R0_CFG_GEAR, cfg->rank[rank].ck.pi.qdr.gear);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_QDR_PI_0_M1_R0_CFG_XCPL, cfg->rank[rank].ck.pi.qdr.xcpl);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_QDR_PI_0_M1_R0_CFG_EN, 0x1);
        ca_reg->DDR_CA_DQS_TX_QDR_PI_0_CFG[msr][rank] = reg_val;

        // DDR
        reg_val = UPDATE_REG_FIELD(0x0, DDR_CA_DQS_TX_DDR_PI_0_M1_R0_CFG_CODE, cfg->rank[rank].ca.pi.ddr.code);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_DDR_PI_0_M1_R0_CFG_GEAR, cfg->rank[rank].ca.pi.ddr.gear);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_DDR_PI_0_M1_R0_CFG_XCPL, cfg->rank[rank].ca.pi.ddr.xcpl);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_DDR_PI_0_M1_R0_CFG_EN, 0x1);
        ca_reg->DDR_CA_DQS_TX_DDR_PI_0_CFG[msr][rank] = reg_val;

        // RT
        reg_val = UPDATE_REG_FIELD(0x0, DDR_CA_DQS_TX_PI_RT_M1_R0_CFG_CODE, cfg->rank[rank].ck.pi.rt.code);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_PI_RT_M1_R0_CFG_GEAR, cfg->rank[rank].ck.pi.rt.gear);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_PI_RT_M1_R0_CFG_XCPL, cfg->rank[rank].ck.pi.rt.xcpl);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_PI_RT_M1_R0_CFG_EN, 0x1);
        ca_reg->DDR_CA_DQS_TX_PI_RT_CFG[msr][rank] = reg_val;

        // SDR
        reg_val = UPDATE_REG_FIELD(0x0, DDR_CA_DQS_TX_SDR_PI_M1_R0_CFG_GEAR, cfg->rank[rank].ck.pi.sdr.gear);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_SDR_PI_M1_R0_CFG_XCPL, cfg->rank[rank].ck.pi.sdr.xcpl);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_SDR_PI_M1_R0_CFG_EN, 0x1);
        ca_reg->DDR_CA_DQS_TX_SDR_PI_CFG[msr][rank] = reg_val;

        // DFI
        reg_val = UPDATE_REG_FIELD(0x0, DDR_CA_DQS_TX_DFI_PI_M1_R0_CFG_GEAR, cfg->rank[rank].ck.pi.dfi.gear);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_DFI_PI_M1_R0_CFG_XCPL, cfg->rank[rank].ck.pi.dfi.xcpl);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_DFI_PI_M1_R0_CFG_EN, 0x1);
        ca_reg->DDR_CA_DQS_TX_DFI_PI_CFG[msr][rank] = reg_val;

        reg_val = UPDATE_REG_FIELD(0,
                                   DDR_CA_DQS_TX_IO_CMN_M1_R0_CFG_NCAL,
                                   cfg->rank[rank].ck.driver.code[DRVR_N_CAL]);

        reg_val = UPDATE_REG_FIELD(reg_val,
                                   DDR_CA_DQS_TX_IO_CMN_M1_R0_CFG_PCAL,
                                   cfg->rank[rank].ck.driver.code[DRVR_P_CAL]);

        reg_val = UPDATE_REG_FIELD(reg_val,
                                   DDR_CA_DQS_TX_IO_CMN_M1_R0_CFG_SE_MODE,
                                   cfg->rank[rank].ck.driver.mode);

        ca_reg->DDR_CA_DQS_TX_IO_CMN_CFG[msr][rank] = reg_val;
    }

    /************************************************
    **                     CA
    ************************************************/
    reg_val = UPDATE_REG_FIELD(ca_reg->DDR_CA_DQ_TX_IO_CFG[msr][0], DDR_CA_DQ_TX_IO_M1_CFG_0_OVRD_SEL, cfg->rank_cmn.ca.driver.override.sel);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_TX_IO_M1_CFG_0_OVRD_VAL, cfg->rank_cmn.ca.driver.override.val_t);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_TX_IO_M1_CFG_0_TX_IMPD, cfg->rank_cmn.ca.driver.tx_impd);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_TX_IO_M1_CFG_0_RX_IMPD, cfg->rank_cmn.ca.driver.rx_impd);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQ_TX_IO_M1_CFG_0_SW_OVR, 0);

    for (uint8_t bit_index = 0; bit_index < WDDR_PHY_CA_SLICE_NUM; bit_index++)
    {
        tx_bit_pipeline_prep_ca(ca_reg, msr, bit_index, cfg);

        ca_reg->DDR_CA_DQ_TX_IO_CFG[msr][bit_index] = reg_val;
        ca_reg->DDR_CA_DQ_TX_EGRESS_DIG_CFG[msr][bit_index] = cfg->rank_cmn.cmn.egress.dig_mode;
        ca_reg->DDR_CA_DQ_TX_EGRESS_ANA_CFG[msr][bit_index] = cfg->rank_cmn.cmn.egress.ana_mode;
    }

    /************************************************
    **                     CK
    ************************************************/
    reg_val = UPDATE_REG_FIELD(ca_reg->DDR_CA_DQS_TX_IO_CFG[msr][0], DDR_CA_DQS_TX_IO_M1_CFG_0_OVRD_SEL, cfg->rank_cmn.ck.driver.override.sel);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_IO_M1_CFG_0_OVRD_VAL_T, cfg->rank_cmn.ck.driver.override.val_t);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_IO_M1_CFG_0_OVRD_VAL_C, cfg->rank_cmn.ck.driver.override.val_c);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_IO_M1_CFG_0_TX_IMPD, cfg->rank_cmn.ck.driver.tx_impd);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_IO_M1_CFG_0_RX_IMPD, cfg->rank_cmn.ck.driver.rx_impd);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_IO_M1_CFG_0_SW_OVR, 0);

    for (uint8_t bit_index = 0; bit_index < WDDR_PHY_CK_SLICE_NUM; bit_index++)
    {
        tx_bit_pipeline_prep_ck(ca_reg, msr, bit_index, cfg);
        ca_reg->DDR_CA_DQS_TX_IO_CFG[msr][bit_index] = reg_val;
        // Egress
        ca_reg->DDR_CA_DQS_TX_EGRESS_DIG_CFG[msr][bit_index] = cfg->rank_cmn.cmn.egress.dig_mode;
        ca_reg->DDR_CA_DQS_TX_EGRESS_ANA_CFG[msr][bit_index] = cfg->rank_cmn.cmn.egress.ana_mode;
    }

    // Gearbox
    reg_val = UPDATE_REG_FIELD(0x0, DDR_CA_DQS_TX_M1_CFG_TGB_MODE, cfg->rank_cmn.ck.gearbox.data_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_M1_CFG_WGB_MODE, cfg->rank_cmn.ck.gearbox.write_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CA_DQS_TX_M1_CFG_CK2WCK_RATIO, cfg->rank_cmn.ck.gearbox.ck2wck_ratio);
    ca_reg->DDR_CA_DQS_TX_CFG[msr] = reg_val;
}

/**
 * @brief   TX Bit Pipeline Prep DQ
 *
 * @details Prepares the DQ TX pipeline for a frequency switch.
 *
 * @param[in]   dq_reg      pointer to dq register space.
 * @param[in]   msr         which MSR register set to prepare.
 * @param[in]   bit_index   which bit register set to prepare.
 * @param[in]   cfg         pointer to configuration structure for dq path.
 *
 * @return      void
 */
static inline void tx_bit_pipeline_prep_dq(dq_reg_t *dq_reg,
                                           wddr_msr_t msr,
                                           uint8_t bit_index,
                                           dq_tx_path_freq_cfg_t *cfg)
{
    uint32_t reg_val;
    for (uint8_t rank = 0; rank < WDDR_PHY_RANK; rank++)
    {

        // SDR
        // FC Delay
        dq_reg->DDR_DQ_DQ_TX_SDR_FC_DLY_CFG[msr][rank][bit_index] = cfg->rank[rank].dq.pipeline.sdr.fc_delay;

        // PIPE EN
        dq_reg->DDR_DQ_DQ_TX_SDR_CFG[msr][rank][bit_index] = cfg->rank[rank].dq.pipeline.sdr.pipe_en;

        // X SEL
        dq_reg->DDR_DQ_DQ_TX_SDR_X_SEL_CFG[msr][rank][bit_index] =cfg->rank[rank].dq.pipeline.sdr.x_sel;

        // DDR
        // PIPE EN
        dq_reg->DDR_DQ_DQ_TX_DDR_CFG[msr][rank][bit_index] = cfg->rank[rank].dq.pipeline.ddr.pipe_en;

        // X SEL
        dq_reg->DDR_DQ_DQ_TX_DDR_X_SEL_CFG[msr][rank][bit_index] =cfg->rank[rank].dq.pipeline.ddr.x_sel;

        // QDR
        // PIPE EN
        dq_reg->DDR_DQ_DQ_TX_QDR_CFG[msr][rank][bit_index] = cfg->rank[rank].dq.pipeline.qdr.pipe_en;

        // X SEL
        dq_reg->DDR_DQ_DQ_TX_QDR_X_SEL_CFG[msr][rank][bit_index] =cfg->rank[rank].dq.pipeline.qdr.x_sel;

        reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQ_TX_LPDE_M0_R0_CFG_0_GEAR, cfg->rank[rank].dq.lpde[bit_index].gear);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_LPDE_M0_R0_CFG_0_CTRL_BIN, cfg->rank[rank].dq.lpde[bit_index].delay);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_LPDE_M0_R0_CFG_0_EN, 0x1);
        dq_reg->DDR_DQ_DQ_TX_LPDE_CFG[msr][rank][bit_index] = reg_val;
    }
}

/**
 * @brief   TX Bit Pipeline Prep DQS
 *
 * @details Prepares the DQS TX pipeline for a frequency switch.
 *
 * @param[in]   dq_reg      pointer to dq register space.
 * @param[in]   msr         which MSR register set to prepare.
 * @param[in]   bit_index   which bit register set to prepare.
 * @param[in]   cfg         pointer to configuration structure for dq path.
 *
 * @return      void
 */
static inline void tx_bit_pipeline_prep_dqs(dq_reg_t *dq_reg,
                                            wddr_msr_t msr,
                                            uint8_t bit_index,
                                            dq_tx_path_freq_cfg_t *cfg)
{
    uint32_t reg_val;
    for (uint8_t rank = 0; rank < WDDR_PHY_RANK; rank++)
    {
        // SDR
        // FC Delay
        dq_reg->DDR_DQ_DQS_TX_SDR_FC_DLY_CFG[msr][rank][bit_index] = cfg->rank[rank].dqs.pipeline[bit_index].sdr.fc_delay;

        // PIPE EN
        dq_reg->DDR_DQ_DQS_TX_SDR_CFG[msr][rank][bit_index] = cfg->rank[rank].dqs.pipeline[bit_index].sdr.pipe_en;

        // X SEL
        dq_reg->DDR_DQ_DQS_TX_SDR_X_SEL_CFG[msr][rank][bit_index] =cfg->rank[rank].dqs.pipeline[bit_index].sdr.x_sel;

        // DDR
        // PIPE EN
        dq_reg->DDR_DQ_DQS_TX_DDR_CFG[msr][rank][bit_index] = cfg->rank[rank].dqs.pipeline[bit_index].ddr.pipe_en;

        // X SEL
        dq_reg->DDR_DQ_DQS_TX_DDR_X_SEL_CFG[msr][rank][bit_index] =cfg->rank[rank].dqs.pipeline[bit_index].ddr.x_sel;

        // QDR
        // PIPE EN
        dq_reg->DDR_DQ_DQS_TX_QDR_CFG[msr][rank][bit_index] = cfg->rank[rank].dqs.pipeline[bit_index].qdr.pipe_en;

        // X SEL
        dq_reg->DDR_DQ_DQS_TX_QDR_X_SEL_CFG[msr][rank][bit_index] =cfg->rank[rank].dqs.pipeline[bit_index].qdr.x_sel;

        if (bit_index < WDDR_PHY_DQS_TXRX_SLICE_NUM)
        {
            reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQS_TX_LPDE_M0_R0_CFG_0_GEAR, cfg->rank[rank].dqs.lpde[bit_index].gear);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_LPDE_M0_R0_CFG_0_CTRL_BIN, cfg->rank[rank].dqs.lpde[bit_index].delay);
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_LPDE_M0_R0_CFG_0_EN, 0x1);
            dq_reg->DDR_DQ_DQS_TX_LPDE_CFG[msr][rank][bit_index] = reg_val;
        }
    }
}

/**
 * @brief   DQ TX Path Frequency Switch Prep
 *
 * @details Prepares DQ TX Path for a frequency switch.
 *
 * @param[in]   dq_reg      pointer to dq register space.
 * @param[in]   msr         which MSR register set to prepare.
 * @param[in]   cfg         pointer to configuration structure for dq path.
 *
 * @return      void
 */
static inline void tx_path_dq_block_freq_switch_prep(dq_reg_t *dq_reg,
                                                     wddr_msr_t msr,
                                                     dq_tx_path_freq_cfg_t *cfg)
{
    uint32_t reg_val = 0;
    for (uint8_t rank = 0; rank < WDDR_PHY_RANK; rank++)
    {
        /************************************************
        **                  DQ
        ************************************************/
        dq_reg->DDR_DQ_DQ_TX_RT_CFG[msr][rank] = cfg->rank[rank].dq.rt.pipe_en;

        // ODR
        reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQ_TX_ODR_PI_M1_R0_CFG_CODE, cfg->rank[rank].dq.pi.odr.code);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_ODR_PI_M1_R0_CFG_GEAR, cfg->rank[rank].dq.pi.odr.gear);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_ODR_PI_M1_R0_CFG_XCPL, cfg->rank[rank].dq.pi.odr.xcpl);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_ODR_PI_M1_R0_CFG_EN, 0x1);
        dq_reg->DDR_DQ_DQ_TX_ODR_PI_CFG[msr][rank] = reg_val;

        // QDR
        reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQ_TX_QDR_PI_0_M1_R0_CFG_CODE, cfg->rank[rank].dq.pi.qdr.code);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_QDR_PI_0_M1_R0_CFG_GEAR, cfg->rank[rank].dq.pi.qdr.gear);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_QDR_PI_0_M1_R0_CFG_XCPL, cfg->rank[rank].dq.pi.qdr.xcpl);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_QDR_PI_0_M1_R0_CFG_EN, 0x1);
        dq_reg->DDR_DQ_DQ_TX_QDR_PI_0_CFG[msr][rank] = reg_val;

        // DDR
        reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQ_TX_DDR_PI_0_M1_R0_CFG_CODE, cfg->rank[rank].dq.pi.ddr.code);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_DDR_PI_0_M1_R0_CFG_GEAR, cfg->rank[rank].dq.pi.ddr.gear);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_DDR_PI_0_M1_R0_CFG_XCPL, cfg->rank[rank].dq.pi.ddr.xcpl);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_DDR_PI_0_M1_R0_CFG_EN, 0x1);
        dq_reg->DDR_DQ_DQ_TX_DDR_PI_0_CFG[msr][rank] = reg_val;

        // RT
        reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQ_TX_PI_RT_M1_R0_CFG_CODE, cfg->rank[rank].dq.pi.rt.code);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_PI_RT_M1_R0_CFG_GEAR, cfg->rank[rank].dq.pi.rt.gear);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_PI_RT_M1_R0_CFG_XCPL, cfg->rank[rank].dq.pi.rt.xcpl);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_PI_RT_M1_R0_CFG_EN, 0x1);
        dq_reg->DDR_DQ_DQ_TX_PI_RT_CFG[msr][rank] = reg_val;

        /************************************************
        **                  DQS
        ************************************************/
        dq_reg->DDR_DQ_DQS_TX_RT_CFG[msr][rank] = cfg->rank[rank].dqs.rt.pipe_en;

        // ODR
        reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQS_TX_ODR_PI_M1_R0_CFG_CODE, cfg->rank[rank].dqs.pi.odr.code);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_ODR_PI_M1_R0_CFG_GEAR, cfg->rank[rank].dqs.pi.odr.gear);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_ODR_PI_M1_R0_CFG_XCPL, cfg->rank[rank].dqs.pi.odr.xcpl);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_ODR_PI_M1_R0_CFG_EN, 0x1);
        dq_reg->DDR_DQ_DQS_TX_ODR_PI_CFG[msr][rank] = reg_val;

        // QDR
        reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQS_TX_QDR_PI_0_M1_R0_CFG_CODE, cfg->rank[rank].dqs.pi.qdr.code);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_QDR_PI_0_M1_R0_CFG_GEAR, cfg->rank[rank].dqs.pi.qdr.gear);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_QDR_PI_0_M1_R0_CFG_XCPL, cfg->rank[rank].dqs.pi.qdr.xcpl);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_QDR_PI_0_M1_R0_CFG_EN, 0x1);
        dq_reg->DDR_DQ_DQS_TX_QDR_PI_0_CFG[msr][rank] = reg_val;

        // DDR
        reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQS_TX_DDR_PI_0_M1_R0_CFG_CODE, cfg->rank[rank].dqs.pi.ddr.code);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_DDR_PI_0_M1_R0_CFG_GEAR, cfg->rank[rank].dqs.pi.ddr.gear);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_DDR_PI_0_M1_R0_CFG_XCPL, cfg->rank[rank].dqs.pi.ddr.xcpl);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_DDR_PI_0_M1_R0_CFG_EN, 0x1);
        dq_reg->DDR_DQ_DQS_TX_DDR_PI_0_CFG[msr][rank] = reg_val;

        // RT
        reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQS_TX_PI_RT_M1_R0_CFG_CODE, cfg->rank[rank].dqs.pi.rt.code);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_PI_RT_M1_R0_CFG_GEAR, cfg->rank[rank].dqs.pi.rt.gear);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_PI_RT_M1_R0_CFG_XCPL, cfg->rank[rank].dqs.pi.rt.xcpl);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_PI_RT_M1_R0_CFG_EN, 0x1);
        dq_reg->DDR_DQ_DQS_TX_PI_RT_CFG[msr][rank] = reg_val;

        // SDR
        reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQS_TX_SDR_PI_M0_R0_CFG_GEAR, cfg->rank[rank].dqs.pi.sdr.gear);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_SDR_PI_M0_R0_CFG_XCPL, cfg->rank[rank].dqs.pi.sdr.xcpl);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_SDR_PI_M0_R0_CFG_EN, 0x1);
        dq_reg->DDR_DQ_DQS_TX_SDR_PI_CFG[msr][rank] = reg_val;

        // DFI
        reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQS_TX_DFI_PI_M0_R0_CFG_GEAR, cfg->rank[rank].dqs.pi.dfi.gear);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_DFI_PI_M0_R0_CFG_XCPL, cfg->rank[rank].dqs.pi.dfi.xcpl);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_DFI_PI_M0_R0_CFG_EN, 0x1);
        dq_reg->DDR_DQ_DQS_TX_DFI_PI_CFG[msr][rank] = reg_val;

        reg_val = UPDATE_REG_FIELD(0,
                                   DDR_DQ_DQS_TX_IO_CMN_M1_R0_CFG_NCAL,
                                   cfg->rank[rank].dqs.driver.code[DRVR_N_CAL]);

        reg_val = UPDATE_REG_FIELD(reg_val,
                                   DDR_DQ_DQS_TX_IO_CMN_M1_R0_CFG_PCAL,
                                   cfg->rank[rank].dqs.driver.code[DRVR_P_CAL]);

        reg_val = UPDATE_REG_FIELD(reg_val,
                                   DDR_DQ_DQS_TX_IO_CMN_M1_R0_CFG_SE_MODE,
                                   cfg->rank[rank].dqs.driver.mode);

        dq_reg->DDR_DQ_DQS_TX_IO_CMN_CFG[msr][rank] = reg_val;
    }

    /************************************************
    **                  DQ
    ************************************************/
    // DQ Driver
    reg_val = UPDATE_REG_FIELD(dq_reg->DDR_DQ_DQ_TX_IO_CFG[msr][0], DDR_DQ_DQ_TX_IO_M1_CFG_0_OVRD_SEL, cfg->rank_cmn.dq.driver.override.sel);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_IO_M1_CFG_0_OVRD_VAL, cfg->rank_cmn.dq.driver.override.val_t);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_IO_M1_CFG_0_TX_IMPD, cfg->rank_cmn.dq.driver.tx_impd);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_IO_M1_CFG_0_RX_IMPD, cfg->rank_cmn.dq.driver.rx_impd);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQ_TX_IO_M1_CFG_0_SW_OVR, 0);

    for (uint8_t bit_index = 0; bit_index < WDDR_PHY_DQ_SLICE_NUM; bit_index++)
    {
        tx_bit_pipeline_prep_dq(dq_reg, msr, bit_index, cfg);
        dq_reg->DDR_DQ_DQ_TX_IO_CFG[msr][bit_index] = reg_val;
        // Egress
        dq_reg->DDR_DQ_DQ_TX_EGRESS_DIG_CFG[msr][bit_index] = cfg->rank_cmn.cmn.egress.dig_mode;
        dq_reg->DDR_DQ_DQ_TX_EGRESS_ANA_CFG[msr][bit_index] = cfg->rank_cmn.cmn.egress.ana_mode;
    }

    /************************************************
    **                  DQS
    ************************************************/

    // DQS Driver
    reg_val = UPDATE_REG_FIELD(dq_reg->DDR_DQ_DQS_TX_IO_CFG[msr][0], DDR_DQ_DQS_TX_IO_M1_CFG_0_OVRD_SEL, cfg->rank_cmn.dqs.driver.override.sel);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_IO_M1_CFG_0_OVRD_VAL_T, cfg->rank_cmn.dqs.driver.override.val_t);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_IO_M1_CFG_0_OVRD_VAL_C, cfg->rank_cmn.dqs.driver.override.val_c);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_IO_M1_CFG_0_TX_IMPD, cfg->rank_cmn.dqs.driver.tx_impd);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_IO_M1_CFG_0_RX_IMPD, cfg->rank_cmn.dqs.driver.rx_impd);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_IO_M1_CFG_0_SW_OVR, 0);

    for (uint8_t bit_index = 0; bit_index < WDDR_PHY_DQS_SLICE_NUM; bit_index++)
    {
        tx_bit_pipeline_prep_dqs(dq_reg, msr, bit_index, cfg);

        // Egress
        dq_reg->DDR_DQ_DQS_TX_EGRESS_DIG_CFG[msr][bit_index] = cfg->rank_cmn.cmn.egress.dig_mode;
        dq_reg->DDR_DQ_DQS_TX_EGRESS_ANA_CFG[msr][bit_index] = cfg->rank_cmn.cmn.egress.ana_mode;

        if (bit_index < WDDR_PHY_DQS_TXRX_SLICE_NUM)
        {
            dq_reg->DDR_DQ_DQS_TX_IO_CFG[msr][bit_index] = reg_val;
        }
    }

    // Gearbox
    reg_val = UPDATE_REG_FIELD(0x0, DDR_DQ_DQS_TX_M1_CFG_TGB_MODE, cfg->rank_cmn.dqs.gearbox.data_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_M1_CFG_WGB_MODE, cfg->rank_cmn.dqs.gearbox.write_mode);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DQ_DQS_TX_M1_CFG_CK2WCK_RATIO, cfg->rank_cmn.dqs.gearbox.ck2wck_ratio);
    dq_reg->DDR_DQ_DQS_TX_CFG[msr] = reg_val;
}

/**
 * @brief   CA Path Frequency Switch Prep
 *
 * @details Prepares entire CA path for a frequency switch.
 *
 * @param[in]   ca_reg      pointer to ca register space.
 * @param[in]   msr         which MSR register set to prepare.
 * @param[in]   cfg         pointer to configuration structure for ca path.
 *
 * @return      void
 */
static inline void ca_block_freq_switch_prep(ca_reg_t *ca_reg,
                                             wddr_msr_t msr,
                                             ca_path_freq_cfg_t *cfg)
{
    // Prep Tx
    tx_path_ca_block_freq_switch_prep(ca_reg, msr, &cfg->tx);

    // Prep Rx
    rx_path_ca_block_freq_switch_prep(ca_reg, msr, &cfg->rx);
}

/**
 * @brief   DQ Path Frequency Switch Prep
 *
 * @details Prepares entire DQ path for a frequency switch.
 *
 * @param[in]   dq_reg      pointer to dq register space.
 * @param[in]   msr         which MSR register set to prepare.
 * @param[in]   cfg         pointer to configuration structure for dq path.
 * @param[in]   common_cfg  pointer to frequency independent configuration
 *                          parameters for dq path.
 *
 * @return      void
 */
static inline void dq_block_freq_switch_prep(dq_reg_t *dq_reg,
                                             wddr_msr_t msr,
                                             dq_path_freq_cfg_t *cfg,
                                             dq_path_common_cfg_t *common_cfg)
{
    // Prep Tx
    tx_path_dq_block_freq_switch_prep(dq_reg, msr, &cfg->tx);

    // Prep Rx
    rx_path_dq_block_freq_switch_prep(dq_reg, msr, &cfg->rx, &common_cfg->rx);
}

/**
 * @brief   Common Block Frequency Switch Prep
 *
 * @details Prepares entire Common block for a frequency switch.
 *
 * @param[in]   cmn_dev     pointer to common device.
 * @param[in]   msr         which msr register set to prepare.
 * @param[in]   cfg         pointer to the frequency configuration parameters
 *                          for the common block.
 *
 * @return      void
 */
static inline void common_block_freq_switch_prep(cmn_dev_t *cmn_dev,
                                                 wddr_msr_t msr,
                                                 common_path_freq_cfg_t *cfg)
{
    // VREF
    cmn_vref_set_code_reg_if(cmn_dev->cmn_reg, msr, cfg->vref.code);
    cmn_vref_set_pwr_mode_reg_if(cmn_dev->cmn_reg, msr, cfg->vref.power_mode);
}

/**
 * @brief  DFI Frequency Switch Prep
 *
 * @details Preps DFI for a frequency switch
 *
 * @param[in]   dfi     pointer to DFI device.
 * @param[in]   msr     which msr register set to prepare.
 * @param[in]   cfg     pointer to frequency configuration parameters for DFI.
 *
 * @return      void
 */
static inline void dfi_freq_switch_prep(dfi_dev_t *dfi, wddr_msr_t msr, dfi_freq_cfg_t *cfg)
{
    dfi_set_wrd_cfg_reg_if(dfi->dfich_reg, msr, &cfg->wrd);
    dfi_set_wrcctrl_cfg_reg_if(dfi->dfich_reg, msr, &cfg->wrcctrl);
    dfi_set_ckctrl_cfg_reg_if(dfi->dfich_reg, msr, &cfg->ckctrl);
    dfi_set_wctrl_cfg_reg_if(dfi->dfich_reg, msr, &cfg->wctrl);
    dfi_set_rctrl_cfg_reg_if(dfi->dfich_reg, msr, &cfg->rctrl);
    dfi_set_rdd_cfg_reg_if(dfi->dfich_reg, msr, &cfg->rdd);
    dfi_set_wckctrl_cfg_reg_if(dfi->dfich_reg, msr, &cfg->wckctrl);
    dfi_set_wrc_cfg_reg_if(dfi->dfich_reg, msr, &cfg->wrc);
    dfi_set_wenctrl_cfg_reg_if(dfi->dfich_reg, msr, &cfg->wenctrl);
    dfi_set_paden_pext_cfg_reg_if(dfi->dfich_reg, msr, &cfg->paden_pext);
    dfi_set_clken_pext_cfg_reg_if(dfi->dfich_reg, msr, &cfg->clken_pext);
    dfi_set_ovr_traffic_cfg_reg_if(dfi->dfich_reg, msr, &cfg->ovr_traffic);
}

/**
 * @brief   WDDR Configure PHY Internal
 *
 * Internal function for preparing WDDR PHY for Frequency Switch. This is
 * separated out so that the public call always forces a switch to the next
 * MSR while internal calls can force use of specific MSR.
 *
 * This preps PHY only. Another call to the Frequency Switch FSM must be made to
 * prep PLL and pefrom the switch (if desired).
 */
static void wddr_configure_phy(wddr_dev_t *wddr, uint8_t freq_id, wddr_msr_t msr)
{
    channel_dev_t *channel_dev;

    // CH Prep
    for (uint8_t channel = 0; channel < WDDR_PHY_CHANNEL_NUM; channel++)
    {
        channel_dev = &wddr->channel[channel];

        for(uint8_t byte = 0; byte < WDDR_PHY_DQ_BYTE_NUM; byte++)
        {
            // DQ Block Prep
            dq_block_freq_switch_prep(channel_dev->dq_reg[byte],
                                    msr,
                                    &wddr->table->cfg.freq[freq_id].channel[channel].dq[byte],
                                    &wddr->table->cfg.common.channel[channel].dq[byte]);
        }

        ca_block_freq_switch_prep(channel_dev->ca_reg,
                                msr,
                                &wddr->table->cfg.freq[freq_id].channel[channel].ca);
    }

    // Common Block Prep
    common_block_freq_switch_prep(&wddr->cmn,
                                  msr,
                                  &wddr->table->cfg.freq[freq_id].common);

    // DFI Prep
    dfi_freq_switch_prep(&wddr->dfi, msr, &wddr->table->cfg.freq[freq_id].dfi);
}

wddr_return_t wddr_train(wddr_dev_t *wddr)
{
    // This must be implemented in an external function.
    return WDDR_SUCCESS;
}
