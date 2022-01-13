/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <channel/device.h>
#include <wddr/memory_map.h>

#define WAV_SA_MIN_CAL_CODE (0)
#define WAV_SA_MAX_CAL_CODE (31)
#define WAV_SA_MID_CAL_CODE ((WAV_SA_MAX_CAL_CODE - WAV_SA_MIN_CAL_CODE + 1) / 2)
#define WAV_SA_CAL_DIR_NEG  (0)
#define WAV_SA_CAL_DIR_POS  (1

/**
 * @brief   Sense Amp (Sensamp) DQ Byte Configure (Internal)
 *
 * @details Configures all Sensamp Devices in a DQ Byte.
 *
 * @param[in]   dq_reg      pointer to DQ register space.
 * @param[in]   calibrate   flag to indicate if data should be calibrated.
 * @param[out]  data        pointer to table to fill in with calibrated codes.
 *
 * @return      returns whether calibraton completed successfully.
 * @retval      WDDR_SUCCESS if successful.
 * @retval      WDDR_ERROR otherwise.
 */
static wddr_return_t __sensamp_dqbyte_configure(dq_reg_t *dq_reg,
                                                bool calibrate,
                                                sensamp_dqbit_cfg_t data[WDDR_PHY_DQ_SLICE_NUM]);


void channel_init(channel_dev_t *channel_dev, uint32_t base)
{
    for (uint8_t byte = 0; byte < WDDR_PHY_DQ_BYTE_NUM; byte++)
    {
        channel_dev->dq_reg[byte] = (dq_reg_t *)(base +
                                                 WDDR_MEMORY_MAP_PHY_DQ_OFFSET * byte);
    }

    channel_dev->ca_reg = (ca_reg_t *)(base +
                                       WDDR_MEMORY_MAP_PHY_CA_OFFSET);
}

