#include <string.h>
#include <kernel/io.h>
#include <wddr/memory_map.h>
#include <wddr/train.h>
#include <dfi/buffer_driver.h>
#include <driver/cmn_driver.h>
#include <pi/driver.h>
#include <pipeline/sdr_driver.h>

#define WRITE_LEVEL_BIT_SHIFT   (7)
#define WRITE_LEVEL_BIT_MASK    (1 << WRITE_LEVEL_BIT_SHIFT)
#define WRITE_LATENCY           (4)
#define BL                      (16)
#define TWTR                    (8)

static uint8_t dq_dqs_src[32] = {0x00, 0x01, 0x02, 0x03,
                                 0x04, 0x05, 0x06, 0x07,
                                 0x08, 0x09, 0x0A, 0x0B,
                                 0x0C, 0x0D, 0x0E, 0x0F,
                                 0x10, 0x11, 0x12, 0x13,
                                 0x14, 0x15, 0x16, 0x17,
                                 0x18, 0x19, 0x1A, 0x1B,
                                 0x1C, 0x1D, 0x1E, 0x1F};

static void read_bscan_result(uint8_t *result, wddr_dq_byte_t dq_byte, wddr_channel_t channel)
{
    // Read BSCAN
    *result = reg_read(WDDR_MEMORY_MAP_PHY_CH_START +
                       WDDR_MEMORY_MAP_PHY_CH_OFFSET * channel +
                       WDDR_MEMORY_MAP_PHY_DQ_OFFSET * dq_byte +
                       DDR_DQ_DQ_RX_BSCAN_STA__ADR);
}

static void reset_tx_driver(wddr_dev_t *wddr, uint8_t freq_id)
{
    uint8_t channel, dq_block_index;

    for (channel = WDDR_CHANNEL_0; channel < WDDR_PHY_CHANNEL_NUM; channel++)
    {
        for (dq_block_index = WDDR_DQ_BYTE_0; dq_block_index < WDDR_PHY_DQ_BYTE_NUM; dq_block_index++)
        {
            // Reset driver Mode
            driver_cmn_set_mode_reg_if(&wddr->channel[channel].dq[dq_block_index].tx.rank[WDDR_RANK_0].dqs.driver,
                               wddr->table->cfg.freq[freq_id].channel[channel].dq[dq_block_index].tx.rank[WDDR_RANK_0].dqs.driver.mode);

            // Reset driver impedance for DQ and DQS
            driver_set_impedance_all_bits(&wddr->channel[channel].dq[dq_block_index].tx.dq.driver,
                                            WDDR_SLICE_TYPE_DQ,
                                            wddr->table->cfg.freq[freq_id].channel[channel].dq[dq_block_index].tx.rank_cmn.dq.driver.tx_impd,
                                            wddr->table->cfg.freq[freq_id].channel[channel].dq[dq_block_index].tx.rank_cmn.dq.driver.rx_impd);
            driver_set_impedance_all_bits(&wddr->channel[channel].dq[dq_block_index].tx.dqs.driver,
                                            WDDR_SLICE_TYPE_DQS,
                                            wddr->table->cfg.freq[freq_id].channel[channel].dq[dq_block_index].tx.rank_cmn.dqs.driver.tx_impd,
                                            wddr->table->cfg.freq[freq_id].channel[channel].dq[dq_block_index].tx.rank_cmn.dqs.driver.rx_impd);

            // Take Driver out of loopback mode
            driver_cmn_set_loopback_reg_if(&wddr->channel[channel].dq[dq_block_index].tx.rank[WDDR_RANK_0].dqs.driver, DRIVER_LOOPBACK_MODE_DISABLE);
        }
    }
}

/**
 * Write Leveling Procedure:
 *
 * Write Leveling used to align CK to DQS relationship.
 * DRAM will indicate alignment by folloiwng values over DQ
 * bits which are sampled with BSCAN feature of the PHY.
 *
 *  Case 0 --> CK sampling DQS as 1
 *  Case 1 --> CK sampling DQS as 0
 *
 *
 * Training is performed for CASE #1. See diagrams below.
 *
 *              CASE #0
 *
 *      ---+       +-------+
 *         |       |       |
 * CK      +-------+       +-------+
 *
 *     --+       +-------+       +-------+
 *       |       |       |       |
 * DQS   +-------+       +-------+
 *
 *               CASE #1
 *
 *      ---+       +-------+
 *         |       |       |
 * CK      +-------+       +-------+
 *
 *        --+       +-------+       +-------+
 *          |       |       |       |
 * DQS      +-------+       +-------+
 */
