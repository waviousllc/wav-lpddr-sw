/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zqcal/device.h>
#include <zqcal/driver.h>

#define ZQCAL_PCAL_CODE_MIN  (0)
#define ZQCAL_PCAL_CODE_MAX  (0x3f)
#define ZQCAL_NCAL_CODE_MIN  (0)
#define ZQCAL_NCAL_CODE_MAX  (0x1f)

/**
 * @brief   ZQCAL Calibrate Common
 *
 * @details Perform ZQCAL calibration procedure for a single mode for a single
 *          VOH.
 *
 * @param[in]   zqcal       pointer to ZQCAL device.
 * @param[in]   mode        ZQCAL mode to calibrate.
 * @param[in]   max_code    stop condition for calibration.
 * @param[out]  code        pointer to store calibrated code value.
 *
 * @return      void
 */
static void zqcal_calibrate_common(zqcal_dev_t *zqcal,
                                   zqcal_mode_t mode,
                                   uint8_t max_code,
                                   uint8_t *code);

/**
 * @brief   ZQCAL Calibrate VOH
 *
 * @details Perform ZQCAL calibration procedure on P and N modes
 *          for a single VOH setting.
 *
 * @param[in]   zqcal   pointer to ZQCAL device.
 * @param[in]   voh     voh to calibrate.
 * @param[out]  cal     pointer to ZQCAL Calibration Structure to fill in with
 *                      calibrated values.
 *
 * @return      returns whether calibraton completed successfully.
 * @retval      WDDR_SUCCESS if successful.
 * @retval      WDDR_ERROR_ZQCAL_NCAL_AT_MIN if minimum code is used for NCAL.
 * @retval      WDDR_ERROR_ZQCAL_NCAL_AT_MAX if maximum code is used for NCAL.
 * @retval      WDDR_ERROR_ZQCAL_PCAL_AT_MIN if minimum code is used for PCAL.
 * @retval      WDDR_ERROR_ZQCAL_PCAL_AT_MAX if maximum code is used for PCAL.
 */
static wddr_return_t zqcal_calibrate_voh(zqcal_dev_t *zqcal,
                                         zqcal_voh_t voh,
                                         zqcal_cal_t *cal);



void zqcal_init(zqcal_dev_t *zqcal, uint32_t base)
{
    zqcal_init_reg_if(zqcal, base);
}

static void zqcal_calibrate_common(zqcal_dev_t *zqcal,
                                   zqcal_mode_t mode,
                                   uint8_t max_code,
                                   uint8_t *code)
{
    uint8_t zqval;
    uint8_t tmp_code;

    tmp_code = *code;
    zqcal_set_mode_reg_if(zqcal, mode);
    do
    {
        zqcal_set_code_reg_if(zqcal, mode, tmp_code);
        // TODO: need wait for it to settle?
        zqcal_get_output_reg_if(zqcal, &zqval);
        *code = tmp_code++;
    } while (zqval && tmp_code <= max_code);
}

static wddr_return_t zqcal_calibrate_voh(zqcal_dev_t *zqcal,
                                         zqcal_voh_t voh,
                                         zqcal_cal_t *cal)
{
    uint8_t n_code = ZQCAL_NCAL_CODE_MIN;
    uint8_t p_code = ZQCAL_PCAL_CODE_MIN;

    zqcal_set_voh_reg_if(zqcal, voh);
    zqcal_calibrate_common(zqcal,
                           ZQCAL_MODE_PULL_DOWN,
                           ZQCAL_NCAL_CODE_MAX,
                           &n_code);

    if (n_code == ZQCAL_NCAL_CODE_MIN)
    {
        return WDDR_ERROR_ZQCAL_NCAL_AT_MIN;
    }

    if (n_code == ZQCAL_NCAL_CODE_MAX)
    {
        return WDDR_ERROR_ZQCAL_NCAL_AT_MAX;
    }

    zqcal_calibrate_common(zqcal,
                           ZQCAL_MODE_PULL_UP,
                           ZQCAL_PCAL_CODE_MAX,
                           &p_code);

    if (p_code == ZQCAL_PCAL_CODE_MIN)
    {
        return WDDR_ERROR_ZQCAL_PCAL_AT_MIN;
    }

    if (p_code == ZQCAL_PCAL_CODE_MAX)
    {
        return WDDR_ERROR_ZQCAL_PCAL_AT_MAX;
    }

    cal->code[voh][ZQCAL_N_CAL] = n_code;
    cal->code[voh][ZQCAL_P_CAL] = p_code;
    return WDDR_SUCCESS;
}

wddr_return_t zqcal_calibrate(zqcal_dev_t *zqcal, zqcal_cal_t *cal)
{
    wddr_return_t ret;
    zqcal_set_state_reg_if(zqcal, ZQCAL_STATE_ENABLED);
    for (uint8_t voh = ZQCAL_VOH_0P5; voh < ZQCAL_VOH_NUM; voh++)
    {
        ret = zqcal_calibrate_voh(zqcal, (zqcal_voh_t) voh, cal);
        if (ret)
        {
            break;
        }
    }
    zqcal_set_state_reg_if(zqcal, ZQCAL_STATE_DISABLED);
    return ret;
}
