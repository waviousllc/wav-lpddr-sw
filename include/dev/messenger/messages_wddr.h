/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _MESSAGES_WDDR_H_
#define _MESSAGES_WDDR_H_

#include <messenger/messages_general.h>

/**
 * @brief   WDDR Specific Message Enumeration
 *
 * @details These messages are supported by the WDDR device.
 *
 * FREQ_PREP_REQ    frequency prep request message.
 * FREQ_PREP_RESP   frequency prep response message.
 * END_OF_MESSAGES  indicates number of general messages.
 */
typedef enum messages_wddr_t {
    MESSAGE_WDDR_FREQ_PREP_REQ = 0x00020002,
    MESSAGE_WDDR_FREQ_PREP_RESP,
    MESSAGE_WDDR_END_OF_MESSAGES,
} messages_wddr_t;

#define WDDR_FREQ_PREP_REQ__FREQ_ID__MSK        (0x000000FF)
#define WDDR_FREQ_PREP_REQ__FREQ_ID__SHFT       (0x00000000)

#define WDDR_FREQ_PREP_RSP__STATUS__MSK         (0x000000FF)
#define WDDR_FREQ_PREP_RSP__STATUS__SHFT        (0x00000000)
#define WDDR_FREQ_PREP_RSP__FREQ_ID__MSK        (0x0000FF00)
#define WDDR_FREQ_PREP_RSP__FREQ_ID__SHFT       (0x00000008)
#define WDDR_FREQ_PREP_RSP__RESP_CODE__MSK      (0x00FF0000)
#define WDDR_FREQ_PREP_RSP__RESP_CODE__SHFT     (0x00000010)

#endif /* _MESSAGES_WDDR_H_ */
