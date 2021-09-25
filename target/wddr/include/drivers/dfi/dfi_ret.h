/**
* Copyright (c) 2021 Wavious LLC.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef _DFI_RET_H_
#define _DFI_RET_H_

/**
 * @brief   DFI Return Type Enumerations
 *
 * SUCCESS              no error.
 * ERROR                general error.
 * ERROR_FIFO_FULL      error that fifo is full.
 * ERROR_FIFO_EMPTY     error that fifo is empty.
 */
typedef enum {
    DFI_SUCCESS,
    DFI_ERROR,
    DFI_ERROR_FIFO_FULL,
    DFI_ERROR_FIFO_EMPTY,
} dfi_return_t;

#endif /* _DFI_RET_H_ */
