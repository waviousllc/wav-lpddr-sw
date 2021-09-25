/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
// MODULE DFI
// RANGE 0x03FF:0x0000
// WIDTH 32

#ifndef _DDR_DFI_CSR_H_
#define _DDR_DFI_CSR_H_

// Word Address 0x00000000 : DDR_DFI_TOP_0_CFG (RW)
#define DDR_DFI_TOP_0_CFG_CA_LPBK_SEL__MSK                                                                   (  0x00000001 )
#define DDR_DFI_TOP_0_CFG_CA_LPBK_SEL__SHFT                                                                  (  0x00000000 )
#define DDR_DFI_TOP_0_CFG__ADR                                                                               (  0x00000000 )
#define DDR_DFI_TOP_0_CFG__WIDTH                                                                             (   1 )
#define DDR_DFI_TOP_0_CFG__POR                                                                               (  0x00000000 )
#define DDR_DFI_TOP_0_CFG__MSK                                                                               (  0x00000001 )


// Word Address 0x00000004 : DDR_DFI_DATA_BIT_ENABLE_CFG (RW)
#define DDR_DFI_DATA_BIT_ENABLE_CFG_VAL__MSK                                                                 (  0xFFFFFFFF )
#define DDR_DFI_DATA_BIT_ENABLE_CFG_VAL__SHFT                                                                (  0x00000000 )
#define DDR_DFI_DATA_BIT_ENABLE_CFG__ADR                                                                     (  0x00000004 )
#define DDR_DFI_DATA_BIT_ENABLE_CFG__WIDTH                                                                   (  32 )
#define DDR_DFI_DATA_BIT_ENABLE_CFG__POR                                                                     (  0x000001FF )
#define DDR_DFI_DATA_BIT_ENABLE_CFG__MSK                                                                     (  0xFFFFFFFF )


// Word Address 0x00000008 : DDR_DFI_PHYFREQ_RANGE_CFG (RW)
#define DDR_DFI_PHYFREQ_RANGE_CFG_VAL__MSK                                                                   (  0xFFFFFFFF )
#define DDR_DFI_PHYFREQ_RANGE_CFG_VAL__SHFT                                                                  (  0x00000000 )
#define DDR_DFI_PHYFREQ_RANGE_CFG__ADR                                                                       (  0x00000008 )
#define DDR_DFI_PHYFREQ_RANGE_CFG__WIDTH                                                                     (  32 )
#define DDR_DFI_PHYFREQ_RANGE_CFG__POR                                                                       (  0x0000EFB5 )
#define DDR_DFI_PHYFREQ_RANGE_CFG__MSK                                                                       (  0xFFFFFFFF )


// Word Address 0x0000000c : DDR_DFI_STATUS_IF_CFG (RW)
#define DDR_DFI_STATUS_IF_CFG_SW_REQ_OVR__MSK                                                                (  0x00000001 )
#define DDR_DFI_STATUS_IF_CFG_SW_REQ_OVR__SHFT                                                               (  0x00000000 )
#define DDR_DFI_STATUS_IF_CFG_SW_REQ_VAL__MSK                                                                (  0x00000002 )
#define DDR_DFI_STATUS_IF_CFG_SW_REQ_VAL__SHFT                                                               (  0x00000001 )
#define DDR_DFI_STATUS_IF_CFG_SW_ACK_OVR__MSK                                                                (  0x00000004 )
#define DDR_DFI_STATUS_IF_CFG_SW_ACK_OVR__SHFT                                                               (  0x00000002 )
#define DDR_DFI_STATUS_IF_CFG_SW_ACK_VAL__MSK                                                                (  0x00000008 )
#define DDR_DFI_STATUS_IF_CFG_SW_ACK_VAL__SHFT                                                               (  0x00000003 )
#define DDR_DFI_STATUS_IF_CFG_SW_EVENT_0_OVR__MSK                                                            (  0x00000010 )
#define DDR_DFI_STATUS_IF_CFG_SW_EVENT_0_OVR__SHFT                                                           (  0x00000004 )
#define DDR_DFI_STATUS_IF_CFG_SW_EVENT_0_VAL__MSK                                                            (  0x00000020 )
#define DDR_DFI_STATUS_IF_CFG_SW_EVENT_0_VAL__SHFT                                                           (  0x00000005 )
#define DDR_DFI_STATUS_IF_CFG_SW_EVENT_1_OVR__MSK                                                            (  0x00000040 )
#define DDR_DFI_STATUS_IF_CFG_SW_EVENT_1_OVR__SHFT                                                           (  0x00000006 )
#define DDR_DFI_STATUS_IF_CFG_SW_EVENT_1_VAL__MSK                                                            (  0x00000080 )
#define DDR_DFI_STATUS_IF_CFG_SW_EVENT_1_VAL__SHFT                                                           (  0x00000007 )
#define DDR_DFI_STATUS_IF_CFG_SW_FREQ_OVR__MSK                                                               (  0x00000100 )
#define DDR_DFI_STATUS_IF_CFG_SW_FREQ_OVR__SHFT                                                              (  0x00000008 )
#define DDR_DFI_STATUS_IF_CFG_SW_FREQ_FSP_VAL__MSK                                                           (  0x00003000 )
#define DDR_DFI_STATUS_IF_CFG_SW_FREQ_FSP_VAL__SHFT                                                          (  0x0000000C )
#define DDR_DFI_STATUS_IF_CFG_SW_FREQ_RATIO_VAL__MSK                                                         (  0x0000C000 )
#define DDR_DFI_STATUS_IF_CFG_SW_FREQ_RATIO_VAL__SHFT                                                        (  0x0000000E )
#define DDR_DFI_STATUS_IF_CFG_SW_FREQUENCY_VAL__MSK                                                          (  0x001F0000 )
#define DDR_DFI_STATUS_IF_CFG_SW_FREQUENCY_VAL__SHFT                                                         (  0x00000010 )
#define DDR_DFI_STATUS_IF_CFG__ADR                                                                           (  0x0000000c )
#define DDR_DFI_STATUS_IF_CFG__WIDTH                                                                         (  21 )
#define DDR_DFI_STATUS_IF_CFG__POR                                                                           (  0x0000000D )
#define DDR_DFI_STATUS_IF_CFG__MSK                                                                           (  0x001FF1FF )


