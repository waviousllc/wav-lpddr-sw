#include <string.h>
#include <kernel/io.h>
#include <wddr/driver.h>
#include <wddr/memory_map.h>
#include <wddr/train.h>
#include <dfi/driver.h>
#include <fsw/driver.h>
#include <pi/driver.h>
#include <receiver/delay_driver.h>
#include <pipeline/sdr_driver.h>

static uint8_t rd_dq_src[32] = {0x55, 0xAA, 0x55, 0xAA,
                                0xAA, 0x55, 0xAA, 0x55,
                                0x55, 0x55, 0xAA, 0xAA,
                                0xAA, 0xAA, 0x55, 0x55,
                                0x55, 0xAA, 0x55, 0xAA,
                                0xAA, 0x55, 0xAA, 0x55,
                                0x55, 0x55, 0xAA, 0xAA,
                                0xAA, 0xAA, 0x55, 0x55};

/**
 * @brief   Align REN PI
 *
 * Function used to find code that results in REN PI alignment with REN edge.
 *
 * There is a dedicated circuit in the PHY that allows us to sample the output of
 * PI relative to REN signal. The sampling result in stored in the RX_PI_STA CSR.
 * To correctly read the received DQ bits into the FIFO, the REN signal must go high
 * before the output of the PI goes high. This can be detected by findng the PI setting that
 * occurs in REN samples a 0 after previously sampling a 1.
 *
 *                      +--------------------+
 *                      |
 * REN          +-------+
 *                         +-------+       +-------+
 *                         |       |       |
 * PI OUT          +-------+       +-------+
 *
 * pi_output
 *
 *
 * @param code  out parameter used to store PI code that results in alignment
 */
static void align_ren_pi(wddr_dev_t *wddr, uint8_t *code, wddr_channel_t channel, wddr_dq_byte_t dq_byte, uint8_t freq_id)
{
    uint32_t reg_val;
    uint32_t pi_sta_addr;
    uint8_t pi_code = 0;
    dfi_tx_packet_buffer_t buffer;

    dfi_tx_packet_buffer_init(&buffer);
    create_rddq_packet_sequence(&buffer, BL_16, wddr->dram.cfg->ratio, wddr->table->cfg.freq[freq_id].dram.phy_rd_en, CS_0, 1);
    create_cke_packet_sequence(&buffer, 1);

    // Push REN PI code until one is sampled
    pi_code = 0;
    pi_sta_addr = WDDR_MEMORY_MAP_PHY_CH_START + channel * WDDR_MEMORY_MAP_PHY_CH_OFFSET + WDDR_MEMORY_MAP_PHY_DQ_OFFSET * dq_byte + DDR_DQ_DQS_RX_PI_STA__ADR;
    do
    {
        // TODO: temp workaround
        pi_set_code_reg_if(&wddr->channel[channel].dq[dq_byte].rx.rank[WDDR_RANK_0].dqs.pi.ren, pi_code);

        dfi_buffer_fill_packets(&wddr->dfi.dfi_buffer, &buffer.list);
        dfi_buffer_send_packets(&wddr->dfi.dfi_buffer, false);

        reg_val = reg_read(pi_sta_addr);
        if (GET_REG_FIELD(reg_val, DDR_DQ_DQS_RX_PI_STA_REN_PI_PHASE))
        {
            break;
        }
        pi_code = (pi_code + 1) % REN_PI_CODE_MAX;
    } while(1);

    // Push REN PI code until zero is sampled
    do
    {
        pi_code = (pi_code + 1) % REN_PI_CODE_MAX;
        pi_set_code_reg_if(&wddr->channel[channel].dq[dq_byte].rx.rank[WDDR_RANK_0].dqs.pi.ren, pi_code);

        dfi_buffer_fill_packets(&wddr->dfi.dfi_buffer, &buffer.list);
        dfi_buffer_send_packets(&wddr->dfi.dfi_buffer, false);

        reg_val = reg_read(pi_sta_addr);
        if (!GET_REG_FIELD(reg_val, DDR_DQ_DQS_RX_PI_STA_REN_PI_PHASE))
        {
            break;
        }
    } while(1);

    // Set PI Code result
    *code = pi_code;
    dfi_tx_packet_buffer_free(&buffer);
}

/**
 * Read DQ is used to determine optimal delay setting at the receiver.
 * This traiig must assumed than REN, IE, and RE are wide enough such
 * that data can be received. These signals are optimized in other trianings.
 * The receiver has a delay setting that can be adjusted and is sweep in this training.
 */
