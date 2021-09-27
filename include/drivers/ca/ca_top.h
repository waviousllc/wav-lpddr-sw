/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _CA_TOP_H_
#define _CA_TOP_H_

#include <stdbool.h>
#include "ca_reg.h"

/**
 * @brief   CA TOP Set Chip Select Register Interface
 *
 * @details Sets the read and write chip select value seen by the PHY.
 *
 * @param[in]   ca_reg      pointer to CA register space.
 * @param[in]   rank_sel    rank select value to override.
 * @param[in]   override    flag to indicate if CS should be overridden.
 *
 * @return      void.
 */
void ca_top_set_chip_select_reg_if(ca_reg_t *ca_reg,
                                   wddr_rank_t rank_sel,
                                   bool override);

/**
 * @brief   CA TOP Clear FIFO Register Interface
 *
 * @details Clears receive fifo associated with CA.
 *
 * @param[in]   ca_reg      pointer to CA register space.
 *
 * @return      void.
 */
void ca_top_clear_fifo_reg_if(ca_reg_t *ca_reg);

/**
 * @brief   CA TOP Get Chip Select Status Register Interface
 *
 * @details Return the current write (wcs) and read (rcs) chip select values.
 *
 * @param[in]   ca_reg      pointer to CA register space.
 * @param[out]  wcs         pointer to store write chip select status.
 * @param[out]  rcs         pointer to store read chip select status.
 *
 * @return      void.
 */
void ca_top_get_chip_select_status_reg_if(ca_reg_t *ca_reg,
                                          uint8_t *wcs,
                                          uint8_t *rcs);

#endif /* _CA_TOP_H_ */
