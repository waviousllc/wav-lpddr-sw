/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stddef.h>
#include <compiler.h>
#include <wddr/memory_map.h>
#include <wddr/notification_map.h>
#include <kernel/io.h>
#include <kernel/irq.h>
#include <kernel/notification.h>
#include <wddr/irq_map.h>
#include <pll/fsm.h>
#include <freq_switch/fsm.h>
#include <dfi/driver.h>
#include <fsw/driver.h>

// Timer is 1ms minimum, add 1 extra tick to ensure > 1ms
#define WD_TIMER_PERIOD ( pdMS_TO_TICKS( 1 ) + 1 )

/** @brief  Guard Function for Frequency Switch PREP_SWITCH state */
static bool freq_switch_prep_guard(fsm_t *fsm, void *data);

/** @brief  Guard Function for Frequency Switch SWITCH state */
static bool freq_switch_sw_switch_guard(fsm_t *fsm, void *data);

/** @brie   Exit Function for Frequency Switch PREP_SWITCH state */
static void freq_switch_prep_exit(fsm_t *fsm);

/** @brie   Exit Function for Frequency Switch POST_SWITCH state */
static void freq_switch_post_switch_exit(fsm_t *fsm);

/** @brief  Frequency Switch State Handler for IDLE state */
static void freq_switch_state_idle(fsm_t *fsm, void *data);

/** @brief  Frequency Switch State Handler for FAIL state */
static void freq_switch_state_fail(fsm_t *fsm, void *data);

/** @brief  Frequency Switch State Handler for WAIT_FOR_SWITCH state */
static void freq_switch_state_wait_for_switch(fsm_t *fsm, void *data);

/** @brief  Frequency Switch State Handler for SWITCH state */
static void freq_switch_state_switch(fsm_t *fsm, void *data);

/** @brief  Frequency Switch State Handler for PREP_SWITCH state */
static void freq_switch_state_prep_switch(fsm_t *fsm, void *data);

/** @brief  Frequency Switch State Handler for POST_SWITCH state */
static void freq_switch_state_post_switch(fsm_t *fsm, void *data);

/** @brief  Frequency Switch State Handler for WAIT_FOR_LOCK state */
static void freq_switch_state_wait_for_lock(fsm_t *fsm, void *data);

/** @brief  IRQ Handler for all Init Start events */
static void handle_phy_init_start(int irq_num, void *args);

/** @brief  IRQ Handler for all Init Complete events */
static void handle_phy_init_complete(int irq_num, void *args);

/** @brief  Callback function for PLL state change events */
static void pll_state_change_cb(fsm_t *pll_fsm, uint8_t state, void *args);

/** @brief  Watchdog Timer Handler for all timeout events */
static void watchdog_expired_handler(TimerHandle_t handle);

/** @brief  Table specifying state, guard, and exit functions for all states */
static const state_func_t state_table[] =
{
    // State Function                       Guard Function  Exit Function
    {freq_switch_state_idle,                NULL,           NULL},
    {freq_switch_state_fail,                NULL,           NULL},
    {freq_switch_state_prep_switch,         NULL,           freq_switch_prep_exit},
    {freq_switch_state_wait_for_switch,     NULL,           NULL},
    {freq_switch_state_switch,              NULL,           NULL},
    {freq_switch_state_post_switch,         NULL,           freq_switch_post_switch_exit},
    {freq_switch_state_wait_for_lock,       NULL,           NULL},
};

