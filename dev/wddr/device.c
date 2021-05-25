/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */

/* Standard includes. */
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* Kernel includes. */
#include <kernel/io.h>
#include <dfi/driver.h>

/* LPDDR includes. */
#include <driver/driver.h>
#include <driver/cmn_driver.h>
#include <fsw/driver.h>
#include <gearbox/tx_driver.h>
#include <gearbox/rx_driver.h>
#include <lpde/driver.h>
#include <lpde/tx_driver.h>
#include <lpde/rx_driver.h>
#include <pi/driver.h>
#include <pi/tx_driver.h>
#include <pi/rx_driver.h>
#include <pipeline/driver.h>
#include <receiver/driver.h>
#include <receiver/delay_driver.h>
#include <sensamp/driver.h>
#include <vref/driver.h>
#include <wddr/device.h>
#include <wddr/driver.h>
#include <wddr/memory_map.h>
#include <wddr/notification_map.h>

/*******************************************************************************
**                            FUNCTION DECLARATIONS
*******************************************************************************/
/** @brief   Internal Notification Handler */
static void notification_handler(Notification_t notification, void *args);

/** @brief  Internal Common WDDR Frequency Switch Implementation */
static wddr_return_t wddr_freq_switch_prv(wddr_dev_t *wddr, uint8_t freq_id, wddr_msr_t msr, bool sw_switch);

/** @brief  Internal Frequency Switch Function */
static wddr_return_t wddr_sw_freq_switch(wddr_dev_t *wddr, uint8_t freq_id, wddr_msr_t msr);

/** @brief  Internal Function to enable all PHY LPDEs and Phase Interpolators */
static void wddr_enable(wddr_dev_t *wddr);

/** @brief  Internal Function to flush DFI interfaces using DFI Buffer */
static void wddr_dfi_buffer_flush(wddr_dev_t *wddr);

/** @brief  Internal Function to clear FIFO for all channels */
static void wddr_clear_fifo_all_channels(wddr_dev_t *wddr);

/** @brief  Internal Function for preparing WDDR PHY for frequency switch */
static void wddr_configure_phy(wddr_dev_t *wddr, uint8_t freq_id, wddr_msr_t msr);

/** @brief  Internal Function passed to DFI Update FSM performing IOCAL Update */
static void wddr_iocal_update_csr(void *dev);

/** @brief  Internal Function passed to DFI Update FSM performing IOCAL Cal */
static void wddr_iocal_calibrate(void *dev);

void wddr_init(wddr_dev_t *wddr, uint32_t base, wddr_table_t *table)
{
    uint32_t channel_base = 0x0;

    // Initialize entire WDDR device
    wddr_init_reg_if(wddr, base);
    vInitCompletion(&wddr->fsw_event);
    wddr->table = table;

    // Channel Configuration
    for (uint8_t channel = 0; channel < WDDR_PHY_CHANNEL_NUM; channel++)
    {
        channel_base = wddr->base +
                       WDDR_MEMORY_MAP_PHY_CH_START +
                       WDDR_MEMORY_MAP_PHY_CH_OFFSET * channel;

        for (uint8_t byte = 0; byte < WDDR_PHY_DQ_BYTE_NUM; byte++)
        {
            dq_path_init(&wddr->channel[channel].dq[byte],
                          channel_base + WDDR_MEMORY_MAP_PHY_DQ_OFFSET * byte);
        }

        ca_path_init(&wddr->channel[channel].ca, channel_base + WDDR_MEMORY_MAP_PHY_CA_OFFSET);
    }

    // DFI
    dfi_init(&wddr->dfi, WDDR_MEMORY_MAP_DFI_CH0);

    // DRAM
    dram_init(&wddr->dram);

    // Notification Init
    vInitNotificationEndpoint(&wddr->endpoint, notification_handler, wddr);
    vRegisterNotificationEndpoint(&wddr->endpoint);

    // FSM
    pll_fsm_init(&wddr->fsm.pll, &wddr->pll);
    freq_switch_fsm_init(&wddr->fsm.fsw, &wddr->fsm.pll);
    dfi_master_fsm_init(&wddr->fsm.dfimstr);
    dfi_update_fsm_init(&wddr->fsm.dfiupd, wddr, wddr_iocal_update_csr, wddr_iocal_calibrate);
}

wddr_return_t wddr_boot(wddr_dev_t *wddr)
{
    uint8_t current_vco_id;

    // Calibrate Boot frequency
    #ifdef CONFIG_CALIBRATE_PLL
    pll_calibrate_vco(&wddr->pll,
                      &wddr->table->cal.freq[WDDR_PHY_BOOT_FREQ].pll,
                      &wddr->table->cfg.freq[WDDR_PHY_BOOT_FREQ].pll);
    #endif

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
        driver_override(&wddr->channel[channel].ca.tx.ca.driver, &cfg, WDDR_SLICE_TYPE_CA, CA_SLICE_CS_0, true);
        driver_override(&wddr->channel[channel].ca.tx.ca.driver, &cfg, WDDR_SLICE_TYPE_CA, CA_SLICE_CS_1, true);
        driver_override(&wddr->channel[channel].ca.tx.ca.driver, &cfg, WDDR_SLICE_TYPE_CA, CA_SLICE_CKE_0, true);
        driver_override(&wddr->channel[channel].ca.tx.ca.driver, &cfg, WDDR_SLICE_TYPE_CA, CA_SLICE_CKE_1, true);
    }

    // Switch to PHY_BOOT Frequency
    PROPAGATE_ERROR(wddr_sw_freq_switch(wddr, WDDR_PHY_BOOT_FREQ, WDDR_MSR_0));

    // Turn on LPDE / Phase Interpolators in PHY
    wddr_enable(wddr);

    // Flush PHY through DFI buffer
    wddr_clear_fifo_all_channels(wddr);
    fsw_csp_sync_reg_if();
    wddr_configure_phy(wddr, WDDR_PHY_BOOT_FREQ, WDDR_MSR_0);
    wddr_dfi_buffer_flush(wddr);
    wddr_clear_fifo_all_channels(wddr);

    // Release override of CS / CKE TX Drivers
    for (uint8_t channel = WDDR_CHANNEL_0; channel < WDDR_CHANNEL_TOTAL; channel++)
    {
        driver_cfg_t *cfg = &wddr->table->cfg.freq[WDDR_PHY_BOOT_FREQ].channel[channel].ca.tx.rank_cmn.ca.driver;
        driver_override(&wddr->channel[channel].ca.tx.ca.driver, cfg, WDDR_SLICE_TYPE_CA, CA_SLICE_CS_0, false);
        driver_override(&wddr->channel[channel].ca.tx.ca.driver, cfg, WDDR_SLICE_TYPE_CA, CA_SLICE_CS_1, false);
        driver_override(&wddr->channel[channel].ca.tx.ca.driver, cfg, WDDR_SLICE_TYPE_CA, CA_SLICE_CKE_0, false);
        driver_override(&wddr->channel[channel].ca.tx.ca.driver, cfg, WDDR_SLICE_TYPE_CA, CA_SLICE_CKE_1, false);
    }

    #ifdef CONFIG_CALIBRATE_ZQCAL
    PROPAGATE_ERROR(zqcal_calibrate(&wddr->cmn.zqcal, &wddr->table->cal.common.common.zqcal));
    #endif

    // Set VREF code
    vref_set_code(&wddr->cmn.vref, wddr->table->cal.freq[WDDR_PHY_BOOT_FREQ].common.vref.code);

    // Either calibrate Sense Amps and configure or just configure Sense Amps
    for (uint8_t channel = WDDR_CHANNEL_0; channel < WDDR_PHY_CHANNEL_NUM; channel++)
    {
        // Force chipselect since calibration assumes CS=0
        wddr_set_chip_select_reg_if(wddr, channel, WDDR_RANK_0, true);

        for (uint8_t byte = 0; byte < WDDR_PHY_DQ_BYTE_NUM; byte++)
        {
            #ifdef CONFIG_CALIBRATE_SA
            rx_gb_cfg_t cfg = {
                .data_mode = DGB_2TO1_IR,
                .fifo_mode = FGB_2TO2,
                .wck_mode = GB_WCK_MODE_0,
            };
            rx_gb_set_mode_reg_if(&wddr->channel[channel].dq[byte].rx.gearbox, WDDR_SLICE_TYPE_DQ, &cfg);
            rx_gb_set_mode_reg_if(&wddr->channel[channel].dq[byte].rx.gearbox, WDDR_SLICE_TYPE_DQS, &cfg);

            // Perform Sense Amp calibration
            sensamp_dqbyte_cal(&wddr->channel[channel].dq[byte].rx.sa,
                                   &wddr->table->cal.common.channel[channel].dq[byte].rx.sa);

            // Reset Gearbox settings
            rx_gb_set_mode_reg_if(&wddr->channel[channel].dq[byte].rx.gearbox,
                                  WDDR_SLICE_TYPE_DQ,
                                  &wddr->table->cfg.freq[WDDR_PHY_BOOT_FREQ].channel[channel].dq[byte].rx.rank_cmn.cmn.gearbox);
            rx_gb_set_mode_reg_if(&wddr->channel[channel].dq[byte].rx.gearbox,
                                  WDDR_SLICE_TYPE_DQS,
                                  &wddr->table->cfg.freq[WDDR_PHY_BOOT_FREQ].channel[channel].dq[byte].rx.rank_cmn.cmn.gearbox);
            #else
            // Write from table to PHY
            for (int8_t msr = WDDR_MSR_1; msr >= WDDR_MSR_0; msr--)
            {
                sensamp_dqbyte_set_msr_reg_if(&wddr->channel[channel].dq[byte].rx.sa, (wddr_msr_t) msr);
                for (uint8_t sa_index = 0; sa_index < WDDR_PHY_CFG; sa_index++)
                {
                    for (uint8_t rank = 0; rank < WDDR_PHY_RANK; rank++)
                    {
                        sensamp_dqbyte_dev_t *p_sa_byte = &wddr->channel[channel].dq[byte].rx.sa;
                        sensamp_dqbyte_common_cal_t *p_sa_byte_cal = &wddr->table->cal.common.channel[channel].dq[byte].rx.sa;
                        for (uint8_t bit_index = 0; bit_index < WDDR_PHY_DQ_SLICE_NUM; bit_index++)
                        {
                            sensamp_dqbit_set_cal_code_reg_if(&p_sa_byte->rank[rank].dq.bit[bit_index],
                                                            (sensamp_index_t) sa_index,
                                                            p_sa_byte_cal->dq[rank][bit_index].code[sa_index],
                                                            false);
                        } // Bit loop
                    } // Rank loop
                } // SA Index loop
            } // MSR loop
            #endif
        } // DQ Byte loop

        // Remove Chip Select Override
        wddr_set_chip_select_reg_if(wddr, channel, WDDR_RANK_0, false);
    } // Channel loop

    // Switch VCOs until on VCO_INDEX_PHY_1
    pll_fsm_get_current_vco_id(&wddr->fsm.pll, &current_vco_id);
    while (current_vco_id != VCO_INDEX_PHY_1)
    {
        PROPAGATE_ERROR(wddr_sw_freq_switch(wddr, WDDR_PHY_BOOT_FREQ, WDDR_MSR_0));
        pll_fsm_get_current_vco_id(&wddr->fsm.pll, &current_vco_id);
    }

    // Switch to HW frequency switch mode
    PROPAGATE_ERROR(freq_switch_event_hw_switch_mode(&wddr->fsm.fsw));

    return WDDR_SUCCESS;
}

