#include <string.h>
#include <kernel/io.h>
#include <wddr/memory_map.h>
#include <dfi/buffer_driver.h>
#include <dfi/driver.h>
#include <driver/cmn_driver.h>
#include <pi/driver.h>
#include <wddr/notification_map.h>
#include <wddr/train.h>

#define CA_TEST_DATA                (0xA5)
#define CA_WIDTH                    (0x6)
#define CA_MASK                     ((1 << CA_WIDTH) - 1)

/**
 * @brief   Frequency Switch Overlay Enumerations
 *
 * @details Support overlays that can be turned on after frequency prep
 *          before final switch.
 *
 * @note    overlays should be powers of 2 (up to 128).
 *
 * @NONE    No overlays.
 * @CBT     Enabled CBT Overlay (HI-Z on DQ/DQS BYTE 1).
 * @BSCAN   Enabled BSCAN Overlay.
 */
enum freq_switch_overlay
{
    FREQ_SWITCH_OVERLAY_NONE = 0,
    FREQ_SWITCH_OVERLAY_CBT = 1,
    FREQ_SWITCH_OVERLAY_BSCAN = 2,
};

static void read_bscan_result(uint8_t *result, wddr_dq_byte_t dq_byte, wddr_channel_t channel)
{
    // Read BSCAN
    *result = reg_read(WDDR_MEMORY_MAP_PHY_CH_START +
                       WDDR_MEMORY_MAP_PHY_CH_OFFSET * channel +
                       WDDR_MEMORY_MAP_PHY_DQ_OFFSET * dq_byte +
                       DDR_DQ_DQ_RX_BSCAN_STA__ADR);
}

