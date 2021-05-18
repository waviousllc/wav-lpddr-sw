/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
// MODULE CTRL
// RANGE 0xFFFF:0x0000
// WIDTH 32

#ifndef _DDR_CTRL_CSR_H_
#define _DDR_CTRL_CSR_H_

// Word Address 0x00000000 : DDR_CTRL_CLK_CFG (RW)
#define DDR_CTRL_CLK_CFG_PLL_CLK_EN__MSK                                                                     (  0x00000001 )
#define DDR_CTRL_CLK_CFG_PLL_CLK_EN__SHFT                                                                    (  0x00000000 )
#define DDR_CTRL_CLK_CFG_REF_CLK_SEL__MSK                                                                    (  0x00000002 )
#define DDR_CTRL_CLK_CFG_REF_CLK_SEL__SHFT                                                                   (  0x00000001 )
#define DDR_CTRL_CLK_CFG_MCU_GFM_SEL__MSK                                                                    (  0x00000004 )
#define DDR_CTRL_CLK_CFG_MCU_GFM_SEL__SHFT                                                                   (  0x00000002 )
#define DDR_CTRL_CLK_CFG_AHBCLK_DIV2_EN__MSK                                                                 (  0x00000008 )
#define DDR_CTRL_CLK_CFG_AHBCLK_DIV2_EN__SHFT                                                                (  0x00000003 )
#define DDR_CTRL_CLK_CFG_AHB_CLK_ON__MSK                                                                     (  0x00000010 )
#define DDR_CTRL_CLK_CFG_AHB_CLK_ON__SHFT                                                                    (  0x00000004 )
#define DDR_CTRL_CLK_CFG_REF_CLK_ON__MSK                                                                     (  0x00000020 )
#define DDR_CTRL_CLK_CFG_REF_CLK_ON__SHFT                                                                    (  0x00000005 )
#define DDR_CTRL_CLK_CFG_MCU_CLK_CGC_EN__MSK                                                                 (  0x00000100 )
#define DDR_CTRL_CLK_CFG_MCU_CLK_CGC_EN__SHFT                                                                (  0x00000008 )
#define DDR_CTRL_CLK_CFG_REF_CLK_CGC_EN__MSK                                                                 (  0x00000200 )
#define DDR_CTRL_CLK_CFG_REF_CLK_CGC_EN__SHFT                                                                (  0x00000009 )
#define DDR_CTRL_CLK_CFG__ADR                                                                                (  0x00000000 )
#define DDR_CTRL_CLK_CFG__WIDTH                                                                              (  10 )
#define DDR_CTRL_CLK_CFG__POR                                                                                (  0x00000308 )
#define DDR_CTRL_CLK_CFG__MSK                                                                                (  0x0000033F )


// Word Address 0x00000004 : DDR_CTRL_CLK_STA (R)
#define DDR_CTRL_CLK_STA_MCU_GFM_SEL0__MSK                                                                   (  0x00000001 )
#define DDR_CTRL_CLK_STA_MCU_GFM_SEL0__SHFT                                                                  (  0x00000000 )
#define DDR_CTRL_CLK_STA_MCU_GFM_SEL1__MSK                                                                   (  0x00000002 )
#define DDR_CTRL_CLK_STA_MCU_GFM_SEL1__SHFT                                                                  (  0x00000001 )
#define DDR_CTRL_CLK_STA_DFI_CLK_ON__MSK                                                                     (  0x00000004 )
#define DDR_CTRL_CLK_STA_DFI_CLK_ON__SHFT                                                                    (  0x00000002 )
#define DDR_CTRL_CLK_STA__ADR                                                                                (  0x00000004 )
#define DDR_CTRL_CLK_STA__WIDTH                                                                              (   3 )
#define DDR_CTRL_CLK_STA__POR                                                                                (  0x00000000 )
#define DDR_CTRL_CLK_STA__MSK                                                                                (  0x00000007 )


// Word Address 0x00000008 : DDR_CTRL_AHB_SNOOP_CFG (RW)
#define DDR_CTRL_AHB_SNOOP_CFG_TS_ENABLE__MSK                                                                (  0x00000001 )
#define DDR_CTRL_AHB_SNOOP_CFG_TS_ENABLE__SHFT                                                               (  0x00000000 )
#define DDR_CTRL_AHB_SNOOP_CFG_TS_RESET__MSK                                                                 (  0x00000002 )
#define DDR_CTRL_AHB_SNOOP_CFG_TS_RESET__SHFT                                                                (  0x00000001 )
#define DDR_CTRL_AHB_SNOOP_CFG_RDATA_CLR__MSK                                                                (  0x00000100 )
#define DDR_CTRL_AHB_SNOOP_CFG_RDATA_CLR__SHFT                                                               (  0x00000008 )
#define DDR_CTRL_AHB_SNOOP_CFG_RDATA_UPDATE__MSK                                                             (  0x00000400 )
#define DDR_CTRL_AHB_SNOOP_CFG_RDATA_UPDATE__SHFT                                                            (  0x0000000A )
#define DDR_CTRL_AHB_SNOOP_CFG_SNOOP_MODE__MSK                                                               (  0x00001000 )
#define DDR_CTRL_AHB_SNOOP_CFG_SNOOP_MODE__SHFT                                                              (  0x0000000C )
#define DDR_CTRL_AHB_SNOOP_CFG__ADR                                                                          (  0x00000008 )
#define DDR_CTRL_AHB_SNOOP_CFG__WIDTH                                                                        (  13 )
#define DDR_CTRL_AHB_SNOOP_CFG__POR                                                                          (  0x00000000 )
#define DDR_CTRL_AHB_SNOOP_CFG__MSK                                                                          (  0x00001503 )


