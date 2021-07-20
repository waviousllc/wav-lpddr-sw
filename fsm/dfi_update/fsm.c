#include <stddef.h>
#include <wddr/memory_map.h>
#include <wddr/irq_map.h>
#include <kernel/io.h>
#include <kernel/irq.h>
#include <dfi_update/fsm.h>
#include <compiler.h>

/** @brief  DFI Update State Handler for IDLE state */
static void dfi_update_state_idle(fsm_t *fsm, void *data);

/** @brief  DFI Update State Handler for REQ state */
static void dfi_update_state_req(fsm_t *fsm, void *data);

/** @brief  DFI Update State Handler for CTRLUPD_WAIT state */
static void dfi_update_state_ctrlupd_wait(fsm_t *fsm, void *data);

/** @brief  DFI Update State Handler for PHYUPD_WAIT state */
static void dfi_update_state_phyupd_wait(fsm_t *fsm, void *data);

/** @brief  DFI Update State Handler for CAL state */
static void dfi_update_state_cal(fsm_t *fsm, void *data);

/** @brief  DFI Update State Handler for UPDATE state */
static void dfi_update_state_update(fsm_t *fsm, void *data);

/** @brief  DFI Update State Handler for UPDATE_EXIT state */
static void dfi_update_state_update_exit(fsm_t *fsm, void *data);

/** @brief  IRQ Handler for all ctrlupd events */
static void handle_dfi_ctrlupd_irq(int irq_num, void *args);

/** @brief  IRQ Handler for all phyupd ACK events */
static void handle_dfi_phyupd_ack(int irq_num, void *args);

/** @brief  Table specifying state, guard, and exit functions for all states */
static const state_func_t state_table[] =
{
    // State Function               Guard Function  Exit Function
    {dfi_update_state_idle,         NULL,           NULL},
    {dfi_update_state_req,          NULL,           NULL},
    {dfi_update_state_ctrlupd_wait, NULL,           NULL},
    {dfi_update_state_phyupd_wait,  NULL,           NULL},
    {dfi_update_state_cal,          NULL,           NULL},
    {dfi_update_state_update,       NULL,           NULL},
    {dfi_update_state_update_exit,  NULL,           NULL},
};

void dfi_update_fsm_init(dfi_update_fsm_t *fsm,
                         void *dev,
                         const iocal_update_phy_fn_t iocal_update_phy,
                         const iocal_calibrate_fn_t iocal_calibrate)
{
    uint32_t reg_val;
    fsm_init(&fsm->fsm, fsm, state_table);
    fsm->intf.dev = dev;
    fsm->intf.iocal_calibrate = iocal_calibrate;
    fsm->intf.iocal_update_phy = iocal_update_phy;

    // Force ACK low to ignore ctrlupd requests
    reg_val = reg_read(WDDR_MEMORY_MAP_DFI + DDR_DFI_CTRLUPD_IF_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_CTRLUPD_IF_CFG_SW_ACK_VAL, 0x0);
    reg_write(WDDR_MEMORY_MAP_DFI + DDR_DFI_CTRLUPD_IF_CFG__ADR, reg_val);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_CTRLUPD_IF_CFG_SW_ACK_OVR, 0x1);
    reg_write(WDDR_MEMORY_MAP_DFI + DDR_DFI_CTRLUPD_IF_CFG__ADR, reg_val);

    // Register interrupt handlers
    request_irq(MCU_FAST_IRQ_CTRLUPD_REQ, handle_dfi_ctrlupd_irq, fsm);
    request_irq(MCU_FAST_IRQ_PHYUPD_ACK, handle_dfi_phyupd_ack, fsm);

    // Turn on sticky bit for IRQ
    uint32_t mask = reg_read(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_STICKY_CFG__ADR);
    mask |= FAST_IRQ_STICKY_MASK(DDR_IRQ_PHYUPD_ACK) |
            FAST_IRQ_STICKY_MASK(DDR_IRQ_CTRLUPD_REQ_ASSERTION) |
            FAST_IRQ_STICKY_MASK(DDR_IRQ_CTRLUPD_REQ_DEASSERTION);
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_STICKY_CFG__ADR, mask);

    // Ensure PHYUPD ACK IRQ not enabled until desired in FSM
    disable_irq(MCU_FAST_IRQ_PHYUPD_ACK);

    /**
     * @note The interrupt for MCU_FAST_IRQ_CTRLUPD_REQ is disabled as requests
     *       from mc should be ignored.
     */
    disable_irq(MCU_FAST_IRQ_CTRLUPD_REQ);
}

