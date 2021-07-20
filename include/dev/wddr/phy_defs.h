/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _WDDR_PHY_DEFS_H_
#define _WDDR_PHY_DEFS_H_

/**
 * @brief   WDDR PHY Channel Index Enuemrations
 *
 * @details Enumerations for supported channels in
 *          the DDR PHY.
 *
 * @note    Each channel is composed of 2 DQ Blocks and
 *          1 CA Block.
 *
 * CHANNEL_0    Channel 0 index.
 * CHANNEL_1    Channel 1 index.
 */
typedef enum wddr_channel_t
{
    WDDR_CHANNEL_0,
    WDDR_CHANNEL_1,
    WDDR_CHANNEL_TOTAL,
} wddr_channel_t;

/**
 * @brief   WDDR PHY Configuration Enumerations
 *
 * @details Enumerations for supported PHY Hardware configurations.
 *
 * DDR mode used for LPDDR4/5
 * QDR mode used for GDDR6
 */
typedef enum wddr_phy_cfg_t
{
    WDDR_PHY_CFG_DDR = 2,
    WDDR_PHY_CFG_QDR = 4,
} wddr_phy_cfg_t;

/**
 * @brief   WDDR PHY Rank Index Enumerations
 *
 * @details Enumeration to specify rank index
 *
 * RANK_0   Rank 0 index.
 * RANK_1   Rank 1 index.
 */
typedef enum wddr_rank_t
{
    WDDR_RANK_0 = 0,
    WDDR_RANK_1 = 1
} wddr_rank_t;

/**
 * @brief   WDDR PHY DQ Byte Enumerations
 *
 * @details Specifies a DQ Byte index.
 *
 * DQ_BYTE_0    DQ Byte 0 index.
 * DQ_BYTE_1    DQ Byte 1 index.
 *
 */
typedef enum wddr_dq_byte_t
{
    WDDR_DQ_BYTE_0,
    WDDR_DQ_BYTE_1,
    WDDR_DQ_BYTE_TOTAL,
} wddr_dq_byte_t;

/**
 * @brief   WDDR PHY DQS Slice Enumerations
 *
 * @details Enumerations for which slice is assocaited with a particular
 *          DQS signal.
 *
 * WCK
 * DQS      DQ Strobe
 * REN      Read Enable
 * OE       Output Enable
 * IE       Input Enable
 * RE       Receive Enable
 * WCK_OE   WCK Output Enable
 * WCS      Write Chip Select
 * RCS      Read Chip Select
 */
typedef enum wddr_dqs_slice_t
{
    DQS_SLICE_WCK,
    DQS_SLICE_DQS,
    DQS_SLICE_REN,
    DQS_SLICE_OE,
    DQS_SLICE_IE,
    DQS_SLICE_RE,
    DQS_SLICE_WCK_OE,
    DQS_SLICE_WCS,
    DQS_SLICE_RCS,
    DQS_SLICE_NUM,
} wddr_dqs_slice_t;

/**
 * @brief   WDDR PHY CA Slice Enumerations
 *
 * CA       Command Address
 * CS_0     Chip Select 0
 * CS_1     Chip Select 1
 * CKE_0    CKE 0
 * CKE_1    CKE 1
 */
typedef enum wddr_ca_slice_t
{
    CA_SLICE_CA = 6,
    CA_SLICE_CS_0,
    CA_SLICE_CS_1,
    CA_SLICE_CKE_0,
    CA_SLICE_CKE_1,
    CA_SLICE_NUM,
} wddr_ca_slice_t;

/**
 * @brief   WDDR PHY Slice Type Enumerations
 *
 * @details Mapping of supported slice types.
 *
 * DQ       Data
 * DQS      Data Strobe
 * CA       Command Address
 * CK       Clock
 */
typedef enum wddr_slice_type_t
{
    WDDR_SLICE_TYPE_DQ,
    WDDR_SLICE_TYPE_DQS,
    WDDR_SLICE_TYPE_CA,
    WDDR_SLICE_TYPE_CK,
} wddr_slice_type_t;

/**
 * @brief   WDDR PHY Routing Mode Enumerations
 *
 * @details Supported Routing Modes.
 *
 * MISSION     Normal mission routing mode.
 * EXTERNAL_LB External Loopback routing mode.
 */
typedef enum wddr_routing_mode_t
{
    WDDR_ROUTING_MODE_MISSION,
    WDDR_ROUTING_MODE_EXTERNAL_LB,
} wddr_routing_mode_t;

/**
 * @brief   WDDR PHY Mode Select Register (MSR) Enumerations
 *
 * @details Supported MSR Values.
 *
 * 0   Mode Set Register Value 0
 * 1   Mode Set Register Value 1
 */
typedef enum wddr_msr_t
{
    WDDR_MSR_0,
    WDDR_MSR_1
} wddr_msr_t;

/**
 * @brief   WDDR Frequency Ratio Enumeration
 *
 * @details DFI frequency to DRAM frequency ratio.
 *
 * 1TO1     DRAM tCK = 1 * DFI tCK
 * 1TO2     DRAM tCK = 2 * DFI tCK
 * 1TO4     DRAM tCK = 4 * DFI tCK
 */
typedef enum wddr_freq_ratio_t
{
    WDDR_FREQ_RATIO_1TO1 = 0,
    WDDR_FREQ_RATIO_1TO2 = 1,
    WDDR_FREQ_RATIO_1TO4 = 2,
} wddr_freq_ratio_t;

#endif /* _WDDR_PHY_DEFS_H_ */
