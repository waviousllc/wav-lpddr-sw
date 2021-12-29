/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
/* Standard includes. */
#include <compiler.h>

/* FreeRTOS includes. */
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <semphr.h>
#include <queue.h>

/* Kernel includes. */
#include <kernel/irq.h>
#include <kernel/completion.h>
#include <kernel/stateMachine.h>

/* LPDDR includes. */
#include <wddr/memory_map.h>
#include <wddr/irq_map.h>
#include <wddr/interface.h>

/* Firmware includes. */
#include <firmware/phy_task.h>

/*******************************************************************************
**                                   MACROS
*******************************************************************************/
#define WDDR_BASE_ADDR              (0x00000000)
#define MSG_QUEUE_LEN               (0x10)

#define DFI_PHYUPD_PERIOD           (pdMS_TO_TICKS(2))
#define PERIODIC_CAL_PERIOD         (pdMS_TO_TICKS(2))

/*******************************************************************************
**                            FUNCTION DECLARATIONS
*******************************************************************************/
/** Firmware Task Entry Function */
static void firmwareTask(void *pvParameters);

#if CONFIG_CAL_PERIODIC
/** Firmware Periodic Calibration Task */
static void firmwarePeriodicCalTask(void *pvParameters);
#endif /* CONFIG_CAL_PERIODIC */

/** Internal function for handling FW Start event */
static fw_response_t handle_start_event(fw_phy_event_t event, void *data);

/** Internal function for handling Frequency Switch events */
static fw_response_t handle_fsw_event(fw_phy_event_t event, void *data);

/** Internal function for handling  DFI events */
static fw_response_t handle_dfi_event(fw_phy_event_t event, void *data);

/** Internal function for handling Low Power events */
static fw_response_t handle_lp_event(fw_phy_event_t event, void *data);

/** Internal callback called when DFI PHYUPD Timer expires */
static void dfi_phyupd_timer_callback(TimerHandle_t xTimer);

/*******************************************************************************
**                           VARIABLE DECLARATIONS
*******************************************************************************/
// Handle to WDDR device
static wddr_handle_t wddr;

// Handle to DFI PHYUPD Timer
static TimerHandle_t xDfiPhyUpdTimer;

// All Firmware states
static struct state errorState;
static struct state fswPrep, fswPending;
static struct state dfiIdle,
                    dfiPhyMstrPending,
                    dfiPhyMstr,
                    dfiPhyUpd,
                    dfiCtrlUpd;

static struct firmware_manager
{
    TaskHandle_t  task;     // Firmware Task Handle
    QueueHandle_t mq;       // Firmware Message Queue
    QueueHandle_t rq;       // Firmware Retry Queue
#if CONFIG_CAL_PERIODIC
    Completion_t phyMstrEvent;
#endif /* CONFIG_CAL_PERIODIC */
    struct
    {
        bool ready;         // Status for if firmware has been started
        bool error;         // Status for if firmware is in error state
        bool rq_ready;      // Status for processing retry queue
    } status;
    struct
    {
        struct stateMachine fsw;    // Frequency Switch State Machine
        struct stateMachine dfi;    // DFI State Machine
    } fsm;
} fw_manager = {
    .status.ready = false,
    .status.error = false,
};