wddr_return_t dfi_update_event_req_update(dfi_update_fsm_t *fsm, dfi_phyupd_type_t type)
{
    // Only state that is allowed is DFI_UPDATE_STATE_IDLE
    if (fsm->fsm.current_state != DFI_UPDATE_STATE_IDLE)
    {
        return WDDR_ERROR;
    }

    // Call into FSM
    fsm_handle_external_event(&fsm->fsm, DFI_UPDATE_STATE_REQ, &type);
    return WDDR_SUCCESS;
}

static void dfi_update_state_idle(__UNUSED__ fsm_t *fsm, __UNUSED__ void *data)
{
    // Do Nothing
}

static void dfi_update_state_req(__UNUSED__ fsm_t *fsm, void *data)
{
    uint32_t reg_val;
    dfi_phyupd_type_t *type = (dfi_phyupd_type_t *) data;

    // Assert PHYUPD REQ and set type
    reg_val = reg_read(WDDR_MEMORY_MAP_DFI + DDR_DFI_PHYUPD_IF_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYUPD_IF_CFG_SW_TYPE, *type);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYUPD_IF_CFG_SW_REQ_OVR, 0x1);
    reg_write(WDDR_MEMORY_MAP_DFI + DDR_DFI_PHYUPD_IF_CFG__ADR, reg_val);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYUPD_IF_CFG_SW_REQ_VAL, 0x1);
    reg_write(WDDR_MEMORY_MAP_DFI + DDR_DFI_PHYUPD_IF_CFG__ADR, reg_val);

    // Wait for PHYUPD ACK IRQ
    fsm_handle_internal_event(fsm, DFI_UPDATE_STATE_PHYUPD_WAIT, NULL);
}

static void dfi_update_state_ctrlupd_wait(__UNUSED__ fsm_t *fsm, __UNUSED__ void *data)
{
    // Do Nothing
}

static void dfi_update_state_phyupd_wait(__UNUSED__ fsm_t *fsm, __UNUSED__ void *data)
{
    // Enable PHYUPD ACK interrupt
    enable_irq(MCU_FAST_IRQ_PHYUPD_ACK);
}

static void dfi_update_state_cal(fsm_t *fsm, __UNUSED__ void *data)
{
    dfi_update_fsm_t *update_fsm = (dfi_update_fsm_t *) (fsm->instance);
    uint32_t reg_val;

    // Perform ZQCAL
    update_fsm->intf.iocal_calibrate(update_fsm->intf.dev);
    update_fsm->intf.iocal_update_phy(update_fsm->intf.dev);

    // Deassert CTRLUPD ACK
    reg_val = reg_read(WDDR_MEMORY_MAP_DFI + DDR_DFI_CTRLUPD_IF_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_CTRLUPD_IF_CFG_SW_ACK_VAL, 0x0);
    reg_write(WDDR_MEMORY_MAP_DFI + DDR_DFI_CTRLUPD_IF_CFG__ADR, reg_val);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_CTRLUPD_IF_CFG_SW_EVENT_1_OVR, 0x1);
    reg_write(WDDR_MEMORY_MAP_DFI + DDR_DFI_CTRLUPD_IF_CFG__ADR, reg_val);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_CTRLUPD_IF_CFG_SW_EVENT_1_VAL, 0x1);
    reg_write(WDDR_MEMORY_MAP_DFI + DDR_DFI_CTRLUPD_IF_CFG__ADR, reg_val);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_CTRLUPD_IF_CFG_SW_ACK_OVR, 0x1);
    reg_write(WDDR_MEMORY_MAP_DFI + DDR_DFI_CTRLUPD_IF_CFG__ADR, reg_val);
    do
    {
        reg_val = reg_read(WDDR_MEMORY_MAP_DFI + DDR_DFI_CTRLUPD_IF_STA__ADR);
    } while (GET_REG_FIELD(reg_val, DDR_DFI_CTRLUPD_IF_STA_REQ) != 0x0);
    reg_val = reg_read(WDDR_MEMORY_MAP_DFI + DDR_DFI_CTRLUPD_IF_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_CTRLUPD_IF_CFG_SW_EVENT_1_VAL, 0x0);
    reg_write(WDDR_MEMORY_MAP_DFI + DDR_DFI_CTRLUPD_IF_CFG__ADR, reg_val);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_CTRLUPD_IF_CFG_SW_EVENT_1_OVR, 0x0);
    reg_write(WDDR_MEMORY_MAP_DFI + DDR_DFI_CTRLUPD_IF_CFG__ADR, reg_val);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_CTRLUPD_IF_CFG_SW_ACK_OVR, 0x0);
    reg_write(WDDR_MEMORY_MAP_DFI + DDR_DFI_CTRLUPD_IF_CFG__ADR, reg_val);

    // Wait for DEASSERT IRQ
    fsm_handle_internal_event(fsm, DFI_UPDATE_STATE_CTRLUPD_WAIT, NULL);
}

