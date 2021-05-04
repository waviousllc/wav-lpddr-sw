/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DFI_PACKET_H_
#define _DFI_PACKET_H_

#include <stdint.h>
#include <stddef.h>
#include <compiler.h>
#include <dfi/command.h>
#include <kernel/list.h>

#define PACKET_BUFFER_DEPTH         (32)
#define PACKET_MAX_NUM_PHASES       (8)

#define DFI_PACK_DCE_WIDTH          (1)
#define DFI_PACK_CS_WIDTH           (2)
#define DFI_PACK_CKE_WIDTH          (2)
#define DFI_PACK_ADDRESS_WIDTH      (7)
#define DFI_PACK_DATA_WIDTH         (8)
#define DFI_PACK_MASK_WIDTH         (1)
#define DFI_PACK_WCK_TOGGLE_WIDTH   (2)
#define DFI_PACK_EN_WIDTH           (1)
#define DFI_PACK_PARITY_WIDTH       (1)
#define DFI_PACK_VALID_WIDTH        (1)
#define DFI_PACK_TIME_WIDTH         (16)

/**
 * @brief   Packet Data Mask Enumerations
 *
 * @details Mask to indicate if even or odd phases should be processed.
 *
 * MASK_EVEN    only even phases are processed.
 * MASK_ODD     only odd phases are processed.
 * MASk_BOT?H   all phases are processed.
 */
typedef enum packet_data_mask_t
{
    PACKET_DATA_MASK_EVEN   = 1,
    PACKET_DATA_MASK_ODD    = 2,
    PACKET_DATA_MASK_BOTH   = (PACKET_DATA_MASK_EVEN | PACKET_DATA_MASK_ODD)
} packet_data_mask_t;

/**
 * @brief Packet Group Enumerations
 *
 * @details Groups of fields assocaited with DFI signal within a DFI packet.
 *
 * DCE              CK Toggle.
 * CKE              Clock enable.
 * CS               Command Chipselect.
 * COMMAND          Command Address.
 * WRDATA           Write Data.
 * WRDATA_EN        Write Data Enable.
 * RDATA_EN         Read Data Enable.
 * RDATA_CS         Read Data Chip select.
 */
typedef enum packet_group_t
{
    PACKET_GROUP_COMMAND_DCE,
    PACKET_GROUP_COMMAND_CKE,
    PACKET_GROUP_COMMAND_CS,
    PACKET_GROUP_COMMAND,
    PACKET_GROUP_WRDATA_EN,
    PACKET_GROUP_WRDATA,
    PACKET_GROUP_RDDATA_EN,
    PACKET_GROUP_RDDATA_CS,
    PACKET_GROUP_TOTAL_NUM
} packet_group_t;

/**
 * @brief Packing Mode Enuemrations
 *
 * @details Supported packing modes for filling in a packet buffer.
 *
 * UNPACKED    No packing. Commands are sequential.
 */
typedef enum packing_mode_t
{
    PACKING_MODE_UNPACKED,
} packing_mode_t;

/**
 * @brief DFI TX Packet Description
 *
 * Representation of DFI TX Packet that is written
 * to DFI HW Buffer.
 */
