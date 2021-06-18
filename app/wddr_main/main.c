/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */

/* Standard includes. */
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Kernel includes. */
#include <kernel/io.h>
#include <kernel/irq.h>
#include <kernel/messenger.h>
#include <kernel/notification.h>

/* LPDDR includes. */
#include <wddr/memory_map.h>
#include <wddr/irq_map.h>
#include <wddr/notification_map.h>
#include <wddr/device.h>
#include <clk/driver.h>
#include <fsw/driver.h>
#include <messenger/device.h>
#include <messenger/messages_wddr.h>

/*******************************************************************************
**                                   MACROS
*******************************************************************************/
#define WDDR_BASE_ADDR              (0x00000000)
#define TIMER_INTERRUPT_MSK         (1 << 0x7)
#define MESSENGER_REQ_INTERRUPT_MSK (1 << MCU_FAST_IRQ_HOST2PHY_REQ)
#define MESSENGER_ACK_INTERRUPT_MSK (1 << MCU_FAST_IRQ_PHY2HOST_ACK)

// Task priority least to greatest
#define MESSENGER_TASK_PRIORITY     (tskIDLE_PRIORITY + 3)
#define FSM_TASK_PRIORITY           (tskIDLE_PRIORITY + 4)
#define MAIN_TASK_PRIORITY          (tskIDLE_PRIORITY + 5)
#define NOTIF_TASK_PRIORITY         (tskIDLE_PRIORITY + 6)

// Event Queues
#define MESSENGER_TASK_QUEUE_LEN    (1)     // only 1 outstanding message at a time
#define FSM_TASK_QUEUE_LEN          (20)    // 20 FSM events oustanding at a time

/*******************************************************************************
**                            FUNCTION DECLARATIONS
*******************************************************************************/
static void vMainTask( void *pvParameters );
static void prvSetupHardware( void );
/** @brief  Internal function to handle received messages */
static void handle_message(const Message_t *message);

/*******************************************************************************
**                           VARIABLE DECLARATIONS
*******************************************************************************/
static DECLARE_WDDR_TABLE(table);
/** @note Place in .data section at cost of image size */
static wddr_dev_t wddr __attribute__ ((section (".data"))) = {0};
static wddr_message_interface_t message_intf;

/*******************************************************************************
**                              IMPLEMENTATIONS
*******************************************************************************/
int main( void )
{
    // Setup Hardware
    prvSetupHardware();

    // Initialize FSM Task
    xFSMTaskInit(FSM_TASK_PRIORITY, configMINIMAL_STACK_SIZE, FSM_TASK_QUEUE_LEN);

    // Initialize Messenger Task
    xMessengerTaskInit(MESSENGER_TASK_PRIORITY, configMINIMAL_STACK_SIZE, MESSENGER_TASK_QUEUE_LEN);

    // Initialize Notification Task
    xNotificationTaskInit(NOTIF_TASK_PRIORITY, configMINIMAL_STACK_SIZE);

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

    // Initialize WDDR
    wddr_init(&wddr, WDDR_BASE_ADDR, &table);
    if (wddr_boot(&wddr) != WDDR_SUCCESS)
    {
        reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_GP3_CFG__ADR, 0x10001);
    }

    /**
     * Drop priority and yield to finish low priority tasks before boot
     * message is sent to the host. Once all tasks complete, control will
     * be given back to vMainTask.
     */
    vTaskPrioritySet(NULL, tskIDLE_PRIORITY + 1);
    taskYIELD();
    vTaskPrioritySet(NULL, MAIN_TASK_PRIORITY);

    // Send Boot Complete Message
    wddr_messenger_send(&message_intf, &message);

    // Main receiving loop
    for (;;)
    {
        if (wddr_messenger_receive(&message_intf, &message))
        {
            handle_message(&message);
        }
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
void vAssertCalled( void )
{
    taskDISABLE_INTERRUPTS();
    _exit(1);
}

/*-----------------------------------------------------------*/
void handle_message(const Message_t *message)
{
    uint8_t freq_id;
    Message_t resp_msg = {0, 0};
    switch(message->id)
    {
        // Prep Request
        case MESSAGE_WDDR_FREQ_PREP_REQ:
            /**
             * Disable unecessary interrupts that would increase prep time if
             * fired.
             */
            interrupt_disable(TIMER_INTERRUPT_MSK |
                              MESSENGER_REQ_INTERRUPT_MSK |
                              MESSENGER_ACK_INTERRUPT_MSK);

            // Extract Frequency ID
            freq_id = GET_REG_FIELD(message->data, WDDR_FREQ_PREP_REQ__FREQ_ID);

            // Prepare for switch
            if (wddr_prep_switch(&wddr, freq_id) != WDDR_SUCCESS)
            {
                // Mark as failure
                resp_msg.data = UPDATE_REG_FIELD(0x0, WDDR_FREQ_PREP_RSP__STATUS, 0x1);
            }

            resp_msg.id = MESSAGE_WDDR_FREQ_PREP_RESP;
            resp_msg.data = UPDATE_REG_FIELD(resp_msg.data, WDDR_FREQ_PREP_RSP__FREQ_ID, freq_id);
            wddr_messenger_send(&message_intf, &resp_msg);

            // Re-enable interrupts
            interrupt_enable(TIMER_INTERRUPT_MSK |
                             MESSENGER_REQ_INTERRUPT_MSK |
                             MESSENGER_ACK_INTERRUPT_MSK);
            break;
        default:
            break;
    }
}