// Word Address 0x0000000c : DDR_CTRL_AHB_SNOOP_STA (R)
#define DDR_CTRL_AHB_SNOOP_STA_EMPTY__MSK                                                                    (  0x00000001 )
#define DDR_CTRL_AHB_SNOOP_STA_EMPTY__SHFT                                                                   (  0x00000000 )
#define DDR_CTRL_AHB_SNOOP_STA_FULL__MSK                                                                     (  0x00000002 )
#define DDR_CTRL_AHB_SNOOP_STA_FULL__SHFT                                                                    (  0x00000001 )
#define DDR_CTRL_AHB_SNOOP_STA__ADR                                                                          (  0x0000000c )
#define DDR_CTRL_AHB_SNOOP_STA__WIDTH                                                                        (   2 )
#define DDR_CTRL_AHB_SNOOP_STA__POR                                                                          (  0x00000000 )
#define DDR_CTRL_AHB_SNOOP_STA__MSK                                                                          (  0x00000003 )


// Word Address 0x00000010 : DDR_CTRL_AHB_SNOOP_DATA_STA (R)
#define DDR_CTRL_AHB_SNOOP_DATA_STA_RDATA__MSK                                                               (  0xFFFFFFFF )
#define DDR_CTRL_AHB_SNOOP_DATA_STA_RDATA__SHFT                                                              (  0x00000000 )
#define DDR_CTRL_AHB_SNOOP_DATA_STA__ADR                                                                     (  0x00000010 )
#define DDR_CTRL_AHB_SNOOP_DATA_STA__WIDTH                                                                   (  32 )
#define DDR_CTRL_AHB_SNOOP_DATA_STA__POR                                                                     (  0x00000000 )
#define DDR_CTRL_AHB_SNOOP_DATA_STA__MSK                                                                     (  0xFFFFFFFF )


// Word Address 0x00000014 : DDR_CTRL_AHB_SNOOP_PATTERN_CFG (RW)
#define DDR_CTRL_AHB_SNOOP_PATTERN_CFG_PAT_0_EN__MSK                                                         (  0x00000001 )
#define DDR_CTRL_AHB_SNOOP_PATTERN_CFG_PAT_0_EN__SHFT                                                        (  0x00000000 )
#define DDR_CTRL_AHB_SNOOP_PATTERN_CFG_PAT_0_POLARITY__MSK                                                   (  0x00000002 )
#define DDR_CTRL_AHB_SNOOP_PATTERN_CFG_PAT_0_POLARITY__SHFT                                                  (  0x00000001 )
#define DDR_CTRL_AHB_SNOOP_PATTERN_CFG_PAT_0_MODE__MSK                                                       (  0x0000000C )
#define DDR_CTRL_AHB_SNOOP_PATTERN_CFG_PAT_0_MODE__SHFT                                                      (  0x00000002 )
#define DDR_CTRL_AHB_SNOOP_PATTERN_CFG_PAT_1_EN__MSK                                                         (  0x00000100 )
#define DDR_CTRL_AHB_SNOOP_PATTERN_CFG_PAT_1_EN__SHFT                                                        (  0x00000008 )
#define DDR_CTRL_AHB_SNOOP_PATTERN_CFG_PAT_1_POLARITY__MSK                                                   (  0x00000200 )
#define DDR_CTRL_AHB_SNOOP_PATTERN_CFG_PAT_1_POLARITY__SHFT                                                  (  0x00000009 )
#define DDR_CTRL_AHB_SNOOP_PATTERN_CFG_PAT_1_MODE__MSK                                                       (  0x00000C00 )
#define DDR_CTRL_AHB_SNOOP_PATTERN_CFG_PAT_1_MODE__SHFT                                                      (  0x0000000A )
#define DDR_CTRL_AHB_SNOOP_PATTERN_CFG__ADR                                                                  (  0x00000014 )
#define DDR_CTRL_AHB_SNOOP_PATTERN_CFG__WIDTH                                                                (  12 )
#define DDR_CTRL_AHB_SNOOP_PATTERN_CFG__POR                                                                  (  0x00000000 )
#define DDR_CTRL_AHB_SNOOP_PATTERN_CFG__MSK                                                                  (  0x00000F0F )