typedef struct dfi_tx_packet_desc_t
{
    uint32_t dq3_wrdata_p7      : DFI_PACK_DATA_WIDTH;
    uint32_t dq3_wrdata_mask_p7 : DFI_PACK_MASK_WIDTH;
    uint32_t dq2_wrdata_p7      : DFI_PACK_DATA_WIDTH;
    uint32_t dq2_wrdata_mask_p7 : DFI_PACK_MASK_WIDTH;
    uint32_t dq1_wrdata_p7      : DFI_PACK_DATA_WIDTH;
    uint32_t dq1_wrdata_mask_p7 : DFI_PACK_MASK_WIDTH;
    uint32_t dq0_wrdata_p7      : DFI_PACK_DATA_WIDTH;
    uint32_t dq0_wrdata_mask_p7 : DFI_PACK_MASK_WIDTH;
    uint32_t wrdata_cs_p7       : DFI_PACK_CS_WIDTH;
    uint32_t parity_in_p7       : DFI_PACK_PARITY_WIDTH;
    uint32_t wrdata_en_p7       : DFI_PACK_EN_WIDTH;
    uint32_t wck_cs_p7          : DFI_PACK_CS_WIDTH;
    uint32_t wck_en_p7          : DFI_PACK_EN_WIDTH;
    uint32_t wck_toggle_p7      : DFI_PACK_WCK_TOGGLE_WIDTH;
    uint32_t rddata_cs_p7       : DFI_PACK_CS_WIDTH;
    uint32_t rddata_en_p7       : DFI_PACK_EN_WIDTH;
    uint32_t dq3_wrdata_p6      : DFI_PACK_DATA_WIDTH;
    uint32_t dq3_wrdata_mask_p6 : DFI_PACK_MASK_WIDTH;
    uint32_t dq2_wrdata_p6      : DFI_PACK_DATA_WIDTH;
    uint32_t dq2_wrdata_mask_p6 : DFI_PACK_MASK_WIDTH;
    uint32_t dq1_wrdata_p6      : DFI_PACK_DATA_WIDTH;
    uint32_t dq1_wrdata_mask_p6 : DFI_PACK_MASK_WIDTH;
    uint32_t dq0_wrdata_p6      : DFI_PACK_DATA_WIDTH;
    uint32_t dq0_wrdata_mask_p6 : DFI_PACK_MASK_WIDTH;
    uint32_t wrdata_cs_p6       : DFI_PACK_CS_WIDTH;
    uint32_t parity_in_p6       : DFI_PACK_PARITY_WIDTH;
    uint32_t wrdata_en_p6       : DFI_PACK_EN_WIDTH;
    uint32_t wck_cs_p6          : DFI_PACK_CS_WIDTH;
    uint32_t wck_en_p6          : DFI_PACK_EN_WIDTH;
    uint32_t wck_toggle_p6      : DFI_PACK_WCK_TOGGLE_WIDTH;
    uint32_t rddata_cs_p6       : DFI_PACK_CS_WIDTH;
    uint32_t rddata_en_p6       : DFI_PACK_EN_WIDTH;
    uint32_t dq3_wrdata_p5      : DFI_PACK_DATA_WIDTH;
    uint32_t dq3_wrdata_mask_p5 : DFI_PACK_MASK_WIDTH;
    uint32_t dq2_wrdata_p5      : DFI_PACK_DATA_WIDTH;
    uint32_t dq2_wrdata_mask_p5 : DFI_PACK_MASK_WIDTH;
    uint32_t dq1_wrdata_p5      : DFI_PACK_DATA_WIDTH;
    uint32_t dq1_wrdata_mask_p5 : DFI_PACK_MASK_WIDTH;
    uint32_t dq0_wrdata_p5      : DFI_PACK_DATA_WIDTH;
    uint32_t dq0_wrdata_mask_p5 : DFI_PACK_MASK_WIDTH;
    uint32_t wrdata_cs_p5       : DFI_PACK_CS_WIDTH;
    uint32_t parity_in_p5       : DFI_PACK_PARITY_WIDTH;
    uint32_t wrdata_en_p5       : DFI_PACK_EN_WIDTH;
    uint32_t wck_cs_p5          : DFI_PACK_CS_WIDTH;
    uint32_t wck_en_p5          : DFI_PACK_EN_WIDTH;
    uint32_t wck_toggle_p5      : DFI_PACK_WCK_TOGGLE_WIDTH;
    uint32_t rddata_cs_p5       : DFI_PACK_CS_WIDTH;
    uint32_t rddata_en_p5       : DFI_PACK_EN_WIDTH;
    uint32_t dq3_wrdata_p4      : DFI_PACK_DATA_WIDTH;
    uint32_t dq3_wrdata_mask_p4 : DFI_PACK_MASK_WIDTH;
    uint32_t dq2_wrdata_p4      : DFI_PACK_DATA_WIDTH;
    uint32_t dq2_wrdata_mask_p4 : DFI_PACK_MASK_WIDTH;
    uint32_t dq1_wrdata_p4      : DFI_PACK_DATA_WIDTH;
    uint32_t dq1_wrdata_mask_p4 : DFI_PACK_MASK_WIDTH;
    uint32_t dq0_wrdata_p4      : DFI_PACK_DATA_WIDTH;
    uint32_t dq0_wrdata_mask_p4 : DFI_PACK_MASK_WIDTH;
    uint32_t wrdata_cs_p4       : DFI_PACK_CS_WIDTH;
    uint32_t parity_in_p4       : DFI_PACK_PARITY_WIDTH;
    uint32_t wrdata_en_p4       : DFI_PACK_EN_WIDTH;
    uint32_t wck_cs_p4          : DFI_PACK_CS_WIDTH;
    uint32_t wck_en_p4          : DFI_PACK_EN_WIDTH;
    uint32_t wck_toggle_p4      : DFI_PACK_WCK_TOGGLE_WIDTH;
    uint32_t rddata_cs_p4       : DFI_PACK_CS_WIDTH;
    uint32_t rddata_en_p4       : DFI_PACK_EN_WIDTH;
    uint32_t dq3_wrdata_p3      : DFI_PACK_DATA_WIDTH;
    uint32_t dq3_wrdata_mask_p3 : DFI_PACK_MASK_WIDTH;
    uint32_t dq2_wrdata_p3      : DFI_PACK_DATA_WIDTH;
    uint32_t dq2_wrdata_mask_p3 : DFI_PACK_MASK_WIDTH;
    uint32_t dq1_wrdata_p3      : DFI_PACK_DATA_WIDTH;
    uint32_t dq1_wrdata_mask_p3 : DFI_PACK_MASK_WIDTH;
    uint32_t dq0_wrdata_p3      : DFI_PACK_DATA_WIDTH;
    uint32_t dq0_wrdata_mask_p3 : DFI_PACK_MASK_WIDTH;
    uint32_t wrdata_cs_p3       : DFI_PACK_CS_WIDTH;
    uint32_t parity_in_p3       : DFI_PACK_PARITY_WIDTH;
    uint32_t wrdata_en_p3       : DFI_PACK_EN_WIDTH;
    uint32_t wck_cs_p3          : DFI_PACK_CS_WIDTH;
    uint32_t wck_en_p3          : DFI_PACK_EN_WIDTH;
    uint32_t wck_toggle_p3      : DFI_PACK_WCK_TOGGLE_WIDTH;
    uint32_t rddata_cs_p3       : DFI_PACK_CS_WIDTH;
    uint32_t rddata_en_p3       : DFI_PACK_EN_WIDTH;
    uint32_t dq3_wrdata_p2      : DFI_PACK_DATA_WIDTH;
    uint32_t dq3_wrdata_mask_p2 : DFI_PACK_MASK_WIDTH;
    uint32_t dq2_wrdata_p2      : DFI_PACK_DATA_WIDTH;
    uint32_t dq2_wrdata_mask_p2 : DFI_PACK_MASK_WIDTH;
    uint32_t dq1_wrdata_p2      : DFI_PACK_DATA_WIDTH;
    uint32_t dq1_wrdata_mask_p2 : DFI_PACK_MASK_WIDTH;
    uint32_t dq0_wrdata_p2      : DFI_PACK_DATA_WIDTH;
    uint32_t dq0_wrdata_mask_p2 : DFI_PACK_MASK_WIDTH;
    uint32_t wrdata_cs_p2       : DFI_PACK_CS_WIDTH;
    uint32_t parity_in_p2       : DFI_PACK_PARITY_WIDTH;
    uint32_t wrdata_en_p2       : DFI_PACK_EN_WIDTH;
    uint32_t wck_cs_p2          : DFI_PACK_CS_WIDTH;
    uint32_t wck_en_p2          : DFI_PACK_EN_WIDTH;
    uint32_t wck_toggle_p2      : DFI_PACK_WCK_TOGGLE_WIDTH;
    uint32_t rddata_cs_p2       : DFI_PACK_CS_WIDTH;
    uint32_t rddata_en_p2       : DFI_PACK_EN_WIDTH;
    uint32_t dq3_wrdata_p1      : DFI_PACK_DATA_WIDTH;
    uint32_t dq3_wrdata_mask_p1 : DFI_PACK_MASK_WIDTH;
    uint32_t dq2_wrdata_p1      : DFI_PACK_DATA_WIDTH;
    uint32_t dq2_wrdata_mask_p1 : DFI_PACK_MASK_WIDTH;
    uint32_t dq1_wrdata_p1      : DFI_PACK_DATA_WIDTH;
    uint32_t dq1_wrdata_mask_p1 : DFI_PACK_MASK_WIDTH;
    uint32_t dq0_wrdata_p1      : DFI_PACK_DATA_WIDTH;
    uint32_t dq0_wrdata_mask_p1 : DFI_PACK_MASK_WIDTH;
    uint32_t wrdata_cs_p1       : DFI_PACK_CS_WIDTH;
    uint32_t parity_in_p1       : DFI_PACK_PARITY_WIDTH;
    uint32_t wrdata_en_p1       : DFI_PACK_EN_WIDTH;
    uint32_t wck_cs_p1          : DFI_PACK_CS_WIDTH;
    uint32_t wck_en_p1          : DFI_PACK_EN_WIDTH;
    uint32_t wck_toggle_p1      : DFI_PACK_WCK_TOGGLE_WIDTH;
    uint32_t rddata_cs_p1       : DFI_PACK_CS_WIDTH;
    uint32_t rddata_en_p1       : DFI_PACK_EN_WIDTH;
    uint32_t dq3_wrdata_p0      : DFI_PACK_DATA_WIDTH;
    uint32_t dq3_wrdata_mask_p0 : DFI_PACK_MASK_WIDTH;
    uint32_t dq2_wrdata_p0      : DFI_PACK_DATA_WIDTH;
    uint32_t dq2_wrdata_mask_p0 : DFI_PACK_MASK_WIDTH;
    uint32_t dq1_wrdata_p0      : DFI_PACK_DATA_WIDTH;
    uint32_t dq1_wrdata_mask_p0 : DFI_PACK_MASK_WIDTH;
    uint32_t dq0_wrdata_p0      : DFI_PACK_DATA_WIDTH;
    uint32_t dq0_wrdata_mask_p0 : DFI_PACK_MASK_WIDTH;
    uint32_t wrdata_cs_p0       : DFI_PACK_CS_WIDTH;
    uint32_t parity_in_p0       : DFI_PACK_PARITY_WIDTH;
    uint32_t wrdata_en_p0       : DFI_PACK_EN_WIDTH;
    uint32_t wck_cs_p0          : DFI_PACK_CS_WIDTH;
    uint32_t wck_en_p0          : DFI_PACK_EN_WIDTH;
    uint32_t wck_toggle_p0      : DFI_PACK_WCK_TOGGLE_WIDTH;
    uint32_t rddata_cs_p0       : DFI_PACK_CS_WIDTH;
    uint32_t rddata_en_p0       : DFI_PACK_EN_WIDTH;
    // seperation for CA vs DATA
    uint32_t address_p7         : DFI_PACK_ADDRESS_WIDTH;
    uint32_t cke_p7             : DFI_PACK_CKE_WIDTH;
    uint32_t cs_p7              : DFI_PACK_CS_WIDTH;
    uint32_t dce_p7             : DFI_PACK_DCE_WIDTH;
    uint32_t address_p6         : DFI_PACK_ADDRESS_WIDTH;
    uint32_t cke_p6             : DFI_PACK_CKE_WIDTH;
    uint32_t cs_p6              : DFI_PACK_CS_WIDTH;
    uint32_t dce_p6             : DFI_PACK_DCE_WIDTH;
    uint32_t address_p5         : DFI_PACK_ADDRESS_WIDTH;
    uint32_t cke_p5             : DFI_PACK_CKE_WIDTH;
    uint32_t cs_p5              : DFI_PACK_CS_WIDTH;
    uint32_t dce_p5             : DFI_PACK_DCE_WIDTH;
    uint32_t address_p4         : DFI_PACK_ADDRESS_WIDTH;
    uint32_t cke_p4             : DFI_PACK_CKE_WIDTH;
    uint32_t cs_p4              : DFI_PACK_CS_WIDTH;
    uint32_t dce_p4             : DFI_PACK_DCE_WIDTH;
    uint32_t address_p3         : DFI_PACK_ADDRESS_WIDTH;
    uint32_t cke_p3             : DFI_PACK_CKE_WIDTH;
    uint32_t cs_p3              : DFI_PACK_CS_WIDTH;
    uint32_t dce_p3             : DFI_PACK_DCE_WIDTH;
    uint32_t address_p2         : DFI_PACK_ADDRESS_WIDTH;
    uint32_t cke_p2             : DFI_PACK_CKE_WIDTH;
    uint32_t cs_p2              : DFI_PACK_CS_WIDTH;
    uint32_t dce_p2             : DFI_PACK_DCE_WIDTH;
    uint32_t address_p1         : DFI_PACK_ADDRESS_WIDTH;
    uint32_t cke_p1             : DFI_PACK_CKE_WIDTH;
    uint32_t cs_p1              : DFI_PACK_CS_WIDTH;
    uint32_t dce_p1             : DFI_PACK_DCE_WIDTH;
    uint32_t address_p0         : DFI_PACK_ADDRESS_WIDTH;
    uint32_t cke_p0             : DFI_PACK_CKE_WIDTH;
    uint32_t cs_p0              : DFI_PACK_CS_WIDTH;
    uint32_t dce_p0             : DFI_PACK_DCE_WIDTH;
    uint32_t time               : DFI_PACK_TIME_WIDTH;
} __ALIGNED_4__ __PACKED__ dfi_tx_packet_desc_t;