void freq_switch_fsm_init(fs_fsm_t *fsm, pll_fsm_t *pll_fsm)
{
    fsm_init(&fsm->fsm, pll_fsm, state_table);

    // Set Init Complete Callback variables to NULL
    fsm->init_complete_cb = NULL;
    fsm->init_complete_cb_args = NULL;

    // Default to SW Switch Only FSM Functionality
    fsm->hw_switch_only = false;

    // Connect Watchdog timer callback
    fsm->timer = xTimerCreate("FsFsmWd",
                              WD_TIMER_PERIOD,
                              pdFALSE,
                              (void *) fsm,
                              watchdog_expired_handler);

    // Connect callback to PLL FSM
    fsm_register_state_change_callback(pll_fsm, pll_state_change_cb, fsm);

    // Register interrupt handlers
    request_irq(MCU_FAST_IRQ_INIT_START, handle_phy_init_start, fsm);
    request_irq(MCU_FAST_IRQ_INIT_COMPLETE, handle_phy_init_complete, fsm);

    // TURN on sticky bit for IRQs
    uint32_t mask = reg_read(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_STICKY_CFG__ADR);
    mask |= FAST_IRQ_STICKY_MASK(DDR_IRQ_INIT_START) | FAST_IRQ_STICKY_MASK(DDR_IRQ_INIT_COMPLETE);
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_STICKY_CFG__ADR, mask);
}

wddr_return_t freq_switch_event_prep(fs_fsm_t *fsm, fs_prep_data_t *prep_data)
{
    // Check if prep guard conditions are satisfied
    if (!freq_switch_prep_guard(&fsm->fsm, NULL))
    {
        return WDDR_ERROR;
    }

    // Stop timer if started
    if (xTimerIsTimerActive(fsm->timer))
    {
        xTimerStop(fsm->timer, 0);
    }

    // Call into FSM
    fsm_handle_external_event(&fsm->fsm, FS_STATE_PREP_SWITCH, prep_data);
    return WDDR_SUCCESS;
}

wddr_return_t freq_switch_event_sw_switch(fs_fsm_t *fsm)
{
    // Check if prep guard conditions are satisfied
    if (!freq_switch_sw_switch_guard(&fsm->fsm, fsm))
    {
       return WDDR_ERROR;
    }

    // Call into FSM
    fsm_handle_external_event(&fsm->fsm, FS_STATE_SWITCH, fsm);
    return WDDR_SUCCESS;
}

wddr_return_t freq_switch_event_hw_switch_mode(fs_fsm_t *fsm)
{
    uint32_t init_start;
    uint8_t curr_vco_id;
    uint8_t curr_msr;
    pll_fsm_t *pll_fsm = (pll_fsm_t *) fsm->fsm.instance;

    // Verify hardware is set to VCO1 and MSR0, if not return ERROR.
    pll_fsm_get_current_vco_id(pll_fsm, &curr_vco_id);
    curr_msr = GET_REG_FIELD(reg_read(WDDR_MEMORY_MAP_FSW + DDR_FSW_CTRL_STA__ADR), DDR_FSW_CTRL_STA_CMN_MSR);
    if (curr_msr != WDDR_MSR_0 || curr_vco_id != VCO_INDEX_PHY_1)
    {
        return WDDR_ERROR;
    }

    // Set override values to MSR0 and VCO1
    fsw_ctrl_set_msr_vco_ovr_val_reg_if(WDDR_MSR_0, VCO_INDEX_PHY_1);
    // Disable overrides to allow hardware to switch MSR and VCO.
    fsw_ctrl_set_msr_vco_ovr_reg_if(false);

    // Release init_complete override indicating dfi interface is ready.
    dfi_set_init_complete_ovr_reg_if(false);
    // init_start must be low before proceeding, wait for this to happen.
    do
    {
        dfi_get_init_start_status_reg_if(&init_start);
    } while (init_start != 0x0);
    // Ensure init_start override is disabled.
    dfi_set_init_start_ovr_reg_if(false);
    fsm->hw_switch_only = true;
    return WDDR_SUCCESS;
}

void freq_switch_register_init_complete_callback(fs_fsm_t *fsm, const init_complete_cb_t cb, void *args)
{
    fsm->init_complete_cb = cb;
    fsm->init_complete_cb_args = args;
}

static void freq_switch_state_idle(__UNUSED__ fsm_t *fsm, __UNUSED__ void *data)
{
    // Do Nothing
}

