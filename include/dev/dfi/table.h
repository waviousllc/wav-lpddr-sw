/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DFI_TABLE_H_
#define _DFI_TABLE_H_

#include <stdint.h>

/**
 * @brief   DFI Write Gearbox Mode
 */
typedef enum dfi_wgb_mode_t
{
    DFIWGB_2TO2,
    DFIWGB_4TO2,
    DFIWGB_4TO4,
    DFIWGB_8TO2,
    DFIWGB_8TO4,
    DFIWGB_8TO8,
    DFIWGB_16TO8,
    DFIWGB_32TO8,
    DFIWGB_32TO16
} dfi_wgb_mode_t;

/**
 * @brief   DFI Read Gearbox Mode
 */
typedef enum dfi_rgb_mode_t
{
    DFIRGB_1TO1,
    DFIRGB_2TO2,
    DFIRGB_2TO4,
    DFIRGB_2TO8,
    DFIRGB_4TO4,
    DFIRGB_4TO8,
    DFIRGB_8TO8,
    DFIRGB_8TO16,
    DFIRGB_8TO32,
    DFIRGB_16TO32
} dfi_rgb_mode_t;

/**
 * @brief   DFI Pipe Stage State Enumerations
 *
 * DFI_PIPE_DISABLE   DFI pipe stage is disabled.
 * DFI_PIPE_ENABLE    DFI pipe stage is enabled.
 */
typedef enum dfi_pipe_state_t
{
    DFI_PIPE_DISABLE,
    DFI_PIPE_ENABLE
} dfi_pipe_state_t;

/**
 * @brief   DFI Pipeline Configuration
 *
 * @details DFI Pipeline configuration structure.
 *
 * @note    This structure directly maps to the CSR layout. Please do not
 *          modify unless the CSR fields change.
 *
 * pipe_en         pipe enable setting in DFI path.
 * post_gb_fc_dly  full cycle delay(s) to DFI path.
 * gb_mode         gearbox mode for DFI path.
 */
typedef struct dfi_pipe_cfg_t
{
    uint16_t pipe_en: 4;
    uint16_t post_gb_fc_dly: 4;
    uint16_t gb_mode: 4;
} dfi_pipe_cfg_t;

/**
 * @brief   DFI Clock Enable Pulse Extension
 *
 * @details DFI Clock Enable Pulse Extension structure. Used to add cycles to
 *          end of clock pulse o extend width by configured number of phases.
 *
 * @note    This structure directly maps to the CSR layout. Please do not
 *          modify unless the CSR fields change.
 *
 * dq_wrclk    DQ Write Clock.
 * dqs_wrclk   DQS Write Clock.
 * ca          CA Clock.
 * ck          CK Clock.
 * rd_clk      Read Clock.
 */
typedef union dfi_clken_pext_cfg_t
{
    struct
    {
        uint32_t dq_wrclk: 4;
        uint32_t dqs_wrclk: 4;
        uint32_t ca_clk: 4;
        uint32_t ck_clk: 4;
        uint32_t rd_clk: 4;
    };
    uint32_t val;
} dfi_clken_pext_cfg_t;

/**
 * @brief   DFI Override Traffic Configuration
 *
 * @details DFI Traffic Override Traffic configuration. Configures dynamic clock
 *          gating override from software.
 *
 * @note    This structure directly maps to the CSR layout. Please do not
 *          modify unless the CSR fields change.
 *
 * sel_dq_wr   sets override for DQ write path.
 * sel_dqs     sets override for DQS path.
 * sel_ca      sets override for CA path.
 * sel_ck      sets override for CK path.
 * val_dq_wr   override value for DQ write path (only applies if sel = 1).
 * val_dqs     override value for DQS path (only applies if sel = 1).
 * val_ca      override value for CA path (only applies if sel = 1).
 * val_ck      override value for CK path (only applies if sel = 1).
 * sel_dq_rd   sets override fro DQ read path.
 * val_dq_rd   override value for DQ read path (only applies if sel = 1).
 */
typedef struct dfi_ovr_traffic_cfg_t
{
    struct
    {
        uint32_t sel_dq_wr:     1;
        uint32_t sel_dqs:       1;
        uint32_t sel_ca:        1;
        uint32_t sel_ck:        1;
        uint32_t val_dq_wr:     1;
        uint32_t val_dqs:       1;
        uint32_t val_ca:        1;
        uint32_t val_ck:        1;
        uint32_t sel_dq_rd:     1;
        uint32_t val_dq_rd:     1;
    };
    uint32_t val;
} dfi_ovr_traffic_cfg_t;

/**
 * @brief   DFI Pad Enable Pulse Extension
 *
 * @details Phase extensions for write / read data and control paths in terms of
 *          number of phases.
 *
 * @note    This structure directly maps to the CSR layout. Please do not
 *          modify unless the CSR fields change.
 *
 * wrd.cs   Write data chip select.
 * wrd.en   Write data enable.
 * wrd.oe   Write data output enable.
 * wck.oe   WCK output enable.
 * wck.cs   WCK chip select.
 * rd.rcs   Read chip select??.
 * rd.ie    Read input enable.
 * rd.re    Read receive enable.
 * rd.ren   Read enable.
 */
typedef struct dfi_paden_pext_cfg_t
{
    union
    {
        struct
        {
            uint8_t cs: 8;
            uint8_t en: 8;
            uint8_t oe: 8;
        } fields;
        uint32_t val;
    } wrd;
    union
    {
        struct
        {
            uint8_t oe: 8;
            uint8_t cs: 8;
        } fields;
        uint32_t val;
    } wck;
    union
    {
        struct
        {
            uint8_t rcs: 8;
            uint8_t ie: 8;
            uint8_t re: 8;
            uint8_t ren: 8;
        } fields;
        uint32_t val;
    } rd;
} dfi_paden_pext_cfg_t;


/**
 * @brief   DFI Frequency Dependent Configuration Structure
 *
 * @details Per-Frequency configuration structure for DFI device.
 */
typedef struct dfi_freq_cfg_t
{
   dfi_pipe_cfg_t           ckctrl;
   dfi_pipe_cfg_t           wckctrl;
   dfi_pipe_cfg_t           wctrl;
   dfi_pipe_cfg_t           wenctrl;
   dfi_pipe_cfg_t           wrcctrl;
   dfi_pipe_cfg_t           wrc;
   dfi_pipe_cfg_t           wrd;
   dfi_pipe_cfg_t           rctrl;
   dfi_pipe_cfg_t           rdd;
   dfi_clken_pext_cfg_t     clken_pext;
   dfi_paden_pext_cfg_t     paden_pext;
   dfi_ovr_traffic_cfg_t    ovr_traffic;
} dfi_freq_cfg_t;

#endif /* _DFI_TABLE_H_ */
