/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DFI_DP_H_
#define _DFI_DP_H_

#include <dfich_reg.h>
#include <dfi/table.h>
#include <wddr/phy_config.h>

/**
 * @brief   DFI Set Read Datapath Configuration Register Interface
 *
 * @details Configures DFI read datapath.
 *
 * @param[in]   dfich_reg   pointer to DFICH register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   cfg         pointer to pipeline configuration data.
 *
 * @return      void
 */
void dfi_set_rdd_cfg_reg_if(dfich_reg_t *dfich_reg,
                            wddr_msr_t msr,
                            dfi_pipe_cfg_t *cfg);

/**
 * @brief   DFI Set Write Datapath Configuration Register Interface
 *
 * @details Configures DFI write datapath.
 *
 * @param[in]   dfich_reg   pointer to DFICH register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   cfg         pointer to pipeline configuration data.
 *
 * @return      void
 */
void dfi_set_wrd_cfg_reg_if(dfich_reg_t *dfich_reg,
                            wddr_msr_t msr,
                            dfi_pipe_cfg_t *cfg);

/**
 * @brief   DFI Set CK Control Datapath Configuration Register Interface
 *
 * @details Configures DFI CK control datapath.
 *
 * @param[in]   dfich_reg   pointer to DFICH register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   cfg         pointer to pipeline configuration data.
 *
 * @return      void
 */
void dfi_set_ckctrl_cfg_reg_if(dfich_reg_t *dfich_reg,
                               wddr_msr_t msr,
                               dfi_pipe_cfg_t *cfg);

/**
 * @brief   DFI Set Write Control Datapath Configuration Register Interface
 *
 * @details Configures DFI write control datapath.
 *
 * @param[in]   dfich_reg   pointer to DFICH register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   cfg         pointer to pipeline configuration data.
 *
 * @return      void
 */
void dfi_set_wctrl_cfg_reg_if(dfich_reg_t *dfich_reg,
                              wddr_msr_t msr,
                              dfi_pipe_cfg_t *cfg);

/**
 * @brief   DFI Set Read Control Datapath Configuration Register Interface
 *
 * @details Configures DFI read control datapath.
 *
 * @param[in]   dfich_reg   pointer to DFICH register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   cfg         pointer to pipeline configuration data.
 *
 * @return      void
 */
void dfi_set_rctrl_cfg_reg_if(dfich_reg_t *dfich_reg,
                              wddr_msr_t msr,
                              dfi_pipe_cfg_t *cfg);

/**
 * @brief   DFI Set WCK Control Datapath Configuration Register Interface
 *
 * @details Configures DFI wck control datapath.
 *
 * @param[in]   dfich_reg   pointer to DFICH register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   cfg         pointer to pipeline configuration data.
 *
 * @return      void
 */
void dfi_set_wckctrl_cfg_reg_if(dfich_reg_t *dfich_reg,
                                wddr_msr_t msr,
                                dfi_pipe_cfg_t *cfg);

/**
 * @brief   DFI Set Write CLK Control Datapath Configuration Register Interface
 *
 * @details Configures DFI write clock control datapath.
 *
 * @param[in]   dfich_reg   pointer to DFICH register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   cfg         pointer to pipeline configuration data.
 *
 * @return      void
 */
void dfi_set_wrcctrl_cfg_reg_if(dfich_reg_t *dfich_reg,
                                wddr_msr_t msr,
                                dfi_pipe_cfg_t *cfg);
/**
 * @brief   DFI Set Write clock Datapath Configuration Register Interface
 *
 * @details Configures DFI write clock datapath.
 *
 * @param[in]   dfich_reg   pointer to DFICH register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   cfg         pointer to pipeline configuration data.
 *
 * @return      void
 */
void dfi_set_wrc_cfg_reg_if(dfich_reg_t *dfich_reg,
                            wddr_msr_t msr,
                            dfi_pipe_cfg_t *cfg);

/**
 * @brief   DFI Set Write En Control Datapath Configuration Register Interface
 *
 * @details Configures DFI write enable control datapath.
 *
 * @param[in]   dfich_reg   pointer to DFICH register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   cfg         pointer to pipeline configuration data.
 *
 * @return      void
 */
void dfi_set_wenctrl_cfg_reg_if(dfich_reg_t *dfich_reg,
                                wddr_msr_t msr,
                                dfi_pipe_cfg_t *cfg);

/**
 * @brief   DFI Set Pad Enable Phase Extension Configuration Register Interface
 *
 * @details Enables pad enable phase extensions.
 *
 * @param[in]   dfich_reg   pointer to DFICH register space.
 * @param[in]   msr         which msr register set to use.
 * @param[in]   cfg         pointer to paden_pext configuration data.
 *
 * @return      void
 */
void dfi_set_paden_pext_cfg_reg_if(dfich_reg_t *dfich_reg,
                                   wddr_msr_t msr,
                                   dfi_paden_pext_cfg_t *cfg);

/**
 * @brief   DFI Set Clk Enable Phase Extension Configuration Register Interface
 *
 * @details Enables clock enable phase extensions.
 *
 * @param[in]   dfich_reg   pointer to DFICH register space.
 * @param[in]   msr         which msr register set to use.
 * @param[in]   cfg         pointer to clken_pext configuration data.
 *
 * @return      void
 */
void dfi_set_clken_pext_cfg_reg_if(dfich_reg_t *dfich_reg,
                                   wddr_msr_t msr,
                                   dfi_clken_pext_cfg_t *cfg);

/**
 * @brief   DFI Set Traffic Override Configuration Register Interface
 *
 * @details Sets traffic override select and value.
 *
 * @param[in]   dfich_reg   pointer to DFICH register space.
 * @param[in]   msr         which msr register set to use.
 * @param[in]   cfg         pointer to traffic override configuration data.
 *
 * @return      void
 */
void dfi_set_ovr_traffic_cfg_reg_if(dfich_reg_t *dfich_reg,
                                    wddr_msr_t msr,
                                    dfi_ovr_traffic_cfg_t *cfg);

#endif /* _DFI_DP_H_ */