// Word Address 0x00000010 : DDR_DFI_STATUS_IF_STA (R)
#define DDR_DFI_STATUS_IF_STA_REQ__MSK                                                                       (  0x00000001 )
#define DDR_DFI_STATUS_IF_STA_REQ__SHFT                                                                      (  0x00000000 )
#define DDR_DFI_STATUS_IF_STA_ACK__MSK                                                                       (  0x00000002 )
#define DDR_DFI_STATUS_IF_STA_ACK__SHFT                                                                      (  0x00000001 )
#define DDR_DFI_STATUS_IF_STA_FREQ_FSP__MSK                                                                  (  0x00000030 )
#define DDR_DFI_STATUS_IF_STA_FREQ_FSP__SHFT                                                                 (  0x00000004 )
#define DDR_DFI_STATUS_IF_STA_FREQ_RATIO__MSK                                                                (  0x000000C0 )
#define DDR_DFI_STATUS_IF_STA_FREQ_RATIO__SHFT                                                               (  0x00000006 )
#define DDR_DFI_STATUS_IF_STA_FREQUENCY__MSK                                                                 (  0x00001F00 )
#define DDR_DFI_STATUS_IF_STA_FREQUENCY__SHFT                                                                (  0x00000008 )
#define DDR_DFI_STATUS_IF_STA__ADR                                                                           (  0x00000010 )
#define DDR_DFI_STATUS_IF_STA__WIDTH                                                                         (  13 )
#define DDR_DFI_STATUS_IF_STA__POR                                                                           (  0x00000000 )
#define DDR_DFI_STATUS_IF_STA__MSK                                                                           (  0x00001FF3 )


// Word Address 0x00000014 : DDR_DFI_STATUS_IF_EVENT_0_CFG (RW)
#define DDR_DFI_STATUS_IF_EVENT_0_CFG_SW_EVENT_CNT__MSK                                                      (  0x000FFFFF )
#define DDR_DFI_STATUS_IF_EVENT_0_CFG_SW_EVENT_CNT__SHFT                                                     (  0x00000000 )
#define DDR_DFI_STATUS_IF_EVENT_0_CFG_SW_EVENT_CNT_SEL__MSK                                                  (  0x80000000 )
#define DDR_DFI_STATUS_IF_EVENT_0_CFG_SW_EVENT_CNT_SEL__SHFT                                                 (  0x0000001F )
#define DDR_DFI_STATUS_IF_EVENT_0_CFG__ADR                                                                   (  0x00000014 )
#define DDR_DFI_STATUS_IF_EVENT_0_CFG__WIDTH                                                                 (  32 )
#define DDR_DFI_STATUS_IF_EVENT_0_CFG__POR                                                                   (  0x00000000 )
#define DDR_DFI_STATUS_IF_EVENT_0_CFG__MSK                                                                   (  0x800FFFFF )


