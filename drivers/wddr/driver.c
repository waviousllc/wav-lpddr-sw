/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <wddr/driver.h>
#include <wddr/memory_map.h>
#include <kernel/io.h>

void wddr_clear_fifo_reg_if(wddr_dev_t *wddr, wddr_channel_t channel)
{
    ca_top_clear_fifo_reg_if(wddr->channel[channel].ca_reg);
    dq_top_clear_fifo_reg_if(wddr->channel[channel].dq_reg[WDDR_DQ_BYTE_0]);
    dq_top_clear_fifo_reg_if(wddr->channel[channel].dq_reg[WDDR_DQ_BYTE_1]);
}

void wddr_set_dram_resetn_pin_reg_if(wddr_dev_t *wddr, bool override, bool high)
{
    cmn_rstn_set_pin_reg_if(wddr->cmn.cmn_reg, override, high);
}

void wddr_set_chip_select_reg_if(wddr_dev_t *wddr, wddr_channel_t channel, wddr_rank_t rank, bool override)
{
    ca_top_set_chip_select_reg_if(wddr->channel[channel].ca_reg, rank, override);
    dq_top_set_chip_select_reg_if(wddr->channel[channel].dq_reg[WDDR_DQ_BYTE_0], rank, override);
    dq_top_set_chip_select_reg_if(wddr->channel[channel].dq_reg[WDDR_DQ_BYTE_1], rank, override);
}

void wddr_read_bscan_result_reg_if(wddr_dev_t *wddr, wddr_dq_byte_t dq_byte, wddr_channel_t channel, uint8_t *result)
{
    // Read BSCAN
    dq_dq_bscan_get_status_reg_if(wddr->channel[channel].dq_reg[dq_byte], result);
}