wddr_return_t wddr_prep_switch(wddr_dev_t *wddr, uint8_t freq_id)
{
    uint32_t reg_val = reg_read(WDDR_MEMORY_MAP_FSW + DDR_FSW_CTRL_STA__ADR);
    uint8_t next_msr = !GET_REG_FIELD(reg_val, DDR_FSW_CTRL_STA_CMN_MSR);
    return wddr_freq_switch_prv(wddr, freq_id, next_msr, false);
}

static wddr_return_t wddr_freq_switch_prv(wddr_dev_t *wddr, uint8_t freq_id, wddr_msr_t msr, bool sw_switch)
{
    fs_prep_data_t fs_prep_data = {
        .msr = msr,
        .prep_data = {
            .freq_id = freq_id,
            .cal = &wddr->table->cal.freq[freq_id].pll,
            .cfg = &wddr->table->cfg.freq[freq_id].pll,
        },
    };

    vReInitCompletion(&wddr->fsw_event);

    freq_switch_event_prep(&wddr->fsm.fsw, &fs_prep_data);

    // Ensure FSM is in WAIT_FOR_SWITCH state before performing switch
    vWaitForCompletion(&wddr->fsw_event);
    configASSERT(wddr->fsm.fsw.fsm.current_state == FS_STATE_WAIT_FOR_SWITCH);
    if (wddr->fsm.fsw.fsm.current_state != FS_STATE_WAIT_FOR_SWITCH)
    {
        return WDDR_ERROR;
    }

    // Perform frequency switch
    if (sw_switch)
    {
        freq_switch_event_sw_switch(&wddr->fsm.fsw);

        // Wait for switch to complete
        vWaitForCompletion(&wddr->fsw_event);
        configASSERT(wddr->fsm.fsw.fsm.current_state == FS_STATE_IDLE);
        if (wddr->fsm.fsw.fsm.current_state != FS_STATE_IDLE)
        {
            return WDDR_ERROR;
        }
    }

    return WDDR_SUCCESS;
}

static wddr_return_t wddr_sw_freq_switch(wddr_dev_t *wddr, uint8_t freq_id, wddr_msr_t msr)
{
    return wddr_freq_switch_prv(wddr, freq_id, msr, true);
}

static void notification_handler(Notification_t notification, void *args)
{
    wddr_dev_t *wddr = (wddr_dev_t *) args;
    if (notification == WDDR_NOTIF_FSW_PREP_DONE ||
        notification == WDDR_NOTIF_FSW_DONE ||
        notification == WDDR_NOTIF_FSW_FAILED)
    {
        vComplete(&wddr->fsw_event);
    }
}

static void wddr_enable(wddr_dev_t *wddr)
{
    for (uint8_t channel = WDDR_CHANNEL_0; channel < WDDR_PHY_CHANNEL_NUM; channel++)
    {
        for (uint8_t byte = 0; byte < WDDR_PHY_DQ_BYTE_NUM; byte++)
        {
            dq_path_enable(&wddr->channel[channel].dq[byte]);
        }
        ca_path_enable(&wddr->channel[channel].ca);
    }
}

static void wddr_dfi_buffer_flush(wddr_dev_t *wddr)
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
    dfi_buffer_fill_and_send_packets(&wddr->dfi.dfi_buffer, &packet_list);

    // Must disable buffer when done
    dfi_buffer_disable(&wddr->dfi.dfi_buffer);
}

static void wddr_clear_fifo_all_channels(wddr_dev_t *wddr)
{
    for (uint8_t channel = WDDR_CHANNEL_0; channel <= WDDR_CHANNEL_1; channel++)
    {
        wddr_clear_fifo_reg_if(wddr, channel);
    }
}

static void wddr_iocal_update_csr(void *dev)
{
    wddr_dev_t *wddr = (wddr_dev_t *) dev;
    uint8_t freq_id;

    // Get current pll frequency id
    pll_fsm_get_current_freq(&wddr->fsm.pll, &freq_id);

    // Update IOCAL
    for (uint8_t channel = 0; channel < WDDR_PHY_CHANNEL_NUM; channel++)
    {
        for (uint8_t rank = 0; rank < WDDR_PHY_RANK; rank++)
        {
            // Update CK
            driver_cmn_set_code_reg_if(&wddr->channel[channel].ca.tx.rank[rank].ck.driver,
                wddr->table->cal.freq[freq_id].channel[channel].ca.tx.rank[rank].ck.driver.code);

            // Update DQS
            for (uint8_t dq_byte = 0; dq_byte < WDDR_PHY_DQ_BYTE_NUM; dq_byte++)
            {
                driver_cmn_set_code_reg_if(&wddr->channel[channel].dq[dq_byte].tx.rank[rank].dqs.driver,
                    wddr->table->cal.freq[freq_id].channel[channel].dq[dq_byte].tx.rank[rank].dqs.driver.code);
            }
        }
    }
}

static void wddr_iocal_calibrate(void *dev)
{
    wddr_dev_t *wddr = (wddr_dev_t *) dev;
    zqcal_calibrate(&wddr->cmn.zqcal, &wddr->table->cal.common.common.zqcal);
}

/*******************************************************************************
**                     WAVIOUS DDR PHY PREP FUNCTIONS
*******************************************************************************/

/**
 * @brief   DQ RX Path Frequency Switch Prep
 *
 * @details Prepares DQ RX path for the given DQ Byte for a frequency switch.
 *
 * @param[in]   dq_path     pointer to dq path structure to prepare.
 * @param[in]   msr         Mode Set Register value to prepare.
 * @param[in]   cal         pointer to the frequency calibration parameters for
 *                          rx path.
 * @param[in]   cfg         pointer to the frequency configuration parameters for
 *                          rx path.
 * @param[in]   common_cal  pointer to the frequency independent calibration
 *                          parameters for rx path.
 *
 * @return      void
 */
static inline void rx_path_dq_block_freq_switch_prep(dq_rx_path_t *dq_path,
                                                     wddr_msr_t msr,
                                                     dq_rx_path_freq_cal_t *cal,
                                                     dq_rx_path_freq_cfg_t *cfg,
                                                     dq_rx_path_common_cal_t *common_cal)
{
    uint32_t reg_val;

    // Initialize DQ Block
    for (uint8_t rank_index = 0; rank_index < WDDR_PHY_RANK; rank_index++)
    {
        // PI
        rx_pi_set_msr_reg_if(&dq_path->rank[rank_index].dqs.pi, msr, WDDR_SLICE_TYPE_DQS, rank_index);

        // REN
        pi_set_reg_if(&dq_path->rank[rank_index].dqs.pi.ren,
                      cal->rank[rank_index].dqs.pi.ren.code,
                      cfg->rank[rank_index].dqs.pi.gear,
                      cfg->rank[rank_index].dqs.pi.xcpl,
                      PI_STATE_ENABLED);

        // RCS
        pi_set_reg_if(&dq_path->rank[rank_index].dqs.pi.rcs,
                      cal->rank[rank_index].dqs.pi.rcs.code,
                      cfg->rank[rank_index].dqs.pi.gear,
                      cfg->rank[rank_index].dqs.pi.xcpl,
                      PI_STATE_ENABLED);

        // RDQS
        pi_set_reg_if(&dq_path->rank[rank_index].dqs.pi.rdqs,
                      cal->rank[rank_index].dqs.pi.rdqs.code,
                      cfg->rank[rank_index].dqs.pi.gear,
                      cfg->rank[rank_index].dqs.pi.xcpl,
                      PI_STATE_ENABLED);

        // LPDE
        rx_lpde_set_msr_reg_if(&dq_path->rank[rank_index].dqs.sdr_lpde,
                               msr,
                               WDDR_SLICE_TYPE_DQS,
                               rank_index);
        lpde_set_reg_if(&dq_path->rank[rank_index].dqs.sdr_lpde,
                        cfg->rank[rank_index].dqs.sdr_lpde.gear,
                        cal->rank[rank_index].dqs.sdr_lpde.delay,
                        LPDE_STATE_ENABLED);

        // RECEIVER
        // Set MSR
        receiver_set_msr_reg_if(&dq_path->rank[rank_index].dqs.receiver, msr, rank_index);
        receiver_delay_set_msr_reg_if(&dq_path->rank[rank_index].dqs.receiver.rx_delay, msr, rank_index);

        // Set Receiver Register in one write
        reg_val = UPDATE_REG_FIELD(DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG__POR,
                                   DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_CAL_P_C,
                                   common_cal->rank[rank_index].dqs.receiver.cal_code[REC_P_SIDE][REC_C_SIDE]);

        reg_val = UPDATE_REG_FIELD(reg_val,
                                   DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_CAL_P_T,
                                   common_cal->rank[rank_index].dqs.receiver.cal_code[REC_P_SIDE][REC_T_SIDE]);

        reg_val = UPDATE_REG_FIELD(reg_val,
                                   DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_CAL_N_C,
                                   common_cal->rank[rank_index].dqs.receiver.cal_code[REC_N_SIDE][REC_C_SIDE]);

        reg_val = UPDATE_REG_FIELD(reg_val,
                                   DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_CAL_N_T,
                                   common_cal->rank[rank_index].dqs.receiver.cal_code[REC_N_SIDE][REC_T_SIDE]);

        reg_val = UPDATE_REG_FIELD(reg_val,
                                   DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_FB_EN,
                                   cfg->rank[rank_index].dqs.receiver.feedback_resistor);

        reg_val = UPDATE_REG_FIELD(reg_val,
                                   DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_SE_MODE,
                                   cfg->rank[rank_index].dqs.receiver.mode);

        reg_val = UPDATE_REG_FIELD(reg_val,
                                   DDR_DQ_DQS_RX_IO_CMN_M0_R0_CFG_DCPATH_EN,
                                   cfg->rank[rank_index].dqs.receiver.path_state);

        reg_write(dq_path->rank[rank_index].dqs.receiver.base, reg_val);

        reg_val = UPDATE_REG_FIELD(0,
                                   DDR_DQ_DQS_RX_IO_M0_R0_CFG_0_DLY_CTRL_T,
                                   cal->rank[rank_index].dqs.receiver.rx_delay[REC_T_SIDE]);

        reg_val = UPDATE_REG_FIELD(reg_val,
                                   DDR_DQ_DQS_RX_IO_M0_R0_CFG_0_DLY_CTRL_C,
                                   cal->rank[rank_index].dqs.receiver.rx_delay[REC_C_SIDE]);

        reg_write(dq_path->rank[rank_index].dqs.receiver.rx_delay.base, reg_val);
    }

    rx_gb_set_msr_reg_if(&dq_path->gearbox, msr, WDDR_SLICE_TYPE_DQ);
    rx_gb_set_mode_reg_if(&dq_path->gearbox, WDDR_SLICE_TYPE_DQ, &cfg->rank_cmn.cmn.gearbox);
    rx_gb_set_mode_reg_if(&dq_path->gearbox, WDDR_SLICE_TYPE_DQS, &cfg->rank_cmn.cmn.gearbox);
}

