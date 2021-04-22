/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
// MODULE FSW
// RANGE 0x03FF:0x0000
// WIDTH 32

#ifndef _DDR_FSW_CSR_H_
#define _DDR_FSW_CSR_H_

// Word Address 0x00000000 : DDR_FSW_CTRL_CFG (RW)
#define DDR_FSW_CTRL_CFG_VCO_SEL_OVR__MSK                                                                    (  0x00000001 )
#define DDR_FSW_CTRL_CFG_VCO_SEL_OVR__SHFT                                                                   (  0x00000000 )
#define DDR_FSW_CTRL_CFG_VCO_SEL_OVR_VAL__MSK                                                                (  0x00000006 )
#define DDR_FSW_CTRL_CFG_VCO_SEL_OVR_VAL__SHFT                                                               (  0x00000001 )
#define DDR_FSW_CTRL_CFG_VCO_TOGGLE_EN__MSK                                                                  (  0x00000010 )
#define DDR_FSW_CTRL_CFG_VCO_TOGGLE_EN__SHFT                                                                 (  0x00000004 )
#define DDR_FSW_CTRL_CFG_MSR_TOGGLE_EN__MSK                                                                  (  0x00000020 )
#define DDR_FSW_CTRL_CFG_MSR_TOGGLE_EN__SHFT                                                                 (  0x00000005 )
#define DDR_FSW_CTRL_CFG_MSR_OVR__MSK                                                                        (  0x00000040 )
#define DDR_FSW_CTRL_CFG_MSR_OVR__SHFT                                                                       (  0x00000006 )
#define DDR_FSW_CTRL_CFG_MSR_OVR_VAL__MSK                                                                    (  0x00000080 )
#define DDR_FSW_CTRL_CFG_MSR_OVR_VAL__SHFT                                                                   (  0x00000007 )
#define DDR_FSW_CTRL_CFG_PREP_DONE__MSK                                                                      (  0x00000100 )
#define DDR_FSW_CTRL_CFG_PREP_DONE__SHFT                                                                     (  0x00000008 )
#define DDR_FSW_CTRL_CFG_PSTWORK_DONE__MSK                                                                   (  0x00000200 )
#define DDR_FSW_CTRL_CFG_PSTWORK_DONE__SHFT                                                                  (  0x00000009 )
#define DDR_FSW_CTRL_CFG_PSTWORK_DONE_OVR__MSK                                                               (  0x00000400 )
#define DDR_FSW_CTRL_CFG_PSTWORK_DONE_OVR__SHFT                                                              (  0x0000000A )
#define DDR_FSW_CTRL_CFG_SWITCH_DONE_OVR__MSK                                                                (  0x00000800 )
#define DDR_FSW_CTRL_CFG_SWITCH_DONE_OVR__SHFT                                                               (  0x0000000B )
#define DDR_FSW_CTRL_CFG_SWITCH_DONE_STICKY_CLR__MSK                                                         (  0x00001000 )
#define DDR_FSW_CTRL_CFG_SWITCH_DONE_STICKY_CLR__SHFT                                                        (  0x0000000C )
#define DDR_FSW_CTRL_CFG_POST_GFMSEL_WAIT__MSK                                                               (  0x000F0000 )
#define DDR_FSW_CTRL_CFG_POST_GFMSEL_WAIT__SHFT                                                              (  0x00000010 )
#define DDR_FSW_CTRL_CFG__ADR                                                                                (  0x00000000 )
#define DDR_FSW_CTRL_CFG__WIDTH                                                                              (  20 )
#define DDR_FSW_CTRL_CFG__POR                                                                                (  0x00040C31 )
#define DDR_FSW_CTRL_CFG__MSK                                                                                (  0x000F1FF7 )


