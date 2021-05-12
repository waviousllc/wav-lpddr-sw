/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
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
#include <kernel/completion.h>

/* LPDDR includes. */
#include <pll/device.h>
#include <pll/fsm.h>
#include <path/common.h>
#include <wddr/memory_map.h>

/*******************************************************************************
**                                   MACROS
*******************************************************************************/
#define PHY_BOOT_FREQ   (2)

/*******************************************************************************
**                            FUNCTION DECLARATIONS
*******************************************************************************/
static void vMainTask( void *pvParameters );
static void prvSetupHardware( void );
static void pll_state_change_callback(fsm_t *fsm, uint8_t state, void *args);

/*******************************************************************************
**                           VARIABLE DECLARATIONS
*******************************************************************************/
static Completion_t lock_event;
static pll_dev_t pll;
static pll_fsm_t pll_fsm;
static common_path_t common_path;

// 422 MHz Configuration
static pll_freq_cal_t pll_cal = {
    .vco_cal[0] = {.band = 0x3, .fine = 0x11},
    .vco_cal[1] = {.band = 0x3, .fine = 0x11},
};

static pll_freq_cfg_t pll_cfg = {
    .vco_cfg[0] = {
        .fll_range = 2,
        .fll_refclk_count = 63,
        .fll_vco_count_target = 704,
        .int_comp = 22,
        .prop_gain = 1,
        .lock_count_threshold = 2,
        .post_div = 1,
    },
    .vco_cfg[1] = {
        .fll_range = 2,
        .fll_refclk_count = 63,
        .fll_vco_count_target = 704,
        .int_comp = 22,
        .prop_gain = 1,
        .lock_count_threshold = 2,
        .post_div = 1,
    },
};
/*******************************************************************************
**                              IMPLEMENTATIONS
*******************************************************************************/
int main( void )
{
    // Initialize FSM Task
    xFSMTaskInit();

    // Setup Hardware
    prvSetupHardware();

    /* At this point, you can create queue,semaphore, task requested for your application */
    xTaskCreate( vMainTask, "Main Task", configMINIMAL_STACK_SIZE, NULL, 2, NULL );

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
    pll_prep_data_t prep_data = {
        .freq_id = PHY_BOOT_FREQ,
        .cal = &pll_cal,
        .cfg = &pll_cfg,
    };

    // Calibrate VCOs
    pll_calibrate_vco(&pll, &pll_cal, &pll_cfg);

    // Initialize FSM
    pll_fsm_init(&pll_fsm, &pll);
    fsm_register_state_change_callback(&pll_fsm, pll_state_change_callback, NULL);

    // Initialize
    vInitCompletion(&lock_event);

    // Prepare PLL
    pll_fsm_prep_event(&pll_fsm, &prep_data);

    // Switch PLL to PHY Frequency
    pll_fsm_switch_event(&pll_fsm);

    // Wait for PLL to lock
    vWaitForCompletion(&lock_event);
    configASSERT(pll_fsm.current_state == PLL_STATE_LOCKED);

    // End test
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_GP3_CFG__ADR, 0x1);
}

/*-----------------------------------------------------------*/
static void pll_state_change_callback(fsm_t *fsm, uint8_t state, void *args)
{
    if (state == PLL_STATE_LOCKED)
    {
        vComplete(&lock_event);
    }
}

/*-----------------------------------------------------------*/
static void prvSetupHardware( void )
{
    uint32_t reg_val, clken_reg;

    common_path_init(&common_path, WDDR_MEMORY_MAP_CMN);
    pll_init(&pll, WDDR_MEMORY_MAP_PLL);

    // Enable Common Block
    common_path_enable(&common_path);

    // Boot PLL to set MCU clk @ 384 MHz
    pll_boot(&pll);

    // Enable Clocks
    reg_val = reg_read(WDDR_MEMORY_MAP_CTRL + DDR_CTRL_CLK_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CTRL_CLK_CFG_PLL_CLK_EN, 0x1);
    reg_write(WDDR_MEMORY_MAP_CTRL + DDR_CTRL_CLK_CFG__ADR, reg_val);

    reg_val = UPDATE_REG_FIELD(reg_val, DDR_CTRL_CLK_CFG_MCU_GFM_SEL, 0x1);
    reg_write(WDDR_MEMORY_MAP_CTRL + DDR_CTRL_CLK_CFG__ADR, reg_val);

    clken_reg = reg_read(WDDR_MEMORY_MAP_CMN + DDR_CMN_CLK_CTRL_CFG__ADR);
    clken_reg = UPDATE_REG_FIELD(clken_reg, DDR_CMN_CLK_CTRL_CFG_PLL0_DIV_CLK_RST, 0x0);
    reg_write(WDDR_MEMORY_MAP_CMN + DDR_CMN_CLK_CTRL_CFG__ADR, clken_reg);

    clken_reg = UPDATE_REG_FIELD(clken_reg, DDR_CMN_CLK_CTRL_CFG_GFCM_EN, 0x1);
    reg_write(WDDR_MEMORY_MAP_CMN + DDR_CMN_CLK_CTRL_CFG__ADR, clken_reg);

    // Turn off PHY CLK gating
    reg_val = reg_read(WDDR_MEMORY_MAP_FSW + DDR_FSW_CSP_1_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CSP_1_CFG_CLK_DISABLE_OVR_VAL, 0x0);
    reg_write(WDDR_MEMORY_MAP_FSW + DDR_FSW_CSP_1_CFG__ADR, reg_val);

    clken_reg = UPDATE_REG_FIELD(clken_reg, DDR_CMN_CLK_CTRL_CFG_PLL0_DIV_CLK_EN, 0x1);
    reg_write(WDDR_MEMORY_MAP_CMN + DDR_CMN_CLK_CTRL_CFG__ADR, clken_reg);
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
    wait_for_interrupt();
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

    write( STDOUT_FILENO, "ERROR Stack overflow on func: ", 30 );
    write( STDOUT_FILENO, pcTaskName, strlen( pcTaskName ) );
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
