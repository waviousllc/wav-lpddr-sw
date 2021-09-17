/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _MISR_DRIVER_H_
#define _MISR_DRIVER_H_

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief   MISR Enable Enumeration
 *
 * CA       Enable all CA MISRs.
 * DQ0      Enable all DQ0 MISRs.
 * DQ1      Enable all DQ1 MISRs.
 * DQ2      Enable all DQ2 MISRs.
 * DQ3      Enable all DQ3 MISRs.
 * ALL      Enable all CA and DQx MISRs.
 */
typedef enum misr_en_t
{
    MISR_EN_CA  = 0x1,
    MISR_EN_DQ0 = 0x2,
    MISR_EN_DQ1 = 0x4,
    MISR_EN_DQ2 = 0x8,
    MISR_EN_DQ3 = 0x10,
    MISR_EN_ALL = 0x1F
} misr_en_t;

/**
 * @brief   MISR Enumeration
 *
 * DQ0_W3_TO_W0     MISR for DQ0 Phases W3 to W0
 * DQ0_W7_TO_W4     MISR for DQ0 Phases W7 to W4
 * DQ1_W3_TO_W0     MISR for DQ1 Phases W3 to W0
 * DQ1_W7_TO_W4     MISR for DQ1 Phases W7 to W4
 * DQ2_W3_TO_W0     MISR for DQ2 Phases W3 to W0
 * DQ2_W7_TO_W4     MISR for DQ2 Phases W7 to W4
 * DQ3_W3_TO_W0     MISR for DQ3 Phases W3 to W0
 * DQ3_W7_TO_W4     MISR for DQ3 Phases W7 to W4
 * CA_W1_TO_W0      MISR for CA Phases W1 to W0
 * CA_W3_TO_W2      MISR for CA Phases W3 to W2
 * CA_W5_TO_W4      MISR for CA Phases W5 to W4
 * CA_W7_TO_W6      MISR for CA Phases W7 to W6
 */
typedef enum misr_t
{
    MISR_DQ0_W3_TO_W0,
    MISR_DQ0_W7_TO_W4,
    MISR_DQ1_W3_TO_W0,
    MISR_DQ1_W7_TO_W4,
    MISR_DQ2_W3_TO_W0,
    MISR_DQ2_W7_TO_W4,
    MISR_DQ3_W3_TO_W0,
    MISR_DQ3_W7_TO_W4,
    MISR_CA_W1_TO_W0,
    MISR_CA_W3_TO_W2,
    MISR_CA_W5_TO_W4,
    MISR_CA_W7_TO_W6
} misr_t;

/**
 * @brief   MISR Set Enable Register Interface
 *
 * @details Sets enable for selected MISR at the register interface level.
 *
 * @param[in]   misr_en     MISR group to enable.
 * @param[in]   enable      Boolean to enable/disable MISR.
 *
 * @return      void
 */
void misr_set_enable_reg_if(misr_en_t misr_en, bool enable);

/**
 * @brief   MISR Clear LFSR Register Interface
 *
 * @details Clears LFSR Seed at the register interface level.
 *
 * @return      void
 */
void misr_clear_lfsr_reg_if(void);

/**
 * @brief   MISR Load LFSR Seed Register Interface
 *
 * @details Loads LFSR Seed value at the register interface level.
 *
 * @param[in]   low     Lower 32 bits of seed value.
 * @param[in]   high    Upper 4 bits of seed value.
 *
 * @return      void
 */
void misr_load_lfsr_seed_reg_if(uint32_t low, uint8_t high);

/**
 * @brief   MISR Read Register Interface
 *
 * @details Reads current MISR value from the register interface level.
 *
 * @param[in]   misr    MISR to read from.
 * @param[out]  low     Lower 32 bits of seed value.
 * @param[out]  high    Upper 4 bits of seed value.
 *
 * @return      void
 */
void misr_read_reg_if(misr_t misr, uint32_t *low, uint8_t *high);
#endif /* _MISR_DRIVER_H_ */