/**
 * @brief   DFI TX Packet Structure
 *
 * @details Union that wraps packet description so it can accessed as a buffer
 *          of 32 bit values which makes it faster to write out to the FIFO.
 *
 * @note   It is recommended that packets are allocated through this union.
 *
 * packet   packet respresentation of the packet.
 * raw_data raw data representation of the packet.
 */
typedef union dfi_tx_packet_t
{
    dfi_tx_packet_desc_t packet;
    // NOTE: This works because of packing of dfi_tx_packet_desc_t
    uint32_t raw_data[sizeof(dfi_tx_packet_desc_t) / sizeof(uint32_t)];
} dfi_tx_packet_t;

/**
 * @brief DFI RX Packet Description
 *
 * @details Representation of DFI RX Packet that is read from DFI HW Buffer.
 */
typedef struct dfi_rx_packet_desc_t
{
    // DQ3
    uint32_t dq3_dfi_rddata_w7         : DFI_PACK_DATA_WIDTH;
    uint32_t dq3_dfi_rddata_dbi_w7     : DFI_PACK_MASK_WIDTH;
    uint32_t dq3_dfi_rddata_valid_w7   : DFI_PACK_VALID_WIDTH;
    uint32_t dq3_dfi_rddata_w6         : DFI_PACK_DATA_WIDTH;
    uint32_t dq3_dfi_rddata_dbi_w6     : DFI_PACK_MASK_WIDTH;
    uint32_t dq3_dfi_rddata_valid_w6   : DFI_PACK_VALID_WIDTH;
    uint32_t dq3_dfi_rddata_w5         : DFI_PACK_DATA_WIDTH;
    uint32_t dq3_dfi_rddata_dbi_w5     : DFI_PACK_MASK_WIDTH;
    uint32_t dq3_dfi_rddata_valid_w5   : DFI_PACK_VALID_WIDTH;
    uint32_t dq3_dfi_rddata_w4         : DFI_PACK_DATA_WIDTH;
    uint32_t dq3_dfi_rddata_dbi_w4     : DFI_PACK_MASK_WIDTH;
    uint32_t dq3_dfi_rddata_valid_w4   : DFI_PACK_VALID_WIDTH;
    uint32_t dq3_dfi_rddata_w3         : DFI_PACK_DATA_WIDTH;
    uint32_t dq3_dfi_rddata_dbi_w3     : DFI_PACK_MASK_WIDTH;
    uint32_t dq3_dfi_rddata_valid_w3   : DFI_PACK_VALID_WIDTH;
    uint32_t dq3_dfi_rddata_w2         : DFI_PACK_DATA_WIDTH;
    uint32_t dq3_dfi_rddata_dbi_w2     : DFI_PACK_MASK_WIDTH;
    uint32_t dq3_dfi_rddata_valid_w2   : DFI_PACK_VALID_WIDTH;
    uint32_t dq3_dfi_rddata_w1         : DFI_PACK_DATA_WIDTH;
    uint32_t dq3_dfi_rddata_dbi_w1     : DFI_PACK_MASK_WIDTH;
    uint32_t dq3_dfi_rddata_valid_w1   : DFI_PACK_VALID_WIDTH;
    uint32_t dq3_dfi_rddata_w0         : DFI_PACK_DATA_WIDTH;
    uint32_t dq3_dfi_rddata_dbi_w0     : DFI_PACK_MASK_WIDTH;
    uint32_t dq3_dfi_rddata_valid_w0   : DFI_PACK_VALID_WIDTH;

    // DQ2
    uint32_t dq2_dfi_rddata_w7         : DFI_PACK_DATA_WIDTH;
    uint32_t dq2_dfi_rddata_dbi_w7     : DFI_PACK_MASK_WIDTH;
    uint32_t dq2_dfi_rddata_valid_w7   : DFI_PACK_VALID_WIDTH;
    uint32_t dq2_dfi_rddata_w6         : DFI_PACK_DATA_WIDTH;
    uint32_t dq2_dfi_rddata_dbi_w6     : DFI_PACK_MASK_WIDTH;
    uint32_t dq2_dfi_rddata_valid_w6   : DFI_PACK_VALID_WIDTH;
    uint32_t dq2_dfi_rddata_w5         : DFI_PACK_DATA_WIDTH;
    uint32_t dq2_dfi_rddata_dbi_w5     : DFI_PACK_MASK_WIDTH;
    uint32_t dq2_dfi_rddata_valid_w5   : DFI_PACK_VALID_WIDTH;
    uint32_t dq2_dfi_rddata_w4         : DFI_PACK_DATA_WIDTH;
    uint32_t dq2_dfi_rddata_dbi_w4     : DFI_PACK_MASK_WIDTH;
    uint32_t dq2_dfi_rddata_valid_w4   : DFI_PACK_VALID_WIDTH;
    uint32_t dq2_dfi_rddata_w3         : DFI_PACK_DATA_WIDTH;
    uint32_t dq2_dfi_rddata_dbi_w3     : DFI_PACK_MASK_WIDTH;
    uint32_t dq2_dfi_rddata_valid_w3   : DFI_PACK_VALID_WIDTH;
    uint32_t dq2_dfi_rddata_w2         : DFI_PACK_DATA_WIDTH;
    uint32_t dq2_dfi_rddata_dbi_w2     : DFI_PACK_MASK_WIDTH;
    uint32_t dq2_dfi_rddata_valid_w2   : DFI_PACK_VALID_WIDTH;
    uint32_t dq2_dfi_rddata_w1         : DFI_PACK_DATA_WIDTH;
    uint32_t dq2_dfi_rddata_dbi_w1     : DFI_PACK_MASK_WIDTH;
    uint32_t dq2_dfi_rddata_valid_w1   : DFI_PACK_VALID_WIDTH;
    uint32_t dq2_dfi_rddata_w0         : DFI_PACK_DATA_WIDTH;
    uint32_t dq2_dfi_rddata_dbi_w0     : DFI_PACK_MASK_WIDTH;
    uint32_t dq2_dfi_rddata_valid_w0   : DFI_PACK_VALID_WIDTH;

    // DQ1
    uint32_t dq1_dfi_rddata_w7         : DFI_PACK_DATA_WIDTH;
    uint32_t dq1_dfi_rddata_dbi_w7     : DFI_PACK_MASK_WIDTH;
    uint32_t dq1_dfi_rddata_valid_w7   : DFI_PACK_VALID_WIDTH;
    uint32_t dq1_dfi_rddata_w6         : DFI_PACK_DATA_WIDTH;
    uint32_t dq1_dfi_rddata_dbi_w6     : DFI_PACK_MASK_WIDTH;
    uint32_t dq1_dfi_rddata_valid_w6   : DFI_PACK_VALID_WIDTH;
    uint32_t dq1_dfi_rddata_w5         : DFI_PACK_DATA_WIDTH;
    uint32_t dq1_dfi_rddata_dbi_w5     : DFI_PACK_MASK_WIDTH;
    uint32_t dq1_dfi_rddata_valid_w5   : DFI_PACK_VALID_WIDTH;
    uint32_t dq1_dfi_rddata_w4         : DFI_PACK_DATA_WIDTH;
    uint32_t dq1_dfi_rddata_dbi_w4     : DFI_PACK_MASK_WIDTH;
    uint32_t dq1_dfi_rddata_valid_w4   : DFI_PACK_VALID_WIDTH;
    uint32_t dq1_dfi_rddata_w3         : DFI_PACK_DATA_WIDTH;
    uint32_t dq1_dfi_rddata_dbi_w3     : DFI_PACK_MASK_WIDTH;
    uint32_t dq1_dfi_rddata_valid_w3   : DFI_PACK_VALID_WIDTH;
    uint32_t dq1_dfi_rddata_w2         : DFI_PACK_DATA_WIDTH;
    uint32_t dq1_dfi_rddata_dbi_w2     : DFI_PACK_MASK_WIDTH;
    uint32_t dq1_dfi_rddata_valid_w2   : DFI_PACK_VALID_WIDTH;
    uint32_t dq1_dfi_rddata_w1         : DFI_PACK_DATA_WIDTH;
    uint32_t dq1_dfi_rddata_dbi_w1     : DFI_PACK_MASK_WIDTH;
    uint32_t dq1_dfi_rddata_valid_w1   : DFI_PACK_VALID_WIDTH;
    uint32_t dq1_dfi_rddata_w0         : DFI_PACK_DATA_WIDTH;
    uint32_t dq1_dfi_rddata_dbi_w0     : DFI_PACK_MASK_WIDTH;
    uint32_t dq1_dfi_rddata_valid_w0   : DFI_PACK_VALID_WIDTH;

    // DQ0
    uint32_t dq0_dfi_rddata_w7         : DFI_PACK_DATA_WIDTH;
    uint32_t dq0_dfi_rddata_dbi_w7     : DFI_PACK_MASK_WIDTH;
    uint32_t dq0_dfi_rddata_valid_w7   : DFI_PACK_VALID_WIDTH;
    uint32_t dq0_dfi_rddata_w6         : DFI_PACK_DATA_WIDTH;
    uint32_t dq0_dfi_rddata_dbi_w6     : DFI_PACK_MASK_WIDTH;
    uint32_t dq0_dfi_rddata_valid_w6   : DFI_PACK_VALID_WIDTH;
    uint32_t dq0_dfi_rddata_w5         : DFI_PACK_DATA_WIDTH;
    uint32_t dq0_dfi_rddata_dbi_w5     : DFI_PACK_MASK_WIDTH;
    uint32_t dq0_dfi_rddata_valid_w5   : DFI_PACK_VALID_WIDTH;
    uint32_t dq0_dfi_rddata_w4         : DFI_PACK_DATA_WIDTH;
    uint32_t dq0_dfi_rddata_dbi_w4     : DFI_PACK_MASK_WIDTH;
    uint32_t dq0_dfi_rddata_valid_w4   : DFI_PACK_VALID_WIDTH;
    uint32_t dq0_dfi_rddata_w3         : DFI_PACK_DATA_WIDTH;
    uint32_t dq0_dfi_rddata_dbi_w3     : DFI_PACK_MASK_WIDTH;
    uint32_t dq0_dfi_rddata_valid_w3   : DFI_PACK_VALID_WIDTH;
    uint32_t dq0_dfi_rddata_w2         : DFI_PACK_DATA_WIDTH;
    uint32_t dq0_dfi_rddata_dbi_w2     : DFI_PACK_MASK_WIDTH;
    uint32_t dq0_dfi_rddata_valid_w2   : DFI_PACK_VALID_WIDTH;
    uint32_t dq0_dfi_rddata_w1         : DFI_PACK_DATA_WIDTH;
    uint32_t dq0_dfi_rddata_dbi_w1     : DFI_PACK_MASK_WIDTH;
    uint32_t dq0_dfi_rddata_valid_w1   : DFI_PACK_VALID_WIDTH;
    uint32_t dq0_dfi_rddata_w0         : DFI_PACK_DATA_WIDTH;
    uint32_t dq0_dfi_rddata_dbi_w0     : DFI_PACK_MASK_WIDTH;
    uint32_t dq0_dfi_rddata_valid_w0   : DFI_PACK_VALID_WIDTH;

    // command
    uint32_t dfi_address_w7     : DFI_PACK_ADDRESS_WIDTH;
    uint32_t dfi_cke_w7         : DFI_PACK_CKE_WIDTH;
    uint32_t dfi_cs_w7          : DFI_PACK_CS_WIDTH;
    uint32_t dfi_address_w6     : DFI_PACK_ADDRESS_WIDTH;
    uint32_t dfi_cke_w6         : DFI_PACK_CKE_WIDTH;
    uint32_t dfi_cs_w6          : DFI_PACK_CS_WIDTH;
    uint32_t dfi_address_w5     : DFI_PACK_ADDRESS_WIDTH;
    uint32_t dfi_cke_w5         : DFI_PACK_CKE_WIDTH;
    uint32_t dfi_cs_w5          : DFI_PACK_CS_WIDTH;
    uint32_t dfi_address_w4     : DFI_PACK_ADDRESS_WIDTH;
    uint32_t dfi_cke_w4         : DFI_PACK_CKE_WIDTH;
    uint32_t dfi_cs_w4          : DFI_PACK_CS_WIDTH;
    uint32_t dfi_address_w3     : DFI_PACK_ADDRESS_WIDTH;
    uint32_t dfi_cke_w3         : DFI_PACK_CKE_WIDTH;
    uint32_t dfi_cs_w3          : DFI_PACK_CS_WIDTH;
    uint32_t dfi_address_w2     : DFI_PACK_ADDRESS_WIDTH;
    uint32_t dfi_cke_w2         : DFI_PACK_CKE_WIDTH;
    uint32_t dfi_cs_w2          : DFI_PACK_CS_WIDTH;
    uint32_t dfi_address_w1     : DFI_PACK_ADDRESS_WIDTH;
    uint32_t dfi_cke_w1         : DFI_PACK_CKE_WIDTH;
    uint32_t dfi_cs_w1          : DFI_PACK_CS_WIDTH;
    uint32_t dfi_address_w0     : DFI_PACK_ADDRESS_WIDTH;
    uint32_t dfi_cke_w0         : DFI_PACK_CKE_WIDTH;
    uint32_t dfi_cs_w0          : DFI_PACK_CS_WIDTH;
    uint32_t time               : DFI_PACK_TIME_WIDTH;
} __ALIGNED_4__ __PACKED__ dfi_rx_packet_desc_t;

