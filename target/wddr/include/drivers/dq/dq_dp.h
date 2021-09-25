/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DQ_DP_H_
#define _DQ_DP_H_

#include <stdbool.h>
#include <dq_reg.h>
#include <wddr/phy_config.h>

/*******************************************************************************
**                            DQ
*******************************************************************************/
/**
 * @brief   DQ DQ Retimer Set Pipestage Enable Register Interface
 *
 * @details Sets the pipestage enable value for the Retimer stage in the
 *          data path.
 *
 * @param[in]   dq_reg      pointer to DQ Byte register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   rank        which Rank register set to use.
 * @param[in]   pipe_en     pipestage enable value to set.
 *
 * @return      void.
 */
void dq_dq_rt_set_pipe_en_reg_if(dq_reg_t *dq_reg,
                                 wddr_msr_t msr,
                                 wddr_rank_t rank,
                                 uint32_t pipe_en);

/**
 * @brief   DQ DQ SDR Set Full Cycle Delay Register Interface
 *
 * @details Sets the full cycle delay value for the SDR stage in the
 *          data path.
 *
 * @param[in]   dq_reg      pointer to DQ Byte register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   rank        which Rank register set to use.
 * @param[in]   bit         which bit register set to use.
 * @param[in]   fc_delay    full cycle delay value to set.
 *
 * @return      void.
 */
void dq_dq_sdr_set_fc_delay_reg_if(dq_reg_t *dq_reg,
                                   wddr_msr_t msr,
                                   wddr_rank_t rank,
                                   uint8_t bit,
                                   uint32_t fc_delay);

/**
 * @brief   DQ DQ SDR Set Pipestage Enable Register Interface
 *
 * @details Sets the pipestage enable value for the SDR stage in the
 *          data path.
 *
 * @param[in]   dq_reg      pointer to DQ Byte register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   rank        which Rank register set to use.
 * @param[in]   bit         which bit register set to use.
 * @param[in]   pipe_en     pipestage enable value to set.
 *
 * @return      void.
 */
void dq_dq_sdr_set_pipe_en_reg_if(dq_reg_t *dq_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  uint8_t bit,
                                  uint32_t pipe_en);

/**
 * @brief   DQ DQ SDR Set X Select Register Interface
 *
 * @details Sets the X Select value for the SDR stage in the
 *          data path. X Select selects order of phases that are
 *          sampled in the stage.
 *
 * @param[in]   dq_reg      pointer to DQ Byte register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   rank        which Rank register set to use.
 * @param[in]   bit         which bit register set to use.
 * @param[in]   x_sel       x select value to set.
 *
 * @return      void.
 */
void dq_dq_sdr_set_x_sel_reg_if(dq_reg_t *dq_reg,
                                wddr_msr_t msr,
                                wddr_rank_t rank,
                                uint8_t bit,
                                uint32_t x_sel);


/**
 * @brief   DQ DQ DDR Set Pipestage Enable Register Interface
 *
 * @details Sets the pipestage enable value for the DDR stage in the
 *          data path.
 *
 * @param[in]   dq_reg      pointer to DQ Byte register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   rank        which Rank register set to use.
 * @param[in]   bit         which bit register set to use.
 * @param[in]   pipe_en     pipestage enable value to set.
 *
 * @return      void.
 */
void dq_dq_ddr_set_pipe_en_reg_if(dq_reg_t *dq_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  uint8_t bit,
                                  uint32_t pipe_en);

/**
 * @brief   DQ DQ DDR Set X Select Register Interface
 *
 * @details Sets the X Select value for the DDR stage in the
 *          data path. X Select selects order of phases that are
 *          sampled in the stage.
 *
 * @param[in]   dq_reg      pointer to DQ Byte register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   rank        which Rank register set to use.
 * @param[in]   bit         which bit register set to use.
 * @param[in]   x_sel       x select value to set.
 *
 * @return      void.
 */
void dq_dq_ddr_set_x_sel_reg_if(dq_reg_t *dq_reg,
                                wddr_msr_t msr,
                                wddr_rank_t rank,
                                uint8_t bit,
                                uint32_t x_sel);

/**
 * @brief   DQ DQ QDR Set Pipestage Enable Register Interface
 *
 * @details Sets the pipestage enable value for the QDR stage in the
 *          data path.
 *
 * @param[in]   dq_reg      pointer to DQ Byte register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   rank        which Rank register set to use.
 * @param[in]   bit         which bit register set to use.
 * @param[in]   pipe_en     pipestage enable value to set.
 *
 * @return      void.
 */
void dq_dq_qdr_set_pipe_en_reg_if(dq_reg_t *dq_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  uint8_t bit,
                                  uint32_t pipe_en);

/**
 * @brief   DQ DQ QDR Set X Select Register Interface
 *
 * @details Sets the X Select value for the QDR stage in the
 *          data path. X Select selects order of phases that are
 *          sampled in the stage.
 *
 * @param[in]   dq_reg      pointer to DQ Byte register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   rank        which Rank register set to use.
 * @param[in]   bit         which bit register set to use.
 * @param[in]   x_sel       x select value to set.
 *
 * @return      void.
 */
void dq_dq_qdr_set_x_sel_reg_if(dq_reg_t *dq_reg,
                                wddr_msr_t msr,
                                wddr_rank_t rank,
                                uint8_t bit,
                                uint32_t x_sel);

