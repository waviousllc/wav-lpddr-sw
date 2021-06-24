/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
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
#include <kernel/notification.h>

/* LPDDR includes. */
#include <wddr/memory_map.h>
#include <wddr/device.h>
#include <clk/driver.h>
#include <fsw/driver.h>

/*******************************************************************************
**                                   MACROS
*******************************************************************************/
#define WDDR_BASE_ADDR              (0x00000000)

// Task priority least to greatest
#define FSM_TASK_PRIORITY           (tskIDLE_PRIORITY + 4)
#define MAIN_TASK_PRIORITY          (tskIDLE_PRIORITY + 5)
#define NOTIF_TASK_PRIORITY         (tskIDLE_PRIORITY + 6)

// Event Queues
#define FSM_TASK_QUEUE_LEN          (20)    // 20 FSM events oustanding at a time


/*******************************************************************************
**                            FUNCTION DECLARATIONS
*******************************************************************************/
static void vMainTask( void *pvParameters );
static void prvSetupHardware( void );

/*******************************************************************************
**                           VARIABLE DECLARATIONS
*******************************************************************************/
static DECLARE_WDDR_TABLE(table);
/** @note Place in .data section at cost of image size */
static wddr_dev_t wddr __attribute__ ((section (".data"))) = {0};

extern uint32_t __start;
img_hdr_t image_hdr __attribute__((section(".image_hdr"))) = {
    .image_magic = IMAGE_MAGIC,
    .image_hdr_version = IMAGE_VERSION_CURRENT,
    .image_type = IMAGE_TYPE_APP,
    .version_major = 1,
    .version_minor = 0,
    .version_patch = 0,
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
    // Setup Hardware
    prvSetupHardware();

    // Initialize Notification Task
    xNotificationTaskInit(NOTIF_TASK_PRIORITY, configMINIMAL_STACK_SIZE);

    // Initialize FSM Task
    xFSMTaskInit(FSM_TASK_PRIORITY, configMINIMAL_STACK_SIZE, FSM_TASK_QUEUE_LEN);

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
    wddr_init(&wddr, WDDR_BASE_ADDR, &table);
    if (wddr_boot(&wddr) != WDDR_SUCCESS)
    {
        reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_GP3_CFG__ADR, 0x10001);
    }

    vTaskDelete(NULL);
}

/*-----------------------------------------------------------*/
static void prvSetupHardware( void )
{
    // Initialize
    common_path_init(&wddr.cmn, WDDR_MEMORY_MAP_CMN);
    pll_init(&wddr.pll, WDDR_MEMORY_MAP_PLL);

    // Enable Common Block
    common_path_enable(&wddr.cmn);

    // Boot PLL to set MCU clk @ 384 MHz
    pll_boot(&wddr.pll);

    // Enable Clocks
    clk_ctrl_set_pll_clk_en_reg_if(true);
    clk_ctrl_set_mcu_gfm_sel_reg_if(CLK_MCU_GFM_SEL_PLL_VCO0);
    clk_cmn_ctrl_set_pll0_div_clk_rst_reg_if(false);
    clk_cmn_ctrl_set_gfcm_en_reg_if(true);

    // Turn off PHY CLK gating
    fsw_csp_set_clk_disable_ovr_val_reg_if(false);

    clk_cmn_ctrl_set_pll0_div_clk_en_reg_if(true);
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
    taskDISABLE_INTERRUPTS();
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_GP3_CFG__ADR, 0x20001);
    _exit(1);
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
    taskDISABLE_INTERRUPTS();
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_GP3_CFG__ADR, 0x30001);
    _exit(1);
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

    if (strcmp(pcFileName, "port.c") && ulLine == 161)
    {
        return;
    }

    taskDISABLE_INTERRUPTS();
    // Write out the file and line number
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_GP1_CFG__ADR, ulLine);
    while (*pcString != '\0')
    {
        reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_GP2_CFG__ADR, *pcString++);
    }
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_GP3_CFG__ADR, 0x40001);
    _exit(1);
}
