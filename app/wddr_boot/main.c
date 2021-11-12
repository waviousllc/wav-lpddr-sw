/**
 * WDDR Boot  - Application that boots Wavious LPDDR PHY HW
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
#include <kernel/printf.h>

/* PHY Firmware includes. */
#include <wddr/memory_map.h>
#include <firmware/phy_api.h>

/*******************************************************************************
**                                   MACROS
*******************************************************************************/
#define MAIN_TASK_PRIORITY          (tskIDLE_PRIORITY + 3)

// Boot configuration
#define BOOT_CALIBRATION            (true)
#define BOOT_TRAINING               (false)

/*******************************************************************************
**                            FUNCTION DECLARATIONS
*******************************************************************************/
static void vMainTask( void *pvParameters );
/** @brief  Centralized shutdown function */
static void shutdown(uint32_t cause);

/*******************************************************************************
**                           VARIABLE DECLARATIONS
*******************************************************************************/
extern uintptr_t __start;
img_hdr_t image_hdr __attribute__((section(".image_hdr"))) = {
    .image_magic = IMAGE_MAGIC,
    .image_hdr_version = IMAGE_VERSION_CURRENT,
    .image_type = IMAGE_TYPE_APP,
    .version_major = FW_VERSION_MAJOR,
    .version_minor = FW_VERSION_MINOR,
    .version_patch = FW_VERSION_PATCH,
    .vector_size = VECTOR_SIZE,
    .vector_addr = (uintptr_t) &__start,
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
    // Boot PHY (calibrate but don't train)
    if (firmware_phy_start(BOOT_CALIBRATION, BOOT_TRAINING) == pdFAIL)
    {
        shutdown(1);
    }

    // Loop forever
    while(1);
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
    shutdown(2);
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
    shutdown(3);
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
     *          will fail for port.c
     */
    memcpy(&cFileName[0], &pcString[ulFileNameLen - 6], 6);

    // Ignore asserts from port.c
    if (!strcmp(pcFileName, "port.c"))
    {
        return;
    }

    // Write out the file and line number
    configPRINTF(("ERROR: Aserrtion in %s on line %lu.\n", pcFileName, ulLine));
    taskDISABLE_INTERRUPTS();
    shutdown(4);
}

/*-----------------------------------------------------------*/
static void shutdown(uint32_t cause)
{
    configPRINTF(("Shutdown\n"));
    taskDISABLE_INTERRUPTS();
    _exit(cause);
}