static void read_dq_sweep(wddr_dev_t *wddr, receiver_tc_side_mask_t rec_tc_side_mask, wddr_channel_t channel, wddr_dq_byte_t dq_byte, uint8_t phy_rd_en, bit_array_t *result)
{
    uint16_t result_index = 0;
    bit_array_t tmp;
    packet_data_mask_t data_mask;
    command_data_t data;
    dfi_tx_packet_buffer_t buffer;
    dfi_rx_packet_buffer_t *rx_buffer = (dfi_rx_packet_buffer_t *) pvPortMalloc(sizeof(dfi_rx_packet_buffer_t));

    // Create Data Frame fro RD-EYE and REN
    memset(&data, 0, sizeof(command_data_t));
    create_data_frame(&data, dq_byte, &rd_dq_src[0], 32, 0);

    // NOTE: converting rec_tc_side_mask to data_mask (not equal)
    data_mask = (packet_data_mask_t) rec_tc_side_mask;

    /*
     * ===========================================
     *         Read DQ Command Initialization
     * ===========================================
     */
    dfi_tx_packet_buffer_init(&buffer);
    create_rddq_packet_sequence(&buffer, BL_16, wddr->dram.cfg->ratio, phy_rd_en, CS_0, 1);
    create_cke_packet_sequence(&buffer, 1);

    /*
     * ===========================================
     *         Read DQ Sweep
     * ===========================================
     */

    for (uint16_t vref_code = RDDQ_VREF_RANGE_START; vref_code < RDDQ_VREF_RANGE_STOP; vref_code += RDDQ_VREF_RANGE_STEP, result_index++)
    {
        // Update VREF
        vref_set_code(&wddr->cmn.vref, vref_code);

        for (uint16_t rcvr_step = RDDQ_RCVR_RANGE_START, shift = 0; rcvr_step < RDDQ_RCVR_RANGE_STOP; rcvr_step += RDDQ_RCVR_RANGE_STEP, shift++)
        {
            // Update Receiver Delay
            receiver_delay_set_delay_code_reg_if(&wddr->channel[channel].dq[dq_byte].rx.rank[WDDR_RANK_0].dqs.receiver.rx_delay, rcvr_step, rec_tc_side_mask);

            reset_rx_fifo(wddr, channel);

            // Send RDDQ command
            dfi_buffer_fill_packets(&wddr->dfi.dfi_buffer, &buffer.list);
            dfi_buffer_send_packets(&wddr->dfi.dfi_buffer, false);

            uint8_t num_packets = wddr->dram.cfg->ratio == WDDR_FREQ_RATIO_1TO1 ? 8 : 4;
            if (dfi_buffer_read_packets(&wddr->dfi.dfi_buffer, rx_buffer, num_packets) == WDDR_SUCCESS)
            {
                validate_dfi_receive_data(rx_buffer, 1 << dq_byte, data_mask, wddr->dram.cfg->ratio, &data, &tmp);
                tmp <<= shift;
                result[result_index] |= tmp;
            }
        }
    }
    tmp = 0;
    dfi_tx_packet_buffer_free(&buffer);
    vPortFree(rx_buffer);
}

