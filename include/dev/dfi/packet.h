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
#include <dram/table.h>
#include <kernel/list.h>

#define PACKET_BUFFER_DEPTH         (32)
#define PACKET_MAX_NUM_PHASES       (8)
#define TX_PACKET_SIZE_BYTES        (sizeof(dfi_tx_packet_t))
#define TX_PACKET_SIZE_WORDS        (TX_PACKET_SIZE_BYTES / sizeof(uint32_t))
#define RX_PACKET_SIZE_BYTES        (sizeof(dfi_rx_packet_t))
#define RX_PACKET_SIZE_WORDS        (RX_PACKET_SIZE_BYTES / sizeof(uint32_t))

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
 * @brief   Packet Group Info Structure
 *
 * @details Structure containing relevant information needed to fill
 *          appropriate phases within a packet.
 *
 * dfi_ts_start     Timestamp offset start of packet in DFI cycles.
 * cycle_offset     DRAM cycle offset used to skip phases in a packet.
 * phase_remaining  Number of phases remaining to be filled.
 */
typedef struct packet_group_info_t
{
    uint16_t dfi_ts_start;
    uint16_t cycle_offset;
    uint16_t phase_remaining;
} packet_group_info_t;

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
    ListItem_t  list_item;
    dfi_tx_packet_t packet;
} packet_item_t;

/**
 * @brief   Packet Storage Structure
 *
 * @details Structure used as storage for allocating packets from. This
 *          can be attached to a dfi_tx_packet_buffer_t instance to use
 *          as storage backend, rather than using malloc.
 *
 * packets  pointer to underlying storage.
 * index    current packet index within storage.
 * len      number of packets pointed to by packets member.
 */
typedef struct packet_storage_t
{
    packet_item_t   *packets;
    uint8_t         index;
    uint8_t         len;
} packet_storage_t;

/**
 * @brief    DFI TX Packet Buffer
 *
 * @details In memory packet buffer that is used to prepare DFI packets
 *          prior to flushing to hardware.
 *
 * ts_last_packet   timestamp of last packet stored in buffer.
 * list             linked list of packets. (This is the "buffer").
 * storage          pointer to optional storage backend to allocate packets
 *                  from rather than using malloc.
 */
