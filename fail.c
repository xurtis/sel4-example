/*
 * Copyright 2021, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <sel4/sel4.h>
#include <sel4runtime.h>
#include "debug.h"

/*
 * libsel4 assumes, when debug is enabled, that this function exists as
 * part of the C library implementation (which is at least true of
 * musl). As we do not provide a C library, we must provide an
 * implementation of this function ourselves.
 *
 * This simply prints to the debug output the details of the assertion
 * and exits.
 */
void __assert_fail(
    const char *assertion,
    const char *file,
    int line,
    const char *function
)
{
    debug_puts("Assertion failed: (");
    debug_puts(assertion);
    debug_puts(") of function ");
    debug_puts(function);
    debug_puts(" at ");
    debug_puts(file);
    debug_puts(":");
    debug_puti(line);
    debug_puts(".\n");
    sel4runtime_exit(-1);
}