/**
 * @brief   DFI RX Packet Structure
 *
 * @details Union that wraps packet description so it can accessed as a buffer
 *          of 32 bit values which makes it faster to read from the FIFO.
 *
 * @note    It is recommended that packets are allocated through this union.
 *
 * @note    Type-punning through union member access is safe in GCC, which is
 *          assumed compiler for this project.
 *
 * packet      packet representation of the packet.
 * raw_data    raw data representation of the packet.
 */
typedef union dfi_rx_packet_t
{
    dfi_rx_packet_desc_t packet;
    // NOTE: This works because of packing of dfi_rx_packet_desc_t
    uint32_t raw_data[sizeof(dfi_rx_packet_desc_t) / sizeof(uint32_t)];
} dfi_rx_packet_t;

/**
 * @brief   Packet Item Structure
 *
 * @details List Item structure used for storing TX Packets in a linked list.
 *
 * list     list item member.
 * packet   actual TX packet data.
 */
typedef struct packet_item_t
{
    list_head_t list;
    dfi_tx_packet_t packet;
} packet_item_t;

/**
 * @brief    DFI TX Packet Buffer
 *
 * @details In memory packet buffer that is used to prepare DFI packets
 *          prior to flushing to hardware.
 *
 * num_packets      number of packets allocated in packet storage.
 * packet_index     index of next packet allocation.
 * list             linked list of packets. (This is the "buffer").
 * packets          pointer to storage where packets have been allocated.
 */