static wddr_return_t switch_frequency(wddr_dev_t *wddr, uint8_t freq_id, uint8_t overlay_mask)
{
    Notification_t notification;
    wddr_return_t ret = WDDR_SUCCESS;

    // Power Down DRAM before switch
    dram_power_down(&wddr->dram, &wddr->dfi.dfi_buffer);

    wddr_prep_switch(wddr, freq_id);

    // Enable any overlays post-prep but before switching
    if (overlay_mask & FREQ_SWITCH_OVERLAY_CBT)
    {
        // Put DQS and DQ into Hi-Z
        // NOTE: This is required per-CBT procedure (This is not a table configuration)
        // TODO: Channel support
        driver_set_impedance_all_bits(&wddr->channel[WDDR_CHANNEL_0].dq[WDDR_DQ_BYTE_1].tx.dqs.driver,
                                      WDDR_SLICE_TYPE_DQS,
                                      DRIVER_IMPEDANCE_HIZ,
                                      DRIVER_IMPEDANCE_HIZ);
        driver_set_impedance_all_bits(&wddr->channel[WDDR_CHANNEL_0].dq[WDDR_DQ_BYTE_1].tx.dq.driver,
                                      WDDR_SLICE_TYPE_DQ,
                                      DRIVER_IMPEDANCE_HIZ,
                                      DRIVER_IMPEDANCE_HIZ);
        driver_set_impedance_all_bits(&wddr->channel[WDDR_CHANNEL_1].dq[WDDR_DQ_BYTE_1].tx.dqs.driver,
                                      WDDR_SLICE_TYPE_DQS,
                                      DRIVER_IMPEDANCE_HIZ,
                                      DRIVER_IMPEDANCE_HIZ);
        driver_set_impedance_all_bits(&wddr->channel[WDDR_CHANNEL_1].dq[WDDR_DQ_BYTE_1].tx.dq.driver,
                                      WDDR_SLICE_TYPE_DQ,
                                      DRIVER_IMPEDANCE_HIZ,
                                      DRIVER_IMPEDANCE_HIZ);
    }

    if (overlay_mask & FREQ_SWITCH_OVERLAY_BSCAN)
    {
        /*
        * ===========================================
        *              BSCAN Configuration
        * ===========================================
        */
        // Differential Mode for Driver (DQS_t / DQS_c)
        // NOTE: This is required for BSCAN. Not necessary for LP4 but will be for LP5
        driver_cmn_set_mode_reg_if(&wddr->channel[WDDR_CHANNEL_0].dq[WDDR_DQ_BYTE_0].tx.rank[WDDR_RANK_0].dqs.driver,
                                   DRIVER_MODE_DIFF);
        driver_cmn_set_mode_reg_if(&wddr->channel[WDDR_CHANNEL_0].dq[WDDR_DQ_BYTE_1].tx.rank[WDDR_RANK_0].dqs.driver,
                                   DRIVER_MODE_DIFF);

        // TODO: Does DQ0 need to be in loopback too?
        // Put DQ1 into loopback (Enables BSCAN path at receiver)
        driver_cmn_set_loopback_reg_if(&wddr->channel[WDDR_CHANNEL_0].dq[WDDR_DQ_BYTE_1].tx.rank[WDDR_RANK_0].dqs.driver,
                                       DRIVER_LOOPBACK_MODE_ENABLE);
    }

    // Stop sending clock to DRAM via DFI
    dfi_buffer_set_wdata_hold_reg_if(&wddr->dfi.dfi_buffer, 0x0);

    // Set blocked task
    wddr->xHandle = xTaskGetCurrentTaskHandle();

    // Switch to new frequency
    xTaskNotifyWait( 0, 0, NULL, 0 );
    PROPAGATE_ERROR(freq_switch_event_sw_switch(&wddr->fsm.fsw));

    // Wait for switch to complete
    xTaskNotifyWait(0, 0, &notification, portMAX_DELAY);

    // Ensure notification is for FSW_DONE
    if (notification != WDDR_NOTIF_FSW_DONE)
    {
        ret = WDDR_ERROR;
    }
    wddr->xHandle = NULL;

    wddr->dram.cfg = &wddr->table->cfg.freq[freq_id].dram;
    wddr->dram.cal = &wddr->table->cal.freq[freq_id].dram;

    // Re-enable DFI hold feature
    dfi_buffer_set_wdata_hold_reg_if(&wddr->dfi.dfi_buffer, 0x1);

    // Using DFI hold feature, start sending clocks again
    // If in CBT, ensure CKE is low
    if (overlay_mask & FREQ_SWITCH_OVERLAY_CBT)
    {
        dram_power_down(&wddr->dram, &wddr->dfi.dfi_buffer);
    }
    else
    {
        dram_idle(&wddr->dram, &wddr->dfi.dfi_buffer);
    }

    return ret;
}

/**
 * Command Bus Training Procedure:
 *
 * Command Bus Training is used to align CK relative to CA
 * signal at the DRAM. The goal of this training is to align
 * CK such that it rises in the middle of the Command Address
 * signal. See Diagram below. The training can either push CK
 * or CA PI to achieve ideal alginment. For this training implementation,
 * CA PI will be used to push the signal to achieve the correct alignment.
 * CK could be used but isn't as PI introduces jitter on the CK signal.
 * This results in an additional cycle added to DFI offsets of Read and write
 * signals as they are algined relative to CA.
 *
 *         Post Training Alignment
 *
 *               +-------+       +-------+
 *               |       |       |       |
 * CK    +-------+       +-------+       +-------+
 *          ------------    ------------    ----
 *      \ /              \ /             \ /
 *       /      CA0       /      CA1      /
 * CA   / \              / \             / \
 *          ------------     ------------    ----
 */