static bool freq_switch_prep_guard(fsm_t *fsm, __UNUSED__ void *data)
{
    return (fsm->current_state == FS_STATE_IDLE ||
            fsm->current_state == FS_STATE_WAIT_FOR_SWITCH);
}

static bool freq_switch_sw_switch_guard(fsm_t *fsm, void *data)
{
    fs_fsm_t *fs_fsm = (fs_fsm_t *) data;
    return (fsm->current_state == FS_STATE_WAIT_FOR_SWITCH &&
            !fs_fsm->hw_switch_only);
}

static void freq_switch_prep_exit(__UNUSED__ fsm_t *fsm)
{
    vSendNotification(WDDR_NOTIF_FSW_PREP_DONE);
}

static void freq_switch_post_switch_exit(__UNUSED__ fsm_t *fsm)
{
    vSendNotification(WDDR_NOTIF_FSW_DONE);
}

static void freq_switch_state_fail(__UNUSED__ fsm_t *fsm, __UNUSED__ void *data)
{
    // Report ERROR
    vSendNotification(WDDR_NOTIF_FSW_FAILED);
}

static void freq_switch_state_wait_for_switch(fsm_t *fsm, void *data)
{
    // INIT_COMPLETE or Set Event needs to be called
    fs_fsm_t *fs_fsm = (fs_fsm_t *) data;

    if (xTimerReset(fs_fsm->timer, 0) == pdFALSE)
    {
        fsm_handle_internal_event(fsm, FS_STATE_FAIL, NULL);
        return;
    }

    // Turn on INIT_START interrupt
    enable_irq(MCU_FAST_IRQ_INIT_START);
}

static void freq_switch_state_switch(fsm_t *fsm, void *data)
{
    uint8_t msr;
    uint8_t next_vco_id;
    fs_fsm_t *fs_fsm = (fs_fsm_t *) data;
    pll_fsm_t *pll_fsm = (pll_fsm_t *) (fsm->instance);

    // Cancel Watchdog Timer
    xTimerStop(fs_fsm->timer, 0);

    pll_fsm_get_next_vco_id(pll_fsm, &next_vco_id);
    if (next_vco_id == UNDEFINED_VCO_ID)
    {
        fsm_handle_internal_event(fsm, FS_STATE_FAIL, NULL);
    }

    msr = GET_REG_FIELD(reg_read(WDDR_MEMORY_MAP_FSW + DDR_FSW_CTRL_STA__ADR), DDR_FSW_CTRL_STA_CMN_MSR);

    // Toggle MSR only if the HW would have during this switch
    if (GET_REG_FIELD(reg_read(WDDR_MEMORY_MAP_FSW + DDR_FSW_CTRL_CFG__ADR), DDR_FSW_CTRL_CFG_MSR_TOGGLE_EN))
    {
        msr = !msr;
    }

    // Override MSR / PLL VCO
    fsw_ctrl_set_msr_vco_ovr_val_reg_if(msr, next_vco_id);
    fsw_ctrl_set_msr_vco_ovr_reg_if(true);

    // Switch PLL to PHY Frequency
    pll_fsm_switch_event(pll_fsm, true);

    fsm_handle_internal_event(fsm, FS_STATE_WAIT_FOR_LOCK, fsm);
}

static void freq_switch_state_prep_switch(fsm_t *fsm, void *data)
{
    uint32_t reg_val;
    pll_fsm_t *pll_fsm = (pll_fsm_t *) (fsm->instance);
    fs_prep_data_t *fs_prep_data = (fs_prep_data_t *) data;

    // Don't toggle MSR if same MSR requested
    if (fs_prep_data->msr ==
        GET_REG_FIELD(reg_read(WDDR_MEMORY_MAP_FSW + DDR_FSW_CTRL_STA__ADR), DDR_FSW_CTRL_STA_CMN_MSR))
    {
        reg_val = reg_read(WDDR_MEMORY_MAP_FSW + DDR_FSW_CTRL_CFG__ADR);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CTRL_CFG_MSR_TOGGLE_EN, 0x0);
        reg_write(WDDR_MEMORY_MAP_FSW + DDR_FSW_CTRL_CFG__ADR, reg_val);
    }

    // Prepare PLL
    pll_fsm_prep_event(pll_fsm, &fs_prep_data->prep_data);
}

