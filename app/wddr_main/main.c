/**
 * WDDR Main  - Application that manages Wavious LPDDR PHY HW
 *
 * Copyright (c) 2021 Wavious LLC.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* Standard includes. */
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* Image header includes. */
#include <image.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Kernel includes. */
#include <kernel/io.h>
#include <kernel/irq.h>
#include <kernel/messenger.h>

/* Messenger includes. */
#include <messenger/device.h>
#include <messenger/messages_wddr.h>

/* PHY Firmware includes. */
#include <wddr/memory_map.h>
#include <wddr/irq_map.h>
#include <firmware/phy_api.h>

/*******************************************************************************
**                                   MACROS
*******************************************************************************/
#define MESSENGER_REQ_INTERRUPT_MSK (1 << MCU_FAST_IRQ_HOST2PHY_REQ)
#define MESSENGER_ACK_INTERRUPT_MSK (1 << MCU_FAST_IRQ_PHY2HOST_ACK)

// Task priority least to greatest
#define MESSENGER_TASK_PRIORITY     (tskIDLE_PRIORITY + 2)
#define MAIN_TASK_PRIORITY          (tskIDLE_PRIORITY + 3)

/*******************************************************************************
**                            FUNCTION DECLARATIONS
*******************************************************************************/
static void vMainTask( void *pvParameters );
/** @brief  Internal function to handle received messages */
static void handle_message(const Message_t *message);
/** @brief  Centralized shutdown function */
static void shutdown(uint32_t cause);

/*******************************************************************************
**                           VARIABLE DECLARATIONS
*******************************************************************************/
static wddr_message_interface_t message_intf;

extern uint32_t __start;
img_hdr_t image_hdr __attribute__((section(".image_hdr"))) = {
    .image_magic = IMAGE_MAGIC,
    .image_hdr_version = IMAGE_VERSION_CURRENT,
    .image_type = IMAGE_TYPE_APP,
    .version_major = FW_VERSION_MAJOR,
    .version_minor = FW_VERSION_MINOR,
    .version_patch = FW_VERSION_PATCH,
    .vector_addr = (uint32_t) &__start,
    .device_id = IMAGE_DEVICE_ID_HOST,
    .git_dirty = GIT_DIRTY,
    .git_ahead = GIT_AHEAD,
    .git_sha = GIT_SHA,
    // populated as part of a post compilation step
    .crc = 0,
    .data_size = 0,
};

/*******************************************************************************
**                              IMPLEMENTATIONS
*******************************************************************************/
int main( void )
{
    // Initialize PHY Firmware
    firmware_phy_init();

    // Create Messenger Task
    xMessengerTaskInit(MESSENGER_TASK_PRIORITY, configMINIMAL_STACK_SIZE, 1);

    /* At this point, you can create queue,semaphore, task requested for your application */
    xTaskCreate( vMainTask, "Main Task", configMINIMAL_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, NULL );

    /* Start the tasks and timer running. */
    /* Here No task are defined, so if we start the Scheduler 2 tasks will running (Timer and Idle) */
    /* This task will run forever without doing nothing, it is why the following line is commented */
    vTaskStartScheduler();

    /* If all is well, the scheduler will now be running, and the following
    line will never be reached.  If the following line does execute, then
    there was insufficient FreeRTOS heap memory available for the Idle and/or
    timer tasks to be created.
    or task have stoppped the Scheduler */
}

/*-----------------------------------------------------------*/
static void vMainTask( void *pvParameters )
{
    // Boot Complete Message as Initial Message
    Message_t message = {
        .id = MESSAGE_GENERAL_MCU_BOOT_RESP,
        .data = 0x00,
    };

    // Initialize messenger interface
    wddr_messenger_init(&message_intf);

    // TODO: Send MSG Interface Ready message
    // TODO: Receive Boot Message to continue

    // Boot PHY
    if (firmware_phy_start() == pdFAIL)
    {
        shutdown(0x10001);
    }

    // Send Boot Complete Message
    wddr_messenger_send(&message_intf, &message);

    // Main receiving loop
    for (;;)
    {
        if (wddr_messenger_receive(&message_intf, &message))
        {
            // Process message
            handle_message(&message);
        }
    }
}

