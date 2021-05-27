/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _RECEIVER_DEV_H_
#define _RECEIVER_DEV_H_

#include <stdint.h>
#include <error.h>
#include <receiver/table.h>
#include <wddr/phy_defs.h>

/**
 * @brief Receiver State Enumerations
 *
 * @details Supported Receiver States.
 *
 * DISABLE  receiver not operational.
 * CAL      receiver in calibration mode.
 * ENABLE   receiver is enabled.
 */
typedef enum receiver_state_t
{
    REC_STATE_DISABLE,
    REC_STATE_CAL,
    REC_STATE_ENABLE
} receiver_state_t;

/**
 * @brief Receiver Delay Device Structure
 *
 * @details Receiver Delay device structure. This device is used to configure
 *          receiver delay.
 *
 * base    base address of the receiver delay device.
 *
 */
typedef struct receiver_delay_dev_t
{
    uint32_t    base;
} receiver_delay_dev_t;

/**
 * @brief   Receiver Device Structure
 *
 * @details Structure for Receiver device.
 *
 * base     base address of receiver device.
 * rx_delay Receiver Delay device used to control receiver delay.
 *
 */
typedef struct receiver_dev_t
{
    uint32_t                base;
    receiver_delay_dev_t    rx_delay;
} receiver_dev_t;

/**
 * @brief  Receiver Initalization
 *
 * @details Initializes Receiver Device at the device level.
 *
 * @param[in]   receiver    pointer to Receiver Device.
 * @param[in]   base        base address of Receiver Device.
 * @param[in]   rank        rank of Receiver Device.
 *
 * @return      void
 */
void receiver_init(receiver_dev_t *receiver, uint32_t base, wddr_rank_t rank);

/**
 * @brief   Receiver Enable
 *
 * @details Enables Reciver device.
 *
 * @param[in]   receiver    pointer to Receiver Device.
 *
 * @return      void
 */
void receiver_enable(receiver_dev_t  *receiver);

/**
 * @brief   Receiver Disable
 *
 * @details Disables Reciver device.
 *
 * @param[in]   receiver    pointer to Receiver Device.
 *
 * @return      void
 */
void receiver_disable(receiver_dev_t  *receiver);

/**
 * @brief   Receiver Calibrate
 *
 * @details Performs Receiver Calibration Procedure. The calibration is
 *          common for all frequencies.
 *
 * @note    WDDR PHY must be configured properly before calling this function.
 *          Calibration can only be performed if TX Driver is in loopback mode
 *          and MSR0 is set. These steps are not done in this procedure to
 *          avoid coupling unrelated devices together.
 *
 * @param[in]   receiver    pointer to Receiver Device.
 * @param[out]  cal         pointer to common frequncy calibration structure
 *                          to fill in with calibration data.
 * @param[in]   output_addr address where output of receiver can be read for
 *                          calibration feedback.
 *
 * @return      returns whether calibration was successful.
 * @return      WDDR_SUCCESS if successful.
 * @return      WDDR_ERROR otherwise.
 */
wddr_return_t receiver_calibrate(receiver_dev_t *receiver,
                                 receiver_common_cal_t *cal,
                                 uint32_t output_addr);

/**
 * @brief   Receiver Set Delay
 *
 * @details Sets the Receiver device delay.
 *
 * @param[in]   receiver    pointer to Receiver Device.
 * @param[in]   delay       delay (code) value to set.
 * @param[in]   side        mask indicating which side should be configured.
 *                          (T, C or both)
 *
 * @return      void
 */
void receiver_set_delay(receiver_dev_t *receiver,
                        uint16_t delay,
                        receiver_tc_side_mask_t side);

#endif /* _RECEIVER_DEV_H_ */
