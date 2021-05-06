/*
 * Copyright 2021, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <sel4runtime.h>
#include "cptr.h"
#include "fail.h"

static seL4_SlotRegion free_region;

__attribute__((constructor(CPTR_INIT_PRIO)))
static void init_cptr(void)
{
    free_region = sel4runtime_bootinfo()->empty;
}

seL4_CPtr alloc_cptr(void)
{
    seL4_CPtr cptr = free_region.start;
    assert(cptr < free_region.end);
    free_region.start += 1;
    return cptr;
}

void set_cptr_region(seL4_CPtr start, seL4_CPtr end)
{
    free_region = (seL4_SlotRegion) {
        .start = start,
        .end = end,
    };
}
