/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _PLL_DEV_H_
#define _PLL_DEV_H_

#include <stdint.h>
#include <stdbool.h>
#include <error.h>
#include <vco/device.h>
#include <pll/table.h>

#define UNDEFINED_FREQ_ID   (255)
#define UNDEFINED_VCO_ID    (255)

/**
 * @brief   PLL Return Types
 *
 * SUCCESS      Successful return value.
 * ERROR        Error return value.
 * NOT_PREPPED  Error return value to indicate PLL not prepped for switch.
 */
typedef enum pll_return_t
{
    PLL_SUCCESS,
    PLL_ERROR,
    PLL_NOT_PREPPED,
} pll_return_t;

/**
 * @brief   PLL Device Structure
 *
 * @details PLL device structure that tracks current PLL state and Frequency.
 *          PLL design has 3 VCOs which drive PLL. One VCO is reserved to drive
 *          MCU clock frequency. The other 2 are used to drive PHY frequency.
 *          There is always a current VCO (which is current PHY frequency) and
 *          next which is used to prepare frequency change to a new
 *          PHY frequency.
 *
 * base             base address of the PLL device.
 * vco              Array of available VCO devices.
 * p_vco_current    pointer to current VCO device driving PLL.
 * p_vco_next       pointer to the next VCO device. Usually NULL unless prepared
 *                  PLL to switch to a new frequency.
 * p_vco_prev       pointer ot the previous VCO device. Usually NULL unless
 *                  recently switched PLL to a new frequency.
 */
typedef struct pll_dev_t
{
    uint32_t    base;
    vco_dev_t   vco[VCO_INDEX_NUM];
    vco_dev_t   *p_vco_current;
    vco_dev_t   *p_vco_next;
    vco_dev_t   *p_vco_prev;
} pll_dev_t;

/**
 * @brief   Phase Lock Loop (PLL) Initialization
 *
 * @details Initializes PLL device at device level.
 *
 * @param[in]   pll     pointer to PLL device.
 * @param[in]   base    base address of PLL device.
 *
 * @return      void
 */
void pll_init(pll_dev_t *pll, uint32_t base);

/**
 * @brief   Phase Lock Loop (PLL) Boot
 *
 * @details Performs PLL Boot seqeunce. Enables PLL to drive MCU at MCU clock
 *          frequency.
 *
 * @param[in]   pll     pointer to PLL device.
 *
 * @return      void
 */
void pll_boot(pll_dev_t *pll);

/**
 * @brief   Phase Lock Loop (PLL) Prepare VCO Switch
 *
 * @details Prepares PLL for a VCO switch. The new VCO will be configured
 *          per the given calibration and configuration structures. This is
 *          how the PLL is moved to a new frequency.
 *
 * @param[in]   pll     pointer to PLL device.
 * @param[in]   freq_id the frequency id of the new frequency being prepared.
 * @param[in]   cfg     pointer to PLL configuration data for the given
 *                      frequency.
 *
 * @return      void
 */
void pll_prepare_vco_switch(pll_dev_t *pll,
                            uint8_t freq_id,
                            pll_freq_cfg_t *cfg);
/**
 * @brief   Phase Lock Loop (PLL) Switch VCO
 *
 * @details Switches PLL to the prepared VCO. Typically, the HW will switch
 *          the VCO during normal operation. This function should still be
 *          called. An argument is provided to indicate if software should
 *          switch the VCO in HW or if it was already done by HW.
 *
 * @note    This function should only be called after calling
 *          pll_prepare_vco_switch.
 *
 * @param[in]   pll             pointer to PLL device.
 * @param[in]   is_sw_switch    flag to indicate if as SW switch should be
 *                              performed.
 *
 * @return      returns whether PLL was switched to the new VCO.
 * @return      PLL_SUCCESS if switched.
 * @return      PLL_NOT_PREPPED if VCO hasn't been prepared.
 */
pll_return_t pll_switch_vco(pll_dev_t *pll, bool is_sw_switch);

/**
 * @brief   Phase Lock Loop (PLL) Disable VCO
 *
 * @details Disables the previously used VCO. After switching VCOs, the other
 *          VCO continues to run to ensure a smooth transition. To save power,
 *          this function should be called to disable the unused VCO.
 *
 * @param[in]   pll     pointer to PLL device.
 *
 * @return      void
 */
void pll_disable_vco(pll_dev_t *pll);

/**
 * @brief   Phase Lock Loop (PLL) Calibrate VCOs
 *
 * @details Calibrates all PHY VCOs for the given frequency configuration.
 *          Calibrated values are stored in the frequency calibration structure.
 *
 * @param[in]   pll     pointer to PLL device.
 * @param[in]   cfg     pointer to PLL configuration structure.
 *
 * @return      void
 */
void pll_calibrate_vco(pll_dev_t *pll,
                       pll_freq_cfg_t *cfg);


/**
 * @brief   Phase Lock Loop (PLL) Set Loss Lock Interrupt State
 *
 * @details Enables or disables the PLL Loss Lock interrupt.
 *
 * @param[in]   pll     pointer to PLL device.
 * @param[in]   enable  state of interrupt.
 *
 * @return      void.
 */
void pll_set_loss_lock_interrupt_state(pll_dev_t *pll, bool enable);

/**
 * @brief   Phase Lock Loop (PLL) Set Lock Interrupt State
 *
 * @details Enables or disables the PLL Lock interrupt.
 *
 * @param[in]   pll     pointer to PLL device.
 * @param[in]   enable  state of interrupt.
 *
 * @return      void.
 */
void pll_set_lock_interrupt_state(pll_dev_t *pll, bool enable);

/**
 * @brief   Phase Lock Loop (PLL) Set Init Lock Interrupt State
 *
 * @details Enables or disables the PLL Init Lock interrupt.
 *
 * @param[in]   pll     pointer to PLL device.
 * @param[in]   enable  state of interrupt.
 *
 * @return      void.
 */
void pll_set_init_lock_interrupt_state(pll_dev_t *pll, bool enable);

/**
 * @brief   Phase Lock Loop (PLL) Get Current VCO
 *
 * @details Gets the VCO ID of the current VCO.
 *
 * @param[in]   pll     pointer to PLL device.
 * @param[out]  vco_id  pointer to store current vco id.
 *
 * @return  void.
 */
void pll_get_current_vco(pll_dev_t *pll, uint8_t *vco_id);

/**
 * @brief   Phase Lock Loop (PLL) Get Next VCO
 *
 * @details Gets the VCO ID of the next VCO. This indicates the VCO that
 *          has been prepared for a frequency switch, but has not been switched
 *          to yet. vco_id will be set to UNDEFINED_VCO_ID if VCO hasn't
 *          been prepared.
 *
 * @param[in]   pll     pointer to PLL device.
 * @param[out]  vco_id  pointer to store current vco id.
 *
 * @return  void.
 */
void pll_get_next_vco(pll_dev_t *pll, uint8_t *vco_id);

/**
 * @brief   Phase Lock Loop (PLL) Get Current Frequency
 *
 * @details Gets the FREQ ID of the current VCO.
 *
 * @param[in]   pll     pointer to PLL device.
 * @param[out]  freq_id pointer to store current freq id.
 *
 * @return  void.
 */
void pll_get_current_freq(pll_dev_t *pll, uint8_t *freq_id);

#endif  /* _PLL_DEV_H_ */
