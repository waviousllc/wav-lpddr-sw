/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DFI_DRIVER_H_
#define _DFI_DRIVER_H_

#include <stdint.h>
#include <stdbool.h>
#include <dfi/device.h>
#include <dfi/table.h>

/**
 * @brief   DFI Device Set MSR Register Interface
 *
 * @details Configures DFI device for given Mode Set Register.
 *
 * @param[in]   dfi     pointer to DFI device.
 * @param[in]   msr     MSR value to configure.
 *
 * @return      void
 */
void dfi_set_msr_reg_if(dfi_dev_t *dfi, wddr_msr_t msr);

/**
 * @brief   DFI Read Datapath Configuration Register Interface
 *
 * @details Configures DFI read datapath via CSR.
 *
 * @param[in]   dfi     pointer to DFI device.
 * @param[in]   cfg     pointer to pipeline configuration data.
 *
 * @return      void
 */
void dfi_rdd_cfg_reg_if(dfi_dev_t *dfi, dfi_pipe_cfg_t *cfg);

/**
 * @brief   DFI Write Datapath Configuration Register Interface
 *
 * @details Configures DFI write datapath via CSR.
 *
 * @param[in]   dfi     pointer to DFI device.
 * @param[in]   cfg     pointer to pipeline configuration data.
 *
 * @return      void
 */
void dfi_wrd_cfg_reg_if(dfi_dev_t *dfi, dfi_pipe_cfg_t *cfg);

/**
 * @brief   DFI CK Control Configuration Register Interface
 *
 * @details Configures DFI CK control via CSR.
 *
 * @param[in]   dfi     pointer to DFI device.
 * @param[in]   cfg     pointer to pipeline configuration data.
 *
 * @return      void
 */
void dfi_ckctrl_cfg_reg_if(dfi_dev_t *dfi, dfi_pipe_cfg_t *cfg);

/**
 * @brief   DFI Write Control Configuration Register Interface
 *
 * @details Configures DFI write control via CSR.
 *
 * @param[in]   dfi     pointer to DFI device.
 * @param[in]   cfg     pointer to pipeline configuration data.
 *
 * @return      void
 */
void dfi_wctrl_cfg_reg_if(dfi_dev_t *dfi, dfi_pipe_cfg_t *cfg);

/**
 * @brief   DFI Read Control Configuration Register Interface
 *
 * @details Configures DFI read control via CSR.
 *
 * @param[in]   dfi     pointer to DFI device.
 * @param[in]   cfg     pointer to pipeline configuration data.
 *
 * @return      void
 */
void dfi_rctrl_cfg_reg_if(dfi_dev_t *dfi, dfi_pipe_cfg_t *cfg);

/**
 * @brief   DFI WCK Control Configuration Register Interface
 *
 * @details Configures DFI WCK control via CSR.
 *
 * @param[in]   dfi     pointer to DFI device.
 * @param[in]   cfg     pointer to pipeline configuration data.
 *
 * @return      void
 */
void dfi_wckctrl_cfg_reg_if(dfi_dev_t *dfi, dfi_pipe_cfg_t *cfg);

/**
 * @brief   DFI Write Clock Control Configuration Register Interface
 *
 * @details Configures DFI write clock control via CSR.
 *
 * @param[in]   dfi     pointer to DFI device.
 * @param[in]   cfg     pointer to pipeline configuration data.
 *
 * @return      void
 */
void dfi_wrcctrl_cfg_reg_if(dfi_dev_t *dfi, dfi_pipe_cfg_t *cfg);

/**
 * @brief   DFI Write Clock Configuration Register Interface
 *
 * @details Configures DFI write clock via CSR.
 *
 * @param[in]   dfi     pointer to DFI device.
 * @param[in]   cfg     pointer to pipeline configuration data.
 *
 * @return      void
 */
void dfi_wrc_cfg_reg_if(dfi_dev_t *dfi, dfi_pipe_cfg_t *cfg);

/**
 * @brief   DFI Write Enable Configuration Register Interface
 *
 * @details Configures DFI write enable via CSR.
 *
 * @param[in]   dfi     pointer to DFI device.
 * @param[in]   cfg     pointer to pipeline configuration data.
 *
 * @return      void
 */
void dfi_wenctrl_cfg_reg_if(dfi_dev_t *dfi, dfi_pipe_cfg_t *cfg);

/**
 * @brief   DFI Pad Enable Phase Extension Configuration Register Interface
 *
 * @details Enables phase extension on pads via CSR.
 *
 * @param[in]   dfi     pointer to DFI device.
 * @param[in]   cfg     pointer to pipeline configuration data.
 *
 * @return      void
 */