static void freq_switch_state_post_switch(fsm_t *fsm, __UNUSED__ void *data)
{
    uint32_t reg_val;

    // POST WORK DONE
    reg_val = reg_read(WDDR_MEMORY_MAP_FSW + DDR_FSW_CTRL_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CTRL_CFG_PSTWORK_DONE, 0x1);
    reg_write(WDDR_MEMORY_MAP_FSW + DDR_FSW_CTRL_CFG__ADR, reg_val);

    // Put back in default state
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CTRL_CFG_PSTWORK_DONE, 0x0);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CTRL_CFG_PSTWORK_DONE_OVR, 0x1);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CTRL_CFG_PREP_DONE, 0x0);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CTRL_CFG_VCO_TOGGLE_EN, 0x1);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CTRL_CFG_MSR_TOGGLE_EN, 0x1);
    reg_write(WDDR_MEMORY_MAP_FSW + DDR_FSW_CTRL_CFG__ADR, reg_val);

    fsm_handle_internal_event(fsm, FS_STATE_IDLE, NULL);
}

static void freq_switch_state_wait_for_lock(fsm_t *fsm, void *data)
{
    fs_fsm_t *fs_fsm = (fs_fsm_t *) data;
    pll_fsm_t *pll_fsm = (pll_fsm_t *) (fsm->instance);

    // If PLL already locked, go to post switch (only happens for SW switch)
    if (pll_fsm->current_state == PLL_STATE_LOCKED)
    {
        // Cancel Watchdog Timer
        xTimerStop(fs_fsm->timer, 0);

        fsm_handle_internal_event(fsm, FS_STATE_POST_SWITCH, NULL);
        return;
    }

    // Start timer
    if (xTimerReset(fs_fsm->timer, 0) == pdFALSE)
    {
        fsm_handle_internal_event(fsm, FS_STATE_FAIL, NULL);
    }

    // Let PLL know that HW switch happened
    if (pll_fsm->current_state == PLL_STATE_PREP_DONE)
    {
        pll_fsm_switch_event(pll_fsm, false);
    }
}

static void handle_phy_init_start(__UNUSED__ int irq_num, void *args)
{
    uint32_t reg_val;
    fs_fsm_t *fsm = (fs_fsm_t *) args;

    // Wait for INIT_START to go low
    do
    {
        reg_val = reg_read(WDDR_MEMORY_MAP_DFI + DDR_DFI_STATUS_IF_STA__ADR);
    } while(GET_REG_FIELD(reg_val, DDR_DFI_STATUS_IF_STA_REQ));

    // Mask interrupt
    disable_irq(MCU_FAST_IRQ_INIT_START);

    // Clear interrupt
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_CLR_CFG__ADR,
              FAST_IRQ_STICKY_MASK(DDR_IRQ_INIT_START));
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_CLR_CFG__ADR, 0x0);

    if (fsm->init_complete_cb)
    {
        // Override Init Complete and force low to Complete MRW
        reg_val = reg_read(WDDR_MEMORY_MAP_DFI + DDR_DFI_STATUS_IF_CFG__ADR);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_STATUS_IF_CFG_SW_ACK_VAL, 0x1);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_STATUS_IF_CFG_SW_ACK_OVR, 0x1);
        reg_write(WDDR_MEMORY_MAP_DFI + DDR_DFI_STATUS_IF_CFG__ADR, reg_val);

        // Set Post Work Done Override
        reg_val = reg_read(WDDR_MEMORY_MAP_FSW + DDR_FSW_CTRL_CFG__ADR);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CTRL_CFG_PSTWORK_DONE_OVR, 0x1);
        reg_write(WDDR_MEMORY_MAP_FSW + DDR_FSW_CTRL_CFG__ADR, reg_val);
    }

    // Enable INIT COMPLETE
    enable_irq(MCU_FAST_IRQ_INIT_COMPLETE);
}