void read_dq_training(wddr_dev_t *wddr, receiver_tc_side_mask_t rec_tc_side_mask, wddr_channel_t channel, wddr_dq_byte_t dq_byte, uint8_t freq_id, bit_array_t *result)
{
    rect_t rect;
    uint16_t receiver_delay = 0, vref = 0;
    uint8_t rddata_en_offset;
    uint8_t ren_pi_code;
    uint32_t max_fom = 0, fom;

    range_t x_range = {0};
    range_t y_range = {0};
    x_range.start = RDDQ_RCVR_RANGE_START;
    x_range.stop = RDDQ_RCVR_RANGE_STOP;
    x_range.step_size = RDDQ_RCVR_RANGE_STEP;
    y_range.start = RDDQ_VREF_RANGE_START;
    y_range.stop = RDDQ_VREF_RANGE_STOP;
    y_range.step_size = RDDQ_VREF_RANGE_STEP;

    // Align REN PI
    // Determine code for REN alignment
    align_ren_pi(wddr, &ren_pi_code, channel, dq_byte, freq_id);
    pi_set_code_reg_if(&wddr->channel[channel].dq[dq_byte].rx.rank[WDDR_RANK_0].dqs.pi.ren, ren_pi_code);

    /*
     * ===========================================
     *            Read DQ Sweep
     * ===========================================
     */

    // Perform this for cycle each cycle offset
    // Save current value of offsets
    rddata_en_offset = wddr->dram.cfg->phy_rd_en;

    for (int8_t offset = RDDQ_CYC_RANGE_START; offset < (RDDQ_CYC_RANGE_STOP << wddr->dram.cfg->ratio); offset += RDDQ_CYC_RANGE_STEP)
    {
        memset(result, 0, sizeof(bit_array_t) * TRAINING_RESULT_ROW_NUM);
        read_dq_sweep(wddr, rec_tc_side_mask, channel, dq_byte, rddata_en_offset + offset, result);
        fom = max_rect(result,
                       (RDDQ_RCVR_RANGE_STOP - RDDQ_RCVR_RANGE_START) / RDDQ_RCVR_RANGE_STEP,
                       (RDDQ_VREF_RANGE_STOP - RDDQ_VREF_RANGE_START) / RDDQ_VREF_RANGE_STEP,
                       &rect);
        if (fom >= max_fom)
        {
            max_fom = fom;

            /*
            * ===========================================
            *          Training Eye Processing
            * ===========================================
            */
            receiver_delay = find_midpoint_and_convert(rect.origin.x, rect.size.w, &x_range);
            vref =  find_midpoint_and_convert(rect.origin.y, rect.size.h, &y_range);
        }
    }

    receiver_delay_set_delay_code_reg_if(&wddr->channel[channel].dq[dq_byte].rx.rank[WDDR_RANK_0].dqs.receiver.rx_delay, receiver_delay, REC_T_SIDE_MASK);
    receiver_delay_set_delay_code_reg_if(&wddr->channel[channel].dq[dq_byte].rx.rank[WDDR_RANK_0].dqs.receiver.rx_delay, receiver_delay, REC_C_SIDE_MASK);
    vref_set_code(&wddr->cmn.vref, vref);
    // TODO: Remove; this is a hack so only channel 0 needs to be trainined
    receiver_delay_set_delay_code_reg_if(&wddr->channel[WDDR_CHANNEL_1].dq[dq_byte].rx.rank[WDDR_RANK_0].dqs.receiver.rx_delay, receiver_delay, REC_T_SIDE_MASK);
    receiver_delay_set_delay_code_reg_if(&wddr->channel[WDDR_CHANNEL_1].dq[dq_byte].rx.rank[WDDR_RANK_0].dqs.receiver.rx_delay, receiver_delay, REC_C_SIDE_MASK);

    // Store in table
    wddr->table->cal.freq[freq_id].channel[channel].dq[dq_byte].rx.rank[WDDR_RANK_0].dqs.receiver.rx_delay[REC_T_SIDE] = receiver_delay;
    wddr->table->cal.freq[freq_id].channel[channel].dq[dq_byte].rx.rank[WDDR_RANK_0].dqs.receiver.rx_delay[REC_C_SIDE] = receiver_delay;
    wddr->table->cal.freq[freq_id].common.vref.code = vref;
    // TODO: Remove; this is a hack so only channel 0 needs to be trainined
    wddr->table->cal.freq[freq_id].channel[WDDR_CHANNEL_1].dq[dq_byte].rx.rank[WDDR_RANK_0].dqs.receiver.rx_delay[REC_T_SIDE] = receiver_delay;
    wddr->table->cal.freq[freq_id].channel[WDDR_CHANNEL_1].dq[dq_byte].rx.rank[WDDR_RANK_0].dqs.receiver.rx_delay[REC_C_SIDE] = receiver_delay;
}