static void write_level_sweep(wddr_dev_t *wddr, wddr_channel_t channel, wddr_dq_byte_t dq_byte, bit_array_t *result)
{
    // Sweep DQS PI
    uint8_t bscan_res = 0;
    bit_array_t tmp;
    dfi_tx_packet_buffer_t buffer;

    // Send Write Leveling (Toggle DQS)
    dfi_tx_packet_buffer_init(&buffer);
    create_wrlvl_packet_sequence(&buffer, wddr->dram.cfg->ratio, CS_0, 1);
    create_cke_packet_sequence(&buffer, 1);

    /*
     * ===========================================
     *         Write Level Sweep
     * ===========================================
     */

    for (uint8_t pi_step = WRLVL_PI_RANGE_START, shift = 0; pi_step < WRLVL_PI_RANGE_STOP; pi_step += WRLVL_PI_RANGE_STEP, shift++)
    {
        // Step DQS PI
        // TODO: Once make this per byte, then pass in pi
        switch(wddr->dram.cfg->ratio)
        {
            case WDDR_FREQ_RATIO_1TO1:
                pi_set_code_reg_if(&wddr->channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pi.ddr, pi_step);
                break;
            case WDDR_FREQ_RATIO_1TO2:
                pi_set_code_reg_if(&wddr->channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pi.qdr, pi_step);
                break;
            case WDDR_FREQ_RATIO_1TO4:
                return;
        }

        // Send Write Level Pattern
        dfi_buffer_fill_packets(&wddr->dfi.dfi_buffer, &buffer.list);
        dfi_buffer_send_packets(&wddr->dfi.dfi_buffer, false);

        // Process results (read BSCAN)
        read_bscan_result(&bscan_res, dq_byte, channel);
        tmp = (bscan_res == 0xFF);
        tmp <<= shift;
        result[0] |= tmp;
    }
    dfi_tx_packet_buffer_free(&buffer);
}

void write_level_training(wddr_dev_t *wddr, wddr_channel_t channel, wddr_dq_byte_t dq_byte, uint8_t freq_id, bit_array_t *result)
{
    uint8_t pi_code;
    rect_t rect;
    uint8_t mr2 = wddr->table->cfg.freq[freq_id].dram.mr2;

    /*
     * ===========================================
     *           Write Level Initialization
     * ===========================================
     */
    // Differential Mode for Driver (DQS_t / DQS_c)
    driver_cmn_set_mode_reg_if(&wddr->channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.driver, DRIVER_MODE_DIFF);

    // Put DQS and DQ into Hi-Z
    driver_set_impedance_all_bits(&wddr->channel[channel].dq[dq_byte].tx.dq.driver, WDDR_SLICE_TYPE_DQ, DRIVER_IMPEDANCE_HIZ, DRIVER_IMPEDANCE_HIZ);

    // Put DQ into loopback (Enables BSCAN path at receiver)
    driver_cmn_set_loopback_reg_if(&wddr->channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.driver, DRIVER_LOOPBACK_MODE_ENABLE);

    /*
     * ===========================================
     *       Write Level Command Initialization
     * ===========================================
     */
    dram_write_mode_register_2(&wddr->dram, &wddr->dfi.dfi_buffer, WRITE_LEVEL_BIT_MASK | mr2);

    /*
     * ===========================================
     *         Write Level Sweep
     * ===========================================
     */
    write_level_sweep(wddr, channel, dq_byte, result);

    /*
     * ===========================================
     *             Training Cleanup
     * ===========================================
     */
    dram_write_mode_register_2(&wddr->dram, &wddr->dfi.dfi_buffer, mr2);

    // Reset TX Driver back to table configuration
    reset_tx_driver(wddr, freq_id);

    /*
     * ===========================================
     *          Training Eye Processing
     * ===========================================
     */
    range_t x_range = {0};
    x_range.start = WRLVL_PI_RANGE_START;
    x_range.stop = WRLVL_PI_RANGE_STOP;
    x_range.step_size = WRLVL_PI_RANGE_STEP;

    get_window(&result[0], &x_range, 1, &rect);
    pi_code = find_midpoint_and_convert(rect.origin.x, rect.size.w, &x_range);

    // Configure DQS PI based on training
    switch(wddr->dram.cfg->ratio)
    {
        case WDDR_FREQ_RATIO_1TO1:
            pi_set_code_reg_if(&wddr->channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pi.ddr, pi_code);
            // Store in table
            wddr->table->cal.freq[freq_id].channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pi.ddr.code = pi_code;
            // TODO: Remove; this is a hack so only channel 0 needs to be trainined
            pi_set_code_reg_if(&wddr->channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pi.ddr, pi_code);
            wddr->table->cal.freq[freq_id].channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pi.ddr.code = pi_code;
            break;
        case WDDR_FREQ_RATIO_1TO2:
            pi_set_code_reg_if(&wddr->channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pi.qdr, pi_code);
            // Store in table
            wddr->table->cal.freq[freq_id].channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pi.qdr.code = pi_code;
            // TODO: Remove; this is a hack so only channel 0 needs to be trainined
            pi_set_code_reg_if(&wddr->channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pi.qdr, pi_code);
            wddr->table->cal.freq[freq_id].channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pi.qdr.code = pi_code;
            break;
        case WDDR_FREQ_RATIO_1TO4:
            break;
    }
}

