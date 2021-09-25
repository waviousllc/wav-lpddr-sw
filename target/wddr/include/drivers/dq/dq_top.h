/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DQ_TOP_H_
#define _DQ_TOP_H_

#include <stdbool.h>
#include <dq_reg.h>

/**
 * @brief   DQ TOP Set Chip Select Register Interface
 *
 * @details Sets the read and write chip select value seen by the PHY.
 *
 * @param[in]   dq_reg      pointer to DQ Byte register space.
 * @param[in]   rank_sel    rank select value to override.
 * @param[in]   override    flag to indicate if CS should be overridden.
 *
 * @return      void.
 */
void dq_top_set_chip_select_reg_if(dq_reg_t *dq_reg,
                                   wddr_rank_t rank_sel,
                                   bool override);

/**
 * @brief   DQ TOP Clear FIFO Register Interface
 *
 * @details Clears receive fifo associated with DQ Byte.
 *
 * @param[in]   dq_reg      pointer to DQ Byte register space.
 *
 * @return      void.
 */
void dq_top_clear_fifo_reg_if(dq_reg_t *dq_reg);

/**
 * @brief   DQ TOP Get Chip Select Status Register Interface
 *
 * @details Return the current write (wcs) and read (rcs) chip select values.
 *
 * @param[in]   dq_reg      pointer to DQ Byte register space.
 * @param[out]  wcs         pointer to store write chip select status.
 * @param[out]  rcs         pointer to store read chip select status.
 *
 * @return      void.
 */
void dq_top_get_chip_select_status_reg_if(dq_reg_t *dq_reg,
                                          uint8_t *wcs,
                                          uint8_t *rcs);

#endif /* _DQ_TOP_H_ */