typedef struct dfi_tx_packet_buffer_t
{
    uint8_t         num_packets;
    uint8_t         packet_index;
    list_head_t     list;
    packet_item_t   *packets;
} dfi_tx_packet_buffer_t;

/**
 * @brief   DFI RX Packet Buffer
 *
 * @details In memory packet buffer that is used to store received DFI packets
 *          from HW.
 *
 * @note    This allocates PACKET_BUFFER_DEPTH number of RX Packets.
 *
 * buffer   the buffer of packets.
 */
typedef struct dfi_rx_packet_buffer_t
{
    dfi_rx_packet_t buffer[PACKET_BUFFER_DEPTH];
} dfi_rx_packet_buffer_t;

/**
 * @brief   DFI Command Configuration Structure
 *
 * @details DFI Command Configuration Structure is used to describe the DFI
 *          timing for DFI packets.
 *
 * ratio                frequency Ratio between DFI and DRAM CK.
 * group_num_phases     number of phases that a packet group can use.
 * group_offsets        relative offset between groups within a packet for a
 *                      given command.
 * group_lengths        length of each group for a given command.
 */
typedef struct dfi_command_cfg_t
{
    wddr_freq_ratio_t ratio;
    uint8_t group_num_phases[PACKET_GROUP_TOTAL_NUM];
    uint16_t group_offsets[COMMAND_TYPE_TOTAL_NUM][PACKET_GROUP_TOTAL_NUM];
    uint16_t group_lengths[COMMAND_TYPE_TOTAL_NUM][PACKET_GROUP_TOTAL_NUM];
} dfi_command_cfg_t;