static void handle_phy_init_complete(__UNUSED__ int irq_num, void *args)
{
    uint32_t reg_val;
    fs_fsm_t *fsm = (fs_fsm_t *) args;

    // Mask interrupt
    disable_irq(MCU_FAST_IRQ_INIT_COMPLETE);

    // Clear interrupt
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_CLR_CFG__ADR,
              FAST_IRQ_STICKY_MASK(DDR_IRQ_INIT_COMPLETE));
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_CLR_CFG__ADR, 0x0);

    // Init Complete CB is high priority, it will starve everything else
    if (fsm->init_complete_cb)
    {
        // Perform init complete callback
        fsm->init_complete_cb(fsm, fsm->init_complete_cb_args);

        // Release Init Complete Override
        reg_val = reg_read(WDDR_MEMORY_MAP_DFI + DDR_DFI_STATUS_IF_CFG__ADR);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_STATUS_IF_CFG_SW_ACK_OVR, 0x0);
        reg_write(WDDR_MEMORY_MAP_DFI + DDR_DFI_STATUS_IF_CFG__ADR, reg_val);
    }

    if (fsm->fsm.current_state == FS_STATE_WAIT_FOR_SWITCH)
    {
        // Wait for PLL Lock
        fsm_handle_interrupt_event(&fsm->fsm, FS_STATE_WAIT_FOR_LOCK, fsm);
    }
    else
    {
        // Error
        fsm_handle_interrupt_event(&fsm->fsm, FS_STATE_FAIL, NULL);
    }
}

static void pll_state_change_cb(__UNUSED__ fsm_t *pll_fsm, uint8_t state, void *args)
{
    fs_fsm_t *fs_fsm = (fs_fsm_t *) args;

    if (state == PLL_STATE_LOCKED &&
        fs_fsm->fsm.current_state == FS_STATE_WAIT_FOR_LOCK)
    {
        // Cancel Watchdog Timerr
        xTimerStop(fs_fsm->timer, 0);

        // State Transition
        fsm_handle_external_event(&fs_fsm->fsm, FS_STATE_POST_SWITCH, NULL);
    }
    else if (state == PLL_STATE_PREP_DONE &&
             fs_fsm->fsm.current_state == FS_STATE_PREP_SWITCH)
    {
        uint32_t reg_val;
        // Indicate Prep complete
        reg_val = reg_read(WDDR_MEMORY_MAP_FSW + DDR_FSW_CTRL_CFG__ADR);
        reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CTRL_CFG_PREP_DONE, 0x1);
        // If init_complete_cb exists, remove post work done override
        if (fs_fsm->init_complete_cb)
        {
            reg_val = UPDATE_REG_FIELD(reg_val, DDR_FSW_CTRL_CFG_PSTWORK_DONE_OVR, 0x0);
        }
        reg_write(WDDR_MEMORY_MAP_FSW + DDR_FSW_CTRL_CFG__ADR, reg_val);

        // Wait for transition
        fsm_handle_external_event(&fs_fsm->fsm, FS_STATE_WAIT_FOR_SWITCH, fs_fsm);
    }
    else if (state == PLL_STATE_NOT_LOCKED &&
             fs_fsm->fsm.current_state != FS_STATE_SWITCH &&
             fs_fsm->fsm.current_state != FS_STATE_WAIT_FOR_LOCK)
    {
        fsm_handle_external_event(&fs_fsm->fsm, FS_STATE_FAIL, NULL);
    }
}

static void watchdog_expired_handler(TimerHandle_t handle)
{
    fs_fsm_t *fsm = (fs_fsm_t *) pvTimerGetTimerID(handle);

    // Disable interrupts
    disable_irq(MCU_FAST_IRQ_INIT_START);
    disable_irq(MCU_FAST_IRQ_INIT_COMPLETE);

    // Error
    fsm_handle_external_event(&fsm->fsm, FS_STATE_FAIL, NULL);
}