void channel_enable(channel_dev_t *channel_dev, bool enable, const channel_freq_cfg_t *cfg)
{
    for (uint8_t rank = 0; rank < WDDR_PHY_RANK; rank++)
    {
        for (uint8_t msr = WDDR_MSR_0; msr <= WDDR_MSR_1; msr++)
        {
            for (uint8_t byte = 0; byte < WDDR_PHY_DQ_BYTE_NUM; byte++)
            {
                // DQ
                dq_dq_pi_odr_set_cfg_reg_if(channel_dev->dq_reg[byte], msr, rank, enable, cfg->dq[byte].tx.rank[rank].dq.pi.odr.val);
                dq_dq_pi_qdr_set_cfg_reg_if(channel_dev->dq_reg[byte], msr, rank, enable, cfg->dq[byte].tx.rank[rank].dq.pi.qdr.val);
                dq_dq_pi_ddr_set_cfg_reg_if(channel_dev->dq_reg[byte], msr, rank, enable, cfg->dq[byte].tx.rank[rank].dq.pi.ddr.val);
                dq_dq_pi_rt_set_cfg_reg_if(channel_dev->dq_reg[byte], msr, rank, enable, cfg->dq[byte].tx.rank[rank].dq.pi.rt.val);
                for (uint8_t bit = 0; bit < WDDR_PHY_DQ_SLICE_NUM; bit++)
                {
                    dq_dq_lpde_set_cfg_reg_if(channel_dev->dq_reg[byte], msr, rank, bit, enable, cfg->dq[byte].tx.rank[rank].dq.lpde[bit].val);
                }

                // DQS
                dq_dqs_pi_odr_set_cfg_reg_if(channel_dev->dq_reg[byte], msr, rank, enable, cfg->dq[byte].tx.rank[rank].dqs.pi.odr.val);
                dq_dqs_pi_qdr_set_cfg_reg_if(channel_dev->dq_reg[byte], msr, rank, enable, cfg->dq[byte].tx.rank[rank].dqs.pi.qdr.val);
                dq_dqs_pi_ddr_set_cfg_reg_if(channel_dev->dq_reg[byte], msr, rank, enable, cfg->dq[byte].tx.rank[rank].dqs.pi.ddr.val);
                dq_dqs_pi_rt_set_cfg_reg_if(channel_dev->dq_reg[byte], msr, rank, enable, cfg->dq[byte].tx.rank[rank].dqs.pi.rt.val);
                dq_dqs_pi_sdr_set_cfg_reg_if(channel_dev->dq_reg[byte], msr, rank, enable, cfg->dq[byte].tx.rank[rank].dqs.pi.sdr.val);
                dq_dqs_pi_dfi_set_cfg_reg_if(channel_dev->dq_reg[byte], msr, rank, enable, cfg->dq[byte].tx.rank[rank].dqs.pi.dfi.val);
                for (uint8_t bit = 0; bit < WDDR_PHY_DQS_TXRX_SLICE_NUM; bit++)
                {
                    dq_dqs_lpde_set_cfg_reg_if(channel_dev->dq_reg[byte], msr, rank, bit, enable, cfg->dq[byte].tx.rank[rank].dqs.lpde[bit].val);
                }

                // RX Phase Interpolators
                dq_dqs_pi_ren_set_cfg_reg_if(channel_dev->dq_reg[byte], msr, rank, enable, cfg->dq[byte].rx.rank[rank].dqs.pi.ren.val);
                dq_dqs_pi_rcs_set_cfg_reg_if(channel_dev->dq_reg[byte], msr, rank, enable, cfg->dq[byte].rx.rank[rank].dqs.pi.rcs.val);
                dq_dqs_pi_rdqs_set_cfg_reg_if(channel_dev->dq_reg[byte], msr, rank, enable, cfg->dq[byte].rx.rank[rank].dqs.pi.rdqs.val);

                // RX LPDE
                dq_dqs_lpde_rx_sdr_set_cfg_reg_if(channel_dev->dq_reg[byte], msr, rank, enable, cfg->dq[byte].rx.rank[rank].dqs.sdr_lpde.val);
            }

            // CA
            ca_dq_pi_odr_set_cfg_reg_if(channel_dev->ca_reg, msr, rank, enable, cfg->ca.tx.rank[rank].ca.pi.odr.val);
            ca_dq_pi_qdr_set_cfg_reg_if(channel_dev->ca_reg, msr, rank, enable, cfg->ca.tx.rank[rank].ca.pi.qdr.val);
            ca_dq_pi_ddr_set_cfg_reg_if(channel_dev->ca_reg, msr, rank, enable, cfg->ca.tx.rank[rank].ca.pi.ddr.val);
            ca_dq_pi_rt_set_cfg_reg_if(channel_dev->ca_reg, msr, rank, enable, cfg->ca.tx.rank[rank].ca.pi.rt.val);
            for (uint8_t bit = 0; bit < WDDR_PHY_CA_SLICE_NUM; bit++)
            {
                ca_dq_lpde_set_cfg_reg_if(channel_dev->ca_reg, msr, rank, bit, enable, cfg->ca.tx.rank[rank].ca.lpde[bit].val);
            }


            // CK
            ca_dqs_pi_odr_set_cfg_reg_if(channel_dev->ca_reg, msr, rank, enable, cfg->ca.tx.rank[rank].ck.pi.odr.val);
            ca_dqs_pi_qdr_set_cfg_reg_if(channel_dev->ca_reg, msr, rank, enable, cfg->ca.tx.rank[rank].ck.pi.qdr.val);
            ca_dqs_pi_ddr_set_cfg_reg_if(channel_dev->ca_reg, msr, rank, enable, cfg->ca.tx.rank[rank].ck.pi.ddr.val);
            ca_dqs_pi_rt_set_cfg_reg_if(channel_dev->ca_reg, msr, rank, enable, cfg->ca.tx.rank[rank].ck.pi.rt.val);
            ca_dqs_pi_sdr_set_cfg_reg_if(channel_dev->ca_reg, msr, rank, enable, cfg->ca.tx.rank[rank].ck.pi.sdr.val);
            ca_dqs_pi_dfi_set_cfg_reg_if(channel_dev->ca_reg, msr, rank, enable, cfg->ca.tx.rank[rank].ck.pi.dfi.val);
            for (uint8_t bit = 0; bit < WDDR_PHY_CK_TXRX_SLICE_NUM; bit++)
            {
                ca_dqs_lpde_set_cfg_reg_if(channel_dev->ca_reg, msr, rank, bit, enable, cfg->ca.tx.rank[rank].ck.lpde[bit].val);
            }

            ca_dqs_lpde_rx_sdr_set_cfg_reg_if(channel_dev->ca_reg, msr, rank, enable, cfg->ca.rx.rank[rank].ck.sdr_lpde.val);
        }
    }
}

wddr_return_t channel_ca_tx_driver_set_impedance_all_bits(channel_dev_t *channel_dev,
                                                          wddr_slice_type_t slice_type,
                                                          wddr_msr_t msr,
                                                          driver_impedance_t tx,
                                                          driver_impedance_t rx)
{
    switch(slice_type)
    {
        case WDDR_SLICE_TYPE_CA:
            for (uint8_t bit = 0; bit < WDDR_PHY_CA_SLICE_NUM; bit++)
            {
                ca_dq_driver_set_impedance_reg_if(channel_dev->ca_reg, msr, bit, tx, rx);
            }
            break;
        case WDDR_SLICE_TYPE_CK:
            for (uint8_t bit = 0; bit < WDDR_PHY_CK_TXRX_SLICE_NUM; bit++)
            {
                ca_dqs_driver_set_impedance_reg_if(channel_dev->ca_reg, msr, bit, tx, rx);
            }
            break;
        default:
            return WDDR_ERROR;
    }

    return WDDR_SUCCESS;
}