/**
 * @brief   CA RX Path Frequency Switch Prep
 *
 * @details Prepares CA RX Path for a frequency switch.
 *
 * @param[in]   ca_path     pointer to ca path structure to prepare.
 * @param[in]   msr         Mode Set Register value to prepare.
 * @param[in]   cal         pointer to the frequency calibration parameters for
 *                          rx path.
 * @param[in]   cfg         pointer to the frequency configuration parameters
 *                          for rx path.
 *
 * @return      void
 */
static inline void rx_path_ca_block_freq_switch_prep(ca_rx_path_t *ca_path,
                                                     wddr_msr_t msr,
                                                     ca_rx_path_freq_cal_t *cal,
                                                     ca_rx_path_freq_cfg_t *cfg)
{
    for (uint8_t rank_index = 0; rank_index < WDDR_PHY_RANK; rank_index++)
    {
        // LPDE
        rx_lpde_set_msr_reg_if(&ca_path->rank[rank_index].ck.sdr_lpde,
                               msr,
                               WDDR_SLICE_TYPE_CK,
                               rank_index);
        lpde_set_reg_if(&ca_path->rank[rank_index].ck.sdr_lpde,
                        cfg->rank[rank_index].ck.sdr_lpde.gear,
                        cal->rank[rank_index].ck.sdr_lpde.delay,
                        LPDE_STATE_ENABLED);
    }

    rx_gb_set_msr_reg_if(&ca_path->gearbox, msr, WDDR_SLICE_TYPE_CA);
    rx_gb_set_mode_reg_if(&ca_path->gearbox, WDDR_SLICE_TYPE_CA, &cfg->rank_cmn.cmn.gearbox);
    rx_gb_set_mode_reg_if(&ca_path->gearbox, WDDR_SLICE_TYPE_CK, &cfg->rank_cmn.cmn.gearbox);
}

/**
 * @brief   TX Bit Pipeline Prep CA MSR0
 *
 * @details Prepares the CA TX pipeline for a frequency switch for MSR0.
 *
 * @param[in]   ca_path     pointer to ca path to prepare.
 * @param[in]   rank_index  rank index to prepare.
 * @param[in]   cal         pointer to calibration structure for ca path.
 * @param[in]   cfg         pointer to configuration structure for ca path.
 *
 * @return      void
 */
static inline void tx_bit_pipeline_prep_ca_msr0(ca_tx_path_t *ca_path,
                                                wddr_rank_t rank_index,
                                                ca_tx_path_freq_cal_t *cal,
                                                ca_tx_path_freq_cfg_t *cfg)
{
    for (uint8_t bit_index = 0; bit_index < WDDR_PHY_CA_SLICE_NUM; bit_index++)
    {
        // MSR
        ca_path->rank[rank_index].ca.pipeline[bit_index].sdr.msr = WDDR_MSR_0;
        ca_path->rank[rank_index].ca.pipeline[bit_index].ddr.msr = WDDR_MSR_0;
        ca_path->rank[rank_index].ca.pipeline[bit_index].qdr.msr = WDDR_MSR_0;

        // SDR
        reg_write(ca_path->rank[rank_index].ca.pipeline[bit_index].sdr.base + SDR_CA_FC_DLY__OFFSET,
                    cal->rank[rank_index].ca.pipeline.sdr.fc_delay);
        reg_write(ca_path->rank[rank_index].ca.pipeline[bit_index].sdr.base + SDR_CA_PIPE_EN__OFFSET,
                    cal->rank[rank_index].ca.pipeline.sdr.pipe_en);
        reg_write(ca_path->rank[rank_index].ca.pipeline[bit_index].sdr.base + SDR_CA_X_SEL__OFFSET,
                    cal->rank[rank_index].ca.pipeline.sdr.x_sel);

        // DDR
        reg_write(ca_path->rank[rank_index].ca.pipeline[bit_index].ddr.base + DDR_CA_PIPE_EN__OFFSET,
                    cal->rank[rank_index].ca.pipeline.ddr.pipe_en);
        reg_write(ca_path->rank[rank_index].ca.pipeline[bit_index].ddr.base + DDR_CA_X_SEL__OFFSET,
                    cal->rank[rank_index].ca.pipeline.ddr.x_sel);

        // QDR
        reg_write(ca_path->rank[rank_index].ca.pipeline[bit_index].qdr.base + QDR_CA_PIPE_EN__OFFSET,
                    cal->rank[rank_index].ca.pipeline.qdr.pipe_en);
        reg_write(ca_path->rank[rank_index].ca.pipeline[bit_index].qdr.base + QDR_CA_X_SEL__OFFSET,
                    cal->rank[rank_index].ca.pipeline.qdr.x_sel);

        // LPDE
        tx_lpde_set_msr_reg_if(&ca_path->rank[rank_index].ca.lpde[bit_index],
                               WDDR_MSR_0,
                               WDDR_SLICE_TYPE_CA,
                               rank_index,
                                bit_index);
        lpde_set_reg_if(&ca_path->rank[rank_index].ca.lpde[bit_index],
                        cfg->rank[rank_index].cmn.lpde.gear,
                        cal->rank[rank_index].ca.lpde[bit_index].delay,
                        LPDE_STATE_ENABLED);
    }
}

/**
 * @brief   TX Bit Pipeline Prep CA MSR1
 *
 * @details Prepares the CA TX pipeline for a frequency switch for MSR1.
 *
 * @param[in]   ca_path     pointer to ca path to prepare.
 * @param[in]   rank_index  rank index to prepare.
 * @param[in]   cal         pointer to calibration structure for ca path.
 * @param[in]   cfg         pointer to configuration structure for ca path.
 *
 * @return      void
 */
static inline void tx_bit_pipeline_prep_ca_msr1(ca_tx_path_t *ca_path,
                                                wddr_rank_t rank_index,
                                                ca_tx_path_freq_cal_t *cal,
                                                ca_tx_path_freq_cfg_t *cfg)
{
    for (uint8_t bit_index = 0; bit_index < WDDR_PHY_CA_SLICE_NUM; bit_index++)
    {
        // MSR
        ca_path->rank[rank_index].ca.pipeline[bit_index].sdr.msr = WDDR_MSR_1;
        ca_path->rank[rank_index].ca.pipeline[bit_index].ddr.msr = WDDR_MSR_1;
        ca_path->rank[rank_index].ca.pipeline[bit_index].qdr.msr = WDDR_MSR_1;

        // SDR
        reg_write(ca_path->rank[rank_index].ca.pipeline[bit_index].sdr.base +
                  SDR_CA_FC_DLY__OFFSET + SDR_CA_MODE__OFFSET,
                  cal->rank[rank_index].ca.pipeline.sdr.fc_delay);

        reg_write(ca_path->rank[rank_index].ca.pipeline[bit_index].sdr.base +
                  SDR_CA_PIPE_EN__OFFSET + SDR_CA_MODE__OFFSET,
                  cal->rank[rank_index].ca.pipeline.sdr.pipe_en);

        reg_write(ca_path->rank[rank_index].ca.pipeline[bit_index].sdr.base +
                  SDR_CA_X_SEL__OFFSET + SDR_CA_MODE__OFFSET,
                  cal->rank[rank_index].ca.pipeline.sdr.x_sel);

        // DDR
        reg_write(ca_path->rank[rank_index].ca.pipeline[bit_index].ddr.base +
                  DDR_CA_PIPE_EN__OFFSET + DDR_CA_MODE__OFFSET,
                  cal->rank[rank_index].ca.pipeline.ddr.pipe_en);

        reg_write(ca_path->rank[rank_index].ca.pipeline[bit_index].ddr.base +
                  DDR_CA_X_SEL__OFFSET + DDR_CA_MODE__OFFSET,
                  cal->rank[rank_index].ca.pipeline.ddr.x_sel);

        // QDR
        reg_write(ca_path->rank[rank_index].ca.pipeline[bit_index].qdr.base +
                  QDR_CA_PIPE_EN__OFFSET + QDR_CA_MODE__OFFSET,
                  cal->rank[rank_index].ca.pipeline.qdr.pipe_en);

        reg_write(ca_path->rank[rank_index].ca.pipeline[bit_index].qdr.base +
                  QDR_CA_X_SEL__OFFSET + QDR_CA_MODE__OFFSET,
                  cal->rank[rank_index].ca.pipeline.qdr.x_sel);

        // LPDE
        tx_lpde_set_msr_reg_if(&ca_path->rank[rank_index].ca.lpde[bit_index],
                               WDDR_MSR_1,
                               WDDR_SLICE_TYPE_CA,
                               rank_index,
                               bit_index);
        lpde_set_reg_if(&ca_path->rank[rank_index].ca.lpde[bit_index],
                        cfg->rank[rank_index].cmn.lpde.gear,
                        cal->rank[rank_index].ca.lpde[bit_index].delay,
                        LPDE_STATE_ENABLED);
    }
}

/**
 * @brief   TX Bit Pipeline Prep CK MSR0
 *
 * @details Prepares the CK TX pipeline for a frequency switch for MSR0.
 *
 * @param[in]   ca_path     pointer to ca path to prepare.
 * @param[in]   rank_index  rank index to prepare.
 * @param[in]   cal         pointer to calibration structure for ca path.
 * @param[in]   cfg         pointer to configuration structure for ca path.
 *
 * @return      void
 */
