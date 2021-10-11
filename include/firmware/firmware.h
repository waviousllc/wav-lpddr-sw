/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _FIRMWARE_H_
#define _FIRMWARE_H_

#include <FreeRTOS.h>
#include <task.h>

/**
 * @brief   Firmware Response Enumerations
 *
 * SUCCESS  firmware request was successful.
 * FAILURE  firmware request failed.
 * RETRY    firmware request couldn't be processed. Caller should retry.
 */
typedef enum firmware_response
{
    FW_RESP_SUCCESS,
    FW_RESP_FAILURE,
    FW_RESP_RETRY,
    FW_RESP_NUM,
} fw_response_t;

/**
 * @brief  Firmware Message Structure
 *
 * @details Structure passed to PHY firmware for processing.
 *
 * event    Firmware event associated with the message.
 * data     pointer to message specific data to process.
 * xSender  pointer to task that sent the message.
 */
typedef struct firmware_message
{
    uint8_t event;
    void *data;
    TaskHandle_t xSender;
} fw_msg_t;

#endif /* _FIRMWARE_H_ */
