/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _WDDR_INTERFACE_H_
#define _WDDR_INTERFACE_H_

/* Standard includes. */
#include <stdint.h>
#include <stdbool.h>

/* Kernel includes. */
#include <kernel/list.h>

/* WDDR includes. */
#include "boot_options.h"
#include <dfi/command.h>

/**
 * @brief   WDDR Return Value Enumerations
 *
 * @details WDDR specific return values.
 *
 * SUCCESS                      No errors.
 * ERROR                        At least one error.
 * ERROR_NOT_VALID_FREQ         Invalid frequency requested.
 * ERROR_FIFO_FULL              The DFI IG FIFO is full.
 * ERROR_FIFO_EMPTY             The DFI EG FIFO is empty.
 */
typedef enum wddr_return_t
{
    WDDR_SUCCESS,
    WDDR_ERROR,
    WDDR_ERROR_NOT_VALID_FREQ,
    WDDR_ERROR_FIFO_FULL,
    WDDR_ERROR_FIFO_EMPTY,
} wddr_return_t;


// Opaque pointer type to represent a WDDR device
typedef struct wddr_device_t *wddr_handle_t;

/**
 * @brief   DFI PHYMSTR Type Enumerations
 *
 * @details Signals to MC which type of PHYMSTR REQ is being performed.
 *          This is system defined and signals expected amount of time that
 *          MC will relinquish control of DRAM to PHY.
 *
 * TYPE_0   Value corresponding to type 0 PHYMSTR timer.
 * TYPE_1   Value corresponding to type 1 PHYMSTR timer.
 * TYPE_2   Value corresponding to type 2 PHYMSTR timer.
 * TYPE_3   Value corresponding to type 3 PHYMSTR timer.
 */
typedef enum dfi_phymstr_type
{
    DFI_MASTER_TYPE_0,
    DFI_MASTER_TYPE_1,
    DFI_MASTER_TYPE_2,
    DFI_MASTER_TYPE_3,
} dfi_phymstr_type_t;

/**
 * @brief   DFI PHYMSTR Chipselect State Enumerations
 *
 * @details Indicates which ranks are active / inactive for PHYMSTR
 *          control.
 *
 * ACTIVE       active while PHY is master.
 * INACTIVE     inactive while PHY is master.
 */
typedef enum dfi_phymstr_cs_state
{
    DFI_MASTER_CS_STATE_ACTIVE = 0,
    DFI_MASTER_CS_STATE_INACTIVE = 1,
} dfi_phymstr_cs_state_t;

/**
 * @brief   DFI PHYMSTR State Select Enumerations
 *
 * @details Indicates which state the MC should place DRAM
 *          into. Only valid for Ranks where cs_state=0.
 *
 * IDLE         MC should place DRAM in IDLE state.
 * REFRESH      MC should place DRAM in REFRESH state.
 */
typedef enum dfi_phymstr_state_sel
{
    DFI_MASTER_STATE_SEL_IDLE = 0,
    DFI_MASTER_STATE_SEL_REFRESH = 1,
} dfi_phymstr_state_sel_t;

/**
 * @brief   DFI PHYMSTR Request Structure
 *
 * type         request type indicating requested time.
 * state_sel    request state DRAM should be placed in.
 * cs_state     request rank active/inactive state.
 */
typedef struct dfi_phymstr_req
{
    dfi_phymstr_type_t type;
    dfi_phymstr_state_sel_t state_sel;
    dfi_phymstr_cs_state_t cs_state;
} dfi_phymstr_req_t;

/**
 * @brief   DFI PHYUPD Type Enumerations
 *
 * TYPE_0   Value corresponding to type 0 PHYUPD timer.
 * TYPE_1   Value corresponding to type 1 PHYUPD timer.
 * TYPE_2   Value corresponding to type 2 PHYUPD timer.
 * TYPE_3   Value corresponding to type 3 PHYUPD timer.
 */
