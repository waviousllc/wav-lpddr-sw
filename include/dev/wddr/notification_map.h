/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */
#ifndef _WDDR_NOTIFICATION_MAP_H_
#define _WDDR_NOTIFICATION_MAP_H_

/**
 * @brief   WDDR Notification Enumerations
 *
 * FSW_PREP_DONE    Frequency Switch Prep Done notification.
 * FSW_DONE         Frequency Switch Done notification.
 * FSW_FAILED       Frequency Switch Failed notification.
 * END              Not a notification but marks end of enumeration. Can be
 *                  used for extension purposes.
 */
enum
{
    WDDR_NOTIF_FSW_PREP_DONE,
    WDDR_NOTIF_FSW_DONE,
    WDDR_NOTIF_FSW_FAILED,
    WDDR_NOTIF_END,
};

#endif /* _WDDR_NOTIFICATION_MAP_H_ */