static inline void tx_bit_pipeline_prep_ck_msr0(ca_tx_path_t *ca_path,
                                                wddr_rank_t rank_index,
                                                ca_tx_path_freq_cal_t *cal,
                                                ca_tx_path_freq_cfg_t *cfg)
{
    for (uint8_t bit_index = 0; bit_index < WDDR_PHY_CK_SLICE_NUM; bit_index++)
    {
        // MSR
        ca_path->rank[rank_index].ck.pipeline[bit_index].sdr.msr = WDDR_MSR_0;
        ca_path->rank[rank_index].ck.pipeline[bit_index].ddr.msr = WDDR_MSR_0;
        ca_path->rank[rank_index].ck.pipeline[bit_index].qdr.msr = WDDR_MSR_0;

        // SDR
        reg_write(ca_path->rank[rank_index].ck.pipeline[bit_index].sdr.base + SDR_CK_FC_DLY__OFFSET,
                    cal->rank[rank_index].ck.pipeline.sdr.fc_delay);
        reg_write(ca_path->rank[rank_index].ck.pipeline[bit_index].sdr.base + SDR_CK_PIPE_EN__OFFSET,
                    cal->rank[rank_index].ck.pipeline.sdr.pipe_en);
        reg_write(ca_path->rank[rank_index].ck.pipeline[bit_index].sdr.base + SDR_CK_X_SEL__OFFSET,
                    cal->rank[rank_index].ck.pipeline.sdr.x_sel);

        // DDR
        reg_write(ca_path->rank[rank_index].ck.pipeline[bit_index].ddr.base + DDR_CK_PIPE_EN__OFFSET,
                    cal->rank[rank_index].ck.pipeline.ddr.pipe_en);
        reg_write(ca_path->rank[rank_index].ck.pipeline[bit_index].ddr.base + DDR_CK_X_SEL__OFFSET,
                    cal->rank[rank_index].ck.pipeline.ddr.x_sel);

        // QDR
        reg_write(ca_path->rank[rank_index].ck.pipeline[bit_index].qdr.base + QDR_CK_PIPE_EN__OFFSET,
                    cal->rank[rank_index].ck.pipeline.qdr.pipe_en);
        reg_write(ca_path->rank[rank_index].ck.pipeline[bit_index].qdr.base + QDR_CK_X_SEL__OFFSET,
                    cal->rank[rank_index].ck.pipeline.qdr.x_sel);

        // LPDE
        if (bit_index < WDDR_PHY_CK_TXRX_SLICE_NUM)
        {
            tx_lpde_set_msr_reg_if(&ca_path->rank[rank_index].ck.lpde[bit_index],
                                   WDDR_MSR_0,
                                   WDDR_SLICE_TYPE_CK,
                                   rank_index,
                                    bit_index);
            lpde_set_reg_if(&ca_path->rank[rank_index].ck.lpde[bit_index],
                            cfg->rank[rank_index].cmn.lpde.gear,
                            cal->rank[rank_index].ck.lpde[bit_index].delay,
                            LPDE_STATE_ENABLED);
        }
    }
}

/**
 * @brief   TX Bit Pipeline Prep CK MSR1
 *
 * @details Prepares the CK TX pipeline for a frequency switch for MSR1.
 *
 * @param[in]   ca_path     pointer to ca path to prepare.
 * @param[in]   rank_index  rank index to prepare.
 * @param[in]   cal         pointer to calibration structure for ca path.
 * @param[in]   cfg         pointer to configuration structure for ca path.
 *
 * @return      void
 */
static inline void tx_bit_pipeline_prep_ck_msr1(ca_tx_path_t *ca_path,
                                                wddr_rank_t rank_index,
                                                ca_tx_path_freq_cal_t *cal,
                                                ca_tx_path_freq_cfg_t *cfg)
{
    for (uint8_t bit_index = 0; bit_index < WDDR_PHY_CK_SLICE_NUM; bit_index++)
    {
        // MSR
        ca_path->rank[rank_index].ck.pipeline[bit_index].sdr.msr = WDDR_MSR_1;
        ca_path->rank[rank_index].ck.pipeline[bit_index].ddr.msr = WDDR_MSR_1;
        ca_path->rank[rank_index].ck.pipeline[bit_index].qdr.msr = WDDR_MSR_1;

        // SDR
        reg_write(ca_path->rank[rank_index].ck.pipeline[bit_index].sdr.base +
                  SDR_CK_FC_DLY__OFFSET + SDR_CK_MODE__OFFSET,
                  cal->rank[rank_index].ck.pipeline.sdr.fc_delay);

        reg_write(ca_path->rank[rank_index].ck.pipeline[bit_index].sdr.base +
                  SDR_CK_PIPE_EN__OFFSET + SDR_CK_MODE__OFFSET,
                  cal->rank[rank_index].ck.pipeline.sdr.pipe_en);

        reg_write(ca_path->rank[rank_index].ck.pipeline[bit_index].sdr.base +
                  SDR_CK_X_SEL__OFFSET + SDR_CK_MODE__OFFSET,
                  cal->rank[rank_index].ck.pipeline.sdr.x_sel);

        // DDR
        reg_write(ca_path->rank[rank_index].ck.pipeline[bit_index].ddr.base +
                  DDR_CK_PIPE_EN__OFFSET + DDR_CK_MODE__OFFSET,
                  cal->rank[rank_index].ck.pipeline.ddr.pipe_en);

        reg_write(ca_path->rank[rank_index].ck.pipeline[bit_index].ddr.base +
                  DDR_CK_X_SEL__OFFSET + DDR_CK_MODE__OFFSET,
                  cal->rank[rank_index].ck.pipeline.ddr.x_sel);

        // QDR
        reg_write(ca_path->rank[rank_index].ck.pipeline[bit_index].qdr.base +
                  QDR_CK_PIPE_EN__OFFSET + QDR_CK_MODE__OFFSET,
                  cal->rank[rank_index].ck.pipeline.qdr.pipe_en);

        reg_write(ca_path->rank[rank_index].ck.pipeline[bit_index].qdr.base +
                  QDR_CK_X_SEL__OFFSET + QDR_CK_MODE__OFFSET,
                  cal->rank[rank_index].ck.pipeline.qdr.x_sel);

        // LPDE
        if (bit_index < WDDR_PHY_CK_TXRX_SLICE_NUM)
        {
            tx_lpde_set_msr_reg_if(&ca_path->rank[rank_index].ck.lpde[bit_index],
                                   WDDR_MSR_1,
                                   WDDR_SLICE_TYPE_CK,
                                   rank_index,
                                   bit_index);
            lpde_set_reg_if(&ca_path->rank[rank_index].ck.lpde[bit_index],
                            cfg->rank[rank_index].cmn.lpde.gear,
                            cal->rank[rank_index].ck.lpde[bit_index].delay,
                            LPDE_STATE_ENABLED);
        }
    }
}

/**
 * @brief   CA TX Path Frequency Switch Prep
 *
 * @details Prepares CA TX Path for a frequency switch.
 *
 * @param[in]   ca_path     pointer to ca path structure to prepare.
 * @param[in]   msr         Mode Set Register value to prepare.
 * @param[in]   cal         pointer to the frequency calibration parameters
 *                          for tx path.
 * @param[in]   cfg         pointer to the frequency configuration parameters
 *                          for tx path.
 *
 * @return      void
 */