static void dq_dqs_sweep(wddr_dev_t *wddr, wddr_channel_t channel, wddr_dq_byte_t dq_byte, uint8_t wr_offset, uint8_t freq_id, uint8_t ratio, bit_array_t *result)
{
    uint8_t result_index = 0;
    command_data_t data;
    bit_array_t tmp;
    dfi_tx_packet_buffer_t buffer;
    dfi_rx_packet_buffer_t *rx_buffer = (dfi_rx_packet_buffer_t *) pvPortMalloc(sizeof(dfi_rx_packet_buffer_t));

    memset(&data, 0, sizeof(command_data_t));
    create_data_frame(&data, dq_byte, &dq_dqs_src[0], 32, 0);

    // Write to FIFO; Read from FIFO
    dfi_tx_packet_buffer_init(&buffer);
    create_wrfifo_packet_sequence(&buffer, &data, BL_16, wddr->dram.cfg, wr_offset, CS_0, 1);
    create_cke_packet_sequence(&buffer, 1);
    create_rdfifo_packet_sequence(&buffer, BL_16, wddr->dram.cfg->ratio, wddr->dram.cfg->phy_rd_en, CS_0, WRITE_LATENCY + BL + TWTR + 1);
    create_cke_packet_sequence(&buffer, 1);

    /*
     * ===========================================
     *        DQ / DQS Sweep
     * ===========================================
     */
    for (uint8_t vref_setting = DQ_DQS_VREF_RANGE_START; vref_setting < DQ_DQS_VREF_RANGE_STOP; vref_setting += DQ_DQS_VREF_RANGE_STEP, result_index++)
    {
        // MR14 DQ VREF
/*        dfi_tx_packet_buffer_init(&buffer);
        create_mrw_packet_sequence(&buffer, wddr->dram.cfg->ratio, CS_0, 0xE, vref_setting, 1);
        create_cke_packet_sequence(&buffer, 1);
        dfi_buffer_fill_packets(&wddr->dfi.dfi_buffer, &buffer.list);
        dfi_buffer_send_packets(&wddr->dfi.dfi_buffer, false);
        dfi_tx_packet_buffer_free(&buffer);*/
        dram_write_mode_register_14(&wddr->dram, &wddr->dfi.dfi_buffer, vref_setting);

        // NOTE: BASED on PI of DQS we don't start expect passed until then.
        // But not starting DQ PI here because want to see this transition.
        for (uint8_t pi_step = DQ_DQS_PI_RANGE_START, shift = 0; pi_step < DQ_DQS_PI_RANGE_STOP; pi_step += DQ_DQS_PI_RANGE_STEP, shift++)
        {
            // Step DQ PI
            // TODO: Once make this per byte, then pass in pi
            switch(ratio)
            {
                case WDDR_FREQ_RATIO_1TO1:
                    pi_set_code_reg_if(&wddr->channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dq.pi.ddr, pi_step);
                    break;
                case WDDR_FREQ_RATIO_1TO2:
                    pi_set_code_reg_if(&wddr->channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dq.pi.qdr, pi_step);
                    break;
                case WDDR_FREQ_RATIO_1TO4:
                    return;
            }

            reset_rx_fifo(wddr, channel);

            dfi_buffer_fill_packets(&wddr->dfi.dfi_buffer, &buffer.list);
            dfi_buffer_send_packets(&wddr->dfi.dfi_buffer, false);

            uint8_t num_packets = wddr->dram.cfg->ratio == WDDR_FREQ_RATIO_1TO1 ? 8 : 4;
            if (dfi_buffer_read_packets(&wddr->dfi.dfi_buffer, rx_buffer, num_packets) == WDDR_SUCCESS)
            {
                validate_dfi_receive_data(rx_buffer, 1 << dq_byte, PACKET_DATA_MASK_BOTH, wddr->dram.cfg->ratio, &data, &tmp);
                tmp <<= shift;
                result[result_index] |= tmp;
            }
        }
        tmp = 0;
    }
    dfi_tx_packet_buffer_free(&buffer);
    vPortFree(rx_buffer);
}

