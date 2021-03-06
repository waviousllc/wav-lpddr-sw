/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _WDDR_IRQ_MAP_H_
#define _WDDR_IRQ_MAP_H_

#define FAST_IRQ_STICKY_MASK(irq)       (1 << irq)

// IRQ Defintions (28 real interrupts)
// Used for Sticky Bits
typedef enum wddr_irq_t
{
    DDR_IRQ_HOST2PHY_REQ,
    DDR_IRQ_PHY2HOST_ACK,
    DDR_IRQ_CH0_IBUF_EMPTY,
    DDR_IRQ_CH0_IBUF_FULL,
    DDR_IRQ_CH0_EBUF_NOT_EMPTY,
    DDR_IRQ_CH0_EBUF_FULL,
    DDR_IRQ_INIT_START,
    DDR_IRQ_INIT_COMPLETE,
    DDR_IRQ_LP_DATA_REQ,
    DDR_IRQ_LP_CTRL_REQ,
    DDR_IRQ_PLL,
    DDR_IRQ_EXT_0,
    DDR_IRQ_EXT_1,
    DDR_IRQ_CTRLUPD_REQ_ASSERTION,
    DDR_IRQ_CTRLUPD_REQ_DEASSERTION,
    DDR_IRQ_PHYUPD_ACK,
    DDR_IRQ_PHYMSTR_ACK,
    DDR_IRQ_AHB_ADR_DET_0,
    DDR_IRQ_AHB_ADR_DET_1,
    DDR_IRQ_CH1_0_IDX,
    DDR_IRQ_CH1_1_IDX,
    DDR_IRQ_CH0_IBUF_OVERFLOW,
    DDR_IRQ_CH0_EBUF_OVERFLOW,
    DDR_IRQ_CH1_IBUF_EMPTY,
    DDR_IRQ_CH1_IBUF_FULL,
    DDR_IRQ_CH1_EBUF_NOT_EMPTY,
    DDR_IRQ_CH1_EBUF_FULL,
    DDR_IRQ_CH1_IBUF_OVERFLOW,
    DDR_IRQ_CH1_EBUF_OVERFLOW,
} wddr_irq_t;

// Fast IRQ Defintions (15 bus interrupts connected to MCU)
typedef enum mcu_fast_irq_t
{
    MCU_FAST_IRQ_HOST2PHY_REQ = 16,
    MCU_FAST_IRQ_PHY2HOST_ACK,
    MCU_FAST_IRQ_IBUF,
    MCU_FAST_IRQ_EBUF,
    MCU_FAST_IRQ_INIT_START,
    MCU_FAST_IRQ_INIT_COMPLETE,
    MCU_FAST_IRQ_LP_REQ,
    MCU_FAST_IRQ_PLL,
    MCU_FAST_IRQ_EXT_0,
    MCU_FAST_IRQ_EXT_1,
    MCU_FAST_IRQ_CTRLUPD_REQ,
    MCU_FAST_IRQ_PHYUPD_ACK,
    MCU_FAST_IRQ_PHYMSTR_ACK,
    MCU_FAST_IRQ_AHB_DET,
    MCU_FAST_IRQ_CH1,
} mcu_fast_irq_t;

#endif /* _WDDR_IRQ_MAP_H_ */