static void command_bus_sweep(wddr_dev_t *wddr, dram_freq_cfg_t *dram_cfg, pi_dev_t *ca_pi, wddr_channel_t channel, bit_array_t *result)
{
    bit_array_t tmp;
    uint8_t command_address = CA_TEST_DATA;
    uint8_t val;
    uint8_t result_index = 0;
    dfi_tx_packet_buffer_t buffer;
    driver_cfg_t drv_cfg;

    memcpy(&drv_cfg,
           &wddr->table->cfg.freq[WDDR_PHY_BOOT_FREQ].channel[WDDR_CHANNEL_0].dq[WDDR_DQ_BYTE_0].tx.rank_cmn.dq.driver,
           sizeof(driver_cfg_t));

    for (uint8_t vref_setting = CBT_VREF_RANGE_START; vref_setting < CBT_VREF_RANGE_STOP; vref_setting += CBT_VREF_RANGE_STEP, command_address = ~command_address, result_index++)
    {
        // Mask Command Address
        command_address &= CA_MASK;

        // Create CBT pattern
        dfi_tx_packet_buffer_init(&buffer);
        create_ck_packet_sequence(&buffer, 1);
        create_cbt_packet_sequence(&buffer, dram_cfg, CS_0, vref_setting, command_address, 1);

        // Override DQ6:0 with desired vref setting
        for (uint8_t bit_index = 0; bit_index < 8; bit_index++)
        {
            drv_cfg.override.val_t = (vref_setting >> bit_index) & 0x1;
            driver_override(&wddr->channel[WDDR_CHANNEL_0].dq[WDDR_DQ_BYTE_0].tx.dq.driver,
                            &drv_cfg,
                            WDDR_SLICE_TYPE_DQ,
                            bit_index,
                            true);
        }

        for (uint8_t pi_step = CBT_PI_RANGE_START, shift = 0; pi_step < CBT_PI_RANGE_STOP; pi_step += CBT_PI_RANGE_STEP, shift++)
        {
            pi_set_code_reg_if(ca_pi, pi_step);

            //dfi_buffer_fill_and_send_packets(&wddr->dfi.dfi_buffer, &buffer.list);
            dfi_buffer_fill_packets(&wddr->dfi.dfi_buffer, &buffer.list);
            dfi_buffer_send_packets(&wddr->dfi.dfi_buffer, false);

            // Process results
            read_bscan_result(&val, WDDR_DQ_BYTE_1, channel);
            val &= CA_MASK;
            tmp = val == command_address;
            tmp <<= shift;
            result[result_index] |= tmp;
        }

        // Remove override of DQ bits
        for (uint8_t bit_index = 0; bit_index < 8; bit_index++)
        {
            driver_override(&wddr->channel[WDDR_CHANNEL_0].dq[WDDR_DQ_BYTE_0].tx.dq.driver,
                            &wddr->table->cfg.freq[WDDR_PHY_BOOT_FREQ].channel[WDDR_CHANNEL_0].dq[WDDR_DQ_BYTE_0].tx.rank_cmn.dq.driver,
                            WDDR_SLICE_TYPE_DQ,
                            bit_index,
                            false);
        }
        // Clean up packet buffer
        dfi_tx_packet_buffer_free(&buffer);
    }
}