/** Table that maps an FW_PHY_EVENT to a specific handler */
static fw_response_t ( *event_handlers[FW_PHY_EVENT_NUM] )(fw_phy_event_t event, void *data) = {
    handle_start_event, // FW_PHY_EVENT_BOOT
    handle_fsw_event,   // FW_PHY_EVENT_PREP
    handle_dfi_event,   // FW_PHY_EVENT_PHYUPD_REQ
    handle_dfi_event,   // FW_PHY_EVENT_PHYMSTR_REQ
    handle_dfi_event,   // FW_PHY_EVENT_PHYMSTR_EXIT
    handle_fsw_event,   // FW_PHY_EVENT_INIT_START
    handle_fsw_event,   // FW_PHY_EVENT_INIT_COMPLETE
    handle_fsw_event,   // FW_PHY_EVENT_PLL_INIT_LOCK
    handle_fsw_event,   // FW_PHY_EVENT_PLL_LOCK
    handle_fsw_event,   // FW_PHY_EVENT_PLL_LOSS_LOCK
    handle_dfi_event,   // FW_PHY_EVENT_PHYMSTR_ACK
    handle_dfi_event,   // FW_PHY_EVENT_PHYMSTR_ABORT
    handle_dfi_event,   // FW_PHY_EVENT_PHYUPD_ACK
    handle_dfi_event,   // FW_PHY_EVENT_PHYUPD_ABORT
    handle_dfi_event,   // FW_PHY_EVENT_CTRLUPD_ASSERT
    handle_dfi_event,   // FW_PHY_EVENT_CTRLUPD_DEASSERT
    handle_lp_event,    // FW_PHY_EVENT_LP_DATA_REQ
    handle_lp_event,    // FW_PHY_EVENT_LP_CTRL_REQ
};

/*******************************************************************************
**                        STATE FUNCTION DECLARATIONS
*******************************************************************************/
/** Internal function when errorState is entered. */
static void error_entry_handler(void *stateData, struct event *event);

/** Internal function called when PREP event occurs. */
static void fsw_prepare_switch_handler(void *currentStateData,
                                       struct event *event,
                                       void *newStateData);

/** Internal function called when PLL_LOCK event occurs. */
static void fsw_post_switch_handler(void *currentStateData,
                                    struct event *event,
                                    void *newStateData);

/** Internal function called when INIT_START event occurs. */
static void fsw_pending_entry_handler(void *stateData, struct event *event);

/** Internal function called when phyMstrPending state is entered. */
static void dfi_phymstr_pending_entry_handler(void *stateData, struct event *event);

/** Internal function called when phyMstr state is left. */
static void dfi_phymstr_exit_handler(void *stateData, struct event *event);

/** Internal function called when phyMstr state is entered. */
static void dfi_phymstr_enter_handler(void *stateData, struct event *event);

/** Internal function called when ctrlUpd state is entered. */
static void dfi_ctrlupd_entry_handler(void *stateData, struct event *event);

/** Internal function called when phyUpd state is entered. */
static void dfi_phyupd_entry_handler(void *stateData, struct event *event);

/** Internal function called when PHYUPD_REQ event occurs. */
static void dfi_phyupd_update(void *currentStateData,
                              struct event *event,
                              void *newStateData);

/** Internal function called when PHYUPD_ABORT event occurs. */
static void dfi_phyupd_abort(void *currentStateData,
                             struct event *event,
                             void *newStateData);

/** Internal function called when PHYMSTR_ABORT event occurs. */
static void dfi_phymstr_abort(void *currentStateData,
                             struct event *event,
                             void *newStateData);


/** Internal function called when exiting CTRLUPD_DEASSERT event occurs. */
static void dfi_ctrlupd_exit_handler(void *currentStateData,
                                     struct event *event,
                                     void *newStateData);

/*******************************************************************************
**                          STATE DECLARATIONS
*******************************************************************************/
static struct state errorState = {
    .entryAction = error_entry_handler,
};

/*-----------------------------------------------------------*/
// Frequency Switch States
static struct state fswPrep = {
    .parentState = NULL,
    .entryState = NULL,
    .transitions = (struct transition[]) {
        {FW_PHY_EVENT_PREP, NULL, NULL, fsw_prepare_switch_handler, &fswPrep},
        {FW_PHY_EVENT_INIT_START, NULL, NULL, NULL, &fswPending},
        {FW_PHY_EVENT_PLL_LOSS_LOCK, NULL, NULL, NULL, &errorState},
    },
    .numTransitions = 3,
    .data = NULL,
    .entryAction = NULL,
    .exitAction = NULL,
};

static struct state fswPending = {
    .parentState = NULL,
    .entryState = NULL,
    .transitions = (struct transition[]) {
        {FW_PHY_EVENT_PLL_LOCK, NULL, NULL, fsw_post_switch_handler, &fswPrep},
    },
    .numTransitions = 1,
    .data = NULL,
    .entryAction = fsw_pending_entry_handler,
    .exitAction = NULL,
};

