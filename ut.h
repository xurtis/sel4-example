/*
 * Copyright 2021, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * This is a leaky allocator that provides no mechanism to reuse
 * allocated kernel objects.
 */

#include "cptr.h"

#define UT_INIT_PRIO (CPTR_INIT_PRIO + 1)

#define MAX_UT_SIZE 17

/* Allocates an non-device object of size 2^size_bits */
seL4_CPtr ut_alloc(unsigned size_bits);

/* Allocates a new non-device object of a particular type */
seL4_CPtr kobj_alloc(unsigned size_bits, seL4_ObjectType type);