typedef enum dfi_phyupd_type
{
    DFI_PHYUPD_TYPE_0,
    DFI_PHYUPD_TYPE_1,
    DFI_PHYUPD_TYPE_2,
    DFI_PHYUPD_TYPE_3
} dfi_phyupd_type_t;

/**
 * @brief   Wavious DDR (WDDR) Initilization
 *
 * @details Initailizes WDDR Device for given base address.
 *
 * @note    Should only be called by firmware and only called once.
 *
 * @param[in]   base    base address of the WDDR device.
 *
 * @return  returns handle to wddr device.
 * @retval  handle to wddr device.
 * @retval  NULL if handle not created.
 */
wddr_handle_t wddr_init(uint32_t base);

/**
 * @brief   Wavious DDR (WDDR) Boot
 *
 * @details Boots WDDR Device. Should perform any startup initialization as
 *          well as any calibration or training sequences.

 * @note    Should only be called by firmware.
 *
 * @param[in]   wddr    WDDR device handle.
 * @param[in]   cfg     boot configuration.
 *
 * @return      returns whether boot completed successfully.
 * @retval      WDDR_SUCCESS if successful.
 * @retval      WDDR_ERROR otherwise.
 */
wddr_return_t wddr_boot(wddr_handle_t wddr, wddr_boot_cfg_t cfg);

/**
 * @brief   Wavious DDR (WDDR) Prep Switch
 *
 * @details Prepares WDDR device for a frequency switch.
 *
 * @note    Should only be called by firmware.
 *
 * @param[in]   wddr    WDDR device handle.
 * @param[in]   freq_id ID of frequency to prepare.
 * @param[in]   msr     mode switch register to prepare.
 *
 * @return      returns whether prep completed successfully.
 * @retval      WDDR_SUCCESS if successful.
 * @retval      WDDR_ERROR otherwise.
 */
wddr_return_t wddr_prep_switch(wddr_handle_t wddr,
                               uint8_t freq_id,
                               wddr_msr_t msr);

/**
 * @brief   Wavious DDR (WDDR) Frequency Switch
 *
 * @details Prepares and switches WDDR PHY Frequency. This doesnt' involve
 *          the DFI interface and switch independent of the Memory Controller.
 *          Thus, this function should only be called when safe (prior to
 *          completion of the boot sequence or during testing).
 *
 * @note    This function can only be used prior to boot. It's publically
 *          available so that it can be used externally during PHY Training.
 *
 * @param[in]   wddr    WDDR device handle.
 * @param[in]   freq_id ID of frequency to prepare and switch to.
 * @param[in]   msr     MSR to switch to.
 *
 * @return      returns whether switch completed sucessfully.
 * @retval      WDDR_SUCCESS if successful.
 * @retval      WDDR_ERROR if switch cannot be called.
 */
wddr_return_t wddr_sw_freq_switch(wddr_handle_t wddr,
                                  uint8_t freq_id,
                                  wddr_msr_t msr);
/**
 * @brief   Wavious DDR (WDDR) Handle Post Switch
 *
 * @details Handler function invoked after PHY switches frequencies. This
 *          function implementation is not required and can be left empty.
 *
 * @param[in]   wddr    WDDR device handle.
 *
 * @return      void.
 */
void wddr_handle_post_switch(wddr_handle_t wddr);

/**
 * @brief   Wavious DDR (WDDR) Handle Pending Switch
 *
 * @details Handler function invoked when a PHY frequency switch should occur.
 *          This function must allow the PHY frequency switch to proceed
 *          (DFI interface handshake / PLL swap).
 *
 * @param[in]   wddr    WDDR device handle.
 *
 * @return      void.
 */
void wddr_handle_pending_switch(wddr_handle_t wddr);

/**
 * @brief   Wavious DDR (WDDR) Get Current Mode Switch Register (MSR)
 *
 * @details Returns the current MSR that the current PHY configuration is
 *          using.
 *
 * @param[in]   wddr    WDDR device handle.
 *
 * @return  void.
 */
