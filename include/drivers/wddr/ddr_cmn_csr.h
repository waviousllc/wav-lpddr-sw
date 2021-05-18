/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
// MODULE CMN
// RANGE 0x03FF:0x0000
// WIDTH 32

#ifndef _DDR_CMN_CSR_H_
#define _DDR_CMN_CSR_H_

// Word Address 0x00000008 : DDR_CMN_VREF_M0_CFG (RW)
#define DDR_CMN_VREF_M0_CFG_CTRL__MSK                                                                        (  0x000000FF )
#define DDR_CMN_VREF_M0_CFG_CTRL__SHFT                                                                       (  0x00000000 )
#define DDR_CMN_VREF_M0_CFG_EN__MSK                                                                          (  0x00000200 )
#define DDR_CMN_VREF_M0_CFG_EN__SHFT                                                                         (  0x00000009 )
#define DDR_CMN_VREF_M0_CFG_HIZ__MSK                                                                         (  0x00000400 )
#define DDR_CMN_VREF_M0_CFG_HIZ__SHFT                                                                        (  0x0000000A )
#define DDR_CMN_VREF_M0_CFG_PWR__MSK                                                                         (  0x00001800 )
#define DDR_CMN_VREF_M0_CFG_PWR__SHFT                                                                        (  0x0000000B )
#define DDR_CMN_VREF_M0_CFG__ADR                                                                             (  0x00000008 )
#define DDR_CMN_VREF_M0_CFG__WIDTH                                                                           (  13 )
#define DDR_CMN_VREF_M0_CFG__POR                                                                             (  0x00000000 )
#define DDR_CMN_VREF_M0_CFG__MSK                                                                             (  0x00001EFF )


// Word Address 0x0000000c : DDR_CMN_VREF_M1_CFG (RW)
#define DDR_CMN_VREF_M1_CFG_CTRL__MSK                                                                        (  0x000000FF )
#define DDR_CMN_VREF_M1_CFG_CTRL__SHFT                                                                       (  0x00000000 )
#define DDR_CMN_VREF_M1_CFG_EN__MSK                                                                          (  0x00000200 )
#define DDR_CMN_VREF_M1_CFG_EN__SHFT                                                                         (  0x00000009 )
#define DDR_CMN_VREF_M1_CFG_HIZ__MSK                                                                         (  0x00000400 )
#define DDR_CMN_VREF_M1_CFG_HIZ__SHFT                                                                        (  0x0000000A )
#define DDR_CMN_VREF_M1_CFG_PWR__MSK                                                                         (  0x00001800 )
#define DDR_CMN_VREF_M1_CFG_PWR__SHFT                                                                        (  0x0000000B )
#define DDR_CMN_VREF_M1_CFG__ADR                                                                             (  0x0000000c )
#define DDR_CMN_VREF_M1_CFG__WIDTH                                                                           (  13 )
#define DDR_CMN_VREF_M1_CFG__POR                                                                             (  0x00000000 )
#define DDR_CMN_VREF_M1_CFG__MSK                                                                             (  0x00001EFF )


// Word Address 0x00000010 : DDR_CMN_ZQCAL_CFG (RW)
#define DDR_CMN_ZQCAL_CFG_NCAL__MSK                                                                          (  0x0000001F )
#define DDR_CMN_ZQCAL_CFG_NCAL__SHFT                                                                         (  0x00000000 )
#define DDR_CMN_ZQCAL_CFG_CAL_EN__MSK                                                                        (  0x00000020 )
#define DDR_CMN_ZQCAL_CFG_CAL_EN__SHFT                                                                       (  0x00000005 )
#define DDR_CMN_ZQCAL_CFG_PD_SEL__MSK                                                                        (  0x00000040 )
#define DDR_CMN_ZQCAL_CFG_PD_SEL__SHFT                                                                       (  0x00000006 )
#define DDR_CMN_ZQCAL_CFG_VOL_0P6_SEL__MSK                                                                   (  0x00000080 )
#define DDR_CMN_ZQCAL_CFG_VOL_0P6_SEL__SHFT                                                                  (  0x00000007 )
#define DDR_CMN_ZQCAL_CFG_PCAL__MSK                                                                          (  0x00003F00 )
#define DDR_CMN_ZQCAL_CFG_PCAL__SHFT                                                                         (  0x00000008 )
#define DDR_CMN_ZQCAL_CFG__ADR                                                                               (  0x00000010 )
#define DDR_CMN_ZQCAL_CFG__WIDTH                                                                             (  14 )
#define DDR_CMN_ZQCAL_CFG__POR                                                                               (  0x00000000 )
#define DDR_CMN_ZQCAL_CFG__MSK                                                                               (  0x00003FFF )