wddr_return_t channel_dq_tx_driver_set_impedance_all_bits(channel_dev_t *channel_dev,
                                                          wddr_slice_type_t slice_type,
                                                          wddr_msr_t msr,
                                                          wddr_dq_byte_t byte,
                                                          driver_impedance_t tx,
                                                          driver_impedance_t rx)
{
    switch(slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            for (uint8_t bit = 0; bit < WDDR_PHY_DQ_SLICE_NUM; bit++)
            {
                dq_dq_driver_set_impedance_reg_if(channel_dev->dq_reg[byte], msr, bit, tx, rx);
            }
            break;
        case WDDR_SLICE_TYPE_DQS:
            for (uint8_t bit = 0; bit < WDDR_PHY_DQS_TXRX_SLICE_NUM; bit++)
            {
                dq_dqs_driver_set_impedance_reg_if(channel_dev->dq_reg[byte], msr, bit, tx, rx);
            }
            break;
        default:
            return WDDR_ERROR;
    }
    return WDDR_SUCCESS;
}

wddr_return_t channel_ca_tx_driver_override_all_bits(channel_dev_t *channel_dev,
                                                     wddr_slice_type_t slice_type,
                                                     wddr_msr_t msr,
                                                     const driver_cfg_t *cfg,
                                                     bool output_enable)
{
    switch(slice_type)
    {
        case WDDR_SLICE_TYPE_CA:
            for (uint8_t bit = 0; bit < WDDR_PHY_CA_SLICE_NUM; bit++)
            {
                ca_dq_driver_set_impedance_reg_if(channel_dev->ca_reg, msr, bit, cfg->tx_impd, cfg->rx_impd);
                ca_dq_driver_set_override_reg_if(channel_dev->ca_reg, msr, bit, cfg->override.sel, cfg->override.val_t);
                ca_dq_driver_set_oe_reg_if(channel_dev->ca_reg, msr, bit, output_enable);
            }
            break;
        case WDDR_SLICE_TYPE_CK:
            for (uint8_t bit = 0; bit < WDDR_PHY_CK_TXRX_SLICE_NUM; bit++)
            {
                ca_dqs_driver_set_impedance_reg_if(channel_dev->ca_reg, msr, bit, cfg->tx_impd, cfg->rx_impd);
                ca_dqs_driver_set_override_reg_if(channel_dev->ca_reg, msr, bit, cfg->override.sel, cfg->override.val_t, cfg->override.val_c);
                ca_dqs_driver_set_oe_reg_if(channel_dev->ca_reg, msr, bit, output_enable);
            }
            break;
        default:
            return WDDR_ERROR;
    }
    return WDDR_SUCCESS;
}

wddr_return_t channel_dq_tx_driver_override_all_bits(channel_dev_t *channel_dev,
                                                     wddr_slice_type_t slice_type,
                                                     wddr_msr_t msr,
                                                     wddr_dq_byte_t byte,
                                                     const driver_cfg_t *cfg,
                                                     bool output_enable)
{
    switch(slice_type)
    {
        case WDDR_SLICE_TYPE_DQ:
            for (uint8_t bit = 0; bit < WDDR_PHY_DQ_SLICE_NUM; bit++)
            {
                dq_dq_driver_set_impedance_reg_if(channel_dev->dq_reg[byte], msr, bit, cfg->tx_impd, cfg->rx_impd);
                dq_dq_driver_set_override_reg_if(channel_dev->dq_reg[byte], msr, bit, cfg->override.sel, cfg->override.val_t);
                dq_dq_driver_set_oe_reg_if(channel_dev->dq_reg[byte], msr, bit, output_enable);
            }
            break;
        case WDDR_SLICE_TYPE_DQS:
            for (uint8_t bit = 0; bit < WDDR_PHY_DQS_TXRX_SLICE_NUM; bit++)
            {
                dq_dqs_driver_set_impedance_reg_if(channel_dev->dq_reg[byte], msr, bit, cfg->tx_impd, cfg->rx_impd);
                dq_dqs_driver_set_override_reg_if(channel_dev->dq_reg[byte], msr, bit, cfg->override.sel, cfg->override.val_t, cfg->override.val_c);
                dq_dqs_driver_set_oe_reg_if(channel_dev->dq_reg[byte], msr, bit, output_enable);
            }
            break;
        default:
            return WDDR_ERROR;
    }
    return WDDR_SUCCESS;
}

