/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* Kernel includes. */
#include <kernel/messenger.h>

/* LPDDR includes. */
#include <wddr/memory_map.h>
#include <wddr/irq_map.h>
#include <messenger/device.h>
#include <messenger/messages_wddr.h>

/**
 * @brief   Messenger Validate Register Interface
 *
 * @details Register Interface function for validating a message from the MCU.
 *
 * @param[in]   dev         generic pointer to messenger device.
 * @param[out]  buff        pointer to where to store data.
 * @param[in]   len         amount of data to send.
 *
 * @return      returns if message is valid.
 * @retval      true if message is valid.
 * @retval      false otherwise.
 */
static bool wddr_messenger_validate(void *dev, void *buff, size_t len);

void wddr_messenger_init(wddr_message_interface_t *messenger)
{
    xMessengerInterfaceRegisterPhyInterface((void *) messenger,
                                            MEMORY_MAP_WAV_MCU_INTF,
                                            messenger_send_reg_if,
                                            messenger_receive_reg_if,
                                            wddr_messenger_validate);

    messenger_init_reg_if(messenger, MEMORY_MAP_WAV_MCU_INTF, MCU_FAST_IRQ_HOST2PHY_REQ, MCU_FAST_IRQ_PHY2HOST_ACK);
}

void wddr_messenger_send(wddr_message_interface_t *messenger, Message_t *message)
{
    xSendMessage(messenger->address, message);
}

bool wddr_messenger_receive(wddr_message_interface_t *messenger, Message_t *message)
{
    return (xReceiveMessage(messenger->address, message) == pdTRUE);
}

static bool wddr_messenger_validate(void *dev, void *buff, size_t len)
{
    if (messenger_validate_reg_if(dev, buff, len))
    {
        return true;
    }

    Message_t *msg = (Message_t *) buff;

    switch(msg->id)
    {
        case MESSAGE_WDDR_FREQ_PREP_REQ:
        case MESSAGE_WDDR_FREQ_PREP_RESP:
            return true;
        default:
            break;
    }

    return false;
}