/*-----------------------------------------------------------*/
// DFI States
static struct state dfiIdle = {
    .parentState = NULL,
    .entryState = NULL,
    .transitions = (struct transition[]) {
        {FW_PHY_EVENT_PHYMSTR_REQ, NULL, NULL, NULL , &dfiPhyMstrPending},
        {FW_PHY_EVENT_PHYUPD_REQ, NULL, NULL, NULL, &dfiPhyUpd},
        {FW_PHY_EVENT_CTRLUPD_ASSERT, NULL, NULL, NULL, &dfiCtrlUpd},
    },
    .numTransitions = 3,
    .data = NULL,
    .entryAction = NULL,
    .exitAction = NULL,
};

static struct state dfiPhyMstrPending = {
    .parentState = NULL,
    .entryState = NULL,
    .transitions = (struct transition[]) {
        {FW_PHY_EVENT_PHYMSTR_ACK, NULL, NULL, NULL, &dfiPhyMstr},
        {FW_PHY_EVENT_PHYMSTR_ABORT, NULL, NULL, dfi_phymstr_abort, &dfiIdle},
    },
    .numTransitions = 2,
    .data = NULL,
    .entryAction = dfi_phymstr_pending_entry_handler,
    .exitAction = NULL,
};

static struct state dfiPhyMstr = {
    .parentState = NULL,
    .entryState = NULL,
    .transitions = (struct transition[]) {
        {FW_PHY_EVENT_PHYMSTR_EXIT, NULL, NULL, NULL, &dfiIdle},
    },
    .numTransitions = 1,
    .data = NULL,
    .entryAction = dfi_phymstr_enter_handler,
    .exitAction = dfi_phymstr_exit_handler,
};

static struct state dfiPhyUpd = {
    .parentState = NULL,
    .entryState = NULL,
    .transitions = (struct transition[]) {
        {FW_PHY_EVENT_PHYUPD_ACK, NULL, NULL, dfi_phyupd_update, &dfiIdle},
        {FW_PHY_EVENT_PHYUPD_ABORT, NULL, NULL, dfi_phyupd_abort, &dfiIdle},
    },
    .numTransitions = 2,
    .data = NULL,
    .entryAction = dfi_phyupd_entry_handler,
    .exitAction = NULL,
};

static struct state dfiCtrlUpd = {
    .parentState = NULL,
    .entryState = NULL,
    .transitions = (struct transition[]) {
        {FW_PHY_EVENT_CTRLUPD_DEASSERT, NULL, NULL, dfi_ctrlupd_exit_handler, &dfiIdle},
    },
    .numTransitions = 1,
    .data = NULL,
    .entryAction = dfi_ctrlupd_entry_handler,
    .exitAction = NULL,
};

/*******************************************************************************
**                              IMPLEMENTATIONS
*******************************************************************************/
void fw_phy_task_init(void)
{
    configASSERT(fw_manager.mq == NULL);
    configASSERT(fw_manager.rq == NULL);
    configASSERT(fw_manager.task == NULL);

    // Initialize PHY
    wddr = wddr_init(WDDR_BASE_ADDR);
    configASSERT(wddr != NULL);

    // State machine initialization
    stateM_init(&fw_manager.fsm.fsw, &fswPrep, &errorState);
    stateM_init(&fw_manager.fsm.dfi, &dfiIdle, &errorState);

    // Create message queue
    fw_manager.mq = xQueueCreate(MSG_QUEUE_LEN, sizeof(fw_msg_t));
    configASSERT(fw_manager.mq != NULL);

    // Create retry queue
    fw_manager.rq = xQueueCreate(MSG_QUEUE_LEN, sizeof(fw_msg_t));
    configASSERT(fw_manager.rq != NULL);

    // Create PHYUPD Timer
    xDfiPhyUpdTimer = xTimerCreate("DFI PHYUPD Timer",
                                    DFI_PHYUPD_PERIOD,
                                    pdTRUE,
                                    NULL,
                                    dfi_phyupd_timer_callback);
    configASSERT(xDfiPhyUpdTimer != NULL);

    // Create the task
    xTaskCreate(firmwareTask,
                "FW Task",
                configMINIMAL_STACK_SIZE,
                NULL,
                configMAX_PRIORITIES - 1,
                &fw_manager.task);

    configASSERT(fw_manager.task != NULL);

#if CONFIG_CAL_PERIODIC
    __UNUSED__ BaseType_t ret = xTaskCreate(firmwarePeriodicCalTask,
                                            "FW Periodic Cal Task",
                                            configMINIMAL_STACK_SIZE,
                                            NULL,
                                            tskIDLE_PRIORITY + 1,
                                            NULL);
    configASSERT(ret == pdTRUE);
#endif /* CONFIG_CAL_PERIODIC */
}