static inline void tx_path_ca_block_freq_switch_prep(ca_tx_path_t *ca_path,
                                                     wddr_msr_t msr,
                                                     ca_tx_path_freq_cal_t *cal,
                                                     ca_tx_path_freq_cfg_t *cfg)
{
    uint32_t reg_val = 0, ca_reg_val = 0, ck_reg_val = 0;
    uint32_t ca_base;
    uint32_t ck_base;

    // Initialize CA Block
    for (uint8_t rank_index = 0; rank_index < WDDR_PHY_RANK; rank_index++)
    {
        if (msr == WDDR_MSR_1)
        {
            tx_bit_pipeline_prep_ca_msr1(ca_path, rank_index, cal, cfg);
            tx_bit_pipeline_prep_ck_msr1(ca_path, rank_index, cal, cfg);
        }
        else
        {
            tx_bit_pipeline_prep_ca_msr0(ca_path, rank_index, cal, cfg);
            tx_bit_pipeline_prep_ck_msr0(ca_path, rank_index, cal, cfg);
        }

        /************************************************
        **                     CA
        ************************************************/
        rt_pipeline_set_msr_reg_if(&ca_path->rank[rank_index].ca.rt, msr);
        rt_pipeline_set_pipe_en_reg_if(&ca_path->rank[rank_index].ca.rt, WDDR_SLICE_TYPE_CA, cal->rank[rank_index].ca.rt.pipe_en);

        // TX PI
        tx_pi_set_msr_reg_if(&ca_path->rank[rank_index].ca.pi,
                             msr,
                             WDDR_SLICE_TYPE_CA,
                             rank_index);

        // ODR
        pi_set_reg_if(&ca_path->rank[rank_index].ca.pi.odr,
                      cal->rank[rank_index].ca.pi.odr.code,
                      cfg->rank[rank_index].ca.pi.odr.gear,
                      cfg->rank[rank_index].ca.pi.odr.xcpl,
                      PI_STATE_ENABLED);

        // QDR
        pi_set_reg_if(&ca_path->rank[rank_index].ca.pi.qdr,
                      cal->rank[rank_index].ca.pi.qdr.code,
                      cfg->rank[rank_index].ca.pi.qdr.gear,
                      cfg->rank[rank_index].ca.pi.qdr.xcpl,
                      PI_STATE_ENABLED);

        // DDR
        pi_set_reg_if(&ca_path->rank[rank_index].ca.pi.ddr,
                      cal->rank[rank_index].ca.pi.ddr.code,
                      cfg->rank[rank_index].ca.pi.ddr.gear,
                      cfg->rank[rank_index].ca.pi.ddr.xcpl,
                      PI_STATE_ENABLED);

        // RT
        pi_set_reg_if(&ca_path->rank[rank_index].ca.pi.rt,
                      cal->rank[rank_index].ca.pi.rt.code,
                      cfg->rank[rank_index].ca.pi.rt.gear,
                      cfg->rank[rank_index].ca.pi.rt.xcpl,
                      PI_STATE_ENABLED);

        /************************************************
        **                     CK
        ************************************************/
        rt_pipeline_set_msr_reg_if(&ca_path->rank[rank_index].ck.rt, msr);
        rt_pipeline_set_pipe_en_reg_if(&ca_path->rank[rank_index].ck.rt, WDDR_SLICE_TYPE_CK, cal->rank[rank_index].ck.rt.pipe_en);

        // TX PI
        tx_pi_set_msr_reg_if(&ca_path->rank[rank_index].ck.pi,
                             msr,
                             WDDR_SLICE_TYPE_CK,
                             rank_index);

        // ODR
        pi_set_reg_if(&ca_path->rank[rank_index].ck.pi.odr,
                      cal->rank[rank_index].ck.pi.odr.code,
                      cfg->rank[rank_index].ck.pi.odr.gear,
                      cfg->rank[rank_index].ck.pi.odr.xcpl,
                      PI_STATE_ENABLED);

        // QDR
        pi_set_reg_if(&ca_path->rank[rank_index].ck.pi.qdr,
                      cal->rank[rank_index].ck.pi.qdr.code,
                      cfg->rank[rank_index].ck.pi.qdr.gear,
                      cfg->rank[rank_index].ck.pi.qdr.xcpl,
                      PI_STATE_ENABLED);

        // DDR
        pi_set_reg_if(&ca_path->rank[rank_index].ck.pi.ddr,
                      cal->rank[rank_index].ck.pi.ddr.code,
                      cfg->rank[rank_index].ck.pi.ddr.gear,
                      cfg->rank[rank_index].ck.pi.ddr.xcpl,
                      PI_STATE_ENABLED);

        // RT
        pi_set_reg_if(&ca_path->rank[rank_index].ck.pi.rt,
                      cal->rank[rank_index].ck.pi.rt.code,
                      cfg->rank[rank_index].ck.pi.rt.gear,
                      cfg->rank[rank_index].ck.pi.rt.xcpl,
                      PI_STATE_ENABLED);

        // SDR
        pi_set_reg_if(&ca_path->rank[rank_index].ck.pi.sdr,
                      0,  // No code for this PI
                      cfg->rank[rank_index].ck.pi.sdr.gear,
                      cfg->rank[rank_index].ck.pi.sdr.xcpl,
                      PI_STATE_ENABLED);

        // DFI
        pi_set_reg_if(&ca_path->rank[rank_index].ck.pi.dfi,
                      0,    // No code for this PI
                      cfg->rank[rank_index].ck.pi.dfi.gear,
                      cfg->rank[rank_index].ck.pi.dfi.xcpl,
                      PI_STATE_ENABLED);

        // CMN
        driver_cmn_set_msr_reg_if(&ca_path->rank[rank_index].ck.driver, msr, WDDR_SLICE_TYPE_CK, rank_index);

        reg_val = UPDATE_REG_FIELD(0,
                                   DDR_CA_DQS_TX_IO_CMN_M0_R0_CFG_NCAL,
                                   cal->rank[rank_index].ck.driver.code[DRVR_N_CAL]);

        reg_val = UPDATE_REG_FIELD(reg_val,
                                   DDR_CA_DQS_TX_IO_CMN_M0_R0_CFG_PCAL,
                                   cal->rank[rank_index].ck.driver.code[DRVR_P_CAL]);

        reg_val = UPDATE_REG_FIELD(reg_val,
                                   DDR_CA_DQS_TX_IO_CMN_M0_R0_CFG_SE_MODE,
                                   cfg->rank[rank_index].ck.driver.mode);

        reg_write(ca_path->rank[rank_index].ck.driver.base, reg_val);
    }

    // Bit TX Driver
    driver_set_msr_reg_if(&ca_path->ca.driver, msr, WDDR_SLICE_TYPE_CA);
    driver_set_msr_reg_if(&ca_path->ck.driver, msr, WDDR_SLICE_TYPE_CK);

    ca_base = ca_path->ca.driver.base;
    ck_base = ca_path->ck.driver.base;

    // CA
    ca_reg_val = UPDATE_REG_FIELD(ca_reg_val, DDR_CA_DQ_TX_IO_M0_CFG_0_OVRD_SEL, cfg->rank_cmn.ca.driver.override.sel);
    ca_reg_val = UPDATE_REG_FIELD(ca_reg_val, DDR_CA_DQ_TX_IO_M0_CFG_0_OVRD_VAL, cfg->rank_cmn.ca.driver.override.val_t);
    ca_reg_val = UPDATE_REG_FIELD(ca_reg_val, DDR_CA_DQ_TX_IO_M0_CFG_0_TX_IMPD, cfg->rank_cmn.ca.driver.tx_impd);
    ca_reg_val = UPDATE_REG_FIELD(ca_reg_val, DDR_CA_DQ_TX_IO_M0_CFG_0_RX_IMPD, cfg->rank_cmn.ca.driver.rx_impd);
    ca_reg_val = UPDATE_REG_FIELD(ca_reg_val, DDR_CA_DQ_TX_IO_M0_CFG_0_SW_OVR, 0);

    // CK
    ck_reg_val = UPDATE_REG_FIELD(ck_reg_val, DDR_CA_DQS_TX_IO_M0_CFG_0_OVRD_SEL, cfg->rank_cmn.ck.driver.override.sel);
    ck_reg_val = UPDATE_REG_FIELD(ck_reg_val, DDR_CA_DQS_TX_IO_M0_CFG_0_OVRD_VAL_T, cfg->rank_cmn.ck.driver.override.val_t);
    ck_reg_val = UPDATE_REG_FIELD(ck_reg_val, DDR_CA_DQS_TX_IO_M0_CFG_0_OVRD_VAL_C, cfg->rank_cmn.ck.driver.override.val_c);
    ck_reg_val = UPDATE_REG_FIELD(ck_reg_val, DDR_CA_DQS_TX_IO_M0_CFG_0_TX_IMPD, cfg->rank_cmn.ck.driver.tx_impd);
    ck_reg_val = UPDATE_REG_FIELD(ck_reg_val, DDR_CA_DQS_TX_IO_M0_CFG_0_RX_IMPD, cfg->rank_cmn.ck.driver.rx_impd);
    ck_reg_val = UPDATE_REG_FIELD(ck_reg_val, DDR_CA_DQS_TX_IO_M0_CFG_0_SW_OVR, 0);

    // CA
    for (uint8_t bit_index = 0; bit_index < WDDR_PHY_CA_SLICE_NUM; bit_index++)
    {

        egress_dig_set_msr_reg_if(&ca_path->ca.pipeline[bit_index].egress_dig, msr);
        egress_ana_set_msr_reg_if(&ca_path->ca.pipeline[bit_index].egress_ana, msr);
        if (msr == WDDR_MSR_1)
        {
            reg_write(ca_path->ca.pipeline[bit_index].egress_dig.base +
                      EGRESS_DIG_CA_CFG__OFFSET + EGRESS_DIG_CA_MODE__OFFSET,
                      cfg->rank_cmn.cmn.pipeline.egress_dig_mode);

            reg_write(ca_path->ca.pipeline[bit_index].egress_ana.base +
                      EGRESS_ANA_CA_CFG__OFFSET + EGRESS_ANA_CA_MODE__OFFSET,
                      cfg->rank_cmn.cmn.pipeline.egress_ana_mode);

        }
        else
        {
            reg_write(ca_path->ca.pipeline[bit_index].egress_dig.base +
                      EGRESS_DIG_CA_CFG__OFFSET,
                      cfg->rank_cmn.cmn.pipeline.egress_dig_mode);

            reg_write(ca_path->ca.pipeline[bit_index].egress_ana.base +
                      EGRESS_ANA_CA_CFG__OFFSET,
                      cfg->rank_cmn.cmn.pipeline.egress_ana_mode);
        }
        reg_write(ca_base, ca_reg_val);
        ca_base += 4;
    }

    // CK
    for (uint8_t bit_index = 0; bit_index < WDDR_PHY_CK_SLICE_NUM; bit_index++)
    {
        egress_dig_set_msr_reg_if(&ca_path->ck.pipeline[bit_index].egress_dig, msr);
        egress_ana_set_msr_reg_if(&ca_path->ck.pipeline[bit_index].egress_ana, msr);
        if (msr == WDDR_MSR_1)
        {
            reg_write(ca_path->ck.pipeline[bit_index].egress_dig.base +
                      EGRESS_DIG_CK_CFG__OFFSET + EGRESS_DIG_CK_MODE__OFFSET,
                      cfg->rank_cmn.cmn.pipeline.egress_dig_mode);

            reg_write(ca_path->ck.pipeline[bit_index].egress_ana.base +
                      EGRESS_ANA_CK_CFG__OFFSET + EGRESS_ANA_CK_MODE__OFFSET,
                      cfg->rank_cmn.cmn.pipeline.egress_ana_mode);

        }
        else
        {
            reg_write(ca_path->ck.pipeline[bit_index].egress_dig.base +
                      EGRESS_DIG_CK_CFG__OFFSET,
                      cfg->rank_cmn.cmn.pipeline.egress_dig_mode);

            reg_write(ca_path->ck.pipeline[bit_index].egress_ana.base +
                      EGRESS_ANA_CK_CFG__OFFSET,
                      cfg->rank_cmn.cmn.pipeline.egress_ana_mode);
        }
        if (bit_index < WDDR_PHY_CK_TXRX_SLICE_NUM)
        {
            reg_write(ck_base, ck_reg_val);
            ck_base += 4;
        }
    }

    // Configure MSR
    tx_gb_set_msr_reg_if(&ca_path->gearbox, msr, WDDR_SLICE_TYPE_CK);
    tx_gb_set_mode_reg_if(&ca_path->gearbox, WDDR_SLICE_TYPE_CK, &cfg->rank_cmn.ck.gearbox);
}

/**
 * @brief   TX Bit Pipeline Prep DQ MSR0
 *
 * @details Prepares the DQ TX pipeline for a frequency switch for MSR0.
 *
 * @param[in]   dq_path     pointer to dq path to prepare.
 * @param[in]   rank_index  rank index to prepare.
 * @param[in]   cal         pointer to calibration structure for dq path.
 * @param[in]   cfg         pointer to configuration structure for dq path.
 *
 * @return      void
 */
static inline void tx_bit_pipeline_prep_dq_msr0(dq_tx_path_t *dq_path,
                                                wddr_rank_t rank_index,
                                                dq_tx_path_freq_cal_t *cal,
                                                dq_tx_path_freq_cfg_t *cfg)
{
    for (uint8_t bit_index = 0; bit_index < WDDR_PHY_DQ_SLICE_NUM; bit_index++)
    {
        // MSR
        dq_path->rank[rank_index].dq.pipeline[bit_index].sdr.msr = WDDR_MSR_0;
        dq_path->rank[rank_index].dq.pipeline[bit_index].ddr.msr = WDDR_MSR_0;
        dq_path->rank[rank_index].dq.pipeline[bit_index].qdr.msr = WDDR_MSR_0;

        // SDR
        reg_write(dq_path->rank[rank_index].dq.pipeline[bit_index].sdr.base + SDR_DQ_FC_DLY__OFFSET,
                    cal->rank[rank_index].dq.pipeline.sdr.fc_delay);
        reg_write(dq_path->rank[rank_index].dq.pipeline[bit_index].sdr.base + SDR_DQ_PIPE_EN__OFFSET,
                    cal->rank[rank_index].dq.pipeline.sdr.pipe_en);
        reg_write(dq_path->rank[rank_index].dq.pipeline[bit_index].sdr.base + SDR_DQ_X_SEL__OFFSET,
                    cal->rank[rank_index].dq.pipeline.sdr.x_sel);

        // DDR
        reg_write(dq_path->rank[rank_index].dq.pipeline[bit_index].ddr.base + DDR_DQ_PIPE_EN__OFFSET,
                    cal->rank[rank_index].dq.pipeline.ddr.pipe_en);
        reg_write(dq_path->rank[rank_index].dq.pipeline[bit_index].ddr.base + DDR_DQ_X_SEL__OFFSET,
                    cal->rank[rank_index].dq.pipeline.ddr.x_sel);

        // QDR
        reg_write(dq_path->rank[rank_index].dq.pipeline[bit_index].qdr.base + QDR_DQ_PIPE_EN__OFFSET,
                    cal->rank[rank_index].dq.pipeline.qdr.pipe_en);
        reg_write(dq_path->rank[rank_index].dq.pipeline[bit_index].qdr.base + QDR_DQ_X_SEL__OFFSET,
                    cal->rank[rank_index].dq.pipeline.qdr.x_sel);

        // LPDE
        tx_lpde_set_msr_reg_if(&dq_path->rank[rank_index].dq.lpde[bit_index],
                               WDDR_MSR_0,
                               WDDR_SLICE_TYPE_DQ,
                               rank_index,
                               bit_index);
        lpde_set_reg_if(&dq_path->rank[rank_index].dq.lpde[bit_index],
                        cfg->rank[rank_index].cmn.lpde.gear,
                        cal->rank[rank_index].dq.lpde[bit_index].delay,
                        LPDE_STATE_ENABLED);
    }
}