// Word Address 0x00000018 : DDR_DFI_STATUS_IF_EVENT_1_CFG (RW)
#define DDR_DFI_STATUS_IF_EVENT_1_CFG_SW_EVENT_CNT__MSK                                                      (  0x000FFFFF )
#define DDR_DFI_STATUS_IF_EVENT_1_CFG_SW_EVENT_CNT__SHFT                                                     (  0x00000000 )
#define DDR_DFI_STATUS_IF_EVENT_1_CFG_SW_EVENT_CNT_SEL__MSK                                                  (  0x80000000 )
#define DDR_DFI_STATUS_IF_EVENT_1_CFG_SW_EVENT_CNT_SEL__SHFT                                                 (  0x0000001F )
#define DDR_DFI_STATUS_IF_EVENT_1_CFG__ADR                                                                   (  0x00000018 )
#define DDR_DFI_STATUS_IF_EVENT_1_CFG__WIDTH                                                                 (  32 )
#define DDR_DFI_STATUS_IF_EVENT_1_CFG__POR                                                                   (  0x00000000 )
#define DDR_DFI_STATUS_IF_EVENT_1_CFG__MSK                                                                   (  0x800FFFFF )


// Word Address 0x0000001c : DDR_DFI_CTRLUPD_IF_CFG (RW)
#define DDR_DFI_CTRLUPD_IF_CFG_SW_REQ_OVR__MSK                                                               (  0x00000001 )
#define DDR_DFI_CTRLUPD_IF_CFG_SW_REQ_OVR__SHFT                                                              (  0x00000000 )
#define DDR_DFI_CTRLUPD_IF_CFG_SW_REQ_VAL__MSK                                                               (  0x00000002 )
#define DDR_DFI_CTRLUPD_IF_CFG_SW_REQ_VAL__SHFT                                                              (  0x00000001 )
#define DDR_DFI_CTRLUPD_IF_CFG_SW_ACK_OVR__MSK                                                               (  0x00000004 )
#define DDR_DFI_CTRLUPD_IF_CFG_SW_ACK_OVR__SHFT                                                              (  0x00000002 )
#define DDR_DFI_CTRLUPD_IF_CFG_SW_ACK_VAL__MSK                                                               (  0x00000008 )
#define DDR_DFI_CTRLUPD_IF_CFG_SW_ACK_VAL__SHFT                                                              (  0x00000003 )
#define DDR_DFI_CTRLUPD_IF_CFG_SW_EVENT_0_OVR__MSK                                                           (  0x00000010 )
#define DDR_DFI_CTRLUPD_IF_CFG_SW_EVENT_0_OVR__SHFT                                                          (  0x00000004 )
#define DDR_DFI_CTRLUPD_IF_CFG_SW_EVENT_0_VAL__MSK                                                           (  0x00000020 )
#define DDR_DFI_CTRLUPD_IF_CFG_SW_EVENT_0_VAL__SHFT                                                          (  0x00000005 )
#define DDR_DFI_CTRLUPD_IF_CFG_SW_EVENT_1_OVR__MSK                                                           (  0x00000040 )
#define DDR_DFI_CTRLUPD_IF_CFG_SW_EVENT_1_OVR__SHFT                                                          (  0x00000006 )
#define DDR_DFI_CTRLUPD_IF_CFG_SW_EVENT_1_VAL__MSK                                                           (  0x00000080 )
#define DDR_DFI_CTRLUPD_IF_CFG_SW_EVENT_1_VAL__SHFT                                                          (  0x00000007 )
#define DDR_DFI_CTRLUPD_IF_CFG__ADR                                                                          (  0x0000001c )
#define DDR_DFI_CTRLUPD_IF_CFG__WIDTH                                                                        (   8 )
#define DDR_DFI_CTRLUPD_IF_CFG__POR                                                                          (  0x00000000 )
#define DDR_DFI_CTRLUPD_IF_CFG__MSK                                                                          (  0x000000FF )


// Word Address 0x00000020 : DDR_DFI_CTRLUPD_IF_STA (R)
#define DDR_DFI_CTRLUPD_IF_STA_REQ__MSK                                                                      (  0x00000001 )
#define DDR_DFI_CTRLUPD_IF_STA_REQ__SHFT                                                                     (  0x00000000 )
#define DDR_DFI_CTRLUPD_IF_STA_ACK__MSK                                                                      (  0x00000002 )
#define DDR_DFI_CTRLUPD_IF_STA_ACK__SHFT                                                                     (  0x00000001 )
#define DDR_DFI_CTRLUPD_IF_STA__ADR                                                                          (  0x00000020 )
#define DDR_DFI_CTRLUPD_IF_STA__WIDTH                                                                        (   2 )
#define DDR_DFI_CTRLUPD_IF_STA__POR                                                                          (  0x00000000 )
#define DDR_DFI_CTRLUPD_IF_STA__MSK                                                                          (  0x00000003 )