static void dfi_update_state_update(fsm_t *fsm, __UNUSED__ void *data)
{
    dfi_update_fsm_t *update_fsm = (dfi_update_fsm_t *) (fsm->instance);
    update_fsm->intf.iocal_update_phy(update_fsm->intf.dev);

    // Done
    fsm_handle_internal_event(fsm, DFI_UPDATE_STATE_UPDATE_EXIT, NULL);
}

static void dfi_update_state_update_exit(fsm_t *fsm, __UNUSED__ void *data)
{
    uint32_t reg_val;

    // Deassert PHYUPD REQ
    reg_val = reg_read(WDDR_MEMORY_MAP_DFI + DDR_DFI_PHYUPD_IF_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYUPD_IF_CFG_SW_REQ_VAL, 0x0);
    reg_write(WDDR_MEMORY_MAP_DFI + DDR_DFI_PHYUPD_IF_CFG__ADR, reg_val);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYUPD_IF_CFG_SW_EVENT_OVR, 0x1);
    reg_write(WDDR_MEMORY_MAP_DFI + DDR_DFI_PHYUPD_IF_CFG__ADR, reg_val);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYUPD_IF_CFG_SW_EVENT_VAL, 0x1);
    reg_write(WDDR_MEMORY_MAP_DFI + DDR_DFI_PHYUPD_IF_CFG__ADR, reg_val);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYUPD_IF_CFG_SW_REQ_OVR, 0x0);
    reg_write(WDDR_MEMORY_MAP_DFI + DDR_DFI_PHYUPD_IF_CFG__ADR, reg_val);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYUPD_IF_CFG_SW_EVENT_VAL, 0x0);
    reg_write(WDDR_MEMORY_MAP_DFI + DDR_DFI_PHYUPD_IF_CFG__ADR, reg_val);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYUPD_IF_CFG_SW_EVENT_OVR, 0x0);
    reg_write(WDDR_MEMORY_MAP_DFI + DDR_DFI_PHYUPD_IF_CFG__ADR, reg_val);

    // Ensure PHYUPD ACK from MC is low before proceeding to IDLE
    do
    {
        reg_val = reg_read(WDDR_MEMORY_MAP_DFI + DDR_DFI_PHYUPD_IF_STA__ADR);
    } while (GET_REG_FIELD(reg_val, DDR_DFI_PHYUPD_IF_STA_ACK) != 0x0);

    // Done
    fsm_handle_internal_event(fsm, DFI_UPDATE_STATE_IDLE, NULL);
}

static void handle_dfi_ctrlupd_irq(__UNUSED__ int irq_num, void *args)
{
    uint32_t reg_val;
    dfi_update_fsm_t *fsm = (dfi_update_fsm_t *) args;
    dfi_update_state_t next;

    // Determine if ASSERT or DEASSERT
    reg_val = reg_read(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_STA__ADR);
    if (reg_val & FAST_IRQ_STICKY_MASK(DDR_IRQ_CTRLUPD_REQ_ASSERTION))
    {
        next = DFI_UPDATE_STATE_CAL;
    }
    else if (reg_val & FAST_IRQ_STICKY_MASK(DDR_IRQ_CTRLUPD_REQ_DEASSERTION))
    {
        next = DFI_UPDATE_STATE_IDLE;
    }
    // TODO: ERROR handling
    else
    {
        return;
    }
    // Clear interrupt
    reg_val = FAST_IRQ_STICKY_MASK(DDR_IRQ_CTRLUPD_REQ_ASSERTION) |
              FAST_IRQ_STICKY_MASK(DDR_IRQ_CTRLUPD_REQ_DEASSERTION);
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_CLR_CFG__ADR, reg_val);
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_CLR_CFG__ADR, 0x0);

    fsm_handle_interrupt_event(&fsm->fsm, next, NULL);
}

static void handle_dfi_phyupd_ack(__UNUSED__ int irq_num, void *args)
{
    dfi_update_fsm_t *fsm = (dfi_update_fsm_t *) args;

    // Disable interrupt to prevent from firing again
    disable_irq(MCU_FAST_IRQ_PHYUPD_ACK);

    // Clear interrupt
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_CLR_CFG__ADR,
              FAST_IRQ_STICKY_MASK(DDR_IRQ_PHYUPD_ACK));
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_CLR_CFG__ADR, 0x0);

    // PHYUPD ACK recieved, move to UPDATE state
    fsm_handle_interrupt_event(&fsm->fsm, DFI_UPDATE_STATE_UPDATE, NULL);
}