/**
 * @brief   DFI TX Packet Buffer Fill
 *
 * @details TX Packet Buffer Fill Algorithm.
 *
 *
 * @param[in]   command         pointer to command to be sent via DFI packets.
 * @param[out]  packet_bufer    pointer to the packet buffer to fill in.
 * @param[in]   cfg             pointer to command configuration used to
 *                              fill in DFI packets.
 * @param[in]   ts_offset       timestamp offset relative to previous packet
 *                              in the buffer. Ignored if packet buffer is empty.
 *
 * @return
 */
wddr_return_t dfi_tx_packet_buffer_fill(command_t *command,
                                       dfi_tx_packet_buffer_t *buffer,
                                       dfi_command_cfg_t *cfg,
                                       uint16_t ts_offset);

/**
 * @brief   DFI TX Packet Buffer Initialization
 *
 * @details Initializes a TX packet buffer.
 *
 * @param[out]  buffer      pointer to tx packet buffer.
 * @param[in]   packet      pointer to packet storage for the buffer.
 * @param[in]   num_packets number of packets allocated via storage.
 *
 * @return      void
 */
void dfi_tx_packet_buffer_init(dfi_tx_packet_buffer_t *buffer,
                               packet_item_t *packets,
                               uint8_t num_packets);

/**
 * @brief   DFI RX Packet Buffer Init
 *
 * @details Initializes a RX packet buffer.
 *
 * @param[out]  buffer     pointer to rx packet buffer.
 *
 * @return      void
 */