typedef struct dfi_tx_packet_buffer_t
{
    uint16_t            ts_last_packet;
    List_t              list;
    packet_storage_t    *storage;
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
 * @brief   DFI TX Packet Buffer Initialization
 *
 * @details Initializes a TX packet buffer.
 *
 * @param[out]  buffer      pointer to tx packet buffer.
 *
 * @return      void
 */
void dfi_tx_packet_buffer_init(dfi_tx_packet_buffer_t *buffer);

/**
 * @brief   DFI TX Packet Buffer Free
 *
 * @details Frees all packets in the TX packet buffer.
 *
 * @param[in]   buffer  pointer to tx packet buffer to free.
 *
 * @return  void
 */
void dfi_tx_packet_buffer_free(dfi_tx_packet_buffer_t *buffer);

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
void dfi_rx_packet_buffer_data_compare(const dfi_rx_packet_buffer_t *buffer,
                                       const command_data_t *expected,
                                       wddr_dq_byte_t dq_byte,
                                       packet_data_mask_t data_mask,
                                       uint8_t num,
                                       uint8_t phases,
                                       uint8_t *is_same);

/**
 * @brief   Create CK Packet Sequence
 *
 * @details Creates a single packet that has only DCE field enabled for all
 *          phases.
 *
 * @param[in]   buffer      pointer to packet buffer to save packet to.
 * @param[in]   time_offset time offset from previous packet that the packet
 *                          should be sent.
 *
 * @return      returns whether packet added to the packet buffer.
 * @retval      WDDR_SUCCESS if added.
 * @retval      WDDR_ERROR otherwise.
 */
packet_item_t * create_ck_packet_sequence(dfi_tx_packet_buffer_t *buffer,
                                          uint16_t time_offset);

/**
 * @brief   Create CKE Packet Sequence
 *
 * @details Creates a single packet that has only DCE and CKE fields enabled for
 *          all phases.
 *
 * @param[in]   buffer      pointer to packet buffer to save packet to.
 * @param[in]   time_offset time offset from previous packet that the packet
 *                          should be sent.
 *
 * @return      returns whether packet added to the packet buffer.
 * @retval      WDDR_SUCCESS if added.
 * @retval      WDDR_ERROR otherwise.
 */
packet_item_t * create_cke_packet_sequence(dfi_tx_packet_buffer_t *buffer,
                                           uint16_t time_offset);

/**
 * @brief   Create Packet Group Info
 *
 * @details Creates group info required for filling packet with the appropriate
 *          phases and offsets being accounted for.
 *
 * @param[in]   group_info      pointer to group info structure to populate.
 * @param[in]   ratio           ratio of DFI CK to DRAM CK.
 * @param[in]   dram_clk_cycles group offset specified in DRAM cycles.
 * @param[in]   phase_length    duration of group specified in phases.
 *
 * @return      void.
 */
void create_packet_group_info(packet_group_info_t *group_info,
                              wddr_freq_ratio_t ratio,
                              uint16_t dram_clk_cycles,
                              uint16_t phase_length);

/**
 * @brief   Fill Address Packet
 *
 * @details Fills correct command phases for the specified packet group by
 *          populating cs and address data in packet.
 *
 * @param[in]   packet              pointer to packet to fill.
 * @param[in]   group_info          pointer to packet group info structure.
 * @param[in]   cycles_per_packet   number of DRAM cycles per packet
 * @param[in]   command             pointer to command structure to send.
 * @param[in]   phase_length        expected total phases to be populated.
 *
 * @return      void.
 */
void fill_address_packet(packet_item_t *packet,
                         packet_group_info_t *group_info,
                         uint8_t cycles_per_packet,
                         command_t *command,
                         uint16_t phase_length);

/**
 * @brief   Fill Read Data Packet
 *
 * @details Fills correct read phases for the specified packet group by
 *          populating rddata_cs in packet.
 *
 * @param[in]   packet              pointer to packet to fill.
 * @param[in]   group_info          pointer to packet group info structure.
 * @param[in]   cs                  chipselect value.
 * @param[in]   cycles_per_packet   number of DRAM cycles per packet
 *
 * @return      void.
 */
void fill_rddata_packet(packet_item_t *packet,
                        packet_group_info_t *group_info,
                        chipselect_t cs,
                        uint8_t cycles_per_packet);

/**
 * @brief   Fill Write Data Enable Packet
 *
 * @details Fills correct write phases for the specified packet group by
 *          populating wrdata_en in packet.
 *
 * @param[in]   packet              pointer to packet to fill.
 * @param[in]   group_info          pointer to packet group info structure.
 * @param[in]   cycles_per_packet   number of DRAM cycles per packet
 *
 * @return      void.
 */
void fill_wrdata_en_packet(packet_item_t *packet,
                           packet_group_info_t *group_info,
                           uint8_t cycles_per_packet);

/**
 * @brief   Fill Write Data Packet
 *
 * @details Fills correct write phases for the specified packet group by
 *          populating wrdata, wrdata_mask, and wrdata_cs in packet.
 *
 * @param[in]   packet              pointer to packet to fill.
 * @param[in]   group_info          pointer to packet group info structure.
 * @param[in]   cs                  chipselect value.
 * @param[in]   data                pointer to command data structure to send.
 * @param[in]   cycles_per_packet   number of DRAM cycles per packet
 * @param[in]   phase_length        expected total phases to be populated.
 *
 * @return      void.
 */
void fill_wrdata_packet(packet_item_t *packet,
                        packet_group_info_t *group_info,
                        chipselect_t cs,
                        const command_data_t *data,
                        uint8_t cycles_per_packet,
                        uint16_t phase_length);

#endif /* _DFI_PACKET_H_ */