// Word Address 0x00000024 : DDR_DFI_CTRLUPD_IF_EVENT_0_CFG (RW)
#define DDR_DFI_CTRLUPD_IF_EVENT_0_CFG_SW_EVENT_CNT__MSK                                                     (  0x000FFFFF )
#define DDR_DFI_CTRLUPD_IF_EVENT_0_CFG_SW_EVENT_CNT__SHFT                                                    (  0x00000000 )
#define DDR_DFI_CTRLUPD_IF_EVENT_0_CFG_SW_EVENT_CNT_SEL__MSK                                                 (  0x80000000 )
#define DDR_DFI_CTRLUPD_IF_EVENT_0_CFG_SW_EVENT_CNT_SEL__SHFT                                                (  0x0000001F )
#define DDR_DFI_CTRLUPD_IF_EVENT_0_CFG__ADR                                                                  (  0x00000024 )
#define DDR_DFI_CTRLUPD_IF_EVENT_0_CFG__WIDTH                                                                (  32 )
#define DDR_DFI_CTRLUPD_IF_EVENT_0_CFG__POR                                                                  (  0x00000000 )
#define DDR_DFI_CTRLUPD_IF_EVENT_0_CFG__MSK                                                                  (  0x800FFFFF )


// Word Address 0x00000028 : DDR_DFI_CTRLUPD_IF_EVENT_1_CFG (RW)
#define DDR_DFI_CTRLUPD_IF_EVENT_1_CFG_SW_EVENT_CNT__MSK                                                     (  0x000FFFFF )
#define DDR_DFI_CTRLUPD_IF_EVENT_1_CFG_SW_EVENT_CNT__SHFT                                                    (  0x00000000 )
#define DDR_DFI_CTRLUPD_IF_EVENT_1_CFG_SW_EVENT_CNT_SEL__MSK                                                 (  0x80000000 )
#define DDR_DFI_CTRLUPD_IF_EVENT_1_CFG_SW_EVENT_CNT_SEL__SHFT                                                (  0x0000001F )
#define DDR_DFI_CTRLUPD_IF_EVENT_1_CFG__ADR                                                                  (  0x00000028 )
#define DDR_DFI_CTRLUPD_IF_EVENT_1_CFG__WIDTH                                                                (  32 )
#define DDR_DFI_CTRLUPD_IF_EVENT_1_CFG__POR                                                                  (  0x00000000 )
#define DDR_DFI_CTRLUPD_IF_EVENT_1_CFG__MSK                                                                  (  0x800FFFFF )


// Word Address 0x0000002c : DDR_DFI_LP_CTRL_IF_CFG (RW)
#define DDR_DFI_LP_CTRL_IF_CFG_SW_REQ_OVR__MSK                                                               (  0x00000001 )
#define DDR_DFI_LP_CTRL_IF_CFG_SW_REQ_OVR__SHFT                                                              (  0x00000000 )
#define DDR_DFI_LP_CTRL_IF_CFG_SW_REQ_VAL__MSK                                                               (  0x00000002 )
#define DDR_DFI_LP_CTRL_IF_CFG_SW_REQ_VAL__SHFT                                                              (  0x00000001 )
#define DDR_DFI_LP_CTRL_IF_CFG_SW_ACK_OVR__MSK                                                               (  0x00000004 )
#define DDR_DFI_LP_CTRL_IF_CFG_SW_ACK_OVR__SHFT                                                              (  0x00000002 )
#define DDR_DFI_LP_CTRL_IF_CFG_SW_ACK_VAL__MSK                                                               (  0x00000008 )
#define DDR_DFI_LP_CTRL_IF_CFG_SW_ACK_VAL__SHFT                                                              (  0x00000003 )
#define DDR_DFI_LP_CTRL_IF_CFG_SW_EVENT_0_OVR__MSK                                                           (  0x00000010 )
#define DDR_DFI_LP_CTRL_IF_CFG_SW_EVENT_0_OVR__SHFT                                                          (  0x00000004 )
#define DDR_DFI_LP_CTRL_IF_CFG_SW_EVENT_0_VAL__MSK                                                           (  0x00000020 )
#define DDR_DFI_LP_CTRL_IF_CFG_SW_EVENT_0_VAL__SHFT                                                          (  0x00000005 )
#define DDR_DFI_LP_CTRL_IF_CFG_SW_EVENT_1_OVR__MSK                                                           (  0x00000040 )
#define DDR_DFI_LP_CTRL_IF_CFG_SW_EVENT_1_OVR__SHFT                                                          (  0x00000006 )
#define DDR_DFI_LP_CTRL_IF_CFG_SW_EVENT_1_VAL__MSK                                                           (  0x00000080 )
#define DDR_DFI_LP_CTRL_IF_CFG_SW_EVENT_1_VAL__SHFT                                                          (  0x00000007 )
#define DDR_DFI_LP_CTRL_IF_CFG__ADR                                                                          (  0x0000002c )
#define DDR_DFI_LP_CTRL_IF_CFG__WIDTH                                                                        (   8 )
#define DDR_DFI_LP_CTRL_IF_CFG__POR                                                                          (  0x00000000 )
#define DDR_DFI_LP_CTRL_IF_CFG__MSK                                                                          (  0x000000FF )


