#include <stddef.h>
#include <wddr/memory_map.h>
#include <wddr/irq_map.h>
#include <kernel/io.h>
#include <kernel/irq.h>
#include <dfi_master/fsm.h>
#include <compiler.h>

/** @brief  DFI Master State Handler for IDLE state */
static void dfi_master_state_idle(fsm_t *fsm, void *data);

/** @brief  DFI Master State Handler for WAIT state */
static void dfi_master_state_wait(fsm_t *fsm, void *data);

/** @brief  DFI Master State Handler for MASTER state */
static void dfi_master_state_master(fsm_t *fsm, void *data);

/** @brief  DFI Master State Handler for REQ state */
static void dfi_master_state_req(fsm_t *fsm, void *data);

/** @brief  DFI Master State Handler for EXIT state */
static void dfi_master_state_exit(fsm_t *fsm, void *data);

/** @brief  IRQ Handler for all phymstr ACK events */
static void handle_dfi_phymstr_ack(int irq_num, void *args);

/** @brief  Table specifying state, guard, and exit functions for all states */
static const state_func_t state_table[] =
{
    // State Function           Guard Function  Exit Function
    {dfi_master_state_idle,     NULL,           NULL},
    {dfi_master_state_req,      NULL,           NULL},
    {dfi_master_state_wait,     NULL,           NULL},
    {dfi_master_state_master,   NULL,           NULL},
    {dfi_master_state_exit,     NULL,           NULL},
};

void dfi_master_fsm_init(dfi_master_fsm_t *fsm)
{
    fsm_init(&fsm->fsm, fsm, state_table);

    // Register interrupt handler
    request_irq(MCU_FAST_IRQ_PHYMSTR_ACK, handle_dfi_phymstr_ack, fsm);

    // Turn on sticky bit for IRQ
    uint32_t mask = reg_read(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_STICKY_CFG__ADR);
    mask |= FAST_IRQ_STICKY_MASK(DDR_IRQ_PHYMSTR_ACK);
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_STICKY_CFG__ADR, mask);

    // Ensure PHYMSTR ACK IRQ not enabled until desired in FSM
    disable_irq(MCU_FAST_IRQ_PHYMSTR_ACK);
}

wddr_return_t dfi_master_event_request(dfi_master_fsm_t *fsm, dfi_master_req_t *req)
{
    // Only state that is allowed is DFI_MASTER_STATE_IDLE
    if (fsm->fsm.current_state != DFI_MASTER_STATE_IDLE)
    {
       return WDDR_ERROR;
    }

    // Call into FSM
    fsm_handle_external_event(&fsm->fsm, DFI_MASTER_STATE_REQ, req);
    return WDDR_SUCCESS;
}

wddr_return_t dfi_master_event_exit(dfi_master_fsm_t *fsm)
{
    // Only state that is allowed is DFI_MASTER_STATE_MASTER
    if (fsm->fsm.current_state != DFI_MASTER_STATE_MASTER)
    {
       return WDDR_ERROR;
    }

    // Call into FSM
    fsm_handle_external_event(&fsm->fsm, DFI_MASTER_STATE_EXIT, NULL);
    return WDDR_SUCCESS;
}

static void dfi_master_state_idle(__UNUSED__ fsm_t *fsm, __UNUSED__ void *data)
{
    // Do Nothing
}

static void dfi_master_state_wait(__UNUSED__ fsm_t *fsm, __UNUSED__ void *data)
{
    enable_irq(MCU_FAST_IRQ_PHYMSTR_ACK);
}

static void dfi_master_state_master(__UNUSED__ fsm_t *fsm, __UNUSED__ void *data)
{
    // Do Nothing
}