/*-----------------------------------------------------------*/
static BaseType_t __phy_task_notify(fw_msg_t *msg, TickType_t xTicksToWait)
{
    return xQueueSendToBack(fw_manager.mq, msg, xTicksToWait);
}

/*-----------------------------------------------------------*/
void fw_phy_task_notify(fw_msg_t *msg)
{
    __phy_task_notify(msg, portMAX_DELAY);
}

/*-----------------------------------------------------------*/
void fw_phy_task_notify_isr(fw_msg_t *msg, BaseType_t *pxHigherPriorityTaskWoken)
{
    xQueueSendToBackFromISR(fw_manager.mq, msg, pxHigherPriorityTaskWoken);
}

/*-----------------------------------------------------------*/
static void firmwareHandleMessage(fw_msg_t *msg)
{
    fw_response_t resp;

    // skip out of bounds events
    if (msg->event >= FW_PHY_EVENT_NUM)
    {
        return;
    }

    // Process event
    resp = event_handlers[msg->event](msg->event, msg->data);

    // Retry event later
    if (resp == FW_RESP_RETRY)
    {
        xQueueSendToBack(fw_manager.rq, msg, 0);
        return;
    }

    // Let caller know message processed
    if (msg->xSender)
    {
        xTaskNotify(msg->xSender, resp, eSetValueWithOverwrite);
    }
}

/*-----------------------------------------------------------*/
static void firmwareTask(void *pvParameters)
{
    fw_msg_t msg;
    for(;;)
    {
        if (fw_manager.status.rq_ready)
        {
            // Exhaust the retry queue
            while (uxQueueMessagesWaiting(fw_manager.rq))
            {
                xQueueReceive(fw_manager.rq, &msg, 0);
                firmwareHandleMessage(&msg);
            }

            // Retry queue shouldn't be processed
            fw_manager.status.rq_ready = false;
        }

        // Block indefinitely
        xQueueReceive(fw_manager.mq, &msg, portMAX_DELAY);
        firmwareHandleMessage(&msg);
    }
}

/*-----------------------------------------------------------*/
static fw_response_t handle_start_event(__UNUSED__ fw_phy_event_t event,
                                        __UNUSED__ void *data)
{
    wddr_boot_cfg_t boot_cfg = WDDR_BOOT_CONFIG_NONE;
    fw_phy_start_cfg_t *start_cfg = (fw_phy_start_cfg_t *) data;

    if (start_cfg->calibrate)
    {
        boot_cfg = UPDATE_BOOT_OPTION(boot_cfg, WDDR_BOOT_OPTION_PLL_CAL, CONFIG_CALIBRATE_PLL);
        boot_cfg = UPDATE_BOOT_OPTION(boot_cfg, WDDR_BOOT_OPTION_SA_CAL, CONFIG_CALIBRATE_SA);
        boot_cfg = UPDATE_BOOT_OPTION(boot_cfg, WDDR_BOOT_OPTION_ZQCAL_CAL, CONFIG_CALIBRATE_ZQCAL);
    }

    if (start_cfg->train_dram)
    {
        boot_cfg = UPDATE_BOOT_OPTION(boot_cfg, WDDR_BOOT_OPTION_TRAIN_DRAM, CONFIG_DRAM_TRAIN);
    }

    // Can only call once
    if (fw_manager.status.ready)
    {
        return FW_RESP_FAILURE;
    }

    if (wddr_boot(wddr, boot_cfg) != WDDR_SUCCESS)
    {
        fw_manager.status.error = true;
        return FW_RESP_FAILURE;
    }

    disable_irq(MCU_FAST_IRQ_CTRLUPD_REQ);
    wddr_disable_ctrlupd_interface(wddr);

    // Start the PHYUPD Timer
    if (xTimerStart(xDfiPhyUpdTimer, 0) == pdFAIL)
    {
        xTimerDelete(xDfiPhyUpdTimer, 0);
        fw_manager.status.error = true;
        return FW_RESP_FAILURE;
    }

    fw_manager.status.ready = true;
    return FW_RESP_SUCCESS;
}