/**
 * @brief   TX Bit Pipeline Prep DQ MSR1
 *
 * @details Prepares the DQ tx pipeline for a frequency switch for MSR1.
 *
 * @param[in]   dq_path     pointer to dq path to prepare.
 * @param[in]   rank_index  rank index to prepare.
 * @param[in]   cal         pointer to calibration structure for dq path.
 * @param[in]   cfg         pointer to configuration structure for dq path.
 *
 * @return      void
 */
static inline void tx_bit_pipeline_prep_dq_msr1(dq_tx_path_t *dq_path,
                                                wddr_rank_t rank_index,
                                                dq_tx_path_freq_cal_t *cal,
                                                dq_tx_path_freq_cfg_t *cfg)
{
    for (uint8_t bit_index = 0; bit_index < WDDR_PHY_DQ_SLICE_NUM; bit_index++)
    {
        // MSR
        dq_path->rank[rank_index].dq.pipeline[bit_index].sdr.msr = WDDR_MSR_1;
        dq_path->rank[rank_index].dq.pipeline[bit_index].ddr.msr = WDDR_MSR_1;
        dq_path->rank[rank_index].dq.pipeline[bit_index].qdr.msr = WDDR_MSR_1;

        // SDR
        reg_write(dq_path->rank[rank_index].dq.pipeline[bit_index].sdr.base +
                  SDR_DQ_FC_DLY__OFFSET + SDR_DQ_MODE__OFFSET,
                  cal->rank[rank_index].dq.pipeline.sdr.fc_delay);

        reg_write(dq_path->rank[rank_index].dq.pipeline[bit_index].sdr.base +
                  SDR_DQ_PIPE_EN__OFFSET + SDR_DQ_MODE__OFFSET,
                  cal->rank[rank_index].dq.pipeline.sdr.pipe_en);

        reg_write(dq_path->rank[rank_index].dq.pipeline[bit_index].sdr.base +
                  SDR_DQ_X_SEL__OFFSET + SDR_DQ_MODE__OFFSET,
                  cal->rank[rank_index].dq.pipeline.sdr.x_sel);

        // DDR
        reg_write(dq_path->rank[rank_index].dq.pipeline[bit_index].ddr.base +
                  DDR_DQ_PIPE_EN__OFFSET + DDR_DQ_MODE__OFFSET,
                  cal->rank[rank_index].dq.pipeline.ddr.pipe_en);

        reg_write(dq_path->rank[rank_index].dq.pipeline[bit_index].ddr.base +
                  DDR_DQ_X_SEL__OFFSET + DDR_DQ_MODE__OFFSET,
                  cal->rank[rank_index].dq.pipeline.ddr.x_sel);

        // QDR
        reg_write(dq_path->rank[rank_index].dq.pipeline[bit_index].qdr.base +
                  QDR_DQ_PIPE_EN__OFFSET + QDR_DQ_MODE__OFFSET,
                  cal->rank[rank_index].dq.pipeline.qdr.pipe_en);

        reg_write(dq_path->rank[rank_index].dq.pipeline[bit_index].qdr.base +
                  QDR_DQ_X_SEL__OFFSET + QDR_DQ_MODE__OFFSET,
                  cal->rank[rank_index].dq.pipeline.qdr.x_sel);

        // LPDE
        tx_lpde_set_msr_reg_if(&dq_path->rank[rank_index].dq.lpde[bit_index],
                               WDDR_MSR_1,
                               WDDR_SLICE_TYPE_DQ,
                               rank_index,
                               bit_index);

        lpde_set_reg_if(&dq_path->rank[rank_index].dq.lpde[bit_index],
                        cfg->rank[rank_index].cmn.lpde.gear,
                        cal->rank[rank_index].dq.lpde[bit_index].delay,
                        LPDE_STATE_ENABLED);
    }
}

/**
 * @brief   TX Bit Pipeline Prep DQS MSR0
 *
 * @details Prepares the DQS TX pipeline for a frequency switch for MSR0.
 *
 * @param[in]   dq_path     pointer to dq tx path to prepare.
 * @param[in]   rank_index  rank index to prepare.
 * @param[in]   cal         pointer to calibration structure for dq path.
 * @param[in]   cfg         pointer to configuration structure for dq path.
 *
 * @return      void
 */
static inline void tx_bit_pipeline_prep_dqs_msr0(dq_tx_path_t *dq_path,
                                                 wddr_rank_t rank_index,
                                                 dq_tx_path_freq_cal_t *cal,
                                                 dq_tx_path_freq_cfg_t *cfg)
{
    for (uint8_t bit_index = 0; bit_index < WDDR_PHY_DQS_SLICE_NUM; bit_index++)
    {
        // MSR
        dq_path->rank[rank_index].dqs.pipeline[bit_index].sdr.msr = WDDR_MSR_0;
        dq_path->rank[rank_index].dqs.pipeline[bit_index].ddr.msr = WDDR_MSR_0;
        dq_path->rank[rank_index].dqs.pipeline[bit_index].qdr.msr = WDDR_MSR_0;

        // SDR
        reg_write(dq_path->rank[rank_index].dqs.pipeline[bit_index].sdr.base + SDR_DQS_FC_DLY__OFFSET,
                    cal->rank[rank_index].dqs.pipeline[bit_index].sdr.fc_delay);
        reg_write(dq_path->rank[rank_index].dqs.pipeline[bit_index].sdr.base + SDR_DQS_PIPE_EN__OFFSET,
                    cal->rank[rank_index].dqs.pipeline[bit_index].sdr.pipe_en);
        reg_write(dq_path->rank[rank_index].dqs.pipeline[bit_index].sdr.base + SDR_DQS_X_SEL__OFFSET,
                    cal->rank[rank_index].dqs.pipeline[bit_index].sdr.x_sel);

        // DDR
        reg_write(dq_path->rank[rank_index].dqs.pipeline[bit_index].ddr.base + DDR_DQS_PIPE_EN__OFFSET,
                    cal->rank[rank_index].dqs.pipeline[bit_index].ddr.pipe_en);
        reg_write(dq_path->rank[rank_index].dqs.pipeline[bit_index].ddr.base + DDR_DQS_X_SEL__OFFSET,
                    cal->rank[rank_index].dqs.pipeline[bit_index].ddr.x_sel);

        // QDR
        reg_write(dq_path->rank[rank_index].dqs.pipeline[bit_index].qdr.base + QDR_DQS_PIPE_EN__OFFSET,
                    cal->rank[rank_index].dqs.pipeline[bit_index].qdr.pipe_en);
        reg_write(dq_path->rank[rank_index].dqs.pipeline[bit_index].qdr.base + QDR_DQS_X_SEL__OFFSET,
                    cal->rank[rank_index].dqs.pipeline[bit_index].qdr.x_sel);

        // LPDE
        if (bit_index < WDDR_PHY_DQS_TXRX_SLICE_NUM)
        {
            tx_lpde_set_msr_reg_if(&dq_path->rank[rank_index].dqs.lpde[bit_index],
                                   WDDR_MSR_0,
                                   WDDR_SLICE_TYPE_DQS,
                                   rank_index,
                                   bit_index);
            lpde_set_reg_if(&dq_path->rank[rank_index].dqs.lpde[bit_index],
                            cfg->rank[rank_index].cmn.lpde.gear,
                            cal->rank[rank_index].dqs.lpde[bit_index].delay,
                            LPDE_STATE_ENABLED);
        }
    }
}

/**
 * @brief   TX Bit Pipeline Prep DQS MSR1
 *
 * @details Prepares the DQS tx pipeline for a frequency switch for MSR1.
 *
 * @param[in]   dq_path     pointer to dq path to prepare.
 * @param[in]   rank_index  rank index to prepare.
 * @param[in]   cal         pointer to calibration structure for dq path.
 * @param[in]   cfg         pointer to configuration structure for dq path.
 *
 * @return      void
 */