uint8_t wddr_get_current_msr(wddr_handle_t wddr);

/**
 * @brief   Wavious DDR (WDDR) Get Next Mode Switch Register (MSR)
 *
 * @details Returns the MSR that should be used to configure the PHY for the
 *          next frequency it will switch to.
 *
 * @param[in]   wddr    WDDR device handle.
 *
 * @return  void.
 */
uint8_t wddr_get_next_msr(wddr_handle_t wddr);

/**
 * @brief   Wavious DDR (WDDR) PHYMASTER Enter
 *
 * @details Puts the PHY into the PHYMASTER state. Should indicate via DFI
 *          interface.
 *
 * @param[in]   wddr    WDDR device handle.
 * @param[in]   req     pointer to PHYMSTR request parameters.
 *
 * @return      void.
 */
void wddr_phymstr_enter(wddr_handle_t wddr, dfi_phymstr_req_t *req);

/**
 * @brief   Wavious DDR (WDDR) PHYMASTER Exit
 *
 * @details Exits the PHYMMASTER state. Should indicate via DFI interface.
 *
 * @param[in]   wddr    WDDR device handle.
 *
 * @return      void.
 */
void wddr_phymstr_exit(wddr_handle_t wddr);

/**
 * @brief   Wavious DDR (WDDR) PHYUPDATE Enter
 *
 * @details Puts the PHY into the PHYUPDATE state. Should indicate via DFI
 *          interface.
 *
 * @param[in]   wddr    WDDR device handle.
 * @param[in]   type    the type of PHYUPDATE to be performed.
 *
 * @return      void.
 */
void wddr_phyupd_enter(wddr_handle_t wddr, dfi_phyupd_type_t type);

/**
 * @brief   Wavious DDR (WDDR) PHYUPDATE Exit
 *
 * @details Exits the PHYUPDATE state. Should indicate via DFI interface.
 *
 * @param[in]   wddr    WDDR device handle.
 *
 * @return      void.
 */
void wddr_phyupd_exit(wddr_handle_t wddr);

/**
 * @brief   Wavious DDR (WDDR) CTRLUPD Done
 *
 * @details Indicates that the CTRLUPD has been performed. Should indicate
 *          via DFI interface.
 *
 * @param[in]   wddr    WDDR device handle.
 *
 * @return  void.
 */
void wddr_ctrlupd_done(wddr_handle_t wddr);

/**
 * @brief   Wavious DDR (WDDR) Disable CTRLUPD Interface
 *
 * @details Disables the DFI CTRLUPD interface. This prevents CTRLUPD request
 *          events from being triggered in the firmware. Firmware should call
 *          this function if PHYUPD interface is being used to update PHY.
 *
 * @param[in]   wddr    WDDR device handle.
 *
 * @return  void.
 */
void wddr_disable_ctrlupd_interface(wddr_handle_t wddr);

/**
 * @brief   Wavious DDR (WDDR) IOCAL Update PHY
 *
 * @details Updates IOCAL CSRs in the PHY with latest calibrated values.
 *          Can only be called when in PHYUPD or CTRLUPD states.
 *          Do not call directly.
 *
 * @note    Should only be called by firmware.
 *
 * @param[in]   wddr    WDDR device handle.
 *
 * @return      void.
 */
void wddr_iocal_update_phy(wddr_handle_t wddr);

/**
 * @brief   Wavious DDR (WDDR) IOCAL Calibrate
 *
 * @details Calibrates IOCAL values. Calibration can be done regardless of
 *          DFI states. See wddr_iocal_update_phy for information about how
 *          to apply calibrated values to the PHY. Do not call directly.
 *
 * @note    Should only be called by firmware.
 *
 * @param[in]   wddr    WDDR device handle.
 *
 * @return      returns whether calibration completed succesfully.
 * @retval      WDDR_SUCCESS if sucessful.
 * @retval      WDDR_ERROR  otherwise.
 */