// Word Address 0x00000004 : DDR_FSW_CTRL_STA (R)
#define DDR_FSW_CTRL_STA_VCO_SEL__MSK                                                                        (  0x00000003 )
#define DDR_FSW_CTRL_STA_VCO_SEL__SHFT                                                                       (  0x00000000 )
#define DDR_FSW_CTRL_STA_SWITCH_DONE__MSK                                                                    (  0x00000004 )
#define DDR_FSW_CTRL_STA_SWITCH_DONE__SHFT                                                                   (  0x00000002 )
#define DDR_FSW_CTRL_STA_CMN_MSR__MSK                                                                        (  0x00000008 )
#define DDR_FSW_CTRL_STA_CMN_MSR__SHFT                                                                       (  0x00000003 )
#define DDR_FSW_CTRL_STA_CORE_READY__MSK                                                                     (  0x00000010 )
#define DDR_FSW_CTRL_STA_CORE_READY__SHFT                                                                    (  0x00000004 )
#define DDR_FSW_CTRL_STA__ADR                                                                                (  0x00000004 )
#define DDR_FSW_CTRL_STA__WIDTH                                                                              (   5 )
#define DDR_FSW_CTRL_STA__POR                                                                                (  0x00000000 )
#define DDR_FSW_CTRL_STA__MSK                                                                                (  0x0000001F )


// Word Address 0x00000008 : DDR_FSW_DEBUG_CFG (RW)
#define DDR_FSW_DEBUG_CFG_DEBUG_BUS_SEL__MSK                                                                 (  0x0000000F )
#define DDR_FSW_DEBUG_CFG_DEBUG_BUS_SEL__SHFT                                                                (  0x00000000 )
#define DDR_FSW_DEBUG_CFG__ADR                                                                               (  0x00000008 )
#define DDR_FSW_DEBUG_CFG__WIDTH                                                                             (   4 )
#define DDR_FSW_DEBUG_CFG__POR                                                                               (  0x00000000 )
#define DDR_FSW_DEBUG_CFG__MSK                                                                               (  0x0000000F )


// Word Address 0x0000002c : DDR_FSW_CSP_0_CFG (RW)
#define DDR_FSW_CSP_0_CFG_PRECLKDIS_WAIT__MSK                                                                (  0x0000000F )
#define DDR_FSW_CSP_0_CFG_PRECLKDIS_WAIT__SHFT                                                               (  0x00000000 )
#define DDR_FSW_CSP_0_CFG_PRERST_WAIT__MSK                                                                   (  0x000000F0 )
#define DDR_FSW_CSP_0_CFG_PRERST_WAIT__SHFT                                                                  (  0x00000004 )
#define DDR_FSW_CSP_0_CFG_RST_PULSE_WIDTH__MSK                                                               (  0x00000F00 )
#define DDR_FSW_CSP_0_CFG_RST_PULSE_WIDTH__SHFT                                                              (  0x00000008 )
#define DDR_FSW_CSP_0_CFG_PSTRST_WAIT__MSK                                                                   (  0x0000F000 )
#define DDR_FSW_CSP_0_CFG_PSTRST_WAIT__SHFT                                                                  (  0x0000000C )
#define DDR_FSW_CSP_0_CFG_PSTCLKEN_WAIT__MSK                                                                 (  0x000F0000 )
#define DDR_FSW_CSP_0_CFG_PSTCLKEN_WAIT__SHFT                                                                (  0x00000010 )
#define DDR_FSW_CSP_0_CFG_PSTPIEN_WAIT__MSK                                                                  (  0x00F00000 )
#define DDR_FSW_CSP_0_CFG_PSTPIEN_WAIT__SHFT                                                                 (  0x00000014 )
#define DDR_FSW_CSP_0_CFG__ADR                                                                               (  0x0000002c )
#define DDR_FSW_CSP_0_CFG__WIDTH                                                                             (  24 )
#define DDR_FSW_CSP_0_CFG__POR                                                                               (  0x00333333 )
#define DDR_FSW_CSP_0_CFG__MSK                                                                               (  0x00FFFFFF )


