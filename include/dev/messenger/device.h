/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#ifndef _MESSENGER_DEV_H_
#define _MESSENGER_DEV_H_

/* Standard includes. */
#include <stdbool.h>

/* Kernel includes. */
#include <messenger/driver.h>
#include <kernel/messenger.h>

/** @brief  WDDR Message Interface Structure */
typedef message_interface_t wddr_message_interface_t;

/**
 * @brief   WDDR Messenger Interface Initialization
 *
 * @details Initializes WDDR Messenger Interface.
 *
 * @param[in]   messenger   pointer to WDDR Messenger Interface.
 *
 * @return      void
 */
void wddr_messenger_init(wddr_message_interface_t *messenger);

/**
 * @brief   WDDR Messenger Interface Send Message
 *
 * @details Initializes WDDR Messenger Interface.
 *
 * @param[in]   messenger   pointer to WDDR Messenger Interface.
 * @param[out]  message     pointer to message to send.
 *
 * @return      void
 */
void wddr_messenger_send(wddr_message_interface_t *messenger, Message_t *message);

/**
 * @brief   WDDR Messenger Interface Receive Message
 *
 * @details Initializes WDDR Messenger Interface.
 *
 * @param[in]   messenger   pointer to WDDR Messenger Interface.
 * @param[out]  message     pointer to store received message.
 *
 * @return      if message has been successfully received.
 * @retval      true if message received successfully.
 * @retval      false if message not able to be received.
 */
bool wddr_messenger_receive(wddr_message_interface_t *messenger, Message_t *message);

#endif /* _MESSENGER_DEV_H_ */