// Word Address 0x00000030 : DDR_DFI_LP_CTRL_IF_STA (R)
#define DDR_DFI_LP_CTRL_IF_STA_REQ__MSK                                                                      (  0x00000001 )
#define DDR_DFI_LP_CTRL_IF_STA_REQ__SHFT                                                                     (  0x00000000 )
#define DDR_DFI_LP_CTRL_IF_STA_ACK__MSK                                                                      (  0x00000002 )
#define DDR_DFI_LP_CTRL_IF_STA_ACK__SHFT                                                                     (  0x00000001 )
#define DDR_DFI_LP_CTRL_IF_STA_WAKEUP__MSK                                                                   (  0x000003F0 )
#define DDR_DFI_LP_CTRL_IF_STA_WAKEUP__SHFT                                                                  (  0x00000004 )
#define DDR_DFI_LP_CTRL_IF_STA__ADR                                                                          (  0x00000030 )
#define DDR_DFI_LP_CTRL_IF_STA__WIDTH                                                                        (  10 )
#define DDR_DFI_LP_CTRL_IF_STA__POR                                                                          (  0x00000000 )
#define DDR_DFI_LP_CTRL_IF_STA__MSK                                                                          (  0x000003F3 )


// Word Address 0x00000034 : DDR_DFI_LP_CTRL_IF_EVENT_0_CFG (RW)
#define DDR_DFI_LP_CTRL_IF_EVENT_0_CFG_SW_EVENT_CNT__MSK                                                     (  0x000FFFFF )
#define DDR_DFI_LP_CTRL_IF_EVENT_0_CFG_SW_EVENT_CNT__SHFT                                                    (  0x00000000 )
#define DDR_DFI_LP_CTRL_IF_EVENT_0_CFG_SW_EVENT_CNT_SEL__MSK                                                 (  0x80000000 )
#define DDR_DFI_LP_CTRL_IF_EVENT_0_CFG_SW_EVENT_CNT_SEL__SHFT                                                (  0x0000001F )
#define DDR_DFI_LP_CTRL_IF_EVENT_0_CFG__ADR                                                                  (  0x00000034 )
#define DDR_DFI_LP_CTRL_IF_EVENT_0_CFG__WIDTH                                                                (  32 )
#define DDR_DFI_LP_CTRL_IF_EVENT_0_CFG__POR                                                                  (  0x00000000 )
#define DDR_DFI_LP_CTRL_IF_EVENT_0_CFG__MSK                                                                  (  0x800FFFFF )


// Word Address 0x00000038 : DDR_DFI_LP_CTRL_IF_EVENT_1_CFG (RW)
#define DDR_DFI_LP_CTRL_IF_EVENT_1_CFG_SW_EVENT_CNT__MSK                                                     (  0x000FFFFF )
#define DDR_DFI_LP_CTRL_IF_EVENT_1_CFG_SW_EVENT_CNT__SHFT                                                    (  0x00000000 )
#define DDR_DFI_LP_CTRL_IF_EVENT_1_CFG_SW_EVENT_CNT_SEL__MSK                                                 (  0x80000000 )
#define DDR_DFI_LP_CTRL_IF_EVENT_1_CFG_SW_EVENT_CNT_SEL__SHFT                                                (  0x0000001F )
#define DDR_DFI_LP_CTRL_IF_EVENT_1_CFG__ADR                                                                  (  0x00000038 )
#define DDR_DFI_LP_CTRL_IF_EVENT_1_CFG__WIDTH                                                                (  32 )
#define DDR_DFI_LP_CTRL_IF_EVENT_1_CFG__POR                                                                  (  0x00000000 )
#define DDR_DFI_LP_CTRL_IF_EVENT_1_CFG__MSK                                                                  (  0x800FFFFF )