/*-----------------------------------------------------------*/
void vApplicationMallocFailedHook( void )
{
    /* vApplicationMallocFailedHook() will only be called if
    configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
    function that will get called if a call to pvPortMalloc() fails.
    pvPortMalloc() is called internally by the kernel whenever a task, queue,
    timer or semaphore is created.  It is also called by various parts of the
    demo application.  If heap_1.c or heap_2.c are used, then the size of the
    heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
    FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
    to query the size of free heap space that remains (although it does not
    provide information on how the remaining heap might be fragmented). */
    shutdown(0x20001);
}

/*-----------------------------------------------------------*/
void vApplicationIdleHook( void )
{
    /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
    to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
    task.  It is essential that code added to this hook function never attempts
    to block in any way (for example, call xQueueReceive() with a block time
    specified, or call vTaskDelay()).  If the application makes use of the
    vTaskDelete() API function (as this demo application does) then it is also
    important that vApplicationIdleHook() is permitted to return to its calling
    function, because it is the responsibility of the idle task to clean up
    memory allocated by the kernel to any task that has since been deleted. */
}

/*-----------------------------------------------------------*/
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) pxTask;

    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected. */
    shutdown(0x30001);
}

/*-----------------------------------------------------------*/
void vApplicationTickHook( void )
{
    /* The tests in the full demo expect some interaction with interrupts. */
}

/*-----------------------------------------------------------*/
void vAssertCalled( const char * const pcFileName, unsigned long ulLine )
{
    const char *pcString = pcFileName;
    uint32_t ulFileNameLen = strlen(pcFileName);
    char cFileName[7] = {'\0'};

    /**
     * @note    This is a patch because on this platform it is known that assert
     *          will fail for port.c line 161.
     */
    memcpy(&cFileName[0], &pcString[ulFileNameLen - 6], 6);

    if (!strcmp(&cFileName[0], "port.c") && ulLine == 161)
    {
        return;
    }

    // Write out the file and line number
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_GP1_CFG__ADR, ulLine);
    while (*pcString != '\0')
    {
        reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_GP2_CFG__ADR, *pcString++);
    }
    shutdown(0x40001);
}

/*-----------------------------------------------------------*/
void handle_message(const Message_t *message)
{
    uint8_t freq_id;
    Message_t resp_msg = {0, 0};

    // Disable message interrupts while processing
    interrupt_disable(MESSENGER_REQ_INTERRUPT_MSK |
                      MESSENGER_ACK_INTERRUPT_MSK);

    switch(message->id)
    {
        // Prep Request
        case MESSAGE_WDDR_FREQ_PREP_REQ:
            // Extract Frequency ID
            freq_id = GET_REG_FIELD(message->data, WDDR_FREQ_PREP_REQ__FREQ_ID);

            // Prepare for switch
            if (firmware_phy_prep_switch(freq_id) == pdFAIL)
            {
                // Mark as failure
                resp_msg.data = UPDATE_REG_FIELD(0x0, WDDR_FREQ_PREP_RSP__STATUS, 0x1);
            }

            resp_msg.id = MESSAGE_WDDR_FREQ_PREP_RESP;
            resp_msg.data = UPDATE_REG_FIELD(resp_msg.data, WDDR_FREQ_PREP_RSP__FREQ_ID, freq_id);
            wddr_messenger_send(&message_intf, &resp_msg);
            break;
        default:
            break;
    }

    // Re-enable messaging interrupts
    interrupt_enable(MESSENGER_REQ_INTERRUPT_MSK |
                     MESSENGER_ACK_INTERRUPT_MSK);
}

/*-----------------------------------------------------------*/
static void shutdown(uint32_t cause)
{
    taskDISABLE_INTERRUPTS();
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_GP3_CFG__ADR, cause);
    _exit(1);
}