void command_bus_training(wddr_dev_t *wddr, wddr_channel_t channel, uint8_t freq_id, bit_array_t *result)
{
    rect_t rect;
    uint16_t ca_pi_code;
    uint8_t curr_freq_id;

    /*
     * NOTES:   It is assumed phy is not operating at the frequency we want to train.
     *          We must be in a stable frequency / MR setup for sending commands.
     *          FSP 0 should still be default values at this point.
     *          WDDR_PHY_BOOT_FREQ is low frequency for stable command communication.
     *          FSP 1 will be used to train all frequencies.
     */

    // Get Current Operating Frequency
    pll_fsm_get_current_freq(&wddr->fsm.pll, &curr_freq_id);

    // Send MRW to put us into FSP 0
    dram_set_fsp_op(&wddr->dram, &wddr->dfi.dfi_buffer, 0x0);

    if (curr_freq_id != WDDR_PHY_BOOT_FREQ)
    {
        // Switch phy frequency to low frequency
        switch_frequency(wddr, WDDR_PHY_BOOT_FREQ, FREQ_SWITCH_OVERLAY_NONE);
    }

    // Change FSP-WR to other slot
    dram_set_fsp_wr(&wddr->dram, &wddr->dfi.dfi_buffer, 0x1);

    // Initalize the MRW for the next frequency
    dram_frequency_init(&wddr->dram, &wddr->dfi.dfi_buffer,
                        &wddr->table->cfg.freq[freq_id].dram,
                        &wddr->table->cal.freq[freq_id].dram);

    // Put DQS and DQ into Hi-Z
    // NOTE: This is required per-CBT procedure (This is not a table configuration)
    driver_set_impedance_all_bits(&wddr->channel[WDDR_CHANNEL_0].dq[WDDR_DQ_BYTE_1].tx.dqs.driver,
                                  WDDR_SLICE_TYPE_DQS,
                                  DRIVER_IMPEDANCE_HIZ,
                                  DRIVER_IMPEDANCE_HIZ);
    driver_set_impedance_all_bits(&wddr->channel[WDDR_CHANNEL_0].dq[WDDR_DQ_BYTE_1].tx.dq.driver,
                                  WDDR_SLICE_TYPE_DQ,
                                  DRIVER_IMPEDANCE_HIZ,
                                  DRIVER_IMPEDANCE_HIZ);
    driver_set_impedance_all_bits(&wddr->channel[WDDR_CHANNEL_1].dq[WDDR_DQ_BYTE_1].tx.dqs.driver,
                                  WDDR_SLICE_TYPE_DQS,
                                  DRIVER_IMPEDANCE_HIZ,
                                  DRIVER_IMPEDANCE_HIZ);
    driver_set_impedance_all_bits(&wddr->channel[WDDR_CHANNEL_1].dq[WDDR_DQ_BYTE_1].tx.dq.driver,
                                  WDDR_SLICE_TYPE_DQ,
                                  DRIVER_IMPEDANCE_HIZ,
                                  DRIVER_IMPEDANCE_HIZ);

    // Enter Command Bus Training Mode
    dram_cbt_enter(&wddr->dram, &wddr->dfi.dfi_buffer);

    /*
     * ===========================================
     *        Switch to Actual Frequency
     * ===========================================
     */
    switch_frequency(wddr, freq_id, FREQ_SWITCH_OVERLAY_CBT | FREQ_SWITCH_OVERLAY_BSCAN);

    // Turn off OE pulse extension
    // This is for capturing DQ for CA VREF
    dfi_paden_pext_cfg_t pext_cfg;
    memcpy(&pext_cfg, &wddr->table->cfg.freq[freq_id].dfi.paden_pext, sizeof(dfi_paden_pext_cfg_t));
    pext_cfg.wrd.fields.en = 0;
    dfi_paden_pext_cfg_reg_if(&wddr->dfi, &pext_cfg);

    // Overwrite CA TRAFFIC to keep clocks running
    dfi_ovr_traffic_cfg_t ovr_cfg;
    memcpy(&ovr_cfg, &wddr->table->cfg.freq[freq_id].dfi.ovr_traffic, sizeof(dfi_ovr_traffic_cfg_t));
    ovr_cfg.sel_ca = 1;
    ovr_cfg.val_ca = 1;
    ovr_cfg.sel_dq_wr = 1;
    ovr_cfg.val_dq_wr = 1;
    ovr_cfg.sel_dqs = 1;
    ovr_cfg.val_dqs = 1;
    dfi_ovr_traffic_cfg_reg_if(&wddr->dfi, &ovr_cfg);

    /*
     * ===========================================
     *              CBT Sweep
     * ===========================================
     */
    // Train (VREF / PI)
    pi_dev_t *ca_pi = wddr->dram.cfg->ratio == WDDR_FREQ_RATIO_1TO1 ?
        &wddr->channel[channel].ca.tx.rank[WDDR_RANK_0].ca.pi.ddr :
        &wddr->channel[channel].ca.tx.rank[WDDR_RANK_0].ca.pi.qdr;
    command_bus_sweep(wddr, &wddr->table->cfg.freq[freq_id].dram, ca_pi, channel, result);

    /*
     * ===========================================
     *             Training Cleanup
     * ===========================================
     */
    // Switch to Low Frequency; Keep DQS / DQ in HI-Z
    switch_frequency(wddr, WDDR_PHY_BOOT_FREQ, FREQ_SWITCH_OVERLAY_CBT);

    // Exit CBT Command
    dram_cbt_exit(&wddr->dram, &wddr->dfi.dfi_buffer);

    /*
     * ===========================================
     *          Training Eye Processing
     * ===========================================
     */
    max_rect(result,
             (CBT_PI_RANGE_STOP - CBT_PI_RANGE_START) / CBT_PI_RANGE_STEP,
             (CBT_VREF_RANGE_STOP - CBT_VREF_RANGE_START) / CBT_VREF_RANGE_STEP,
             &rect);

    range_t x_range = {0};
    range_t y_range = {0};
    x_range.start = CBT_PI_RANGE_START;
    x_range.stop = CBT_PI_RANGE_STOP;
    x_range.step_size = CBT_PI_RANGE_STEP;
    y_range.start = CBT_VREF_RANGE_START;
    y_range.stop = CBT_VREF_RANGE_STOP;
    y_range.step_size = CBT_VREF_RANGE_STEP;

    // Process 2-D Eye result
    ca_pi_code = find_midpoint_and_convert(rect.origin.x, rect.size.w, &x_range);

    /** TODO: Extract optimal CA VREF point for DRAM apply and store */
    uint8_t ca_vref = find_midpoint_and_convert(rect.origin.y, rect.size.h, &y_range);

    // Store in table
    if (ca_pi == &wddr->channel[channel].ca.tx.rank[WDDR_RANK_0].ca.pi.ddr)
    {
        wddr->table->cal.freq[freq_id].channel[channel].ca.tx.rank[WDDR_RANK_0].ca.pi.ddr.code = ca_pi_code;
    }
    else
    {
        wddr->table->cal.freq[freq_id].channel[channel].ca.tx.rank[WDDR_RANK_0].ca.pi.qdr.code = ca_pi_code;
    }

    wddr->table->cal.freq[freq_id].dram.mr12 = ca_vref;

    /**
     * NOTE:
     * One cycle offset is added because of relationship of CK to CA. Add one
     * cycle offset because of relationship with CK to CA. In this training,
     * CA was pushed relative to CK. This means that CA0 will not be sampled a
     * t0 (DFI launch time) but rather at t1. See Diagram below.
     *
     * This means that one cycle will need to be added to Read and Write paths.
     * Previously, the one cycle delay was added directly into the DFI command
     * configuration. However, it was later decided to not add one cycle, and
     * let the training for Read and Write find that an extra cycle is required.
     *
     *          Post Training Alignment @ DRAM
     *
     *          +-------+       +-------+       +-------+
     *          |       |       |       |       |       |
     * CK ------+  t0   +-------+  t1   +-------+  t2   +-------+
     *        ---------     ----------     ----------
     *                  \ /            \ /            \ /
     *           DES     /      CA0     /      CA1     /
     * CA               / \            / \            / \
     *        ---------     ----------     ----------
     *                          t0
     */

    // Write MR settings based on calibrated values
    dram_frequency_init(&wddr->dram, &wddr->dfi.dfi_buffer,
                        &wddr->table->cfg.freq[freq_id].dram,
                        &wddr->table->cal.freq[freq_id].dram);

    // Set FSP-OP to new frequency
    dram_set_fsp_op(&wddr->dram, &wddr->dfi.dfi_buffer, 0x1);

    // Switch back to actual frequency
    switch_frequency(wddr, freq_id, FREQ_SWITCH_OVERLAY_NONE);
}