static inline void tx_bit_pipeline_prep_dqs_msr1(dq_tx_path_t *dq_path,
                                                 wddr_rank_t rank_index,
                                                 dq_tx_path_freq_cal_t *cal,
                                                 dq_tx_path_freq_cfg_t *cfg)
{
    for (uint8_t bit_index = 0; bit_index < WDDR_PHY_DQS_SLICE_NUM; bit_index++)
    {
        // MSR
        dq_path->rank[rank_index].dqs.pipeline[bit_index].sdr.msr = WDDR_MSR_1;
        dq_path->rank[rank_index].dqs.pipeline[bit_index].ddr.msr = WDDR_MSR_1;
        dq_path->rank[rank_index].dqs.pipeline[bit_index].qdr.msr = WDDR_MSR_1;

        // SDR
        reg_write(dq_path->rank[rank_index].dqs.pipeline[bit_index].sdr.base +
                  SDR_DQS_FC_DLY__OFFSET + SDR_DQS_MODE__OFFSET,
                  cal->rank[rank_index].dqs.pipeline[bit_index].sdr.fc_delay);

        reg_write(dq_path->rank[rank_index].dqs.pipeline[bit_index].sdr.base +
                  SDR_DQS_PIPE_EN__OFFSET + SDR_DQS_MODE__OFFSET,
                  cal->rank[rank_index].dqs.pipeline[bit_index].sdr.pipe_en);

        reg_write(dq_path->rank[rank_index].dqs.pipeline[bit_index].sdr.base +
                  SDR_DQS_X_SEL__OFFSET + SDR_DQS_MODE__OFFSET,
                  cal->rank[rank_index].dqs.pipeline[bit_index].sdr.x_sel);

        // DDR
        reg_write(dq_path->rank[rank_index].dqs.pipeline[bit_index].ddr.base +
                  DDR_DQS_PIPE_EN__OFFSET + DDR_DQS_MODE__OFFSET,
                  cal->rank[rank_index].dqs.pipeline[bit_index].ddr.pipe_en);

        reg_write(dq_path->rank[rank_index].dqs.pipeline[bit_index].ddr.base +
                  DDR_DQS_X_SEL__OFFSET + DDR_DQS_MODE__OFFSET,
                  cal->rank[rank_index].dqs.pipeline[bit_index].ddr.x_sel);

        // QDR
        reg_write(dq_path->rank[rank_index].dqs.pipeline[bit_index].qdr.base +
                  QDR_DQS_PIPE_EN__OFFSET + QDR_DQS_MODE__OFFSET,
                  cal->rank[rank_index].dqs.pipeline[bit_index].qdr.pipe_en);

        reg_write(dq_path->rank[rank_index].dqs.pipeline[bit_index].qdr.base +
                  QDR_DQS_X_SEL__OFFSET + QDR_DQS_MODE__OFFSET,
                  cal->rank[rank_index].dqs.pipeline[bit_index].qdr.x_sel);

        // LPDE
        if (bit_index < WDDR_PHY_DQS_TXRX_SLICE_NUM)
        {
            tx_lpde_set_msr_reg_if(&dq_path->rank[rank_index].dqs.lpde[bit_index],
                                   WDDR_MSR_1,
                                   WDDR_SLICE_TYPE_DQS,
                                   rank_index,
                                   bit_index);

            lpde_set_reg_if(&dq_path->rank[rank_index].dqs.lpde[bit_index],
                            cfg->rank[rank_index].cmn.lpde.gear,
                            cal->rank[rank_index].dqs.lpde[bit_index].delay,
                            LPDE_STATE_ENABLED);
        }
    }
}

/**
 * @brief   DQ TX Path Frequency Switch Prep
 *
 * @details Prepares DQ TX Path for a frequency switch.
 *
 * @param[in]   dq_path     pointer to dq path structure to prepare.
 * @param[in]   msr         Mode Set Register value to prepare.
 * @param[in]   cal         pointer to the frequency calibration parameters
 *                          for tx path.
 * @param[in]   cfg         pointer to the frequency configuration parameters
 *                          for tx path.
 *
 * @return      void
 */
static inline void tx_path_dq_block_freq_switch_prep(dq_tx_path_t *dq_path,
                                                     wddr_msr_t msr,
                                                     dq_tx_path_freq_cal_t *cal,
                                                     dq_tx_path_freq_cfg_t *cfg)
{
    uint32_t reg_val = 0, dq_reg_val = 0, dqs_reg_val = 0;
    uint32_t dq_base;
    uint32_t dqs_base;

    for (uint8_t rank_index = 0; rank_index < WDDR_PHY_RANK; rank_index++)
    {
        if (msr == WDDR_MSR_1)
        {
            tx_bit_pipeline_prep_dq_msr1(dq_path, rank_index, cal, cfg);
            tx_bit_pipeline_prep_dqs_msr1(dq_path, rank_index, cal, cfg);
        }
        else
        {
            tx_bit_pipeline_prep_dq_msr0(dq_path, rank_index, cal, cfg);
            tx_bit_pipeline_prep_dqs_msr0(dq_path, rank_index, cal, cfg);
        }

        /************************************************
        **                  DQ
        ************************************************/

        rt_pipeline_set_msr_reg_if(&dq_path->rank[rank_index].dq.rt, msr);
        rt_pipeline_set_pipe_en_reg_if(&dq_path->rank[rank_index].dq.rt,
                                       WDDR_SLICE_TYPE_DQ,
                                       cal->rank[rank_index].dq.rt.pipe_en);

        // TX Pi Slice
        tx_pi_set_msr_reg_if(&dq_path->rank[rank_index].dq.pi,
                             msr,
                             WDDR_SLICE_TYPE_DQ,
                             rank_index);

        // ODR
        pi_set_reg_if(&dq_path->rank[rank_index].dq.pi.odr,
                      cal->rank[rank_index].dq.pi.odr.code,
                      cfg->rank[rank_index].dq.pi.odr.gear,
                      cfg->rank[rank_index].dq.pi.odr.xcpl,
                      PI_STATE_ENABLED);

        // QDR
        pi_set_reg_if(&dq_path->rank[rank_index].dq.pi.qdr,
                      cal->rank[rank_index].dq.pi.qdr.code,
                      cfg->rank[rank_index].dq.pi.qdr.gear,
                      cfg->rank[rank_index].dq.pi.qdr.xcpl,
                      PI_STATE_ENABLED);

        // DDR
        pi_set_reg_if(&dq_path->rank[rank_index].dq.pi.ddr,
                      cal->rank[rank_index].dq.pi.ddr.code,
                      cfg->rank[rank_index].dq.pi.ddr.gear,
                      cfg->rank[rank_index].dq.pi.ddr.xcpl,
                      PI_STATE_ENABLED);

        // RT
        pi_set_reg_if(&dq_path->rank[rank_index].dq.pi.rt,
                      cal->rank[rank_index].dq.pi.rt.code,
                      cfg->rank[rank_index].dq.pi.rt.gear,
                      cfg->rank[rank_index].dq.pi.rt.xcpl,
                      PI_STATE_ENABLED);


        /************************************************
        **                  DQS
        ************************************************/

        rt_pipeline_set_msr_reg_if(&dq_path->rank[rank_index].dqs.rt, msr);
        rt_pipeline_set_pipe_en_reg_if(&dq_path->rank[rank_index].dqs.rt,
                                       WDDR_SLICE_TYPE_DQS,
                                       cal->rank[rank_index].dqs.rt.pipe_en);

        // TX Pi Slice
        tx_pi_set_msr_reg_if(&dq_path->rank[rank_index].dqs.pi,
                             msr,
                             WDDR_SLICE_TYPE_DQS,
                             rank_index);

        // ODR
        pi_set_reg_if(&dq_path->rank[rank_index].dqs.pi.odr,
                      cal->rank[rank_index].dqs.pi.odr.code,
                      cfg->rank[rank_index].dqs.pi.odr.gear,
                      cfg->rank[rank_index].dqs.pi.odr.xcpl,
                      PI_STATE_ENABLED);

        // QDR
        pi_set_reg_if(&dq_path->rank[rank_index].dqs.pi.qdr,
                      cal->rank[rank_index].dqs.pi.qdr.code,
                      cfg->rank[rank_index].dqs.pi.qdr.gear,
                      cfg->rank[rank_index].dqs.pi.qdr.xcpl,
                      PI_STATE_ENABLED);

        // DDR
        pi_set_reg_if(&dq_path->rank[rank_index].dqs.pi.ddr,
                      cal->rank[rank_index].dqs.pi.ddr.code,
                      cfg->rank[rank_index].dqs.pi.ddr.gear,
                      cfg->rank[rank_index].dqs.pi.ddr.xcpl,
                      PI_STATE_ENABLED);

        // RT
        pi_set_reg_if(&dq_path->rank[rank_index].dqs.pi.rt,
                      cal->rank[rank_index].dqs.pi.rt.code,
                      cfg->rank[rank_index].dqs.pi.rt.gear,
                      cfg->rank[rank_index].dqs.pi.rt.xcpl,
                      PI_STATE_ENABLED);

        // SDR
        pi_set_reg_if(&dq_path->rank[rank_index].dqs.pi.sdr,
                      0,  // No code for this PI
                      cfg->rank[rank_index].dqs.pi.sdr.gear,
                      cfg->rank[rank_index].dqs.pi.sdr.xcpl,
                      PI_STATE_ENABLED);

        // DFI
        pi_set_reg_if(&dq_path->rank[rank_index].dqs.pi.dfi,
                      0,    // No code for this PI
                      cfg->rank[rank_index].dqs.pi.dfi.gear,
                      cfg->rank[rank_index].dqs.pi.dfi.xcpl,
                      PI_STATE_ENABLED);


        // CMN
        driver_cmn_set_msr_reg_if(&dq_path->rank[rank_index].dqs.driver, msr, WDDR_SLICE_TYPE_DQS, rank_index);

        reg_val = UPDATE_REG_FIELD(0,
                                   DDR_DQ_DQS_TX_IO_CMN_M0_R0_CFG_NCAL,
                                   cal->rank[rank_index].dqs.driver.code[DRVR_N_CAL]);

        reg_val = UPDATE_REG_FIELD(reg_val,
                                   DDR_DQ_DQS_TX_IO_CMN_M0_R0_CFG_PCAL,
                                   cal->rank[rank_index].dqs.driver.code[DRVR_P_CAL]);

        reg_val = UPDATE_REG_FIELD(reg_val,
                                   DDR_DQ_DQS_TX_IO_CMN_M0_R0_CFG_SE_MODE,
                                   cfg->rank[rank_index].dqs.driver.mode);

        reg_write(dq_path->rank[rank_index].dqs.driver.base, reg_val);
    }

    // Bit TX Driver
    driver_set_msr_reg_if(&dq_path->dq.driver, msr, WDDR_SLICE_TYPE_DQ);
    driver_set_msr_reg_if(&dq_path->dqs.driver,msr, WDDR_SLICE_TYPE_DQS);

    dq_base = dq_path->dq.driver.base;
    dqs_base = dq_path->dqs.driver.base;

    // DQ
    dq_reg_val = UPDATE_REG_FIELD(dq_reg_val, DDR_DQ_DQ_TX_IO_M0_CFG_0_OVRD_SEL, cfg->rank_cmn.dq.driver.override.sel);
    dq_reg_val = UPDATE_REG_FIELD(dq_reg_val, DDR_DQ_DQ_TX_IO_M0_CFG_0_OVRD_VAL, cfg->rank_cmn.dq.driver.override.val_t);
    dq_reg_val = UPDATE_REG_FIELD(dq_reg_val, DDR_DQ_DQ_TX_IO_M0_CFG_0_TX_IMPD, cfg->rank_cmn.dq.driver.tx_impd);
    dq_reg_val = UPDATE_REG_FIELD(dq_reg_val, DDR_DQ_DQ_TX_IO_M0_CFG_0_RX_IMPD, cfg->rank_cmn.dq.driver.rx_impd);
    dq_reg_val = UPDATE_REG_FIELD(dq_reg_val, DDR_DQ_DQ_TX_IO_M0_CFG_0_SW_OVR, 0);

    // DQS
    dqs_reg_val = UPDATE_REG_FIELD(dqs_reg_val, DDR_DQ_DQS_TX_IO_M0_CFG_0_OVRD_SEL, cfg->rank_cmn.dqs.driver.override.sel);
    dqs_reg_val = UPDATE_REG_FIELD(dqs_reg_val, DDR_DQ_DQS_TX_IO_M0_CFG_0_OVRD_VAL_T, cfg->rank_cmn.dqs.driver.override.val_t);
    dqs_reg_val = UPDATE_REG_FIELD(dqs_reg_val, DDR_DQ_DQS_TX_IO_M0_CFG_0_OVRD_VAL_C, cfg->rank_cmn.dqs.driver.override.val_c);
    dqs_reg_val = UPDATE_REG_FIELD(dqs_reg_val, DDR_DQ_DQS_TX_IO_M0_CFG_0_TX_IMPD, cfg->rank_cmn.dqs.driver.tx_impd);
    dqs_reg_val = UPDATE_REG_FIELD(dqs_reg_val, DDR_DQ_DQS_TX_IO_M0_CFG_0_RX_IMPD, cfg->rank_cmn.dqs.driver.rx_impd);
    dqs_reg_val = UPDATE_REG_FIELD(dqs_reg_val, DDR_DQ_DQS_TX_IO_M0_CFG_0_SW_OVR, 0);

    // DQ
    for (uint8_t bit_index = 0; bit_index < WDDR_PHY_DQ_SLICE_NUM; bit_index++)
    {

        egress_dig_set_msr_reg_if(&dq_path->dq.pipeline[bit_index].egress_dig, msr);
        egress_ana_set_msr_reg_if(&dq_path->dq.pipeline[bit_index].egress_ana, msr);
        if (msr == WDDR_MSR_1)
        {
            reg_write(dq_path->dq.pipeline[bit_index].egress_dig.base +
                      EGRESS_DIG_DQ_CFG__OFFSET + EGRESS_DIG_DQ_MODE__OFFSET,
                      cfg->rank_cmn.cmn.pipeline.egress_dig_mode);

            reg_write(dq_path->dq.pipeline[bit_index].egress_ana.base +
                      EGRESS_ANA_DQ_CFG__OFFSET + EGRESS_ANA_DQ_MODE__OFFSET,
                      cfg->rank_cmn.cmn.pipeline.egress_ana_mode);

        }
        else
        {
            reg_write(dq_path->dq.pipeline[bit_index].egress_dig.base +
                      EGRESS_DIG_DQ_CFG__OFFSET,
                      cfg->rank_cmn.cmn.pipeline.egress_dig_mode);

            reg_write(dq_path->dq.pipeline[bit_index].egress_ana.base +
                      EGRESS_ANA_DQ_CFG__OFFSET,
                      cfg->rank_cmn.cmn.pipeline.egress_ana_mode);
        };
        reg_write(dq_base, dq_reg_val);
        dq_base += 4;
    }

    // DQS
    for (uint8_t bit_index = 0; bit_index < WDDR_PHY_DQS_SLICE_NUM; bit_index++)
    {

        egress_dig_set_msr_reg_if(&dq_path->dqs.pipeline[bit_index].egress_dig, msr);
        egress_ana_set_msr_reg_if(&dq_path->dqs.pipeline[bit_index].egress_ana, msr);
        if (msr == WDDR_MSR_1)
        {
            reg_write(dq_path->dqs.pipeline[bit_index].egress_dig.base +
                      EGRESS_DIG_DQS_CFG__OFFSET + EGRESS_DIG_DQS_MODE__OFFSET,
                      cfg->rank_cmn.cmn.pipeline.egress_dig_mode);

            reg_write(dq_path->dqs.pipeline[bit_index].egress_ana.base +
                      EGRESS_ANA_DQS_CFG__OFFSET + EGRESS_ANA_DQS_MODE__OFFSET,
                      cfg->rank_cmn.cmn.pipeline.egress_ana_mode);

        }
        else
        {
            reg_write(dq_path->dqs.pipeline[bit_index].egress_dig.base +
                      EGRESS_DIG_DQS_CFG__OFFSET,
                      cfg->rank_cmn.cmn.pipeline.egress_dig_mode);

            reg_write(dq_path->dqs.pipeline[bit_index].egress_ana.base +
                      EGRESS_ANA_DQS_CFG__OFFSET,
                      cfg->rank_cmn.cmn.pipeline.egress_ana_mode);
        }

        if (bit_index < WDDR_PHY_DQS_TXRX_SLICE_NUM)
        {
            reg_write(dqs_base, dqs_reg_val);
            dqs_base += 4;
        }
    }

    tx_gb_set_msr_reg_if(&dq_path->gearbox, msr, WDDR_SLICE_TYPE_DQS);
    tx_gb_set_mode_reg_if(&dq_path->gearbox, WDDR_SLICE_TYPE_DQS, &cfg->rank_cmn.dqs.gearbox);
}