static void ren_sweep(wddr_dev_t *wddr, wddr_channel_t channel, wddr_dq_byte_t dq_byte, uint8_t ren_pi_code, uint8_t freq_id, bit_array_t *result)
{
    uint8_t rddata_en_offset;
    uint8_t result_index = 0;
    command_data_t data;
    bit_array_t tmp = 0;
    dfi_tx_packet_buffer_t buffer;
    dfi_rx_packet_buffer_t *rx_buffer = (dfi_rx_packet_buffer_t *) pvPortMalloc(sizeof(dfi_rx_packet_buffer_t));

    // Create Data Frame
    memset(&data, 0, sizeof(command_data_t));
    create_data_frame(&data, dq_byte, &rd_dq_src[0], 32, 0);

    // save current value of offsets
    rddata_en_offset = wddr->dram.cfg->phy_rd_en;

    for (int8_t ren_cycle = REN_CYC_RANGE_START; ren_cycle < (REN_CYC_RANGE_STOP << wddr->dram.cfg->ratio); ren_cycle += REN_CYC_RANGE_STEP, result_index++)
    {
        // Repack for new cycle offset
        dfi_tx_packet_buffer_init(&buffer);
        create_rddq_packet_sequence(&buffer, BL_16, wddr->dram.cfg->ratio, rddata_en_offset + ren_cycle, CS_0, 1);
        create_cke_packet_sequence(&buffer, 1);


        for (uint16_t pi_code = ren_pi_code + REN_PI_RANGE_START, shift = 0; pi_code < ren_pi_code + REN_PI_RANGE_STOP; pi_code += REN_PI_RANGE_STEP, shift++)
        {
            // Update REN-PI
            pi_set_code_reg_if(&wddr->channel[channel].dq[dq_byte].rx.rank[WDDR_RANK_0].dqs.pi.ren, pi_code % REN_PI_CODE_MAX);

            reset_rx_fifo(wddr, channel);

            // Send RDDQ command
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
        dfi_tx_packet_buffer_free(&buffer);
    }
    vPortFree(rx_buffer);
}

void ren_training(wddr_dev_t *wddr, wddr_channel_t channel, wddr_dq_byte_t dq_byte, uint8_t freq_id, bit_array_t *result)
{
    rect_t rect;
    int8_t delay = 0;
    uint8_t ren_pi_code = 0, i = 0, width = 0;
    uint32_t fc_delay = 0, pipe_en = 0, x_sel = 0;

    /*
     * ===========================================
     *        REN Training
     * ===========================================
     */

    // Determine code for REN alignment
    align_ren_pi(wddr, &ren_pi_code, channel, dq_byte, freq_id);

    ren_sweep(wddr, channel, dq_byte, ren_pi_code, freq_id, result);

    range_t x_range = {0};
    x_range.start = ren_pi_code + REN_PI_RANGE_START;
    x_range.stop = ren_pi_code + REN_PI_RANGE_STOP;
    x_range.step_size = REN_PI_RANGE_STEP;

    /*
     * ===========================================
     *          Training Eye Processing
     * ===========================================
     */
    /*
     * Process each cycle independently since window can span multiple cycles
     * and shouldn't fall on an edge
     */
    for (int16_t cycle = REN_CYC_RANGE_START; cycle < (REN_CYC_RANGE_STOP << wddr->dram.cfg->ratio); cycle += REN_CYC_RANGE_STEP, i++)
    {
        get_window(&result[i], &x_range, 1, &rect);
        if (rect.size.w > width)
        {
            ren_pi_code = find_midpoint_and_convert(rect.origin.x, rect.size.w, &x_range) % REN_PI_CODE_MAX;
            delay = cycle;
            width = rect.size.w;
        }
    }

    // Convert from DRAM Domain to SDR Domain
    fc_delay = MAX(delay, 0);
    convert_dram_delay_to_sdr_delay(&fc_delay, wddr->dram.cfg->ratio, &pipe_en, &x_sel);

    // Update Table
    wddr->table->cal.freq[freq_id].channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_IE].sdr.fc_delay = fc_delay;
    wddr->table->cal.freq[freq_id].channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_RE].sdr.fc_delay = fc_delay;
    wddr->table->cal.freq[freq_id].channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_REN].sdr.fc_delay = fc_delay;
    wddr->table->cal.freq[freq_id].channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_IE].sdr.pipe_en = pipe_en;
    wddr->table->cal.freq[freq_id].channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_RE].sdr.pipe_en = pipe_en;
    wddr->table->cal.freq[freq_id].channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_REN].sdr.pipe_en = pipe_en;
    wddr->table->cal.freq[freq_id].channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_IE].sdr.x_sel = x_sel;
    wddr->table->cal.freq[freq_id].channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_RE].sdr.x_sel = x_sel;
    wddr->table->cal.freq[freq_id].channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_REN].sdr.x_sel = x_sel;
    // TODO: Remove; this is a hack so only channel 0 needs to be trainined
    wddr->table->cal.freq[freq_id].channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_IE].sdr.fc_delay = fc_delay;
    wddr->table->cal.freq[freq_id].channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_RE].sdr.fc_delay = fc_delay;
    wddr->table->cal.freq[freq_id].channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_REN].sdr.fc_delay = fc_delay;
    wddr->table->cal.freq[freq_id].channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_IE].sdr.pipe_en = pipe_en;
    wddr->table->cal.freq[freq_id].channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_RE].sdr.pipe_en = pipe_en;
    wddr->table->cal.freq[freq_id].channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_REN].sdr.pipe_en = pipe_en;
    wddr->table->cal.freq[freq_id].channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_IE].sdr.x_sel = x_sel;
    wddr->table->cal.freq[freq_id].channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_RE].sdr.x_sel = x_sel;
    wddr->table->cal.freq[freq_id].channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_REN].sdr.x_sel = x_sel;

    // Apply in the PHY
    sdr_pipeline_set_fc_delay_reg_if(&wddr->channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_RE].sdr,
                                    WDDR_SLICE_TYPE_DQS, fc_delay);
    sdr_pipeline_set_fc_delay_reg_if(&wddr->channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_REN].sdr,
                                    WDDR_SLICE_TYPE_DQS, fc_delay);
    sdr_pipeline_set_fc_delay_reg_if(&wddr->channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_IE].sdr,
                                    WDDR_SLICE_TYPE_DQS, fc_delay);
    sdr_pipeline_set_pipe_en_reg_if(&wddr->channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_RE].sdr,
                                    WDDR_SLICE_TYPE_DQS, pipe_en);
    sdr_pipeline_set_pipe_en_reg_if(&wddr->channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_REN].sdr,
                                    WDDR_SLICE_TYPE_DQS, pipe_en);
    sdr_pipeline_set_pipe_en_reg_if(&wddr->channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_IE].sdr,
                                    WDDR_SLICE_TYPE_DQS, pipe_en);
    sdr_pipeline_set_x_sel_reg_if(&wddr->channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_RE].sdr,
                                    WDDR_SLICE_TYPE_DQS, x_sel);
    sdr_pipeline_set_x_sel_reg_if(&wddr->channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_REN].sdr,
                                    WDDR_SLICE_TYPE_DQS, x_sel);
    sdr_pipeline_set_x_sel_reg_if(&wddr->channel[channel].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_IE].sdr,
                                    WDDR_SLICE_TYPE_DQS, x_sel);

    // TODO: Remove; this is a hack so only channel 0 needs to be trainined
    sdr_pipeline_set_fc_delay_reg_if(&wddr->channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_RE].sdr,
                                    WDDR_SLICE_TYPE_DQS, fc_delay);
    sdr_pipeline_set_fc_delay_reg_if(&wddr->channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_REN].sdr,
                                    WDDR_SLICE_TYPE_DQS, fc_delay);
    sdr_pipeline_set_fc_delay_reg_if(&wddr->channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_IE].sdr,
                                    WDDR_SLICE_TYPE_DQS, fc_delay);
    sdr_pipeline_set_pipe_en_reg_if(&wddr->channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_RE].sdr,
                                    WDDR_SLICE_TYPE_DQS, pipe_en);
    sdr_pipeline_set_pipe_en_reg_if(&wddr->channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_REN].sdr,
                                    WDDR_SLICE_TYPE_DQS, pipe_en);
    sdr_pipeline_set_pipe_en_reg_if(&wddr->channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_IE].sdr,
                                    WDDR_SLICE_TYPE_DQS, pipe_en);
    sdr_pipeline_set_x_sel_reg_if(&wddr->channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_RE].sdr,
                                    WDDR_SLICE_TYPE_DQS, x_sel);
    sdr_pipeline_set_x_sel_reg_if(&wddr->channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_REN].sdr,
                                    WDDR_SLICE_TYPE_DQS, x_sel);
    sdr_pipeline_set_x_sel_reg_if(&wddr->channel[WDDR_CHANNEL_1].dq[dq_byte].tx.rank[WDDR_RANK_0].dqs.pipeline[DQS_SLICE_IE].sdr,
                                    WDDR_SLICE_TYPE_DQS, x_sel);

    // Configure REN PI based on training
    pi_set_code_reg_if(&wddr->channel[channel].dq[dq_byte].rx.rank[WDDR_RANK_0].dqs.pi.ren, ren_pi_code);

    // Store in table
    wddr->table->cal.freq[freq_id].channel[channel].dq[dq_byte].rx.rank[WDDR_RANK_0].dqs.pi.ren.code = ren_pi_code;

    // TODO: Remove; this is a hack so only channel 0 needs to be trainined
    pi_set_code_reg_if(&wddr->channel[WDDR_CHANNEL_1].dq[dq_byte].rx.rank[WDDR_RANK_0].dqs.pi.ren, ren_pi_code);
    wddr->table->cal.freq[freq_id].channel[WDDR_CHANNEL_1].dq[dq_byte].rx.rank[WDDR_RANK_0].dqs.pi.ren.code = ren_pi_code;
}