wddr_return_t channel_rx_sa_dqbyte_configure(channel_dev_t *channel_dev,
                                             wddr_dq_byte_t byte,
                                             bool calibrate,
                                             sensamp_dqbyte_common_cfg_t *cfg)
{
    dq_dqs_sa_cmn_set_state_reg_if(channel_dev->dq_reg[byte], SA_STATE_CAL_ENABLED);
    PROPAGATE_ERROR(__sensamp_dqbyte_configure(channel_dev->dq_reg[byte], calibrate, cfg->dq));
    dq_dqs_sa_cmn_set_state_reg_if(channel_dev->dq_reg[byte], SA_STATE_DYNAMIC);
    return WDDR_SUCCESS;
}

typedef struct cal_params
{
    uint8_t start;
    uint8_t stop;
    int8_t step;
    uint8_t criteria;
} cal_params_t;

typedef struct dqbit_params
{
    uint8_t bit_index;
    uint8_t sa_index;
} dqbit_params_t;

static wddr_return_t __sensamp_dqbyte_calibrate(dq_reg_t *dq_reg,
                                                cal_params_t cal_params,
                                                dqbit_params_t dqbit_params,
                                                uint8_t *code)
{
    uint8_t val;
    int8_t cal_code = cal_params.start;
    do
    {
        dq_dq_sa_set_cal_code_reg_if(dq_reg, WDDR_MSR_0, WDDR_RANK_0, dqbit_params.bit_index, dqbit_params.sa_index, cal_code);
        dq_dq_sa_get_status_reg_if(dq_reg, 0, 0, &val);

        // val for first cal_code must be 1
        if (cal_code == WAV_SA_MIN_CAL_CODE && val == 0)
        {
            return WDDR_ERROR;
        }
        // val for last cal code must be 0
        else if (cal_code == WAV_SA_MAX_CAL_CODE && val == 1 )
        {
            return WDDR_ERROR;
        }
        else if (val == cal_params.criteria)
        {
            break;
        }

        cal_code += cal_params.step;

    } while (cal_code <= cal_params.stop);

    *code = cal_code;
    return WDDR_SUCCESS;
}

static wddr_return_t __sensamp_dqbyte_configure(dq_reg_t *dq_reg,
                                                bool calibrate,
                                                sensamp_dqbit_cfg_t data[WDDR_PHY_DQ_SLICE_NUM])
{
    uint8_t code_down = 0, code_up = 0;
    uint8_t bit_index, sa_index;
    cal_params_t cal_params;
    dqbit_params_t dqbit_params;

    for (bit_index = 0; bit_index < WDDR_PHY_DQ_SLICE_NUM; bit_index++)
    {
        for (sa_index = SA_0_INDEX; sa_index < WDDR_PHY_CFG; sa_index++)
        {
            // Calibrate before updating value
            if (calibrate)
            {
                dq_dq_sa_clear_cal_code_reg_if(dq_reg, WDDR_MSR_0, WDDR_RANK_0, bit_index);
                dqbit_params.sa_index = sa_index;
                dqbit_params.bit_index = bit_index;

                cal_params.start = WAV_SA_MIN_CAL_CODE;
                cal_params.stop = WAV_SA_MAX_CAL_CODE;
                cal_params.criteria = 0;
                cal_params.step = 1;
                __sensamp_dqbyte_calibrate(dq_reg, cal_params, dqbit_params, &code_up);

                cal_params.start = WAV_SA_MAX_CAL_CODE;
                cal_params.stop = WAV_SA_MIN_CAL_CODE;
                cal_params.criteria = 1;
                cal_params.step = -1;
                __sensamp_dqbyte_calibrate(dq_reg, cal_params, dqbit_params, &code_down);

                // save code into calibration table
                data[bit_index].code[sa_index] = (code_down + code_up) >> 1;
            } // if (calibrate)

            // Update value for RANK 0 and RANK 1; and both MSRs
            for (uint8_t rank_index = WDDR_RANK_0; rank_index < WDDR_PHY_RANK; rank_index++)
            {
                dq_dq_sa_set_cal_code_reg_if(dq_reg,
                                             WDDR_MSR_0,
                                             rank_index,
                                             bit_index,
                                             sa_index,
                                             data[bit_index].code[sa_index]);
                dq_dq_sa_set_cal_code_reg_if(dq_reg,
                                             WDDR_MSR_1,
                                             rank_index,
                                             bit_index,
                                             sa_index,
                                             data[bit_index].code[sa_index]);
            }
        }
    }
    return WDDR_SUCCESS;
}
