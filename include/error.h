/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _ERROR_H_
#define _ERROR_H_

/**
 * @brief WLPDDR Return Value Enumerations
 *
 * @details Common return types.
 *
 * SUCCESS                      No errors.
 * ERROR                        At least one error.
 * ERROR_NOT_VALID_FREQ         Invalid frequency requested.
 * ERROR_DFI_LOAD_BUFFER_FULL   The DFI load buffer is full.
 * ERROR_DFI_PACKET_FIFO_FULL   The DFI Packet FIFO is full.
 * ERROR_ZQCAL_CODE_AT_MIN      The ZQCAL code is at the min value.
 * ERROR_ZQCAL_CODE_AT_MAX      The ZQCAL code is at the max value.
 */
typedef enum wlpddr_return_t
{
    /* COMMON RETURN CODES */
    WAV_LPDDR_SUCCESS,
    WAV_LPDDR_ERROR,
    WAV_LPDDR_ERROR_NOT_VALID_FREQ,
    /* IP SPECIFIC RETURN CODES */
    WAV_LPDDR_ERROR_DFI_LOAD_BUFFER_FULL,
    WAV_LPDDR_ERROR_DFI_PACKET_FIFO_FULL,
    WAV_LPDDR_ERROR_ZQCAL_PCAL_AT_MIN,
    WAV_LPDDR_ERROR_ZQCAL_PCAL_AT_MAX,
    WAV_LPDDR_ERROR_ZQCAL_NCAL_AT_MIN,
    WAV_LPDDR_ERROR_ZQCAL_NCAL_AT_MAX,
} wlpddr_return_t;

#endif /* _ERROR_H_ */