// Word Address 0x00000014 : DDR_CMN_ZQCAL_STA (R)
#define DDR_CMN_ZQCAL_STA_COMP__MSK                                                                          (  0x00000001 )
#define DDR_CMN_ZQCAL_STA_COMP__SHFT                                                                         (  0x00000000 )
#define DDR_CMN_ZQCAL_STA__ADR                                                                               (  0x00000014 )
#define DDR_CMN_ZQCAL_STA__WIDTH                                                                             (   1 )
#define DDR_CMN_ZQCAL_STA__POR                                                                               (  0x00000000 )
#define DDR_CMN_ZQCAL_STA__MSK                                                                               (  0x00000001 )


// Word Address 0x00000018 : DDR_CMN_IBIAS_CFG (RW)
#define DDR_CMN_IBIAS_CFG_EN__MSK                                                                            (  0x00000001 )
#define DDR_CMN_IBIAS_CFG_EN__SHFT                                                                           (  0x00000000 )
#define DDR_CMN_IBIAS_CFG__ADR                                                                               (  0x00000018 )
#define DDR_CMN_IBIAS_CFG__WIDTH                                                                             (   1 )
#define DDR_CMN_IBIAS_CFG__POR                                                                               (  0x00000000 )
#define DDR_CMN_IBIAS_CFG__MSK                                                                               (  0x00000001 )


// Word Address 0x0000001c : DDR_CMN_TEST_CFG (RW)
#define DDR_CMN_TEST_CFG_ATB_MODE__MSK                                                                       (  0x00000003 )
#define DDR_CMN_TEST_CFG_ATB_MODE__SHFT                                                                      (  0x00000000 )
#define DDR_CMN_TEST_CFG_ATST_SEL__MSK                                                                       (  0x0000003C )
#define DDR_CMN_TEST_CFG_ATST_SEL__SHFT                                                                      (  0x00000002 )
#define DDR_CMN_TEST_CFG_DTST_DRVR_IMPD__MSK                                                                 (  0x00000700 )
#define DDR_CMN_TEST_CFG_DTST_DRVR_IMPD__SHFT                                                                (  0x00000008 )
#define DDR_CMN_TEST_CFG_DTST_EXT_SEL__MSK                                                                   (  0x0001F000 )
#define DDR_CMN_TEST_CFG_DTST_EXT_SEL__SHFT                                                                  (  0x0000000C )
#define DDR_CMN_TEST_CFG_DTST_DIV_EN__MSK                                                                    (  0x00020000 )
#define DDR_CMN_TEST_CFG_DTST_DIV_EN__SHFT                                                                   (  0x00000011 )
#define DDR_CMN_TEST_CFG__ADR                                                                                (  0x0000001c )
#define DDR_CMN_TEST_CFG__WIDTH                                                                              (  18 )
#define DDR_CMN_TEST_CFG__POR                                                                                (  0x00000000 )
#define DDR_CMN_TEST_CFG__MSK                                                                                (  0x0003F73F )


// Word Address 0x00000020 : DDR_CMN_LDO_M0_CFG (RW)
#define DDR_CMN_LDO_M0_CFG_VREF_CTRL__MSK                                                                    (  0x000000FF )
#define DDR_CMN_LDO_M0_CFG_VREF_CTRL__SHFT                                                                   (  0x00000000 )
#define DDR_CMN_LDO_M0_CFG_EN__MSK                                                                           (  0x00000100 )
#define DDR_CMN_LDO_M0_CFG_EN__SHFT                                                                          (  0x00000008 )
#define DDR_CMN_LDO_M0_CFG_TRAN_ENH_EN__MSK                                                                  (  0x00000200 )
#define DDR_CMN_LDO_M0_CFG_TRAN_ENH_EN__SHFT                                                                 (  0x00000009 )
#define DDR_CMN_LDO_M0_CFG_ATST_SEL__MSK                                                                     (  0x00001C00 )
#define DDR_CMN_LDO_M0_CFG_ATST_SEL__SHFT                                                                    (  0x0000000A )
#define DDR_CMN_LDO_M0_CFG_HIZ__MSK                                                                          (  0x00002000 )
#define DDR_CMN_LDO_M0_CFG_HIZ__SHFT                                                                         (  0x0000000D )
#define DDR_CMN_LDO_M0_CFG__ADR                                                                              (  0x00000020 )
#define DDR_CMN_LDO_M0_CFG__WIDTH                                                                            (  14 )
#define DDR_CMN_LDO_M0_CFG__POR                                                                              (  0x00000000 )
#define DDR_CMN_LDO_M0_CFG__MSK                                                                              (  0x00003FFF )


