/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _FIRMWARE_PHY_TASK_H_
#define _FIRMWARE_PHY_TASK_H_

#include <firmware/firmware.h>

/**
 * @brief   PHY Firmware Events
 *
 *  BOOT                Event used to boot the PHY.
 *  PREP                Event used to request PHY Manager to prep for a switch.
 *  PHYUPD_REQ          Event used to request PHYUPD.
 *  PHYMSTR_REQ         Event used to request PHYMSTR control.
 *  PHYMSTR_EXIT        Event used to exit PHYMSTR control.
 *  INIT_START          Event used to indicate INIT_START assert occurred.
 *  INIT_COMPLETE       Event used to indicate INIT_COMPLETE ack occurred.
 *  PLL_INIT_LOCK       Event used to indicate PLL initally locked.
 *  PLL_LOCK            Event used to indicate PLL locked.
 *  PLL_LOSS_LOCK       Event used to indicate PLL lost lock.
 *  PHYMSTR_ACK         Event used to indicate that PHYMSTR_REQ was acknowledged.
 *  PHYMSTR_ABORT       Event used to indicate that PHYMSTR_REQ should be aborted.
 *  PHYUPD_ACK          Event used to indicate that PHUPD_REQ was acknowledged.
 *  PHYUPD_ABORT        Event used to indicate that PHUPD_REQ should be aborted.
 *  CTRLUPD_ASSERT      Event used to indicate CTRLUPD REQ was asserted.
 *  CTRLUPD_DEASSERT    Event used to indicate CTRLUPD REQ was dasserted.
 *  LP_DATA_REQ         Event used to indicate LP_DATA REQ was asserted.
 *  LP_CTRL_REQ         Event used to indicate LP_CTRL_REQ was asserted.
 */
typedef enum firmware_phy_event
{
    FW_PHY_EVENT_BOOT,
    FW_PHY_EVENT_PREP,
    FW_PHY_EVENT_PHYUPD_REQ,
    FW_PHY_EVENT_PHYMSTR_REQ,
    FW_PHY_EVENT_PHYMSTR_EXIT,
    FW_PHY_EVENT_INIT_START,
    FW_PHY_EVENT_INIT_COMPLETE,
    FW_PHY_EVENT_PLL_INIT_LOCK,
    FW_PHY_EVENT_PLL_LOCK,
    FW_PHY_EVENT_PLL_LOSS_LOCK,
    FW_PHY_EVENT_PHYMSTR_ACK,
    FW_PHY_EVENT_PHYMSTR_ABORT,
    FW_PHY_EVENT_PHYUPD_ACK,
    FW_PHY_EVENT_PHYUPD_ABORT,
    FW_PHY_EVENT_CTRLUPD_ASSERT,
    FW_PHY_EVENT_CTRLUPD_DEASSERT,
    FW_PHY_EVENT_LP_DATA_REQ,
    FW_PHY_EVENT_LP_CTRL_REQ,
    FW_PHY_EVENT_NUM,
} fw_phy_event_t;

/**
 * @brief   Firmware PHY Task Initialization
 *
 * @details Initializes PHY Firmware Task.
 *
 * @return  void.
 */
void fw_phy_task_init(void);

/**
 * @brief   Firmware PHY Task Notify
 *
 * @details Notify firmware PHY task of an event via the given message.
 *
 * @param[in]   msg     pointer to message to send.
 *
 * @return      void.
 */
void fw_phy_task_notify(fw_msg_t *msg);

/**
 * @brief   Firmware PHY Task Notify ISR
 *
 * @details ISR friendly version of Firmware PHY Task Notify.
 *
 * @param[in]   msg                         pointer to message to send.
 * @param[out]  pxHigherPriorityTaskWoken   set to indicate if higher priority
 *                                          task was woken.
 *
 * @return      void.
 */
void fw_phy_task_notify_isr(fw_msg_t *msg,
                            BaseType_t *pxHigherPriorityTaskWoken);

#endif /* _FIRMWARE_PHY_TASK_H_ */