// Word Address 0x00000030 : DDR_FSW_CSP_1_CFG (RW)
#define DDR_FSW_CSP_1_CFG_REQ_OVR__MSK                                                                       (  0x00000001 )
#define DDR_FSW_CSP_1_CFG_REQ_OVR__SHFT                                                                      (  0x00000000 )
#define DDR_FSW_CSP_1_CFG_REQ_COMPLETE_OVR__MSK                                                              (  0x00000002 )
#define DDR_FSW_CSP_1_CFG_REQ_COMPLETE_OVR__SHFT                                                             (  0x00000001 )
#define DDR_FSW_CSP_1_CFG_CGC_EN_OVR__MSK                                                                    (  0x00000004 )
#define DDR_FSW_CSP_1_CFG_CGC_EN_OVR__SHFT                                                                   (  0x00000002 )
#define DDR_FSW_CSP_1_CFG_REQ_OVR_VAL__MSK                                                                   (  0x00000008 )
#define DDR_FSW_CSP_1_CFG_REQ_OVR_VAL__SHFT                                                                  (  0x00000003 )
#define DDR_FSW_CSP_1_CFG_REQ_COMPLETE_OVR_VAL__MSK                                                          (  0x00000010 )
#define DDR_FSW_CSP_1_CFG_REQ_COMPLETE_OVR_VAL__SHFT                                                         (  0x00000004 )
#define DDR_FSW_CSP_1_CFG_CGC_EN_OVR_VAL__MSK                                                                (  0x00000020 )
#define DDR_FSW_CSP_1_CFG_CGC_EN_OVR_VAL__SHFT                                                               (  0x00000005 )
#define DDR_FSW_CSP_1_CFG_PI_DISABLE_OVR_VAL__MSK                                                            (  0x00000040 )
#define DDR_FSW_CSP_1_CFG_PI_DISABLE_OVR_VAL__SHFT                                                           (  0x00000006 )
#define DDR_FSW_CSP_1_CFG_DIV_RST_OVR_VAL__MSK                                                               (  0x00000080 )
#define DDR_FSW_CSP_1_CFG_DIV_RST_OVR_VAL__SHFT                                                              (  0x00000007 )
#define DDR_FSW_CSP_1_CFG_CLK_DISABLE_OVR_VAL__MSK                                                           (  0x00000100 )
#define DDR_FSW_CSP_1_CFG_CLK_DISABLE_OVR_VAL__SHFT                                                          (  0x00000008 )
#define DDR_FSW_CSP_1_CFG_REQ_COMPLETE_STA_CLR__MSK                                                          (  0x00000200 )
#define DDR_FSW_CSP_1_CFG_REQ_COMPLETE_STA_CLR__SHFT                                                         (  0x00000009 )
#define DDR_FSW_CSP_1_CFG__ADR                                                                               (  0x00000030 )
#define DDR_FSW_CSP_1_CFG__WIDTH                                                                             (  10 )
#define DDR_FSW_CSP_1_CFG__POR                                                                               (  0x00000100 )
#define DDR_FSW_CSP_1_CFG__MSK                                                                               (  0x000003FF )


// Word Address 0x00000034 : DDR_FSW_CSP_STA (R)
#define DDR_FSW_CSP_STA_REQ_COMPLETE__MSK                                                                    (  0x00000001 )
#define DDR_FSW_CSP_STA_REQ_COMPLETE__SHFT                                                                   (  0x00000000 )
#define DDR_FSW_CSP_STA__ADR                                                                                 (  0x00000034 )
#define DDR_FSW_CSP_STA__WIDTH                                                                               (   1 )
#define DDR_FSW_CSP_STA__POR                                                                                 (  0x00000000 )
#define DDR_FSW_CSP_STA__MSK                                                                                 (  0x00000001 )


#endif /* _DDR_FSW_CSR_H_ */
