/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _COMPILER_H_
#define _COMPILER_H_

#define __UNUSED__          __attribute__((unused))
#define __INTERRUPT__       __attribute__((interrupt))
#define __FALLTHROUGH__     __attribute__((fallthrough))
#define __PACKED__          __attribute__((__packed__))
#define __ALIGNED_4__       __attribute__((aligned(4)))
#define __ALIGNED_8__       __attribute__((aligned(8)))
#define __O1__              __attribute__((optimize("O1")))
#define __O2__              __attribute__((optimize("O2")))
#define __O3__              __attribute__((optimize("O3")))

#endif /* _COMPILER_H_ */