// Word Address 0x0000003c : DDR_DFI_LP_DATA_IF_CFG (RW)
#define DDR_DFI_LP_DATA_IF_CFG_SW_REQ_OVR__MSK                                                               (  0x00000001 )
#define DDR_DFI_LP_DATA_IF_CFG_SW_REQ_OVR__SHFT                                                              (  0x00000000 )
#define DDR_DFI_LP_DATA_IF_CFG_SW_REQ_VAL__MSK                                                               (  0x00000002 )
#define DDR_DFI_LP_DATA_IF_CFG_SW_REQ_VAL__SHFT                                                              (  0x00000001 )
#define DDR_DFI_LP_DATA_IF_CFG_SW_ACK_OVR__MSK                                                               (  0x00000004 )
#define DDR_DFI_LP_DATA_IF_CFG_SW_ACK_OVR__SHFT                                                              (  0x00000002 )
#define DDR_DFI_LP_DATA_IF_CFG_SW_ACK_VAL__MSK                                                               (  0x00000008 )
#define DDR_DFI_LP_DATA_IF_CFG_SW_ACK_VAL__SHFT                                                              (  0x00000003 )
#define DDR_DFI_LP_DATA_IF_CFG_SW_EVENT_0_OVR__MSK                                                           (  0x00000010 )
#define DDR_DFI_LP_DATA_IF_CFG_SW_EVENT_0_OVR__SHFT                                                          (  0x00000004 )
#define DDR_DFI_LP_DATA_IF_CFG_SW_EVENT_0_VAL__MSK                                                           (  0x00000020 )
#define DDR_DFI_LP_DATA_IF_CFG_SW_EVENT_0_VAL__SHFT                                                          (  0x00000005 )
#define DDR_DFI_LP_DATA_IF_CFG_SW_EVENT_1_OVR__MSK                                                           (  0x00000040 )
#define DDR_DFI_LP_DATA_IF_CFG_SW_EVENT_1_OVR__SHFT                                                          (  0x00000006 )
#define DDR_DFI_LP_DATA_IF_CFG_SW_EVENT_1_VAL__MSK                                                           (  0x00000080 )
#define DDR_DFI_LP_DATA_IF_CFG_SW_EVENT_1_VAL__SHFT                                                          (  0x00000007 )
#define DDR_DFI_LP_DATA_IF_CFG__ADR                                                                          (  0x0000003c )
#define DDR_DFI_LP_DATA_IF_CFG__WIDTH                                                                        (   8 )
#define DDR_DFI_LP_DATA_IF_CFG__POR                                                                          (  0x00000000 )
#define DDR_DFI_LP_DATA_IF_CFG__MSK                                                                          (  0x000000FF )


// Word Address 0x00000040 : DDR_DFI_LP_DATA_IF_STA (R)
#define DDR_DFI_LP_DATA_IF_STA_REQ__MSK                                                                      (  0x00000001 )
#define DDR_DFI_LP_DATA_IF_STA_REQ__SHFT                                                                     (  0x00000000 )
#define DDR_DFI_LP_DATA_IF_STA_ACK__MSK                                                                      (  0x00000002 )
#define DDR_DFI_LP_DATA_IF_STA_ACK__SHFT                                                                     (  0x00000001 )
#define DDR_DFI_LP_DATA_IF_STA_WAKEUP__MSK                                                                   (  0x000003F0 )
#define DDR_DFI_LP_DATA_IF_STA_WAKEUP__SHFT                                                                  (  0x00000004 )
#define DDR_DFI_LP_DATA_IF_STA__ADR                                                                          (  0x00000040 )
#define DDR_DFI_LP_DATA_IF_STA__WIDTH                                                                        (  10 )
#define DDR_DFI_LP_DATA_IF_STA__POR                                                                          (  0x00000000 )
#define DDR_DFI_LP_DATA_IF_STA__MSK                                                                          (  0x000003F3 )


// Word Address 0x00000044 : DDR_DFI_LP_DATA_IF_EVENT_0_CFG (RW)
#define DDR_DFI_LP_DATA_IF_EVENT_0_CFG_SW_EVENT_CNT__MSK                                                     (  0x000FFFFF )
#define DDR_DFI_LP_DATA_IF_EVENT_0_CFG_SW_EVENT_CNT__SHFT                                                    (  0x00000000 )
#define DDR_DFI_LP_DATA_IF_EVENT_0_CFG_SW_EVENT_CNT_SEL__MSK                                                 (  0x80000000 )
#define DDR_DFI_LP_DATA_IF_EVENT_0_CFG_SW_EVENT_CNT_SEL__SHFT                                                (  0x0000001F )
#define DDR_DFI_LP_DATA_IF_EVENT_0_CFG__ADR                                                                  (  0x00000044 )
#define DDR_DFI_LP_DATA_IF_EVENT_0_CFG__WIDTH                                                                (  32 )
#define DDR_DFI_LP_DATA_IF_EVENT_0_CFG__POR                                                                  (  0x00000000 )
#define DDR_DFI_LP_DATA_IF_EVENT_0_CFG__MSK                                                                  (  0x800FFFFF )