void read_window_training(wddr_dev_t *wddr, wddr_channel_t channel, uint8_t freq_id, bit_array_t *result)
{
    command_data_t data;
    dfi_paden_pext_cfg_t pext_cfg;
    uint8_t result_index = 0;
    uint32_t reg_val;
    rect_t rect;
    bit_array_t tmp;
    wddr_return_t ret;
    dfi_tx_packet_buffer_t buffer;
    dfi_rx_packet_buffer_t *rx_buffer = (dfi_rx_packet_buffer_t *) pvPortMalloc(sizeof(dfi_rx_packet_buffer_t));

    /*
     * ===========================================
     *  Read Window Training Initialization
     * ===========================================
     */
    // Create Data Frame
    memset(&data, 0, sizeof(command_data_t));
    create_data_frame(&data, WDDR_DQ_BYTE_0, &rd_dq_src[0], 32, 0);
    create_data_frame(&data, WDDR_DQ_BYTE_1, &rd_dq_src[0], 32, 0);

    memcpy(&pext_cfg, &wddr->table->cfg.freq[freq_id].dfi.paden_pext, sizeof(dfi_paden_pext_cfg_t));

    dfi_tx_packet_buffer_init(&buffer);
    create_rddq_packet_sequence(&buffer, BL_16, wddr->dram.cfg->ratio, wddr->table->cfg.freq[freq_id].dram.phy_rd_en, CS_0, 1);
    create_cke_packet_sequence(&buffer, 1);

    /*
     * ===========================================
     *      IE / RE Training
     * ===========================================
     */
    do
    {
        dfi_paden_pext_cfg_reg_if(&wddr->dfi, &pext_cfg);

        // Reset FIFO
        reset_rx_fifo(wddr, channel);

        // Send RDDQ command
        dfi_buffer_fill_packets(&wddr->dfi.dfi_buffer, &buffer.list);
        dfi_buffer_send_packets(&wddr->dfi.dfi_buffer, false);

        uint8_t num_packets = wddr->dram.cfg->ratio == WDDR_FREQ_RATIO_1TO1 ? 8 : 4;
        if ((ret = dfi_buffer_read_packets(&wddr->dfi.dfi_buffer, rx_buffer, num_packets)) == WDDR_SUCCESS)
        {
            validate_dfi_receive_data(rx_buffer, DQ_BYTE_BOTH_MASK, PACKET_DATA_MASK_BOTH, wddr->dram.cfg->ratio, &data, &tmp);
            tmp <<= result_index;
            result[0] |= tmp;
        }

        // Failure if receive buffer isn't empty
        reg_val = reg_read(wddr->dfi.dfi_buffer.base + DDR_DFICH_TOP_STA__ADR);

        if (GET_REG_FIELD(reg_val, DDR_DFICH_TOP_STA_EG_STATE) != DFI_FIFO_STATE_EMPTY)
        {
            // Mark as failure
            result[0] &= ~(((bit_array_t) 1) << result_index);
        }

        result_index++;

        // Can't progress any further; done
        if (pext_cfg.rd.fields.ie == 0)
        {
            break;
        }
        pext_cfg.rd.fields.ie -= 1;
        pext_cfg.rd.fields.re -= 1;
    } while(ret == WDDR_SUCCESS);

    /*
     * ===========================================
     *      IE / RE EYE Processing
     * ===========================================
     */
    // Now find optimal setting
    range_t x_range = {0};
    x_range.start = 0;
    x_range.stop = result_index;
    x_range.step_size = 1;

    get_window(&result[0], &x_range, 1, &rect);

    // Window is backwards so index is how much to subtract
    uint8_t offset = find_midpoint_and_convert(rect.origin.x, rect.size.w, &x_range);

    // Update Tables
    wddr->table->cfg.freq[freq_id].dfi.paden_pext.rd.fields.ie -= offset;
    wddr->table->cfg.freq[freq_id].dfi.paden_pext.rd.fields.re -= offset;

    // Apply updated setting
    dfi_paden_pext_cfg_reg_if(&wddr->dfi,
                              &wddr->table->cfg.freq[freq_id].dfi.paden_pext);

    dfi_tx_packet_buffer_free(&buffer);
    vPortFree(rx_buffer);
}