wddr_return_t wddr_iocal_calibrate(wddr_handle_t wddr);

/**
 * @brief   Wavious DDR (WDDR) Gate External Interfaces
 *
 * @details Gates signals to the DRAM and MC. It keeps the DRAM in RESET state
 *          and gates any DFI signals from strobbing. It is recommended that
 *          this function is only used when DRAM / MC access is not needed. This
 *          would only be during times valid per the DFI specification.
 *
 * @note    This function is ideal for test applications that use the DFI
 *          buffer.
 *
 * @param[in]   wddr            WDDR device handle.
 * @param[in]   should_gate     flag to indicate if external interfaces should
 *                              be gated.
 *
 * @return      void.
 */
void wddr_gate_external_interfaces(wddr_handle_t wddr, bool should_gate);

/**
 * @brief   Wavious DDR (WDDR) Enable Loopback Mode
 *
 * @details Puts the PHY into loopback mode. It enables the output drivers
 *          for loopback mode. It also configures any FIFOs for loopback. It
 *          doesn't configure other PHY components, i.e. datapath, for loopback
 *          mode. It is assumed the PHY is configured for loopback mode via a
 *          frequency switch prep and frequency switch.
 *
 * @note    Currenlty, this function doesn't support exiting loopback mode.
 *          There isn't a need to exit loopback mode once entering as it's
 *          only used for test applications. If it's ever needed for mission
 *          mode, then the interface will be updated to provide a mechanism to
 *          exit loopback mode.
 *
 * @param[in]   wddr            WDDR device handle.
 *
 * @return      void.
 */
void wddr_enable_loopback(wddr_handle_t wddr);

/**
 * @brief   Wavious DDR (WDDR) Load Packets
 *
 * @details Loads WDDR IG FIFO with given set of packets.
 *
 * @note    Call wddr_send_packets to send the loaded packets.
 *
 * @note    This function allows for packet loaded and packet sending to be
 *          separated. If you want to send packets immediately, then just call
 *          wddr_send_packets.
 *
 * @param[in]   wddr    WDDR device handle.
 * @param[in]   packets List of packets to load.
 *
 * @return      returns whether packets were loaded successfully.
 * @retval      WDDR_SUCCESS if successful.
 * @retval      WDDR_ERROR_FIFO_FULL if FIFO is full before all packets loaded.
 * @retval      WDDR_ERROR otherwise.
 */
wddr_return_t wddr_load_packets(wddr_handle_t wddr, const List_t *packets);

/**
 * @brief   Wavious DDR (WDDR) Send Packets
 *
 * @details Sends packets. Loads IG FIFO with given packets if FIFO is not
 *          already loaded.
 *
 * @param[in]   wddr    WDDR device handle.
 * @param[in]   packets List of packets to load (if not already loaded).
 *
 * @return      returns whether packets were loaded successfully.
 * @retval      WDDR_SUCCESS if successful.
 * @retval      WDDR_ERROR_FIFO_FULL if FIFO is full before all packets loaded.
 * @retval      WDDR_ERROR otherwise.
 */
wddr_return_t wddr_send_packets(wddr_handle_t wddr, const List_t *packets);

/**
 * @brief   Wavious DDR (WDDR) Validate Receive Data
 *
 * @details Validates that data received matches expected values.
 *
 * @param[in]   wddr            WDDR device handle.
 * @param[in]   data            data that is expected to be received.
 * @param[in]   bl              Burst length of data expected to recieve.
 * @param[in]   dq_byte_mask    mask to indicate which DQ bytes to validate.
 *
 * @return      returns whether received data was valid.
 * @retval      true if received data matches expected data.
 * @retval      false otherwise.
 */
bool wddr_validate_recv_data(wddr_handle_t wddr,
                             const command_data_t *data,
                             burst_length_t bl,
                             uint8_t dq_byte_mask);

#endif /* _WDDR_INTERFACE_H_ */