/*-----------------------------------------------------------*/
static fw_response_t handle_dfi_event(fw_phy_event_t event, void *data)
{
    enum stateM_handleEventRetVals ret;
    struct event fsm_event = {
        .type = event,
        .data = data,
    };

    if (!fw_manager.status.ready || fw_manager.status.error)
    {
        return FW_RESP_FAILURE;
    }

    // DFI events shouldn't be performed until switch completes
    if (fw_manager.fsm.fsw.currentState == &fswPending)
    {
        return FW_RESP_RETRY;
    }

    ret = stateM_handleEvent(&fw_manager.fsm.dfi, &fsm_event);
    if (ret == stateM_errorStateReached)
    {
        return FW_RESP_FAILURE;
    }

    return FW_RESP_SUCCESS;
}

/*-----------------------------------------------------------*/
static fw_response_t handle_fsw_event(fw_phy_event_t event, void *data)
{
    enum stateM_handleEventRetVals ret;
    struct event fsm_event = {
        .type = event,
        .data = data,
    };

    if (!fw_manager.status.ready || fw_manager.status.error)
    {
        return FW_RESP_FAILURE;
    }

    // Switch shouldn't be performed during CTRLUPD
    if (fw_manager.fsm.dfi.currentState == &dfiCtrlUpd)
    {
        return FW_RESP_RETRY;
    }

    ret = stateM_handleEvent(&fw_manager.fsm.fsw, &fsm_event);
    if (ret == stateM_errorStateReached)
    {
        return FW_RESP_FAILURE;
    }
    return FW_RESP_SUCCESS;
}

/*-----------------------------------------------------------*/
static fw_response_t handle_lp_event(fw_phy_event_t event, void *data)
{
    // Not supported
    return FW_RESP_FAILURE;
}

/*-----------------------------------------------------------*/
static void dfi_phyupd_timer_callback(TimerHandle_t xTimer)
{
    __UNUSED__ BaseType_t ret;
    fw_msg_t msg = {
        .event = FW_PHY_EVENT_PHYUPD_REQ,
        .data = (void *) DFI_PHYUPD_TYPE_0,
        .xSender = NULL,
    };

    // Perform calibration. Should there be a dedicated task instead?
    wddr_iocal_calibrate(wddr);

    // Submit event without blocking
    ret = __phy_task_notify(&msg, 0);
    configASSERT(ret != pdFALSE);
}

