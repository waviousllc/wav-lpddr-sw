#include <string.h>
#include <kernel/io.h>
#include <wddr/driver.h>
#include <wddr/memory_map.h>
#include <wddr/train.h>
#include <fsw/driver.h>

#define MAX_SDR_FC_DELAY    (0x3)

void validate_dfi_receive_data(dfi_rx_packet_buffer_t *rx_buffer,
                               uint8_t dq_byte_mask,
                               packet_data_mask_t data_mask,
                               uint8_t ratio,
                               command_data_t *expected,
                               bit_array_t *cmp_result)
{
    uint8_t res = 1, tmp_res = 0;
    uint8_t num_packets = ratio == WDDR_FREQ_RATIO_1TO1 ? 8 : 4;
    if (dq_byte_mask & DQ_BYTE_0_MASK)
    {
        dfi_rx_packet_buffer_data_compare(rx_buffer,
                                          expected,
                                          WDDR_DQ_BYTE_0,
                                          data_mask,
                                          num_packets,
                                          2 << ratio,
                                          &tmp_res);
        res &= tmp_res;
    }
    if (dq_byte_mask & DQ_BYTE_1_MASK)
    {
        dfi_rx_packet_buffer_data_compare(rx_buffer,
                                          expected,
                                          WDDR_DQ_BYTE_1,
                                          data_mask,
                                          num_packets,
                                          2 << ratio,
                                          &tmp_res);
        res &= tmp_res;
    }
    *cmp_result = (res & 0b1);
}

void convert_dram_delay_to_sdr_delay(uint32_t *fc_delay, wddr_freq_ratio_t ratio, uint32_t *pipe_en, uint32_t *x_sel)
{
    uint8_t dram_fc_delay = *fc_delay;
    uint32_t tmp;

    switch(ratio)
    {
        // 1 SDR Clock Cycle = 1 DRAM Clock Cycle (tCK)
        case WDDR_FREQ_RATIO_1TO1:
            dram_fc_delay = MIN(dram_fc_delay, MAX_SDR_FC_DELAY);
            *x_sel = 0x76543210;
            *pipe_en = 0x00000000;
            break;
        // 1 SDR Clock Cycle = 2 DRAM Clock Cycle (tCK)
        case WDDR_FREQ_RATIO_1TO2:
            *x_sel = 0x76543120;
            *pipe_en = 0x00000000;

            /* To account for half cycle delay, we have to delay phase 2 and
             * phase 3 by 1 cycle (pipe_en), while swapping rise and fall phases
             * of latch mux (x_sel), which gives an effective delay of 0.5 SDR
             * cycles or 1 DRAM Clock cycle (tCK)
             */
            if ((dram_fc_delay & 0x1) == 0x1)
            {
                *pipe_en = 0x0000000C;
                *x_sel = 0x76541302;
            }
            dram_fc_delay = MIN(dram_fc_delay >> 1, MAX_SDR_FC_DELAY);
            break;
        // 1 SDR Clock Cycle = 4 DRAM Clock Cycle (tCK)
        case WDDR_FREQ_RATIO_1TO4:
            break;
    }

    // TODO: Expand to all 8 phases when needed
    tmp = UPDATE_REG_FIELD(0x0, DDR_DQ_DQS_TX_SDR_FC_DLY_M0_R0_CFG_0_DLY_P0, dram_fc_delay);
    tmp = UPDATE_REG_FIELD(tmp, DDR_DQ_DQS_TX_SDR_FC_DLY_M0_R0_CFG_0_DLY_P1, dram_fc_delay);
    tmp = UPDATE_REG_FIELD(tmp, DDR_DQ_DQS_TX_SDR_FC_DLY_M0_R0_CFG_0_DLY_P2, dram_fc_delay);
    tmp = UPDATE_REG_FIELD(tmp, DDR_DQ_DQS_TX_SDR_FC_DLY_M0_R0_CFG_0_DLY_P3, dram_fc_delay);
    *fc_delay = tmp;
}

void reset_rx_fifo(wddr_dev_t *wddr, wddr_channel_t channel)
{
    wddr_clear_fifo_reg_if(wddr, channel);
    fsw_csp_sync_reg_if();
}
