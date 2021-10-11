/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* Firmware includes. */
#include <firmware/phy_task.h>

/*******************************************************************************
**                                   MACROS
*******************************************************************************/
#define PREP_TIMEOUT        (pdMS_TO_TICKS(5)) // 5 milliseconds
#define BOOT_TRY_COUNT      (1)
#define PREP_TRY_COUNT      (3)

/*******************************************************************************
**                            FUNCTION DECLARATIONS
*******************************************************************************/
/** Internal Function for sending blocking message to the FW */
static UBaseType_t __send_fw_msg(fw_msg_t *msg,
                                 TickType_t xTicksToWait,
                                 uint8_t try_count);

/*******************************************************************************
**                              IMPLEMENTATIONS
*******************************************************************************/
void firmware_phy_init(void)
{
    fw_phy_task_init();
}

UBaseType_t firmware_phy_start(bool calibrate, bool train_dram)
{
    fw_msg_t msg;
    fw_phy_start_cfg_t cfg = {
        .calibrate = calibrate,
        .train_dram = train_dram,
    };
    msg.event = FW_PHY_EVENT_BOOT;
    msg.data = &cfg;
    return __send_fw_msg(&msg, portMAX_DELAY, BOOT_TRY_COUNT);
}

UBaseType_t firmware_phy_prep_switch(uint8_t freq_id)
{
    fw_msg_t msg;
    msg.event = FW_PHY_EVENT_PREP;
    msg.data = (void *) (uintptr_t) freq_id;
    return __send_fw_msg(&msg, PREP_TIMEOUT, PREP_TRY_COUNT);
}

static UBaseType_t __send_fw_msg(fw_msg_t *msg, TickType_t xTicksToWait, uint8_t ucTryCount)
{
    UBaseType_t resp;

    msg->xSender = xTaskGetCurrentTaskHandle();

    // Clear notifications
    xTaskNotifyWait(0, 0, NULL, 0);

    do
    {
        // Send message
        fw_phy_task_notify(msg);

        // Wait for it to be handled
        xTaskNotifyWait(0, 0, &resp, xTicksToWait);

        if (resp == FW_RESP_RETRY)
        {
            continue;
        }

        return resp == FW_RESP_SUCCESS ? pdPASS : pdFAIL;
    } while (--ucTryCount);

    return pdFAIL;
}
