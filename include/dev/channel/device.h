/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _CHANNEL_DEV_H_
#define _CHANNEL_DEV_H_

#include <dq/driver.h>
#include <ca/driver.h>
#include <channel/table.h>
#include <sensamp/table.h>
#include <wddr/interface.h>
#include <wddr/phy_config.h>
#include <error.h>

/**
 * @brief   Channel Device Structure
 *
 * @details Channel device structure that aggegrates DQ and CA paths in a
 *          channel.
 *
 * dq_reg   DQ Register Space
 * ca_reg   CA Register Space
 */
typedef struct channel_device
{
    dq_reg_t    *dq_reg[WDDR_PHY_DQ_BYTE_NUM];
    ca_reg_t    *ca_reg;
} channel_dev_t;


/**
 * @brief   Channel Device Initialzation
 *
 * @details Initializes Channel Device.
 *
 * @param[in]   channel_dev pointer to channel device.
 * @param[in]   base        base address of channel device.
 *
 * @return      void
 */
void channel_init(channel_dev_t *channel_dev, uint32_t base);

/**
 * @brief   Channel Enable
 *
 * @details Enables entire channel device (lpde and pis)
 *
 * @param[in]   channel_dev pointer to channel device.
 * @param[in]   enable      flag to indicate if channel should be enabled.
 * @param[in]   cfg         pointer to channel configuration data for the
 *                          current PHY frequency.
 *
 * @return      void.
 */
void channel_enable(channel_dev_t *channel_dev,
                    bool enable,
                    const channel_freq_cfg_t *cfg);

/**
 * @brief   Channel CA TX Driver Set Impedance All Bits
 *
 * @details Sets the Driver impedance for all bits for the given CA
 *          slice.
 *
 * @note    slice_type can only be CA or CK.
 *
 * @param[in]   channel_dev pointer to channel device.
 * @param[in]   slice_type  slice type of Driver device.
 * @param[in]   msr         current msr of the phy.
 * @param[in]   tx          tx impedance to set.
 * @param[in]   rx          rx impedance to set.
 *
 * @return      returns whether setting impedance was successful.
 * @retval      WDDR_SUCCESS if successful.
 * @retval      WDDR_ERROR if invalid slice_type given.
 */
wddr_return_t channel_ca_tx_driver_set_impedance_all_bits(channel_dev_t *channel_dev,
                                                          wddr_slice_type_t slice_type,
                                                          wddr_msr_t msr,
                                                          driver_impedance_t tx,
                                                          driver_impedance_t rx);

/**
 * @brief   Channel DQ TX Driver Set Impedance All Bits
 *
 * @details Sets the Driver impedance for all bits for the given DQ
 *          slice.
 *
 * @note    slice_type can only be DQ or DQS.
 *
 * @param[in]   channel_dev pointer to channel device.
 * @param[in]   slice_type  slice type of Driver device.
 * @param[in]   msr         current msr of the phy.
 * @param[in]   byte        which byte to set.
 * @param[in]   tx          tx impedance to set.
 * @param[in]   rx          rx impedance to set.
 *
 * @return      returns whether setting impedance was successful.
 * @retval      WDDR_SUCCESS if successful.
 * @retval      WDDR_ERROR if invalid slice_type given.
 */
wddr_return_t channel_dq_tx_driver_set_impedance_all_bits(channel_dev_t *channel_dev,
                                                          wddr_slice_type_t slice_type,
                                                          wddr_msr_t msr,
                                                          wddr_dq_byte_t byte,
                                                          driver_impedance_t tx,
                                                          driver_impedance_t rx);

/**
 * @brief   Channel CA DQ TX Driver Override All Bits
 *
 * @details Sets the Driver settings for all bits for the given CA slice.
 *
 * @note    slice_type can only be CA or CK.
 *
 * @param[in]   channel_dev     pointer to channel device.
 * @param[in]   slice_type      slice type of Driver device.
 * @param[in]   msr             current msr of the phy.
 * @param[in]   cfg             pointer to Driver device configuration.
 * @param[in]   output_enable   flag to indicate if output is enabled
 *
 * @return      returns whether override was successful.
 * @retval      WDDR_SUCCESS if successful.
 * @retval      WDDR_ERROR if invalid slice_type given.
 */
wddr_return_t channel_ca_tx_driver_override_all_bits(channel_dev_t *channel_dev,
                                                     wddr_slice_type_t slice_type,
                                                     wddr_msr_t msr,
                                                     const driver_cfg_t *cfg,
                                                     bool output_enable);


/**
 * @brief   Channel DQ TX Driver Override All Bits
 *
 * @details Sets the Driver settings for all bits for the given DQ slice.
 *
 * @note    slice_type can only be DQ or DQS.
 *
 * @param[in]   channel_dev     pointer to channel device.
 * @param[in]   slice_type      slice type of Driver device.
 * @param[in]   msr             current msr of the phy.
 * @param[in]   byte            which byte to set.
 * @param[in]   cfg             pointer to Driver device configuration.
 * @param[in]   output_enable   flag to indicate if output is enabled
 *
 * @return      returns whether override was successful.
 * @retval      WDDR_SUCCESS if successful.
 * @retval      WDDR_ERROR if invalid slice_type given.
 */
wddr_return_t channel_dq_tx_driver_override_all_bits(channel_dev_t *channel_dev,
                                                     wddr_slice_type_t slice_type,
                                                     wddr_msr_t msr,
                                                     wddr_dq_byte_t byte,
                                                     const driver_cfg_t *cfg,
                                                     bool output_enable);

/**
 * @brief   Channel RX SA DQ Byte Configure
 *
 * @details Configures all sensamp DQ bits in the DQ Byte.
 *
 * @param[in]       channel_dev     pointer to channel device.
 * @param[in]       byte            which byte to set.
 * @param[in]       calibrate       flag to indicate if calibration should be
 *                                  peformed.
 * @param[inout]    cfg             pointer to configuration table to fill in
 *                                  with calibrated codes.
 *
 * @return      returns whether calibraton completed successfully.
 * @retval      WDDR_SUCCESS if successful.
 * @retval      WDDR_ERROR otherwise.
 */
wddr_return_t channel_rx_sa_dqbyte_configure(channel_dev_t *channel_dev,
                                             wddr_dq_byte_t byte,
                                             bool calibrate,
                                             sensamp_dqbyte_common_cfg_t *cfg);

#endif /* _CHANNEL_DEV_H_ */