void dfi_paden_pext_cfg_reg_if(dfi_dev_t *dfi, dfi_paden_pext_cfg_t *cfg);

/**
 * @brief   DFI Clock Enable Phase Extension Configuration Register Interface
 *
 * @details Enables phase extensions on clocks via CSR.
 *
 * @param[in]   dfi     pointer to DFI device.
 * @param[in]   cfg     pointer to pipeline configuration data.
 *
 * @return      void
 */
void dfi_clken_pext_cfg_reg_if(dfi_dev_t *dfi, dfi_clken_pext_cfg_t *cfg);

/**
 * @brief   DFI Override Traffic Confguration Register Interface
 *
 * @details Override traffic select and value via CSR.
 *
 * @param[in]   dfi     pointer to DFI device.
 * @param[in]   cfg     pointer to pipeline configuration data.
 *
 * @return      void
 */
void dfi_ovr_traffic_cfg_reg_if(dfi_dev_t *dfi, dfi_ovr_traffic_cfg_t *cfg);

/**
 * @brief   DFI Set Init Complete SW Override Register Interface
 *
 * @details Set Init Complete Override via CSR.
 *
 * @param[in]   override    Init Complete sw override enable.
 * @param[in]   val         Init Complete override value.
 *
 * @return      void
 */
void dfi_set_init_complete_ovr_reg_if(bool override, uint8_t val);

/**
 * @brief   DFI Set Init Start SW Override Register Interface
 *
 * @details Set Init Start Override via CSR.
 *
 * @param[in]   override    Init Start sw override enable.
 * @param[in]   val         Init Start override value.
 *
 * @return      void
 */
void dfi_set_init_start_ovr_reg_if(bool override, uint8_t val);

/**
 * @brief   DFI Get Init Start Status Register Interface
 *
 * @details Get Init Start Status via CSR.
 *
 * @return  returns status of init_start.
 * @retval  0 if init_start is deasserted (low).
 * @retval  1 if init_start is asserted (high).
 */
uint8_t dfi_get_init_start_status_reg_if(void);

/**
 * @brief   DFI Get Init Complete Status Register Interface
 *
 * @details Get Init Complete Status via CSR.
 *
 * @return  returns status of init_complete.
 * @retval  0 if init_complete is asserted (high).
 * @retval  1 if init_complete is deasserted (low).
 */
uint8_t dfi_get_init_complete_status_reg_if(void);

/**
 * @brief   DFI Command Address Read Loopback Enable Register Interface
 *
 * @details Enables Command Address (CA) Read Loopback path via CSR.
 *
 * @param[in]   enable  flag to indicate if CA Read loopback path
 *                      should be enabled.
 *
 * @return  void
 */
void dfi_ca_read_loopback_enable_reg_if(bool enable);

/**
 * @brief   DFI PHYMSTR Request Assert Register Interface
 *
 * @details Asserts PHYMSTR REQ to the Memory Controller.
 *
 * @param[in]   req     pointer to PHYMSTR request parameters.
 *
 * @return      void.
 */
void dfi_phymstr_req_assert_reg_if(dfi_phymstr_req_t *req);

/**
 * @brief   DFI PHYMSTR Request Deassert Register Interface
 *
 * @details Deasserts PHYMASTER REQ to the Memory Controller.
 *
 * @return  void.
 */
void dfi_phymstr_req_deassert_reg_if(void);

/**
 * @brief   DFI CTRLUPD Deassert Acknowledge Register Interface
 *
 * @details Deasserts CTRLUPD Acknowledge to the Memory Controller.
 *
 * @return  void.
 */
void dfi_ctrlupd_deassert_ack_reg_if(void);

/**
 * @brief   DFI CTRLUPD ACK Override Register Interface
 *
 * @details Overrides acknowledge signal of DFI CTRLUPD interface.
 *
 * @param[in]   override    flag to indicate if SW should override ack.
 * @param[in]   val         value to override (if enabled).
 *
 * @return      void.
 */
void dfi_ctrlupd_ack_override_reg_if(bool override, uint8_t val);

/**
 * @brief   DFI PHYUPD Request Assert Register Interface
 *
 * @details Asserts REQ signal of DFI PHYUPD interface. Singals to Memory
 *          Controller that IOCAL Update is required.
 *
 * @param[in]   type    The type of PHYUPD requested.
 *
 * @return      void.
 */
void dfi_phyupd_req_assert_reg_if(dfi_phyupd_type_t type);

/**
 * @brief   DFI PHYUPD Request Deassert Register Interface
 *
 * @details Deasserts REQ signal of DFI PHYUPD interface.
 *
 * @return      void.
 */
void dfi_phyupd_req_deassert_reg_if(void);

#endif /* _DFI_DRIVER_H_ */