void dfi_rx_packet_buffer_init(dfi_rx_packet_buffer_t *buffer);

/**
 * @brief   DFI RX Packet Buffer Data Comparison
 *
 * @details Compares given data to data read via received DFI Packets.
 *
 * @param[in]   buffer      pointer to rx packet buffer.
 * @param[in]   expected    pointer to expected data.
 * @param[in]   dq_byte     which DQ Byte to compare.
 * @param[in]   data_mask   data mask for which phases to compare.
 * @param[in]   num         number of bytes to compare.
 * @param[in]   phases      number of phases of data to processes. This is
 *                          depedent on current DRAM to DFI Freq ratio.
 * @param[out]  is_same     pointer to store if data matches.
 *
 * @return      void
 */
void dfi_rx_packet_buffer_data_compare(dfi_rx_packet_buffer_t *buffer,
                                       command_data_t *expected,
                                       wddr_dq_byte_t dq_byte,
                                       packet_data_mask_t data_mask,
                                       uint8_t num,
                                       uint8_t phases,
                                       uint8_t *is_same);

/**
 * @brief   DFI TX Packet Get Mission Configuration
 *
 * @details Returns a pointer to pre allocated DFI Command Configuraton.
 *
 * @param[out]  cfg   pointer to a pointer storing dfi command config structure.
 *
 * @return      void
 */
void dfi_tx_packet_get_mission_cfg(dfi_command_cfg_t **cfg);

#endif /* _DFI_PACKET_H_ */