// Word Address 0x00000024 : DDR_CMN_LDO_M1_CFG (RW)
#define DDR_CMN_LDO_M1_CFG_VREF_CTRL__MSK                                                                    (  0x000000FF )
#define DDR_CMN_LDO_M1_CFG_VREF_CTRL__SHFT                                                                   (  0x00000000 )
#define DDR_CMN_LDO_M1_CFG_EN__MSK                                                                           (  0x00000100 )
#define DDR_CMN_LDO_M1_CFG_EN__SHFT                                                                          (  0x00000008 )
#define DDR_CMN_LDO_M1_CFG_TRAN_ENH_EN__MSK                                                                  (  0x00000200 )
#define DDR_CMN_LDO_M1_CFG_TRAN_ENH_EN__SHFT                                                                 (  0x00000009 )
#define DDR_CMN_LDO_M1_CFG_ATST_SEL__MSK                                                                     (  0x00001C00 )
#define DDR_CMN_LDO_M1_CFG_ATST_SEL__SHFT                                                                    (  0x0000000A )
#define DDR_CMN_LDO_M1_CFG_HIZ__MSK                                                                          (  0x00002000 )
#define DDR_CMN_LDO_M1_CFG_HIZ__SHFT                                                                         (  0x0000000D )
#define DDR_CMN_LDO_M1_CFG__ADR                                                                              (  0x00000024 )
#define DDR_CMN_LDO_M1_CFG__WIDTH                                                                            (  14 )
#define DDR_CMN_LDO_M1_CFG__POR                                                                              (  0x00000000 )
#define DDR_CMN_LDO_M1_CFG__MSK                                                                              (  0x00003FFF )


// Word Address 0x00000028 : DDR_CMN_CLK_CTRL_CFG (RW)
#define DDR_CMN_CLK_CTRL_CFG_PLL0_DIV_CLK_BYP__MSK                                                           (  0x00000001 )
#define DDR_CMN_CLK_CTRL_CFG_PLL0_DIV_CLK_BYP__SHFT                                                          (  0x00000000 )
#define DDR_CMN_CLK_CTRL_CFG_PLL0_DIV_CLK_RST__MSK                                                           (  0x00000002 )
#define DDR_CMN_CLK_CTRL_CFG_PLL0_DIV_CLK_RST__SHFT                                                          (  0x00000001 )
#define DDR_CMN_CLK_CTRL_CFG_PLL0_DIV_CLK_EN__MSK                                                            (  0x00000004 )
#define DDR_CMN_CLK_CTRL_CFG_PLL0_DIV_CLK_EN__SHFT                                                           (  0x00000002 )
#define DDR_CMN_CLK_CTRL_CFG_GFCM_EN__MSK                                                                    (  0x00000008 )
#define DDR_CMN_CLK_CTRL_CFG_GFCM_EN__SHFT                                                                   (  0x00000003 )
#define DDR_CMN_CLK_CTRL_CFG__ADR                                                                            (  0x00000028 )
#define DDR_CMN_CLK_CTRL_CFG__WIDTH                                                                          (   4 )
#define DDR_CMN_CLK_CTRL_CFG__POR                                                                            (  0x00000002 )
#define DDR_CMN_CLK_CTRL_CFG__MSK                                                                            (  0x0000000F )


// Word Address 0x00000038 : DDR_CMN_PMON_ANA_CFG (RW)
#define DDR_CMN_PMON_ANA_CFG_NAND_EN__MSK                                                                    (  0x00000001 )
#define DDR_CMN_PMON_ANA_CFG_NAND_EN__SHFT                                                                   (  0x00000000 )
#define DDR_CMN_PMON_ANA_CFG_NOR_EN__MSK                                                                     (  0x00000002 )
#define DDR_CMN_PMON_ANA_CFG_NOR_EN__SHFT                                                                    (  0x00000001 )
#define DDR_CMN_PMON_ANA_CFG__ADR                                                                            (  0x00000038 )
#define DDR_CMN_PMON_ANA_CFG__WIDTH                                                                          (   2 )
#define DDR_CMN_PMON_ANA_CFG__POR                                                                            (  0x00000000 )
#define DDR_CMN_PMON_ANA_CFG__MSK                                                                            (  0x00000003 )