// Word Address 0x00000048 : DDR_DFI_LP_DATA_IF_EVENT_1_CFG (RW)
#define DDR_DFI_LP_DATA_IF_EVENT_1_CFG_SW_EVENT_CNT__MSK                                                     (  0x000FFFFF )
#define DDR_DFI_LP_DATA_IF_EVENT_1_CFG_SW_EVENT_CNT__SHFT                                                    (  0x00000000 )
#define DDR_DFI_LP_DATA_IF_EVENT_1_CFG_SW_EVENT_CNT_SEL__MSK                                                 (  0x80000000 )
#define DDR_DFI_LP_DATA_IF_EVENT_1_CFG_SW_EVENT_CNT_SEL__SHFT                                                (  0x0000001F )
#define DDR_DFI_LP_DATA_IF_EVENT_1_CFG__ADR                                                                  (  0x00000048 )
#define DDR_DFI_LP_DATA_IF_EVENT_1_CFG__WIDTH                                                                (  32 )
#define DDR_DFI_LP_DATA_IF_EVENT_1_CFG__POR                                                                  (  0x00000000 )
#define DDR_DFI_LP_DATA_IF_EVENT_1_CFG__MSK                                                                  (  0x800FFFFF )


// Word Address 0x0000004c : DDR_DFI_PHYUPD_IF_CFG (RW)
#define DDR_DFI_PHYUPD_IF_CFG_SW_REQ_OVR__MSK                                                                (  0x00000001 )
#define DDR_DFI_PHYUPD_IF_CFG_SW_REQ_OVR__SHFT                                                               (  0x00000000 )
#define DDR_DFI_PHYUPD_IF_CFG_SW_REQ_VAL__MSK                                                                (  0x00000002 )
#define DDR_DFI_PHYUPD_IF_CFG_SW_REQ_VAL__SHFT                                                               (  0x00000001 )
#define DDR_DFI_PHYUPD_IF_CFG_SW_ACK_OVR__MSK                                                                (  0x00000004 )
#define DDR_DFI_PHYUPD_IF_CFG_SW_ACK_OVR__SHFT                                                               (  0x00000002 )
#define DDR_DFI_PHYUPD_IF_CFG_SW_ACK_VAL__MSK                                                                (  0x00000008 )
#define DDR_DFI_PHYUPD_IF_CFG_SW_ACK_VAL__SHFT                                                               (  0x00000003 )
#define DDR_DFI_PHYUPD_IF_CFG_SW_EVENT_OVR__MSK                                                              (  0x00000010 )
#define DDR_DFI_PHYUPD_IF_CFG_SW_EVENT_OVR__SHFT                                                             (  0x00000004 )
#define DDR_DFI_PHYUPD_IF_CFG_SW_EVENT_VAL__MSK                                                              (  0x00000020 )
#define DDR_DFI_PHYUPD_IF_CFG_SW_EVENT_VAL__SHFT                                                             (  0x00000005 )
#define DDR_DFI_PHYUPD_IF_CFG_SW_TYPE__MSK                                                                   (  0x00000300 )
#define DDR_DFI_PHYUPD_IF_CFG_SW_TYPE__SHFT                                                                  (  0x00000008 )
#define DDR_DFI_PHYUPD_IF_CFG__ADR                                                                           (  0x0000004c )
#define DDR_DFI_PHYUPD_IF_CFG__WIDTH                                                                         (  10 )
#define DDR_DFI_PHYUPD_IF_CFG__POR                                                                           (  0x00000000 )
#define DDR_DFI_PHYUPD_IF_CFG__MSK                                                                           (  0x0000033F )


// Word Address 0x00000050 : DDR_DFI_PHYUPD_IF_STA (R)
#define DDR_DFI_PHYUPD_IF_STA_REQ__MSK                                                                       (  0x00000001 )
#define DDR_DFI_PHYUPD_IF_STA_REQ__SHFT                                                                      (  0x00000000 )
#define DDR_DFI_PHYUPD_IF_STA_ACK__MSK                                                                       (  0x00000002 )
#define DDR_DFI_PHYUPD_IF_STA_ACK__SHFT                                                                      (  0x00000001 )
#define DDR_DFI_PHYUPD_IF_STA_EVENT__MSK                                                                     (  0x00000004 )
#define DDR_DFI_PHYUPD_IF_STA_EVENT__SHFT                                                                    (  0x00000002 )
#define DDR_DFI_PHYUPD_IF_STA__ADR                                                                           (  0x00000050 )
#define DDR_DFI_PHYUPD_IF_STA__WIDTH                                                                         (   3 )
#define DDR_DFI_PHYUPD_IF_STA__POR                                                                           (  0x00000000 )
#define DDR_DFI_PHYUPD_IF_STA__MSK                                                                           (  0x00000007 )


