/*
 * Copyright 2021, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <sel4/sel4.h>

#define CPTR_INIT_PRIO 101

/* Allocate an unused CPtr */
seL4_CPtr alloc_cptr(void);

/* Set the maximum CPtr */
void set_cptr_region(seL4_CPtr start, seL4_CPtr end);