/**
 * @brief   CA Path Frequency Switch Prep
 *
 * @details Prepares entire CA path for a frequency switch.
 *
 * @param[in]   ca_path     pointer to ca path structure to prepare.
 * @param[in]   msr         Mode Set Register value to prepare.
 * @param[in]   cal         pointer to the frequency calibration parameters
 *                          for the ca block.
 * @param[in]   cfg         pointer to the frequency configuration parameters
 *                          for the ca block.
 *
 * @return      void
 */
static inline void ca_block_freq_switch_prep(ca_path_t *ca_path,
                                             wddr_msr_t msr,
                                             ca_path_freq_cal_t *cal,
                                             ca_path_freq_cfg_t *cfg)
{
    // Prep Tx
    tx_path_ca_block_freq_switch_prep(&ca_path->tx, msr, &cal->tx, &cfg->tx);

    // Prep Rx
    rx_path_ca_block_freq_switch_prep(&ca_path->rx, msr, &cal->rx, &cfg->rx);
}

/**
 * @brief   DQ Path Frequency Switch Prep
 *
 * @details Prepares entire DQ path for a frequency switch.
 *
 * @param[in]   dq_path     pointer to ca path structure to prepare.
 * @param[in]   msr         Mode Set Register value to prepare.
 * @param[in]   cal         pointer to the frequency calibration parameters
 *                          for the dq block.
 * @param[in]   cfg         pointer to the frequency configuration parameters
 *                          for the dq block.
 * @param[in]   common_cal  pointer to the frequency independent calibartion
 *                          parameters for the dq block.
 *
 * @return      void
 */
static inline void dq_block_freq_switch_prep(dq_path_t *dq_path,
                                             wddr_msr_t msr,
                                             dq_path_freq_cal_t *cal,
                                             dq_path_freq_cfg_t *cfg,
                                             dq_path_common_cal_t *common_cal)
{
    // Prep Tx
    tx_path_dq_block_freq_switch_prep(&dq_path->tx, msr, &cal->tx, &cfg->tx);

    // Prep Rx
    rx_path_dq_block_freq_switch_prep(&dq_path->rx, msr, &cal->rx, &cfg->rx, &common_cal->rx);
}

/**
 * @brief   Common Path Frequency Switch Prep
 *
 * @details Prepares entire Common path for a frequency switch.
 *
 * @param[in]   cmn_path    pointer to common path structure to prepare.
 * @param[in]   msr         Mode Set Register value to prepare.
 * @param[in]   cal         pointer to the frequency calibration parameters
 *                          for the common block.
 * @param[in]   cfg         pointer to the frequency configuration parameters
 *                          for the common block.
 *
 * @return      void
 */
static inline void common_block_freq_switch_prep(common_path_t *cmn_path,
                                                 wddr_msr_t msr,
                                                 common_path_freq_cal_t *cal,
                                                 common_path_freq_cfg_t *cfg)
{
    // VREF
    vref_set_msr_reg_if(&cmn_path->vref, msr);
    vref_set_code(&cmn_path->vref, cal->vref.code);
    vref_set_pwr_mode(&cmn_path->vref, cfg->vref.power_mode);
}

/**
 * @brief  DFI Frequency Switch Prep
 *
 * @details Preps DFI for a frequency switch
 *
 * @param[in]   dfi     pointer to DFI device.
 * @param[in]   msr     Mode Set Register value to prepare.
 * @param[in]   cfg     pointer to frequency configuration parameters for DFI.
 *
 * @return      void
 */
static inline void dfi_freq_switch_prep(dfi_dev_t *dfi, wddr_msr_t msr, dfi_freq_cfg_t *cfg)
{
    dfi_set_msr_reg_if(dfi, msr);
    dfi_wrd_cfg_reg_if(dfi, &cfg->wrd);
    dfi_wrcctrl_cfg_reg_if(dfi, &cfg->wrcctrl);
    dfi_ckctrl_cfg_reg_if(dfi, &cfg->ckctrl);
    dfi_wctrl_cfg_reg_if(dfi, &cfg->wctrl);
    dfi_rctrl_cfg_reg_if(dfi, &cfg->rctrl);
    dfi_rdd_cfg_reg_if(dfi, &cfg->rdd);
    dfi_wckctrl_cfg_reg_if(dfi, &cfg->wckctrl);
    dfi_wrc_cfg_reg_if(dfi, &cfg->wrc);
    dfi_wenctrl_cfg_reg_if(dfi, &cfg->wenctrl);

    dfi_paden_pext_cfg_reg_if(dfi, &cfg->paden_pext);
    dfi_clken_pext_cfg_reg_if(dfi, &cfg->clken_pext);

    dfi_ovr_traffic_cfg_reg_if(dfi, &cfg->ovr_traffic);
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
    // CH Prep
    for (uint8_t channel = 0; channel < WDDR_PHY_CHANNEL_NUM; channel++)
    {
        for(uint8_t byte = 0; byte < WDDR_PHY_DQ_BYTE_NUM; byte++)
        {
            // DQ Block Prep
            dq_block_freq_switch_prep(&wddr->channel[channel].dq[byte],
                                      msr,
                                      &wddr->table->cal.freq[freq_id].channel[channel].dq[byte],
                                      &wddr->table->cfg.freq[freq_id].channel[channel].dq[byte],
                                      &wddr->table->cal.common.channel[channel].dq[byte]);
        }

        // CA Block Prep
        ca_block_freq_switch_prep(&wddr->channel[channel].ca,
                                  msr,
                                  &wddr->table->cal.freq[freq_id].channel[channel].ca,
                                  &wddr->table->cfg.freq[freq_id].channel[channel].ca);
    }

    // Common Block Prep
    common_block_freq_switch_prep(&wddr->cmn,
                                  msr,
                                  &wddr->table->cal.freq[freq_id].common,
                                  &wddr->table->cfg.freq[freq_id].common);

    // DFI Prep
    dfi_freq_switch_prep(&wddr->dfi, msr, &wddr->table->cfg.freq[freq_id].dfi);
}