// Word Address 0x00000054 : DDR_DFI_PHYMSTR_IF_CFG (RW)
#define DDR_DFI_PHYMSTR_IF_CFG_SW_REQ_OVR__MSK                                                               (  0x00000001 )
#define DDR_DFI_PHYMSTR_IF_CFG_SW_REQ_OVR__SHFT                                                              (  0x00000000 )
#define DDR_DFI_PHYMSTR_IF_CFG_SW_REQ_VAL__MSK                                                               (  0x00000002 )
#define DDR_DFI_PHYMSTR_IF_CFG_SW_REQ_VAL__SHFT                                                              (  0x00000001 )
#define DDR_DFI_PHYMSTR_IF_CFG_SW_ACK_OVR__MSK                                                               (  0x00000004 )
#define DDR_DFI_PHYMSTR_IF_CFG_SW_ACK_OVR__SHFT                                                              (  0x00000002 )
#define DDR_DFI_PHYMSTR_IF_CFG_SW_ACK_VAL__MSK                                                               (  0x00000008 )
#define DDR_DFI_PHYMSTR_IF_CFG_SW_ACK_VAL__SHFT                                                              (  0x00000003 )
#define DDR_DFI_PHYMSTR_IF_CFG_SW_EVENT_OVR__MSK                                                             (  0x00000010 )
#define DDR_DFI_PHYMSTR_IF_CFG_SW_EVENT_OVR__SHFT                                                            (  0x00000004 )
#define DDR_DFI_PHYMSTR_IF_CFG_SW_EVENT_VAL__MSK                                                             (  0x00000020 )
#define DDR_DFI_PHYMSTR_IF_CFG_SW_EVENT_VAL__SHFT                                                            (  0x00000005 )
#define DDR_DFI_PHYMSTR_IF_CFG_SW_STATE_SEL__MSK                                                             (  0x00000040 )
#define DDR_DFI_PHYMSTR_IF_CFG_SW_STATE_SEL__SHFT                                                            (  0x00000006 )
#define DDR_DFI_PHYMSTR_IF_CFG_SW_CS_STATE__MSK                                                              (  0x00000180 )
#define DDR_DFI_PHYMSTR_IF_CFG_SW_CS_STATE__SHFT                                                             (  0x00000007 )
#define DDR_DFI_PHYMSTR_IF_CFG_SW_TYPE__MSK                                                                  (  0x00000600 )
#define DDR_DFI_PHYMSTR_IF_CFG_SW_TYPE__SHFT                                                                 (  0x00000009 )
#define DDR_DFI_PHYMSTR_IF_CFG__ADR                                                                          (  0x00000054 )
#define DDR_DFI_PHYMSTR_IF_CFG__WIDTH                                                                        (  11 )
#define DDR_DFI_PHYMSTR_IF_CFG__POR                                                                          (  0x00000000 )
#define DDR_DFI_PHYMSTR_IF_CFG__MSK                                                                          (  0x000007FF )


// Word Address 0x00000058 : DDR_DFI_PHYMSTR_IF_STA (R)
#define DDR_DFI_PHYMSTR_IF_STA_REQ__MSK                                                                      (  0x00000001 )
#define DDR_DFI_PHYMSTR_IF_STA_REQ__SHFT                                                                     (  0x00000000 )
#define DDR_DFI_PHYMSTR_IF_STA_ACK__MSK                                                                      (  0x00000002 )
#define DDR_DFI_PHYMSTR_IF_STA_ACK__SHFT                                                                     (  0x00000001 )
#define DDR_DFI_PHYMSTR_IF_STA_EVENT__MSK                                                                    (  0x00000004 )
#define DDR_DFI_PHYMSTR_IF_STA_EVENT__SHFT                                                                   (  0x00000002 )
#define DDR_DFI_PHYMSTR_IF_STA__ADR                                                                          (  0x00000058 )
#define DDR_DFI_PHYMSTR_IF_STA__WIDTH                                                                        (   3 )
#define DDR_DFI_PHYMSTR_IF_STA__POR                                                                          (  0x00000000 )
#define DDR_DFI_PHYMSTR_IF_STA__MSK                                                                          (  0x00000007 )


// Word Address 0x0000005c : DDR_DFI_DEBUG_CFG (RW)
#define DDR_DFI_DEBUG_CFG_DEBUG_BUS_SEL__MSK                                                                 (  0x0000000F )
#define DDR_DFI_DEBUG_CFG_DEBUG_BUS_SEL__SHFT                                                                (  0x00000000 )
#define DDR_DFI_DEBUG_CFG__ADR                                                                               (  0x0000005c )
#define DDR_DFI_DEBUG_CFG__WIDTH                                                                             (   4 )
#define DDR_DFI_DEBUG_CFG__POR                                                                               (  0x00000000 )
#define DDR_DFI_DEBUG_CFG__MSK                                                                               (  0x0000000F )


#endif /* _DDR_DFI_CSR_H_ */