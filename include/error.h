/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _ERROR_H_
#define _ERROR_H_

#define ASSERT_ERROR(cause)         \
    do {                            \
        return cause;               \
    } while(0)

#define PROPAGATE_ERROR(func)       \
    do {                            \
        int err = func;             \
        if (err)                    \
        {                           \
            return err;             \
        }                           \
    } while(0)

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
typedef enum wddr_return_t
{
    /* COMMON RETURN CODES */
    WDDR_SUCCESS,
    WDDR_ERROR,
    WDDR_ERROR_NOT_VALID_FREQ,
    /* IP SPECIFIC RETURN CODES */
    WDDR_ERROR_DFI_PACKET_FIFO_FULL,
    WDDR_ERROR_DFI_PACKET_FIFO_EMPTY,
    WDDR_ERROR_ZQCAL_PCAL_AT_MIN,
    WDDR_ERROR_ZQCAL_PCAL_AT_MAX,
    WDDR_ERROR_ZQCAL_NCAL_AT_MIN,
    WDDR_ERROR_ZQCAL_NCAL_AT_MAX,
} wddr_return_t;

#endif /* _ERROR_H_ */