// Word Address 0x0000003c : DDR_CMN_PMON_DIG_CFG (RW)
#define DDR_CMN_PMON_DIG_CFG_INITWAIT__MSK                                                                   (  0x000000FF )
#define DDR_CMN_PMON_DIG_CFG_INITWAIT__SHFT                                                                  (  0x00000000 )
#define DDR_CMN_PMON_DIG_CFG_REFCLK_RST__MSK                                                                 (  0x00000100 )
#define DDR_CMN_PMON_DIG_CFG_REFCLK_RST__SHFT                                                                (  0x00000008 )
#define DDR_CMN_PMON_DIG_CFG__ADR                                                                            (  0x0000003c )
#define DDR_CMN_PMON_DIG_CFG__WIDTH                                                                          (   9 )
#define DDR_CMN_PMON_DIG_CFG__POR                                                                            (  0x00000100 )
#define DDR_CMN_PMON_DIG_CFG__MSK                                                                            (  0x000001FF )


// Word Address 0x00000040 : DDR_CMN_PMON_DIG_NAND_CFG (RW)
#define DDR_CMN_PMON_DIG_NAND_CFG_REFCOUNT__MSK                                                              (  0x00000FFF )
#define DDR_CMN_PMON_DIG_NAND_CFG_REFCOUNT__SHFT                                                             (  0x00000000 )
#define DDR_CMN_PMON_DIG_NAND_CFG_COUNT_EN__MSK                                                              (  0x00001000 )
#define DDR_CMN_PMON_DIG_NAND_CFG_COUNT_EN__SHFT                                                             (  0x0000000C )
#define DDR_CMN_PMON_DIG_NAND_CFG__ADR                                                                       (  0x00000040 )
#define DDR_CMN_PMON_DIG_NAND_CFG__WIDTH                                                                     (  13 )
#define DDR_CMN_PMON_DIG_NAND_CFG__POR                                                                       (  0x00000000 )
#define DDR_CMN_PMON_DIG_NAND_CFG__MSK                                                                       (  0x00001FFF )


// Word Address 0x00000044 : DDR_CMN_PMON_DIG_NOR_CFG (RW)
#define DDR_CMN_PMON_DIG_NOR_CFG_REFCOUNT__MSK                                                               (  0x00000FFF )
#define DDR_CMN_PMON_DIG_NOR_CFG_REFCOUNT__SHFT                                                              (  0x00000000 )
#define DDR_CMN_PMON_DIG_NOR_CFG_COUNT_EN__MSK                                                               (  0x00001000 )
#define DDR_CMN_PMON_DIG_NOR_CFG_COUNT_EN__SHFT                                                              (  0x0000000C )
#define DDR_CMN_PMON_DIG_NOR_CFG__ADR                                                                        (  0x00000044 )
#define DDR_CMN_PMON_DIG_NOR_CFG__WIDTH                                                                      (  13 )
#define DDR_CMN_PMON_DIG_NOR_CFG__POR                                                                        (  0x00000000 )
#define DDR_CMN_PMON_DIG_NOR_CFG__MSK                                                                        (  0x00001FFF )


// Word Address 0x00000048 : DDR_CMN_PMON_NAND_STA (R)
#define DDR_CMN_PMON_NAND_STA_COUNT__MSK                                                                     (  0x00FFFFFF )
#define DDR_CMN_PMON_NAND_STA_COUNT__SHFT                                                                    (  0x00000000 )
#define DDR_CMN_PMON_NAND_STA_DONE__MSK                                                                      (  0x01000000 )
#define DDR_CMN_PMON_NAND_STA_DONE__SHFT                                                                     (  0x00000018 )
#define DDR_CMN_PMON_NAND_STA__ADR                                                                           (  0x00000048 )
#define DDR_CMN_PMON_NAND_STA__WIDTH                                                                         (  25 )
#define DDR_CMN_PMON_NAND_STA__POR                                                                           (  0x00000000 )
#define DDR_CMN_PMON_NAND_STA__MSK                                                                           (  0x01FFFFFF )


