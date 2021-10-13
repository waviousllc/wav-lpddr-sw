/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _ERROR_H_
#define _ERROR_H_

#define ASSERT_ERROR(cause)         \
    do {                            \
        return cause;               \
    } while(0)

#define PROPAGATE_ERROR(func)       \
    do {                            \
        int err = func;             \
        if (err)                    \
        {                           \
            return err;             \
        }                           \
    } while(0)

#endif /* _ERROR_H_ */
