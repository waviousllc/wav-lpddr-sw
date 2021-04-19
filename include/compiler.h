/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _COMPILER_H_
#define _COMPILER_H_

#define __UNUSED__          __attribute__((unused))
#define __INTERRUPT__       __attribute__((interrupt))
#define __FALLTHROUGH__     __attribute__((fallthrough))
#define __O1__              __attribute__((optimize("O1")))
#define __O2__              __attribute__((optimize("O2")))
#define __O3__              __attribute__((optimize("O3")))

#endif /* _COMPILER_H_ */