/*-----------------------------------------------------------*/
#if CONFIG_CAL_PERIODIC
/** Firmware Periodic Calibration Task */
static void firmwarePeriodicCalTask(void *pvParameters)
{
    TickType_t xLastWakeTime;
    UBaseType_t resp;
    const TickType_t xFrequency = PERIODIC_CAL_PERIOD;
    const dfi_phymstr_req_t request = {
        .cs_state = DFI_MASTER_CS_STATE_ACTIVE,
        .state_sel = DFI_MASTER_STATE_SEL_REFRESH,
        .type = DFI_MASTER_TYPE_0,
    };

    fw_msg_t msg = {
        .data = (void *) &request,
        .xSender = NULL,
    };

    xLastWakeTime = xTaskGetTickCount();

    for(;;)
    {
        // Wait for the next cycle
        vTaskDelayUntil(&xLastWakeTime, xFrequency);

        // Try again later
        if (fw_manager.fsm.dfi.currentState != &dfiIdle)
        {
            continue;
        }

        // Clear completion variable
        vInitCompletion(&fw_manager.phyMstrEvent);

        // Request PHYMSTR control
        msg.event = FW_PHY_EVENT_PHYMSTR_REQ;
        msg.xSender = xTaskGetCurrentTaskHandle();

        // Clear notifications
        xTaskNotifyWait(0, 0, NULL, 0);

        // Send message
        fw_phy_task_notify(&msg);

        // Wait for it to be handled
        xTaskNotifyWait(0, 0, &resp, portMAX_DELAY);

        if (resp != FW_RESP_SUCCESS)
        {
            continue;
        }

        // Block until in PHY MASTER or it was aborted
        vWaitForCompletion(&fw_manager.phyMstrEvent);

        // Try again later
        if (fw_manager.fsm.dfi.currentState != &dfiPhyMstr)
        {
            continue;
        }

        // TODO: DRAM Calibration or Training goes here

        // TODO: Put the DRAM back into correct state

        // Exit PHYMSTR
        msg.event = FW_PHY_EVENT_PHYMSTR_EXIT;
        msg.xSender = NULL;
        fw_phy_task_notify(&msg);
    }
}
#endif /* CONFIG_CAL_PERIODIC */

/*******************************************************************************
**                          STATE IMPLEMENTATIONS
*******************************************************************************/
static void error_entry_handler(__UNUSED__ void *stateData,
                                __UNUSED__ struct event *event)
{
    fw_manager.status.error = true;
    configASSERT(false);
}

/*-----------------------------------------------------------*/
static void fsw_prepare_switch_handler(__UNUSED__ void *currentStateData,
                                       struct event *event,
                                       __UNUSED__ void *newStateData)
{
    __UNUSED__ wddr_return_t ret;
    wddr_msr_t next_msr;
    uint8_t freq_id = (uint8_t) (uintptr_t) event->data;

    // Make sure INIT_START IRQ not enabled
    disable_irq(MCU_FAST_IRQ_INIT_START);

    // Prepare PHY
    next_msr = wddr_get_next_msr(wddr);
    ret = wddr_prep_switch(wddr, freq_id, next_msr);
    configASSERT(ret == WDDR_SUCCESS);

    // Enable INIT_START IRQ
    enable_irq(MCU_FAST_IRQ_INIT_START);
}

/*-----------------------------------------------------------*/
static void fsw_post_switch_handler(__UNUSED__ void *currentStateData,
                                    __UNUSED__ struct event *event,
                                    __UNUSED__ void *newStateData)
{
    wddr_handle_post_switch(wddr);

    // Process any retry events
    fw_manager.status.rq_ready = true;
}

/*-----------------------------------------------------------*/
static void fsw_pending_entry_handler(__UNUSED__ void *stateData,
                                      __UNUSED__ struct event *event)
{

    /**
     * Per DFI 5.0 Specification - Section 4.21:
     *
     * PHYUPDATE:
     * If dfi_init_start and dfi_phyupd_req are both asserted, the MC is
     * permitted to not assert dfi_phyupd_ack. In this case, the PHY should
     * de-assert dfi_init_complete to initiate the frequency change protocol.
     * In this case it must also de-assert the dfi_phyupd_req signal before
     * asserting the dfi_init_complete signal again.
     *
     * PHYMASTER:
     * If the dfi_init_start and dfi_phymstr_req signals are both asserted,
     * the MC cannot assert the dfi_phymstr_ack signal. In this case,
     * the PHY may de-assert the dfi_init_complete signal to initiate the
     * frequency change protocol and it must also de-assert the dfi_phymstr_req
     * signal before asserting the dfi_init_complete signal again.
     */
    if (fw_manager.fsm.dfi.currentState == &dfiPhyUpd)
    {
        // Abort PHYUPD
        stateM_handleEvent(&fw_manager.fsm.dfi, &(struct event){FW_PHY_EVENT_PHYUPD_ABORT, NULL});
        configASSERT(fw_manager.fsm.dfi.currentState == &dfiIdle);
    }
    else if (fw_manager.fsm.dfi.currentState == &dfiPhyMstrPending)
    {
        // Abort PHYMSTR
        stateM_handleEvent(&fw_manager.fsm.dfi, &(struct event){FW_PHY_EVENT_PHYMSTR_ABORT, NULL});
        configASSERT(fw_manager.fsm.dfi.currentState == &dfiIdle);
    }

    wddr_handle_pending_switch(wddr);
}