static void dfi_master_state_req(fsm_t *fsm, void *data)
{
    // Get master signal states from input data
    dfi_master_req_t *req = (dfi_master_req_t *) data;

    // Update CSRs based on request state; ACK CSR
    uint32_t reg_val = reg_read(WDDR_MEMORY_MAP_DFI + DDR_DFI_PHYMSTR_IF_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYMSTR_IF_CFG_SW_TYPE, req->type);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYMSTR_IF_CFG_SW_CS_STATE, req->cs_state);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYMSTR_IF_CFG_SW_STATE_SEL, req->state_sel);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYMSTR_IF_CFG_SW_REQ_OVR, 0x1);
    reg_write(WDDR_MEMORY_MAP_DFI + DDR_DFI_PHYMSTR_IF_CFG__ADR, reg_val);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYMSTR_IF_CFG_SW_REQ_VAL, 0x1);
    reg_write(WDDR_MEMORY_MAP_DFI + DDR_DFI_PHYMSTR_IF_CFG__ADR, reg_val);

    // Wait for PHYMSTR_ACK
    fsm_handle_internal_event(fsm, DFI_MASTER_STATE_WAIT, NULL);
}

static void dfi_master_state_exit(fsm_t *fsm, __UNUSED__ void *data)
{
    // TODO: Should DFI MASTER be responsible for putting back DRAM???

    // Deassert Request
    uint32_t reg_val = reg_read(WDDR_MEMORY_MAP_DFI + DDR_DFI_PHYMSTR_IF_CFG__ADR);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYMSTR_IF_CFG_SW_REQ_VAL, 0x0);
    reg_write(WDDR_MEMORY_MAP_DFI + DDR_DFI_PHYMSTR_IF_CFG__ADR, reg_val);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYMSTR_IF_CFG_SW_EVENT_OVR, 0x1);
    reg_write(WDDR_MEMORY_MAP_DFI + DDR_DFI_PHYMSTR_IF_CFG__ADR, reg_val);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYMSTR_IF_CFG_SW_EVENT_VAL, 0x1);
    reg_write(WDDR_MEMORY_MAP_DFI + DDR_DFI_PHYMSTR_IF_CFG__ADR, reg_val);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYMSTR_IF_CFG_SW_REQ_OVR, 0x0);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYMSTR_IF_CFG_SW_TYPE, 0x0);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYMSTR_IF_CFG_SW_CS_STATE, 0x0);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYMSTR_IF_CFG_SW_STATE_SEL, 0x0);
    reg_write(WDDR_MEMORY_MAP_DFI + DDR_DFI_PHYMSTR_IF_CFG__ADR, reg_val);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYMSTR_IF_CFG_SW_EVENT_VAL, 0x0);
    reg_write(WDDR_MEMORY_MAP_DFI + DDR_DFI_PHYMSTR_IF_CFG__ADR, reg_val);
    reg_val = UPDATE_REG_FIELD(reg_val, DDR_DFI_PHYMSTR_IF_CFG_SW_EVENT_OVR, 0x0);
    reg_write(WDDR_MEMORY_MAP_DFI + DDR_DFI_PHYMSTR_IF_CFG__ADR, reg_val);

    // Ensure PHYMSTR ACK from MC is low before proceeding to IDLE
    do
    {
        reg_val = reg_read(WDDR_MEMORY_MAP_DFI + DDR_DFI_PHYMSTR_IF_STA__ADR);
    } while (GET_REG_FIELD(reg_val, DDR_DFI_PHYMSTR_IF_STA_ACK) != 0x0);

    // Go to IDLE
    fsm_handle_internal_event(fsm, DFI_MASTER_STATE_IDLE, NULL);
}

static void handle_dfi_phymstr_ack(__UNUSED__ int irq_num, void *args)
{
    dfi_master_fsm_t *fsm = (dfi_master_fsm_t *) args;

    disable_irq(MCU_FAST_IRQ_PHYMSTR_ACK);

    // Clear interrupt
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_CLR_CFG__ADR,
              FAST_IRQ_STICKY_MASK(DDR_IRQ_PHYMSTR_ACK));
    reg_write(WDDR_MEMORY_MAP_MCU + WAV_MCU_IRQ_FAST_CLR_CFG__ADR, 0x0);

    // PHYMSTR ACK recieved, move to MASTER state
    fsm_handle_interrupt_event(&fsm->fsm, DFI_MASTER_STATE_MASTER, NULL);
}