void dq_dqs_training(wddr_dev_t *wddr, wddr_channel_t channel, wddr_dq_byte_t dq_byte, uint8_t freq_id, bit_array_t *result)
{
    rect_t rect;
    uint8_t dq_pi_code = 0;
    int8_t delay = 0;
    uint32_t fc_delay, pipe_en, x_sel;
    uint32_t max_fom = 0, fom;

    /*
     * ===========================================
     *        DQ / DQS Training
     * ===========================================
     */

    range_t x_range = {0};
    x_range.start = DQ_DQS_PI_RANGE_START;
    x_range.stop = DQ_DQS_PI_RANGE_STOP;
    x_range.step_size = DQ_DQS_PI_RANGE_STEP;

    // MR13 VRCG = 1
    dram_vrcg_enable(&wddr->dram, &wddr->dfi.dfi_buffer);

    for (int8_t offset = DQ_DQS_CYC_RANGE_START; offset < (DQ_DQS_CYC_RANGE_STOP << wddr->dram.cfg->ratio); offset += DQ_DQS_CYC_RANGE_STEP)
    {
        memset(result, 0, sizeof(bit_array_t) * TRAINING_RESULT_ROW_NUM);
        dq_dqs_sweep(wddr, channel, dq_byte, offset, freq_id, wddr->dram.cfg->ratio, result);

        // TODO: DQ VREF extraction and stored in MR14 setting in DRAM table.
        fom = max_rect(result,
                       (DQ_DQS_PI_RANGE_STOP - DQ_DQS_PI_RANGE_START) / DQ_DQS_PI_RANGE_STEP,
                       (DQ_DQS_VREF_RANGE_STOP - DQ_DQS_VREF_RANGE_START) / DQ_DQS_VREF_RANGE_STEP,
                       &rect);
        if (fom >= max_fom)
        {
            max_fom = fom;

            /*
            * ===========================================
            *          Training Eye Processing
            * ===========================================
            */
            delay = offset;
            dq_pi_code = find_midpoint_and_convert(rect.origin.x, rect.size.w, &x_range);
        }
    }

    // MR13 VRCG = 0
    dram_vrcg_disable(&wddr->dram, &wddr->dfi.dfi_buffer);

    // Apply PI Code
    switch(wddr->dram.cfg->ratio)
    {
        case WDDR_FREQ_RATIO_1TO1:
            pi_set_code_reg_if(&wddr->channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dq.pi.ddr, dq_pi_code);
            // Store in table
            wddr->table->cal.freq[freq_id].channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dq.pi.ddr.code = dq_pi_code;
            break;
        case WDDR_FREQ_RATIO_1TO2:
            pi_set_code_reg_if(&wddr->channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dq.pi.qdr, dq_pi_code);
            // Store in table
            wddr->table->cal.freq[freq_id].channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dq.pi.qdr.code = dq_pi_code;
            break;
        case WDDR_FREQ_RATIO_1TO4:
            break;
    }

    // Convert from DRAM Domain to SDR Domain
    fc_delay = MAX(delay, 0);
    convert_dram_delay_to_sdr_delay(&fc_delay, wddr->dram.cfg->ratio, &pipe_en, &x_sel);

    /*
    * ===========================================
    *          Write Enable DELAY
    * ===========================================
    */

    // Update Table
    wddr->table->cal.freq[freq_id].channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_OE].sdr.fc_delay = fc_delay;
    wddr->table->cal.freq[freq_id].channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_DQS].sdr.fc_delay = fc_delay;
    wddr->table->cal.freq[freq_id].channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_OE].sdr.pipe_en = pipe_en;
    wddr->table->cal.freq[freq_id].channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_DQS].sdr.pipe_en = pipe_en;
    wddr->table->cal.freq[freq_id].channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_OE].sdr.x_sel = x_sel;
    wddr->table->cal.freq[freq_id].channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_DQS].sdr.x_sel = x_sel;

    // TODO: Remove; this is a hack so only channel 0 needs to be trainined
    wddr->table->cal.freq[freq_id].channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_OE].sdr.fc_delay = fc_delay;
    wddr->table->cal.freq[freq_id].channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_DQS].sdr.fc_delay = fc_delay;
    wddr->table->cal.freq[freq_id].channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_OE].sdr.pipe_en = pipe_en;
    wddr->table->cal.freq[freq_id].channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_DQS].sdr.pipe_en = pipe_en;
    wddr->table->cal.freq[freq_id].channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_OE].sdr.x_sel = x_sel;
    wddr->table->cal.freq[freq_id].channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_DQS].sdr.x_sel = x_sel;

    // Apply in the PHY
    sdr_pipeline_set_fc_delay_reg_if(&wddr->channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_OE].sdr,
                                    WDDR_SLICE_TYPE_DQS, fc_delay);
    sdr_pipeline_set_fc_delay_reg_if(&wddr->channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_DQS].sdr,
                                    WDDR_SLICE_TYPE_DQS, fc_delay);
    sdr_pipeline_set_pipe_en_reg_if(&wddr->channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_OE].sdr,
                                    WDDR_SLICE_TYPE_DQS, pipe_en);
    sdr_pipeline_set_pipe_en_reg_if(&wddr->channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_DQS].sdr,
                                    WDDR_SLICE_TYPE_DQS, pipe_en);
    sdr_pipeline_set_x_sel_reg_if(&wddr->channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_OE].sdr,
                                    WDDR_SLICE_TYPE_DQS, x_sel);
    sdr_pipeline_set_x_sel_reg_if(&wddr->channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_DQS].sdr,
                                    WDDR_SLICE_TYPE_DQS, x_sel);

    // TODO: Remove; this is a hack so only channel 0 needs to be trainined
    sdr_pipeline_set_fc_delay_reg_if(&wddr->channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_OE].sdr,
                                    WDDR_SLICE_TYPE_DQS, fc_delay);
    sdr_pipeline_set_fc_delay_reg_if(&wddr->channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_DQS].sdr,
                                    WDDR_SLICE_TYPE_DQS, fc_delay);
    sdr_pipeline_set_pipe_en_reg_if(&wddr->channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_OE].sdr,
                                    WDDR_SLICE_TYPE_DQS, pipe_en);
    sdr_pipeline_set_pipe_en_reg_if(&wddr->channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_DQS].sdr,
                                    WDDR_SLICE_TYPE_DQS, pipe_en);
    sdr_pipeline_set_x_sel_reg_if(&wddr->channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_OE].sdr,
                                    WDDR_SLICE_TYPE_DQS, x_sel);
    sdr_pipeline_set_x_sel_reg_if(&wddr->channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_DQS].sdr,
                                    WDDR_SLICE_TYPE_DQS, x_sel);

    /*
    * ===========================================
    *          Write Data DELAY
    * ===========================================
    */
    // Update Table
    wddr->table->cal.freq[freq_id].channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dq.pipeline.sdr.fc_delay = fc_delay;
    wddr->table->cal.freq[freq_id].channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dq.pipeline.sdr.pipe_en = pipe_en;
    wddr->table->cal.freq[freq_id].channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dq.pipeline.sdr.x_sel = x_sel;

    // TODO: Remove; this is a hack so only channel 0 needs to be trainined
    wddr->table->cal.freq[freq_id].channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dq.pipeline.sdr.fc_delay = fc_delay;
    wddr->table->cal.freq[freq_id].channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dq.pipeline.sdr.pipe_en = pipe_en;
    wddr->table->cal.freq[freq_id].channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dq.pipeline.sdr.x_sel = x_sel;

    // Apply in the PHY
    for (uint8_t bit_index = 0; bit_index < WDDR_PHY_DQ_SLICE_NUM; bit_index++)
    {
        sdr_pipeline_set_fc_delay_reg_if(&wddr->channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dq.pipeline[bit_index].sdr,
                                        WDDR_SLICE_TYPE_DQ, fc_delay);
        sdr_pipeline_set_pipe_en_reg_if(&wddr->channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dq.pipeline[bit_index].sdr,
                                        WDDR_SLICE_TYPE_DQ, pipe_en);
        sdr_pipeline_set_x_sel_reg_if(&wddr->channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dq.pipeline[bit_index].sdr,
                                        WDDR_SLICE_TYPE_DQ, x_sel);

        // TODO: Remove; this is a hack so only channel 0 needs to be trainined
        sdr_pipeline_set_fc_delay_reg_if(&wddr->channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dq.pipeline[bit_index].sdr,
                                        WDDR_SLICE_TYPE_DQ, fc_delay);
        sdr_pipeline_set_pipe_en_reg_if(&wddr->channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dq.pipeline[bit_index].sdr,
                                        WDDR_SLICE_TYPE_DQ, pipe_en);
        sdr_pipeline_set_x_sel_reg_if(&wddr->channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dq.pipeline[bit_index].sdr,
                                        WDDR_SLICE_TYPE_DQ, x_sel);
    }
}