// Word Address 0x00000018 : DDR_CTRL_AHB_SNOOP_PATTERN_0_CFG (RW)
#define DDR_CTRL_AHB_SNOOP_PATTERN_0_CFG_PAT_VAL__MSK                                                        (  0xFFFFFFFF )
#define DDR_CTRL_AHB_SNOOP_PATTERN_0_CFG_PAT_VAL__SHFT                                                       (  0x00000000 )
#define DDR_CTRL_AHB_SNOOP_PATTERN_0_CFG__ADR                                                                (  0x00000018 )
#define DDR_CTRL_AHB_SNOOP_PATTERN_0_CFG__WIDTH                                                              (  32 )
#define DDR_CTRL_AHB_SNOOP_PATTERN_0_CFG__POR                                                                (  0x00000000 )
#define DDR_CTRL_AHB_SNOOP_PATTERN_0_CFG__MSK                                                                (  0xFFFFFFFF )


// Word Address 0x0000001c : DDR_CTRL_AHB_SNOOP_PATTERN_1_CFG (RW)
#define DDR_CTRL_AHB_SNOOP_PATTERN_1_CFG_PAT_VAL__MSK                                                        (  0xFFFFFFFF )
#define DDR_CTRL_AHB_SNOOP_PATTERN_1_CFG_PAT_VAL__SHFT                                                       (  0x00000000 )
#define DDR_CTRL_AHB_SNOOP_PATTERN_1_CFG__ADR                                                                (  0x0000001c )
#define DDR_CTRL_AHB_SNOOP_PATTERN_1_CFG__WIDTH                                                              (  32 )
#define DDR_CTRL_AHB_SNOOP_PATTERN_1_CFG__POR                                                                (  0x00000000 )
#define DDR_CTRL_AHB_SNOOP_PATTERN_1_CFG__MSK                                                                (  0xFFFFFFFF )


// Word Address 0x00000020 : DDR_CTRL_AHB_SNOOP_PATTERN_STA (W1C)
#define DDR_CTRL_AHB_SNOOP_PATTERN_STA_PAT_0_DETECT__MSK                                                     (  0x00000001 )
#define DDR_CTRL_AHB_SNOOP_PATTERN_STA_PAT_0_DETECT__SHFT                                                    (  0x00000000 )
#define DDR_CTRL_AHB_SNOOP_PATTERN_STA_PAT_1_DETECT__MSK                                                     (  0x00000002 )
#define DDR_CTRL_AHB_SNOOP_PATTERN_STA_PAT_1_DETECT__SHFT                                                    (  0x00000001 )
#define DDR_CTRL_AHB_SNOOP_PATTERN_STA__ADR                                                                  (  0x00000020 )
#define DDR_CTRL_AHB_SNOOP_PATTERN_STA__WIDTH                                                                (   2 )
#define DDR_CTRL_AHB_SNOOP_PATTERN_STA__POR                                                                  (  0x00000000 )
#define DDR_CTRL_AHB_SNOOP_PATTERN_STA__MSK                                                                  (  0x00000003 )


// Word Address 0x00000024 : DDR_CTRL_DEBUG_CFG (RW)
#define DDR_CTRL_DEBUG_CFG_VAL__MSK                                                                          (  0xFFFFFFFF )
#define DDR_CTRL_DEBUG_CFG_VAL__SHFT                                                                         (  0x00000000 )
#define DDR_CTRL_DEBUG_CFG__ADR                                                                              (  0x00000024 )
#define DDR_CTRL_DEBUG_CFG__WIDTH                                                                            (  32 )
#define DDR_CTRL_DEBUG_CFG__POR                                                                              (  0x00000000 )
#define DDR_CTRL_DEBUG_CFG__MSK                                                                              (  0xFFFFFFFF )


// Word Address 0x00000028 : DDR_CTRL_DEBUG1_CFG (RW)
#define DDR_CTRL_DEBUG1_CFG_OVR_SEL__MSK                                                                     (  0x00000001 )
#define DDR_CTRL_DEBUG1_CFG_OVR_SEL__SHFT                                                                    (  0x00000000 )
#define DDR_CTRL_DEBUG1_CFG__ADR                                                                             (  0x00000028 )
#define DDR_CTRL_DEBUG1_CFG__WIDTH                                                                           (   1 )
#define DDR_CTRL_DEBUG1_CFG__POR                                                                             (  0x00000000 )
#define DDR_CTRL_DEBUG1_CFG__MSK                                                                             (  0x00000001 )


#endif /* _DDR_CTRL_CSR_H_ */