/*-----------------------------------------------------------*/
static void dfi_phymstr_pending_entry_handler(__UNUSED__ void *stateData,
                                              struct event *event)
{
    // Get master signal states from event data
    dfi_phymstr_req_t *req = (dfi_phymstr_req_t *) event->data;

    // Request PHY MASTER control
    wddr_phymstr_enter(wddr, req);

    // Enable PHY MASTER ACK IRQ
    enable_irq(MCU_FAST_IRQ_PHYMSTR_ACK);
}

/*-----------------------------------------------------------*/
static void dfi_phymstr_abort(void *currentStateData,
                             struct event *event,
                             void *newStateData)
{
    disable_irq(MCU_FAST_IRQ_PHYMSTR_ACK);
    wddr_phymstr_exit(wddr);
#if CONFIG_CAL_PERIODIC
    // Notify Periodic Calibration Task that event was aborted
    vComplete(&fw_manager.phyMstrEvent);
#endif /* CONFIG_CAL_PERIODIC */
}

/*-----------------------------------------------------------*/
static void dfi_phymstr_enter_handler(void *stateData, struct event *event)
{
#if CONFIG_CAL_PERIODIC
    vComplete(&fw_manager.phyMstrEvent);
#endif /* CONFIG_CAL_PERIODIC */
}

/*-----------------------------------------------------------*/
static void dfi_phymstr_exit_handler(void *stateData, struct event *event)
{
    /**
     * @note    It's expected that creator of PHYMSTR_EXIT event would have
     *          put the DRAM back into the correct state. The correct state
     *          is the state that the DRAM was put into by the Memory Controller
     *          per the PHY MASTER request parameters.
     */
    wddr_phymstr_exit(wddr);
}

/*-----------------------------------------------------------*/
static void dfi_ctrlupd_entry_handler(void *stateData, struct event *event)
{
    // Perform IOCAL and update
    wddr_iocal_calibrate(wddr);
    wddr_iocal_update_phy(wddr);

    // Inform PHY that update is done
    wddr_ctrlupd_done(wddr);
}

/*-----------------------------------------------------------*/
static void dfi_phyupd_entry_handler(void *stateData, struct event *event)
{
    // Get update type from event data
    dfi_phyupd_type_t type = (dfi_phyupd_type_t) (uintptr_t) event->data;

    wddr_phyupd_enter(wddr, type);

    // Enable PHY UPDATE ACK IRQ
    enable_irq(MCU_FAST_IRQ_PHYUPD_ACK);
}

/*-----------------------------------------------------------*/
static void dfi_phyupd_update(void *currentStateData,
                              struct event *event,
                              void *newStateData)
{
    wddr_iocal_update_phy(wddr);
    wddr_phyupd_exit(wddr);
}

/*-----------------------------------------------------------*/
static void dfi_phyupd_abort(void *currentStateData,
                             struct event *event,
                             void *newStateData)
{
    disable_irq(MCU_FAST_IRQ_PHYUPD_ACK);
    wddr_phyupd_exit(wddr);
}

/*-----------------------------------------------------------*/
static void dfi_ctrlupd_exit_handler(void *currentStateData,
                                     struct event *event,
                                     void *newStateData)
{
    // Process any retry events
    fw_manager.status.rq_ready = true;
}