// Word Address 0x0000004c : DDR_CMN_PMON_NOR_STA (R)
#define DDR_CMN_PMON_NOR_STA_COUNT__MSK                                                                      (  0x00FFFFFF )
#define DDR_CMN_PMON_NOR_STA_COUNT__SHFT                                                                     (  0x00000000 )
#define DDR_CMN_PMON_NOR_STA_DONE__MSK                                                                       (  0x01000000 )
#define DDR_CMN_PMON_NOR_STA_DONE__SHFT                                                                      (  0x00000018 )
#define DDR_CMN_PMON_NOR_STA__ADR                                                                            (  0x0000004c )
#define DDR_CMN_PMON_NOR_STA__WIDTH                                                                          (  25 )
#define DDR_CMN_PMON_NOR_STA__POR                                                                            (  0x00000000 )
#define DDR_CMN_PMON_NOR_STA__MSK                                                                            (  0x01FFFFFF )


// Word Address 0x00000050 : DDR_CMN_CLK_STA (R)
#define DDR_CMN_CLK_STA_GFCM0_CLKA_SEL__MSK                                                                  (  0x00000001 )
#define DDR_CMN_CLK_STA_GFCM0_CLKA_SEL__SHFT                                                                 (  0x00000000 )
#define DDR_CMN_CLK_STA_GFCM0_CLKB_SEL__MSK                                                                  (  0x00000002 )
#define DDR_CMN_CLK_STA_GFCM0_CLKB_SEL__SHFT                                                                 (  0x00000001 )
#define DDR_CMN_CLK_STA_GFCM1_CLKA_SEL__MSK                                                                  (  0x00000004 )
#define DDR_CMN_CLK_STA_GFCM1_CLKA_SEL__SHFT                                                                 (  0x00000002 )
#define DDR_CMN_CLK_STA_GFCM1_CLKB_SEL__MSK                                                                  (  0x00000008 )
#define DDR_CMN_CLK_STA_GFCM1_CLKB_SEL__SHFT                                                                 (  0x00000003 )
#define DDR_CMN_CLK_STA__ADR                                                                                 (  0x00000050 )
#define DDR_CMN_CLK_STA__WIDTH                                                                               (   4 )
#define DDR_CMN_CLK_STA__POR                                                                                 (  0x00000000 )
#define DDR_CMN_CLK_STA__MSK                                                                                 (  0x0000000F )


// Word Address 0x00000054 : DDR_CMN_RSTN_CFG (RW)
#define DDR_CMN_RSTN_CFG_RSTN_OVR_SEL__MSK                                                                   (  0x00000001 )
#define DDR_CMN_RSTN_CFG_RSTN_OVR_SEL__SHFT                                                                  (  0x00000000 )
#define DDR_CMN_RSTN_CFG_RSTN_OVR_VAL__MSK                                                                   (  0x00000002 )
#define DDR_CMN_RSTN_CFG_RSTN_OVR_VAL__SHFT                                                                  (  0x00000001 )
#define DDR_CMN_RSTN_CFG_RSTN_LPBK_ENA__MSK                                                                  (  0x00000004 )
#define DDR_CMN_RSTN_CFG_RSTN_LPBK_ENA__SHFT                                                                 (  0x00000002 )
#define DDR_CMN_RSTN_CFG_RSTN_BS_DIN__MSK                                                                    (  0x00000008 )
#define DDR_CMN_RSTN_CFG_RSTN_BS_DIN__SHFT                                                                   (  0x00000003 )
#define DDR_CMN_RSTN_CFG_RSTN_BS_ENA__MSK                                                                    (  0x00000010 )
#define DDR_CMN_RSTN_CFG_RSTN_BS_ENA__SHFT                                                                   (  0x00000004 )
#define DDR_CMN_RSTN_CFG__ADR                                                                                (  0x00000054 )
#define DDR_CMN_RSTN_CFG__WIDTH                                                                              (   5 )
#define DDR_CMN_RSTN_CFG__POR                                                                                (  0x00000000 )
#define DDR_CMN_RSTN_CFG__MSK                                                                                (  0x0000001F )


// Word Address 0x00000058 : DDR_CMN_RSTN_STA (R)
#define DDR_CMN_RSTN_STA_RSTN_LPBK__MSK                                                                      (  0x00000001 )
#define DDR_CMN_RSTN_STA_RSTN_LPBK__SHFT                                                                     (  0x00000000 )
#define DDR_CMN_RSTN_STA__ADR                                                                                (  0x00000058 )
#define DDR_CMN_RSTN_STA__WIDTH                                                                              (   1 )
#define DDR_CMN_RSTN_STA__POR                                                                                (  0x00000000 )
#define DDR_CMN_RSTN_STA__MSK                                                                                (  0x00000001 )


#endif /* _DDR_CMN_CSR_H_ */
