/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
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
 * @param[in]   enable  Init Complete Override CSR value.
 *
 * @return      void
 */
void dfi_set_init_complete_ovr_reg_if(bool enable);

/**
 * @brief   DFI Set Init Start SW Override Register Interface
 *
 * @details Set Init Start Override via CSR.
 *
 * @param[in]   enable  Init Start Override CSR value.
 *
 * @return      void
 */
void dfi_set_init_start_ovr_reg_if(bool enable);

/**
 * @brief   DFI Get Init Start Status Register Interface
 *
 * @details Get Init Start Status via CSR.
 *
 * @param[out]  init_start  Pointer to return current Init Start Status.
 *
 * @return      void
 */
void dfi_get_init_start_status_reg_if(uint32_t *init_start);

#endif /* _DFI_DRIVER_H_ */