/*******************************************************************************
**                            DQS
*******************************************************************************/
/**
 * @brief   DQ DQS Set Retimer Pipestage Enable Register Interface
 *
 * @details Sets the pipestage enable value for the Retimer stage in the
 *          data path.
 *
 * @param[in]   dq_reg      pointer to DQ Byte register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   rank        which Rank register set to use.
 * @param[in]   pipe_en     Pipestage enable value to set.
 *
 * @return      void.
 */
void dq_dqs_rt_set_pipe_en_reg_if(dq_reg_t *dq_reg,
                                  wddr_msr_t msr,
                                  wddr_rank_t rank,
                                  uint32_t pipe_en);

/**
 * @brief   DQ DQS SDR Set Full Cycle Delay Register Interface
 *
 * @details Sets the full cycle delay value for the SDR stage in the
 *          data path.
 *
 * @param[in]   dq_reg      pointer to DQ Byte register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   rank        which Rank register set to use.
 * @param[in]   bit         which bit register set to use.
 * @param[in]   fc_delay    full cycle delay value to set.
 *
 * @return      void.
 */
void dq_dqs_sdr_set_fc_delay_reg_if(dq_reg_t *dq_reg,
                                    wddr_msr_t msr,
                                    wddr_rank_t rank,
                                    uint8_t bit,
                                    uint32_t fc_delay);

/**
 * @brief   DQ DQS SDR Set Pipestage Enable Register Interface
 *
 * @details Sets the pipestage enable value for the SDR stage in the
 *          data path.
 *
 * @param[in]   dq_reg      pointer to DQ Byte register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   rank        which Rank register set to use.
 * @param[in]   bit         which bit register set to use.
 * @param[in]   pipe_en     pipestage enable value to set.
 *
 * @return      void.
 */
void dq_dqs_sdr_set_pipe_en_reg_if(dq_reg_t *dq_reg,
                                   wddr_msr_t msr,
                                   wddr_rank_t rank,
                                   uint8_t bit,
                                   uint32_t pipe_en);

/**
 * @brief   DQ DQS SDR Set X Select Register Interface
 *
 * @details Sets the X Select value for the SDR stage in the
 *          data path. X Select selects order of phases that are
 *          sampled in the stage.
 *
 * @param[in]   dq_reg      pointer to DQ Byte register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   rank        which Rank register set to use.
 * @param[in]   bit         which bit register set to use.
 * @param[in]   x_sel       x select value to set.
 *
 * @return      void.
 */
void dq_dqs_sdr_set_x_sel_reg_if(dq_reg_t *dq_reg,
                                 wddr_msr_t msr,
                                 wddr_rank_t rank,
                                 uint8_t bit,
                                 uint32_t x_sel);

/**
 * @brief   DQ DQS DDR Set Pipestage Enable Register Interface
 *
 * @details Sets the pipestage enable value for the DDR stage in the
 *          data path.
 *
 * @param[in]   dq_reg      pointer to DQ Byte register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   rank        which Rank register set to use.
 * @param[in]   bit         which bit register set to use.
 * @param[in]   pipe_en     pipestage enable value to set.
 *
 * @return      void.
 */
void dq_dqs_ddr_set_pipe_en_reg_if(dq_reg_t *dq_reg,
                                   wddr_msr_t msr,
                                   wddr_rank_t rank,
                                   uint8_t bit,
                                   uint32_t x_sel);

/**
 * @brief   DQ DQS DDR Set X Select Register Interface
 *
 * @details Sets the X Select value for the DDR stage in the
 *          data path. X Select selects order of phases that are
 *          sampled in the stage.
 *
 * @param[in]   dq_reg      pointer to DQ Byte register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   rank        which Rank register set to use.
 * @param[in]   bit         which bit register set to use.
 * @param[in]   x_sel       x select value to set.
 *
 * @return      void.
 */
void dq_dqs_ddr_set_x_sel_reg_if(dq_reg_t *dq_reg,
                                 wddr_msr_t msr,
                                 wddr_rank_t rank,
                                 uint8_t bit,
                                 uint32_t x_sel);

/**
 * @brief   DQ DQS QDR Set Pipestage Enable Register Interface
 *
 * @details Sets the pipestage enable value for the QDR stage in the
 *          data path.
 *
 * @param[in]   dq_reg      pointer to DQ Byte register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   rank        which Rank register set to use.
 * @param[in]   bit         which bit register set to use.
 * @param[in]   pipe_en     pipestage enable value to set.
 *
 * @return      void.
 */
void dq_dqs_qdr_set_pipe_en_reg_if(dq_reg_t *dq_reg,
                                   wddr_msr_t msr,
                                   wddr_rank_t rank,
                                   uint8_t bit,
                                   uint32_t pipe_en);

/**
 * @brief   DQ DQS QDR Set X Select Register Interface
 *
 * @details Sets the X Select value for the QDR stage in the
 *          data path. X Select selects order of phases that are
 *          sampled in the stage.
 *
 * @param[in]   dq_reg      pointer to DQ Byte register space.
 * @param[in]   msr         which MSR register set to use.
 * @param[in]   rank        which Rank register set to use.
 * @param[in]   bit         which bit register set to use.
 * @param[in]   x_sel       x select value to set.
 *
 * @return      void.
 */
void dq_dqs_qdr_set_x_sel_reg_if(dq_reg_t *dq_reg,
                                 wddr_msr_t msr,
                                 wddr_rank_t rank,
                                 uint8_t bit,
                                 uint32_t x_sel);

#endif /* _DQ_DP_H_ */