/**
 * @brief Baseline Write / Read
 *
 * Issues Write / Read FIFO commands to DRAM. Ensures that data written matches
 * data that was read. This is called post-training to validate that training
 * for a given frequency worked.
 *
 * @return WAV_SUCCESS if correct data received, WAV_ERRROR otherwise
 */
wddr_return_t baseline_write_read(wddr_dev_t *wddr, uint8_t freq_id)
{
    command_data_t data;
    bit_array_t result;
    dfi_tx_packet_buffer_t buffer;
    dfi_rx_packet_buffer_t *rx_buffer = (dfi_rx_packet_buffer_t *) pvPortMalloc(sizeof(dfi_rx_packet_buffer_t));

    memset(&data, 0, sizeof(command_data_t));

    create_data_frame(&data, WDDR_DQ_BYTE_0, &dq_dqs_src[0], 32, 0);
    create_data_frame(&data, WDDR_DQ_BYTE_1, &dq_dqs_src[0], 32, 0);

    // Write to FIFO; Read from FIFO
    dfi_tx_packet_buffer_init(&buffer);
    create_wrfifo_packet_sequence(&buffer, &data, BL_16, &wddr->table->cfg.freq[freq_id].dram, 0, CS_0, 1);
    create_cke_packet_sequence(&buffer, 1);
    create_rdfifo_packet_sequence(&buffer, BL_16, wddr->dram.cfg->ratio, wddr->table->cfg.freq[freq_id].dram.phy_rd_en, CS_0, WRITE_LATENCY + BL + TWTR + 1);
    create_cke_packet_sequence(&buffer, 1);

    // Reset FIFO
    reset_rx_fifo(wddr, WDDR_CHANNEL_0);

    // Send Write / Read FIFO packets
    dfi_buffer_fill_packets(&wddr->dfi.dfi_buffer, &buffer.list);
    dfi_buffer_send_packets(&wddr->dfi.dfi_buffer, false);

    uint8_t num_packets = wddr->dram.cfg->ratio == WDDR_FREQ_RATIO_1TO1 ? 8 : 4;
    if (dfi_buffer_read_packets(&wddr->dfi.dfi_buffer, rx_buffer, num_packets) == WDDR_SUCCESS)
    {
        validate_dfi_receive_data(rx_buffer, DQ_BYTE_BOTH_MASK, PACKET_DATA_MASK_BOTH, wddr->dram.cfg->ratio, &data, &result);
    }

    dfi_tx_packet_buffer_free(&buffer);
    vPortFree(rx_buffer);

    if (!(result & 0x1))
    {
        return WDDR_ERROR;
    }
    return WDDR_SUCCESS;
}
